#pragma once
#include "RTMTest.h"

namespace rtm
{
	using namespace std;

	class RTMGroups : public RTMTest
	{
	public:
		RTMGroups(const string& endpoint, int64_t pid, int64_t uid, const string& token)
		{
			_endpoint = endpoint;
			_pid = pid;
			_uid = uid;
			_token = token;
		}

		virtual void Start()
		{
			_thread = thread(&RTMGroups::Test, this);
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

			UE_LOG(LogTemp, Warning, TEXT("========== Get group members =========="));
			GetGroupMembers();
			UE_LOG(LogTemp, Warning, TEXT("========== Add group members =========="));
			AddGroupMembers({ 99688868, 99688878, 99688888 });
			FPlatformProcess::Sleep(2.0f);
			UE_LOG(LogTemp, Warning, TEXT("========== Get group members =========="));
			GetGroupMembers();
			UE_LOG(LogTemp, Warning, TEXT("========== Delete group members =========="));
			DeleteGroupMembers({ 99688878 });
			FPlatformProcess::Sleep(2.0f);
			UE_LOG(LogTemp, Warning, TEXT("========== Get group members =========="));
			GetGroupMembers();
			GetGroupMembersAndOnlineMembers();
					
			UE_LOG(LogTemp, Warning, TEXT("========== Get group member count =========="));
			GetGroupMemberCount();
			GetGroupMemberCountWithOnlineMemberCount();

			UE_LOG(LogTemp, Warning, TEXT("========== Get self groups =========="));
			GetSelfGroups();

			UE_LOG(LogTemp, Warning, TEXT("========== Change group infos =========="));
			SetGroupInfos("", "private info");
			GetGroupInfos();

			SetGroupInfos("public info", "");
			GetGroupInfos();

			GetGroupsPublicInfo({ 223344, 334455, 445566, 667788, 778899 });
			UE_LOG(LogTemp, Warning, TEXT("========== Demo completed =========="));
		}

		void GetGroupMembers()
		{
			set<int64_t> uids;
			int32_t errorCode = _client->GetGroupMembers(uids, _groupId);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Get group members in sync succeed, member count = %u"), uids.size());
				for (int64_t uid : uids)
					UE_LOG(LogTemp, Warning, TEXT(" -- member uid : %lld"), uid);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Get group members in sync failed, errorCode = %d"), errorCode);
			}
		}

		void AddGroupMembers(const set<int64_t>& uids)
		{
			int32_t errorCode = _client->AddGroupMembers(_groupId, uids);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Add group members in sync succeed."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Add group members in sync failed, errorCode = %d"), errorCode);
			}
		}

		void DeleteGroupMembers(const set<int64_t>& uids)
		{
			int32_t errorCode = _client->DeleteGroupMembers(_groupId, uids);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Delete group members in sync succeed."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Delete group members in sync failed, errorCode = %d"), errorCode);
			}
		}

		void GetGroupMembersAndOnlineMembers()
		{
			set<int64_t> uids;
			set<int64_t> onlines;
			int32_t errorCode = _client->GetGroupMembers(uids, onlines, _groupId);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Get group members in sync succeed, member count = %u"), uids.size());
				for (int64_t uid : uids)
					UE_LOG(LogTemp, Warning, TEXT(" -- member uid : %lld"), uid);
				for (int64_t uid : onlines)
					UE_LOG(LogTemp, Warning, TEXT(" -- online member uid : "), uid);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Get group members in sync failed, errorCode = %d"), errorCode);
			}

			bool status = _client->GetGroupMembers([this](set<int64_t> uids, set<int64_t> onlines, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Get group members in async succeed, member count = %u"), uids.size());
					for (int64_t uid : uids)
						UE_LOG(LogTemp, Warning, TEXT(" -- member uid : %lld"), uid);
					for (int64_t uid : onlines)
						UE_LOG(LogTemp, Warning, TEXT(" -- online member uid : "), uid);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Get group members in async failed, errorCode = %d"), errorCode);
				}
				}, _groupId);
			if (!status)
				UE_LOG(LogTemp, Warning, TEXT("Get group members in async failed"));
		}

		void GetGroupMemberCount()
		{
			int32_t memberCount = 0;
			int32_t errorCode = _client->GetGroupCount(memberCount, _groupId);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Get group member count in sync succeed, member count = %d"), memberCount);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Get group member count in sync failed, errorCode = %d"), errorCode);
			}

			bool status = _client->GetGroupCount([this](int32_t memberCount, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Get group member count in async succeed, member count = %d"), memberCount);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Get group member count in async failed, errorCode = %d"), errorCode);
				}

				}, _groupId);
			if (!status)
				UE_LOG(LogTemp, Warning, TEXT("Get group member count in async failed, errorCode = %d"), errorCode);
		}

		void GetGroupMemberCountWithOnlineMemberCount()
		{
			int32_t memberCount = 0;
			int32_t onlineCount = 0;
			int32_t errorCode = _client->GetGroupCount(memberCount, onlineCount, _groupId);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Get group member count in sync succeed, member count = %d, online count = %d"), memberCount, onlineCount);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Get group member count in sync failed, errorCode = %d"), errorCode);
			}

			bool status = _client->GetGroupCount([this](int32_t memberCount, int32_t onlineCount, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Get group member count in async succeed, member count = %d, online count = %d"), memberCount, onlineCount);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Get group member count in async failed, errorCode = %d"), errorCode);
				}
				}, _groupId);
			if (!status)
				UE_LOG(LogTemp, Warning, TEXT("Get group member count in async failed"));
		}

		void GetSelfGroups()
		{
			set<int64_t> gids;
			int32_t errorCode = _client->GetUserGroups(gids);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Get user groups in sync succeed."));
				for (int64_t gid : gids)
				{
					UE_LOG(LogTemp, Warning, TEXT(" -- group id: %lld"), gid);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Get user groups count in sync failed, errorCode = %d"), errorCode);
			}
		}

		void SetGroupInfos(const string& publicInfos, const string& privateInfos)
		{
			int32_t errorCode = _client->SetGroupInfo(_groupId, publicInfos, privateInfos);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Set group infos in sync succeed."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Set group infos in sync failed, errorCode = %d"), errorCode);
			}
		}

		void GetGroupInfos()
		{
			string publicInfos;
			string privateInfos;

			int32_t errorCode = _client->GetGroupInfo(publicInfos, privateInfos, _groupId);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Get group infos in sync succeed."));
				UE_LOG(LogTemp, Warning, TEXT(" -- public info: %s"), UTF8_TO_TCHAR(publicInfos.c_str()));
				UE_LOG(LogTemp, Warning, TEXT(" -- private info: %s"), UTF8_TO_TCHAR(privateInfos.c_str()));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Get group infos in sync failed, errorCode = %d"), errorCode);
			}
		}

		void GetGroupsPublicInfo(const set<int64_t>& groupIDs)
		{
			map<int64_t, string> publicInfoMap;
			int32_t errorCode = _client->GetGroupsPublicInfo(publicInfoMap, groupIDs);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Get groups info in sync succeed."));
				for (auto kv : publicInfoMap)
				{
					UE_LOG(LogTemp, Warning, TEXT(" -- group id: %lld info: [%s]"), kv.first, UTF8_TO_TCHAR(kv.second.c_str()));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Get groups info in sync failed, errorCode = %d"), errorCode);
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
