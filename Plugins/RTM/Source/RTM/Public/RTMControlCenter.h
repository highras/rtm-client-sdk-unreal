#pragma once

#include <atomic>
#include <unordered_map>
#include "RTMClient.h"

namespace rtm
{
	using namespace std;
	using namespace fpnn;

	class RTM_API RTMControlCenter
	{
	public:
		RTMControlCenter() {}
		~RTMControlCenter() {}

		static void Init();
		static void Close();

		static void RegisterSession(uint64_t connectionID, RTMClientPtr client);
		static void UnregisterSession(uint64_t connectionID);
		static void CloseSession(uint64_t connectionID);
		static RTMClientPtr GetClient(uint64_t connectionID);
		static ClientStatus GetClientStatus(uint64_t connectionID);

		static void DelayRelogin(RTMClientPtr client, int64_t intervalMs);
		static void ReloginCheck();

		
		static void ActiveFileGate(const string& endpoint, TCPClientPtr client);
		static TCPClientPtr GetFileGate(const string& endpoint);
		static void CheckFileGate();
	private:
		static void Routine();
	private:
		static mutex _mutex;
		static thread _routine;
		static atomic<bool> _inited;
		static atomic<bool> _running;
		static unordered_map<uint64_t, RTMClientPtr> _rtmClientMap;
		static unordered_map<RTMClientPtr, int64_t> _reloginClientMap;
		static unordered_map<string, unordered_map<TCPClientPtr, int64_t>> _fileGateMap;
	};
}
