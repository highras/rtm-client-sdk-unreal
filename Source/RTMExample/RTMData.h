#pragma once
#include "RTMTest.h"

namespace rtm
{
	using namespace std;

	class RTMData : public RTMTest
	{
	public:
		RTMData(const string& endpoint, int64_t pid, int64_t uid, const string& token)
		{
			_endpoint = endpoint;
			_pid = pid;
			_uid = uid;
			_token = token;
		}

		virtual void Start()
		{
			_thread = thread(&RTMData::Test, this);
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

			UE_LOG(LogTemp, Warning, TEXT("=========== Begin set user data ==========="));

			SetData("key1", "value1");
			SetData("key2", "value2");

			UE_LOG(LogTemp, Warning, TEXT("=========== Begin get user data ==========="));

			GetData("key1");
			GetData("key2");

			UE_LOG(LogTemp, Warning, TEXT("=========== Begin delete user data ==========="));

			DeleteData("key2");

			UE_LOG(LogTemp, Warning, TEXT("=========== Begin get user data again ==========="));

			GetData("key1");
			GetData("key2");

			UE_LOG(LogTemp, Warning, TEXT("=========== User logout ==========="));

			_client->Bye(false);

			UE_LOG(LogTemp, Warning, TEXT("=========== User relogin ==========="));

			LoginRTM(_endpoint, _pid, _uid, _token);

			UE_LOG(LogTemp, Warning, TEXT("=========== Begin get user data after relogin ==========="));

			GetData("key1");
			GetData("key2");

			UE_LOG(LogTemp, Warning, TEXT("=========== Demo completed ==========="));
		}

		void SetData(const string& key, const string& value)
		{
			int32_t errorCode = _client->DataSet(key, value);

			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Set user data with key %s in sync succeed."), UTF8_TO_TCHAR(key.c_str()));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Set user data with key %s in sync failed."), UTF8_TO_TCHAR(key.c_str()));
			}
		}

		void GetData(const string& key)
		{
			string value;
			int32_t errorCode = _client->DataGet(value, key);

			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Get user data with key %s in sync succeed, value is %s."), UTF8_TO_TCHAR(key.c_str()), UTF8_TO_TCHAR(value.c_str()));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Get user data with key %s in sync failed."), UTF8_TO_TCHAR(key.c_str()));
			}
		}

		void DeleteData(const string& key)
		{
			string value;
			int32_t errorCode = _client->DataDelete(key);

			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Delete user data with key %s in sync succeed, value is %s."), UTF8_TO_TCHAR(key.c_str()), UTF8_TO_TCHAR(value.c_str()));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Delete user data with key %s in sync failed."), UTF8_TO_TCHAR(key.c_str()));
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
