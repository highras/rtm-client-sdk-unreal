#pragma once
#include "RTMTest.h"

namespace rtm
{
	using namespace std;

	class RTMFriends : public RTMTest
	{
	public:
		RTMFriends(const string& endpoint, int64_t pid, int64_t uid, const string& token)
		{
			_endpoint = endpoint;
			_pid = pid;
			_uid = uid;
			_token = token;
		}

		virtual void Start()
		{
			_thread = thread(&RTMFriends::Test, this);
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

			AddFriends({ 123456, 234567, 345678, 456789 });
			GetFriends();
			DeleteFriends({ 234567, 345678 });

			FPlatformProcess::Sleep(2.0f);

			GetFriends();

			AddBlackList({ 123456, 234567, 345678, 456789 });
			GetBlackList();
			DeleteBlackList({ 234567, 345678 });
			GetBlackList();
			DeleteBlackList({ 123456, 234567, 345678, 456789 });
			GetBlackList();

			UE_LOG(LogTemp, Warning, TEXT("=========== Demo completed ==========="));
		}

		void AddFriends(const set<int64_t>& uids)
		{
			int32_t errorCode = _client->AddFriends(uids);

			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Add friends in sync succeed."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Add friends in sync failed, error code is %d"), errorCode);
			}
		}

		void GetFriends()
		{
			set<int64_t> friends;
			int32_t errorCode = _client->GetFriends(friends);

			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Get friends in sync succeed."));
				for (auto uid : friends)
				{
					UE_LOG(LogTemp, Warning, TEXT(" -- Friend uid: %lld"), uid);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Get friends in sync failed, error code is %d"), errorCode);
			}
		}

		void DeleteFriends(const set<int64_t>& uids)
		{
			int32_t errorCode = _client->DeleteFriends(uids);

			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Delete friends in sync succeed."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Delete friends in sync failed, error code is %d"), errorCode);
			}
		}

		void AddBlackList(const set<int64_t>& uids)
		{
			int32_t errorCode = _client->AddBlackList(uids);

			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Add users to blacklist in sync succeed."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Add users to blacklist in sync failed, error code is %d"), errorCode);
			}
		}

		void GetBlackList()
		{
			set<int64_t> blacklist;
			int32_t errorCode = _client->GetBlackList(blacklist);

			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Get blacklist in sync succeed."));
				for (auto uid : blacklist)
				{
					UE_LOG(LogTemp, Warning, TEXT(" -- blocked uid: %lld"), uid);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Get blacklist in sync failed, error code is %d"), errorCode);
			}
		}

		void DeleteBlackList(const set<int64_t>& uids)
		{
			int32_t errorCode = _client->DeleteBlackList(uids);

			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Delete users to blacklist in sync succeed."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Delete users to blacklist in sync failed, error code is %d"), errorCode);
			}
		}
	public:
		thread _thread;
		string _endpoint;
		int64_t _pid;
		int64_t _uid;
		string _token;
	};
}

