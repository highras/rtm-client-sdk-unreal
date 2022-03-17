#include "RTMControlCenter.h"
#include "RTMConfig.h"

namespace rtm
{
	using namespace std;
	using namespace fpnn;

	mutex RTMControlCenter::_mutex;
	thread RTMControlCenter::_routine;
	atomic<bool> RTMControlCenter::_inited(false);
	atomic<bool> RTMControlCenter::_running;
	unordered_map<uint64_t, RTMClientPtr> RTMControlCenter::_rtmClientMap;
	unordered_map<RTMClientPtr, int64_t> RTMControlCenter::_reloginClientMap;
	unordered_map<string, unordered_map<TCPClientPtr, int64_t>> RTMControlCenter::_fileGateMap;

	void RTMControlCenter::Init()
	{
		if (_inited)
			return;
		_inited = true;
		_running = true;
		_routine = thread(&RTMControlCenter::Routine);
	}
	void RTMControlCenter::Close()
	{
		_running = false;
		_routine.join();
		{
			lock_guard<mutex> locker(_mutex);
			for (auto kv : _rtmClientMap)
				kv.second->Close();
		}
	}

	void RTMControlCenter::RegisterSession(uint64_t connectionID, RTMClientPtr client)
	{
		lock_guard<mutex> locker(_mutex);
		auto iter = _rtmClientMap.find(connectionID);
		if (iter == _rtmClientMap.end())
			_rtmClientMap.insert(make_pair(connectionID, client));
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("same connectionID %llu"), connectionID);
			_rtmClientMap[connectionID] = client;
		}
	}

	void RTMControlCenter::UnregisterSession(uint64_t connectionID)
	{
		lock_guard<mutex> locker(_mutex);
		_rtmClientMap.erase(connectionID);
	}

	void RTMControlCenter::CloseSession(uint64_t connectionID)
	{
		RTMClientPtr client = GetClient(connectionID);
		if (client == nullptr)
			return;
		client->Close();
	}

	RTMClientPtr RTMControlCenter::GetClient(uint64_t connectionID)
	{
		lock_guard<mutex> locker(_mutex);
		auto iter = _rtmClientMap.find(connectionID);
		if (iter == _rtmClientMap.end())
			return nullptr;
		return iter->second;
	}

	ClientStatus RTMControlCenter::GetClientStatus(uint64_t connectionID)
	{
		lock_guard<mutex> locker(_mutex);
		auto iter = _rtmClientMap.find(connectionID);
		if (iter == _rtmClientMap.end())
			return ClientStatus::Closed;
		return iter->second->GetClientStatus();
	}

	void RTMControlCenter::DelayRelogin(RTMClientPtr client, int64_t intervalMs)
	{
		int64_t now = TimeUtil::curr_msec();
		{
			lock_guard<mutex> locker(_mutex);
			_reloginClientMap[client] = now + intervalMs;
		}
	}

	void RTMControlCenter::ReloginCheck()
	{
		int64_t now = TimeUtil::curr_msec();
		vector<RTMClientPtr> clientList;
		{
			lock_guard<mutex> locker(_mutex);
			for (auto iter = _reloginClientMap.begin(); iter != _reloginClientMap.end(); )
			{
				if (iter->second <= now)
				{
					clientList.push_back(iter->first);
					iter = _reloginClientMap.erase(iter);
				}
				else
					++iter;
			}
		}

		for (auto client : clientList)
		{
			ClientEngine::runTask([client]() {
				client->StartRelogin();
				});
		}
	}

	void RTMControlCenter::ActiveFileGate(const string& endpoint, TCPClientPtr client)
	{
		auto iterEndpoint = _fileGateMap.find(endpoint);
		if (iterEndpoint == _fileGateMap.end())
		{
			_fileGateMap[endpoint][client] = TimeUtil::curr_sec();
			return;
		}
		auto iterClient = iterEndpoint->second.find(client);
		if (iterClient == iterEndpoint->second.end())
		{
			iterEndpoint->second.insert(make_pair(client, TimeUtil::curr_sec()));
			return;
		}
		iterEndpoint->second[client] = TimeUtil::curr_sec();
	}

	TCPClientPtr RTMControlCenter::GetFileGate(const string& endpoint)
	{
		auto iter = _fileGateMap.find(endpoint);
		if (iter == _fileGateMap.end())
			return nullptr;
		if (iter->second.empty())
			return nullptr;
		auto iterClient = iter->second.begin();
		return iterClient->first;
	}

	void RTMControlCenter::CheckFileGate()
	{
		int64_t timeout = TimeUtil::curr_sec() - RTMConfig::fileGateClientHoldingSeconds;
		lock_guard<mutex> locker(_mutex);

		for (auto iterTime = _fileGateMap.begin(); iterTime != _fileGateMap.end();)
		{
			unordered_map<TCPClientPtr, int64_t>& fileGateMap = iterTime->second;
			for (auto iter = fileGateMap.begin(); iter != fileGateMap.end();)
			{
				if (iter->second <= timeout)
					iter = fileGateMap.erase(iter);
				else
					++iter;
			}
			if (fileGateMap.empty())
				iterTime = _fileGateMap.erase(iterTime);
			else
				++iterTime;
		}
	}

	void RTMControlCenter::Routine()
	{
		while (_running)
		{
			FPlatformProcess::Sleep(1.0f);
			{
				lock_guard<mutex> locker(_mutex);
				for (auto kv : _rtmClientMap)
				{
					if (!kv.second->IsAlive())
						kv.second->Close(false);
				}
			}

			CheckFileGate();
			ReloginCheck();
		}
	}
}