#pragma once
#include "RTMTest.h"

namespace rtm
{
	using namespace std;

	class RTMLogin : public RTMTest
	{
	public:
		RTMLogin(const string& endpoint, int64_t pid, int64_t uid, const string& token)
		{
			_endpoint = endpoint;
			_pid = pid;
			_uid = uid;
			_token = token;
		}

		virtual void Start()
		{
			_thread = thread(&RTMLogin::Test, this);
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

			LoginInSync();
			LoginInAsync();

			UE_LOG(LogTemp, Warning, TEXT("========== [Demo completed] =========="));
		}

		void LoginInSync()
		{
			RTMClientPtr client = RTMClient::CreateRTMClient(_pid, _uid, _endpoint, make_shared<RTMTestQuestProcessor>());
			bool ok = false;
			int32_t errorCode = client->Login(ok, _token);
			UE_LOG(LogTemp, Warning, TEXT("Login sync ok: %d, errorCode: %d."), ok, errorCode);
			FPlatformProcess::Sleep(2.0f);
			client->Close();
			FPlatformProcess::Sleep(2.0f);
		}

		void LoginInAsync()
		{
			RTMClientPtr client = RTMClient::CreateRTMClient(_pid, _uid, _endpoint, make_shared<RTMTestQuestProcessor>());
			bool status = client->Login([this](int64_t pid, int64_t uid, bool ok, int32_t errorCode) {
				UE_LOG(LogTemp, Warning, TEXT("Login async ok: %d, errorCode: %d."), ok, errorCode);
				}, _token);
			if (!status)
			{
				UE_LOG(LogTemp, Warning, TEXT("Login async failed."));
			}
			FPlatformProcess::Sleep(2.0f);
			client->Close();
			FPlatformProcess::Sleep(2.0f);
		}
	private:
		thread _thread;
		string _endpoint;
		int64_t _pid;
		int64_t _uid;
		string _token;
	};
}

