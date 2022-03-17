#pragma once
#pragma once
#include "RTMTest.h"

namespace rtm
{
	using namespace std;

	class RTMSystem : public RTMTest
	{
	public:
		RTMSystem(const string& endpoint, int64_t pid, int64_t uid, const string& token)
		{
			_endpoint = endpoint;
			_pid = pid;
			_uid = uid;
			_token = token;
		}

		virtual void Start()
		{
			_thread = thread(&RTMSystem::Test, this);
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

			UE_LOG(LogTemp, Warning, TEXT("========== [Add Attributes] =========="));
			AddAttributes();
			UE_LOG(LogTemp, Warning, TEXT("========== [Get Attributes] =========="));
			GetAttributes();

			UE_LOG(LogTemp, Warning, TEXT("========== [Add Device Push] =========="));
			GetDevicePushOption();
			AddDevicePushOption(MessageCategory_P2PMessage, 12345, {});
			AddDevicePushOption(MessageCategory_GroupMessage, 223344, {});

			AddDevicePushOption(MessageCategory_P2PMessage, 34567, { 23, 35, 56, 67, 78, 89 });
			AddDevicePushOption(MessageCategory_GroupMessage, 445566, { 23, 35, 56, 67, 78, 89 });

			GetDevicePushOption();

			UE_LOG(LogTemp, Warning, TEXT("========== [Remove Device Push] =========="));
			RemoveDevicePushOption(MessageCategory_GroupMessage, 223344, {});
			RemoveDevicePushOption(MessageCategory_GroupMessage, 445566, { 23, 35, 56, 67, 78, 89 });

			GetDevicePushOption();

			RemoveDevicePushOption(MessageCategory_P2PMessage, 12345, {});
			RemoveDevicePushOption(MessageCategory_P2PMessage, 34567, { 23, 35, 56, 67, 78, 89 });

			RemoveDevicePushOption(MessageCategory_GroupMessage, 223344, {});
			RemoveDevicePushOption(MessageCategory_GroupMessage, 445566, { 23, 35, 56, 67, 78, 89 });

			GetDevicePushOption();

			UE_LOG(LogTemp, Warning, TEXT("========== [Demo Completed] =========="));
		}

		void AddAttributes()
		{
			map<string, string> attrs;
			attrs["key1"] = "value1";
			attrs["key2"] = "value2";
			int32_t errorCode = _client->AddAttributes(attrs);

			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Add attributes in sync succeed."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Add attributes in sync failed, errorCode = %d."), errorCode);
			}
		}

		void GetAttributes()
		{
			map<string, string> attrs;
			int32_t errorCode = _client->GetAttributes(attrs);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Get attributes in sync succeed."));
				for (auto kv : attrs)
				{
					UE_LOG(LogTemp, Warning, TEXT(" -- key: %s, value: %s"), UTF8_TO_TCHAR(kv.first.c_str()), UTF8_TO_TCHAR(kv.second.c_str()));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Get attributes in sync failed, errorCode = %d."), errorCode);
			}
		}

		void GetDevicePushOption()
		{
			map<int64_t, set<int8_t>> p2p;
			map<int64_t, set<int8_t>> group;
			int32_t errorCode = _client->GetDevicePushOption(p2p, group);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Get device push option in sync succeed."));
				UE_LOG(LogTemp, Warning, TEXT("---------- [P2P] ----------"));
				for (auto kv : p2p)
				{
					UE_LOG(LogTemp, Warning, TEXT(" -- uid : %lld"), kv.first);
					for (auto mtype : kv.second)
						UE_LOG(LogTemp, Warning, TEXT(" -- mtype : %d"), mtype);
				}
				UE_LOG(LogTemp, Warning, TEXT("---------- [Group] ----------"));
				for (auto kvGroup : group)
				{
					UE_LOG(LogTemp, Warning, TEXT(" -- group : %lld"), kvGroup.first);
					for (auto mtype : kvGroup.second)
						UE_LOG(LogTemp, Warning, TEXT(" -- mtype : %d"), mtype);
				}

			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Get device push option in sync failed, errorCode = %d."), errorCode);
			}
		}

		void AddDevicePushOption(MessageCategory messageCategory, int64_t targetID, const set<int8_t>& mtypes)
		{
			int32_t errorCode = _client->AddDevicePushOption(messageCategory, targetID, mtypes);

			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Add device push option in sync succeed."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Add device push option in sync failed, errorCode = %d."), errorCode);
			}
		}

		void RemoveDevicePushOption(MessageCategory messageCategory, int64_t targetID, const set<int8_t>& mtypes)
		{
			int32_t errorCode = _client->RemoveDevicePushOption(messageCategory, targetID, mtypes);

			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Remove device push option in sync succeed."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Remove device push option in sync failed, errorCode = %d."), errorCode);
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
