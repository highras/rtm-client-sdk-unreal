#pragma once
#include "RTMTest.h"

namespace rtm
{
	using namespace std;

	class RTMRooms : public RTMTest
	{
	public:
		RTMRooms(const string& endpoint, int64_t pid, int64_t uid, const string& token)
		{
			_endpoint = endpoint;
			_pid = pid;
			_uid = uid;
			_token = token;
		}

		virtual void Start()
		{
			_thread = thread(&RTMRooms::Test, this);
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

			UE_LOG(LogTemp, Warning, TEXT("========== [Enter Room] =========="));
			EnterRoom();

			UE_LOG(LogTemp, Warning, TEXT("========== [Get Self Rooms] =========="));
			GetSelfRooms();

			UE_LOG(LogTemp, Warning, TEXT("========== [Leave Room] =========="));
			LeaveRoom();

			UE_LOG(LogTemp, Warning, TEXT("========== [Get Self Rooms] =========="));
			GetSelfRooms();

			UE_LOG(LogTemp, Warning, TEXT("========== [Enter Room] =========="));
			EnterRoom();

			UE_LOG(LogTemp, Warning, TEXT("========== [Set Room Infos] =========="));
			SetRoomInfos("public info", "private info");
			GetRoomInfos();

			UE_LOG(LogTemp, Warning, TEXT("========== [Change Room Infos] =========="));
			SetRoomInfos("", "private info");
			GetRoomInfos();

			UE_LOG(LogTemp, Warning, TEXT("========== [Change Room Infos] =========="));
			SetRoomInfos("public info", "");
			GetRoomInfos();

			GetRoomsPublicInfo({ _roomId, 778899, 445566, 334455, 1234 });

			UE_LOG(LogTemp, Warning, TEXT("========== [Get Room Members] =========="));
			GetRoomMemberCount({ _roomId, 778899, 445566, 334455, 1234 });
			GetRoomMembers();

			UE_LOG(LogTemp, Warning, TEXT("========== [Demo Complete] =========="));
		}

		void EnterRoom()
		{
			int32_t errorCode = _client->EnterRoom(_roomId);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Enter room %lld in sync succeed"), _roomId);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Enter room %lld in sync failed, errorCode = %d"), _roomId, errorCode);
			}
		}

		void GetSelfRooms()
		{
			set<int64_t> rids;
			int32_t errorCode = _client->GetUserRooms(rids);

			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Get user rooms in sync succeed"));
				for (auto rid : rids)
					UE_LOG(LogTemp, Warning, TEXT(" -- room id : %lld"), rid);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Get user rooms in sync failed, errorCode = %d"), _roomId, errorCode);
			}
		}

		void LeaveRoom()
		{
			int32_t errorCode = _client->LeaveRoom(_roomId);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Leave room %lld in sync succeed"), _roomId);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Leave room %lld in sync failed"), _roomId);
			}
		}

		void SetRoomInfos(const string& publicInfo, const string& privateInfo)
		{
			int32_t errorCode = _client->SetRoomInfo(_roomId, publicInfo, privateInfo);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Set room info in sync succeed"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Set room info in sync failed, errorCode = %d"), errorCode);
			}
		}

		void GetRoomInfos()
		{
			string publicInfo, privateInfo;
			int32_t errorCode = _client->GetRoomInfo(publicInfo, privateInfo, _roomId);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Get room info in sync succeed"));
				UE_LOG(LogTemp, Warning, TEXT("Public info : %s"), UTF8_TO_TCHAR(publicInfo.c_str()));
				UE_LOG(LogTemp, Warning, TEXT("Private info : %s"), UTF8_TO_TCHAR(privateInfo.c_str()));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Get room info in sync failed, errorCode = %d"), errorCode);
			}
		}

		void GetRoomsPublicInfo(const set<int64_t>& rooms)
		{
			map<int64_t, string> infos;
			int32_t errorCode = _client->GetRoomsPublicInfo(infos, rooms);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Get room public info in sync succeed"));
				for (auto kv : infos)
					UE_LOG(LogTemp, Warning, TEXT(" -- room id : %lld, info: [ %s ]"), kv.first, UTF8_TO_TCHAR(kv.second.c_str()));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Get room public info in sync failed, errorCode = %d"), errorCode);
			}
		}

		void GetRoomMemberCount(const set<int64_t>& rooms)
		{
			map<int64_t, int32_t> counts;
			int32_t errorCode = _client->GetRoomMemberCount(counts, rooms);

			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Get room member count in sync succeed"));
				for (auto kv : counts)
					UE_LOG(LogTemp, Warning, TEXT(" -- room: %lld, count: %d"), kv.first, kv.second);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Get room member count in sync failed, errorCode = %d"), errorCode);
			}

			bool status = _client->GetRoomMemberCount([this](map<int64_t, int32_t> counts, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Get room member count in async succeed"));
					for (auto kv : counts)
						UE_LOG(LogTemp, Warning, TEXT(" -- room: %lld, count: %d"), kv.first, kv.second);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Get room member count in async failed, errorCode = %d"), errorCode);
				}
				}, rooms);
			if (!status)
				UE_LOG(LogTemp, Warning, TEXT("Get room member count in async failed"));
			FPlatformProcess::Sleep(3.0f);
		}

		void GetRoomMembers()
		{
			set<int64_t> uids;
			int32_t errorCode = _client->GetRoomMembers(uids, _roomId);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Get room members in sync succeed"));
				for (auto uid : uids)
					UE_LOG(LogTemp, Warning, TEXT(" -- room member: %lld"), uid);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Get room members in sync failed, errorCode = %d"), errorCode);
			}

			bool status = _client->GetRoomMembers([this](set<int64_t> uids, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Get room members in sync succeed"));
					for (auto uid : uids)
						UE_LOG(LogTemp, Warning, TEXT(" -- room member: %lld"), uid);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Get room members in sync failed, errorCode = %d"), errorCode);
				}
				}, _roomId);
			if (!status)
				UE_LOG(LogTemp, Warning, TEXT("Get room members in async failed"));
			FPlatformProcess::Sleep(3.0f);
		}
	private:
		thread _thread;
		string _endpoint;
		int64_t _pid;
		int64_t _uid;
		string _token;
	};
}
