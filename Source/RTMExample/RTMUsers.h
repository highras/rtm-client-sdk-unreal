#pragma once
#include "RTMTest.h"

namespace rtm
{
	using namespace std;

	class RTMUsers : public RTMTest
	{
	public:
		RTMUsers(const string& endpoint, int64_t pid, int64_t uid, const string& token)
		{
			_endpoint = endpoint;
			_pid = pid;
			_uid = uid;
			_token = token;
		}

		virtual void Start()
		{
			_thread = thread(&RTMUsers::Test, this);
		}
		virtual void Stop()
		{
			_thread.join();
		}
		void Test()
		{
			LoginRTM(_endpoint, _pid, _uid, _token);
			if (_client == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("User %lld login RTM failed"), _uid);
				return;
			}

			GetOnlineUsers({ 99688848, 123456, 234567, 345678, 456789 });

			SetUserInfos("public info", "private info");
			GetUserInfos();

			SetUserInfos("", "private info");
			GetUserInfos();

			SetUserInfos("public info", "");
			GetUserInfos();

			GetUserPublicInfos({ 99688848, 123456, 234567, 345678, 456789 });
		}

		void GetOnlineUsers(const set<int64_t>& uids)
		{
			set<int64_t> onlines;
			int32_t errorCode = _client->GetOnlineUsers(onlines, uids);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Get online users in sync succeed."));
				for (auto uid : onlines)
					UE_LOG(LogTemp, Warning, TEXT(" -- online uid: %lld"), uid);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Get online users in sync failed, errorCode = %d."), errorCode);
			}
		}

		void SetUserInfos(const string& publicInfo, const string& privateInfo)
		{
			int32_t errorCode = _client->SetUserInfo(publicInfo, privateInfo);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Set user infos in sync succeed."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Set user infos in sync failed, errorCode = %d."), errorCode);
			}
		}

		void GetUserInfos()
		{
			string publicInfo, privateInfo;
			int32_t errorCode = _client->GetUserInfo(publicInfo, privateInfo);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Get user infos in sync succeed."));
				UE_LOG(LogTemp, Warning, TEXT("Public info: %s"), UTF8_TO_TCHAR(publicInfo.c_str()));
				UE_LOG(LogTemp, Warning, TEXT("Private info: %s"), UTF8_TO_TCHAR(privateInfo.c_str()));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Get user infos in sync failed."));
			}
		}

		void GetUserPublicInfos(const set<int64_t>& uids)
		{
			map<int64_t, string> publicInfos;
			int32_t errorCode = _client->GetUserPublicInfo(publicInfos, uids);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Get user public infos in sync succeed."));
				for (auto kv : publicInfos)
					UE_LOG(LogTemp, Warning, TEXT(" -- uid: %lld, info: %s"), kv.first, UTF8_TO_TCHAR(kv.second.c_str()));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Get user public infos in sync failed, errorCode = %d."), errorCode);
			}
		}

	private:
		thread _thread;
		string _endpoint;
		int64_t _pid;
		int64_t _uid;
		string _token;
	};
}
