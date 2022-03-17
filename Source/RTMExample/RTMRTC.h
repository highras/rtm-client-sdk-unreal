#pragma once
#include "RTMTest.h"

namespace rtm
{
	using namespace std;

	class RTMRTC : public RTMTest
	{
	public:
		RTMRTC(const string& endpoint, const string& rtcEndpoint, int64_t pid, int64_t uid, const string& token, const string& token2)
		{
			_endpoint = endpoint;
			_rtcEndpoint = rtcEndpoint;
			_pid = pid;
			_uid = uid;
			_token = token;
			_token2 = token2;
		}

		virtual void Start()
		{
			_thread = thread(&RTMRTC::Test, this);
		}
		virtual void Stop()
		{
			_thread.join();
		}
		void Test()
		{
			_client = LoginRTC(_endpoint, _rtcEndpoint, _pid, _uid, _token);
			_client2 = LoginRTC(_endpoint, _rtcEndpoint, _pid, _uid + 1, _token2);
			if (_client == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("User %lld login RTM failed"), _uid);
				return;
			}
			if (_client2 == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("User %lld login RTM failed"), _uid + 1);
				return;
			}

			_client->CreateRTCRoom([this](int64_t roomID, int32_t errorCode) {
				UE_LOG(LogTemp, Warning, TEXT("CreateRTCRoom roomId = %lld, errorCode = %d"), roomID, errorCode);
				if (errorCode != FPNN_EC_OK)
				{
					_client->EnterRTCRoom([](int64_t roomID, RTCRoomType roomType, int32_t errorCode) {
						UE_LOG(LogTemp, Warning, TEXT("EnterRTCRoom roomId = %lld, errorCode = %d"), roomID, errorCode);
						}, _roomId);
					return;
				}
				}, _roomId, VoiceRoom);
			FPlatformProcess::Sleep(1.0f);

			_client->InviteUserIntoRTCRoom([this](int32_t errorCode) {
				UE_LOG(LogTemp, Warning, TEXT("InviteUserIntoRTCRoom errorCode = %d"), errorCode);
				}, _roomId, { _uid + 1 });
			FPlatformProcess::Sleep(1.0f);

			_client2->EnterRTCRoom([this](int64_t roomID, RTCRoomType roomType, int32_t errorCode) {
				UE_LOG(LogTemp, Warning, TEXT("EnterRTCRoom roomId = %lld, errorCode = %d"), roomID, errorCode);
				}, _roomId);
			FPlatformProcess::Sleep(1.0f);
			_client->AdminCommand([](int32_t errorCode) {
				UE_LOG(LogTemp, Warning, TEXT("AdminCommand errorCode = %d"), errorCode);
				}, _roomId, { _uid + 1 }, AppointAdministrator);

			_client->GetRTCRoomMemberCount([this](int32_t count, int32_t errorCode) {
				UE_LOG(LogTemp, Warning, TEXT("GetRTCRoomMemberCount count = %d, errorCode = %d"), count, errorCode);
				}, _roomId);

			_client->GetRTCRoomMembers([](set<int64_t> uids, set<int64_t> administrators, int64_t owner, int32_t errorCode) {
				if (errorCode != FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("GetRTCRoomMembers errorCode = %d"), errorCode);
					return;
				}
				UE_LOG(LogTemp, Warning, TEXT("GetRTCRoomMembers owner = %lld"), owner);
				for (auto uid : uids)
					UE_LOG(LogTemp, Warning, TEXT(" -- member = %lld"), uid);
				for (auto administrator : administrators)
					UE_LOG(LogTemp, Warning, TEXT(" -- administrator = %lld"), administrator);
				}, _roomId);
			FPlatformProcess::Sleep(1.0f);
			_client2->ExitRTCRoom([this](int32_t errorCode) {
				UE_LOG(LogTemp, Warning, TEXT("ExitRTCRoom errorCode = %d"), errorCode);
				}, _roomId);
			FPlatformProcess::Sleep(1.0f);
			UE_LOG(LogTemp, Warning, TEXT("========== [Demo Complete] =========="));
		}

		RTMClientPtr LoginRTC(const string& rtmEndpoint, const string& rtcEndpoint, int64_t pid, int64_t uid, const string& token)
		{
			RTMClientPtr client = RTMClient::CreateRTMClient(pid, uid, rtmEndpoint, make_shared<RTMTestQuestProcessor>());
			bool ok = false;
			int32_t errorCode = client->Login(ok, token);
			if (ok)
			{
				UE_LOG(LogTemp, Warning, TEXT("login success"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("login failed, errorCode = %d"), errorCode);
			}

			return client;
		}
	private:
		thread _thread;
		string _endpoint;
		string _rtcEndpoint;
		int64_t _pid;
		int64_t _uid;
		string _token;
		string _token2;
		RTMClientPtr _client;
		RTMClientPtr _client2;
	};
}

