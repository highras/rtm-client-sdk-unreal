#pragma once

#include "RTMTest.h"

namespace rtm
{
	using namespace std;
	class RTMMessages : public RTMTest
	{
	public:
		RTMMessages(const string& endpoint, int64_t pid, int64_t uid, const string& token)
		{
			_endpoint = endpoint;
			_pid = pid;
			_uid = uid;
			_token = token;
		}

		virtual void Start()
		{
			_thread = thread(&RTMMessages::Test, this);
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

			SendP2PMessage();
			SendGroupMessage();

			if (EnterRoom())
				SendRoomMessage();

			UE_LOG(LogTemp, Warning, TEXT("Running for receiving server pushed messsage if those are being demoed ..."));
		}

		void SendP2PMessage()
		{
			// sync
			int64_t messageID = 0;
			int32_t errorCode = _client->SendMessage(messageID, _peerUid, _customMType, _textMessage);

			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send text message to user %lld in sync successed, messageID is %lld."), _peerUid, messageID);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Send text message to user %lld in sync failed."), _peerUid);
			}

			// async
			bool status = _client->SendMessage([this](int64_t messageID, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Send text message to user %lld in async successed, messageID is %lld."), _peerUid, messageID);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Send text message to user %lld in async failed."), _peerUid);
				}
			}, _peerUid, _customMType, _textMessage);
			if (!status)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send text message to user %lld in async failed."), _peerUid);
			}
		}

		void SendGroupMessage()
		{
			int64_t messageID = 0;
			int32_t errorCode = _client->SendGroupMessage(messageID, _groupId, _customMType, _textMessage);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send text message to group %lld in sync successed, messageID is %lld."), _groupId, messageID);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Send text message to group %lld in sync failed."), _groupId);
			}

			bool status = _client->SendGroupMessage([this](int64_t messageID, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Send text message to group %lld in async successed, messageID is %lld."), _groupId, messageID);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Send text message to group %lld in async failed."), _groupId);
				}
				}, _groupId, _customMType, _textMessage);
			if (!status)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send text message to group %lld in async failed."), _groupId);
			}
		}

		void SendRoomMessage()
		{
			int64_t messageID = 0;
			int32_t errorCode = _client->SendRoomMessage(messageID, _roomId, _customMType, _textMessage);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send text message to room %lld in sync successed, messageID is %lld."), _roomId, messageID);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Send text message to room %lld in sync failed."), _roomId);
			}

			bool status = _client->SendRoomMessage([this](int64_t messageID, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Send text message to room %lld in async successed, messageID is %lld."), _roomId, messageID);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Send text message to room %lld in async failed."), _roomId);
				}
				}, _roomId, _customMType, _textMessage);
			if (!status)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send text message to room %lld in async failed."), _roomId);
			}
		}

		bool EnterRoom()
		{
			int32_t errorCode = _client->EnterRoom(_roomId);
			if (errorCode != FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Enter room  %lld in sync failed."), _roomId);
				return false;
			}
			else
				return true;
		}

	private:
		thread _thread;
		string _endpoint;
		int64_t _pid;
		int64_t _uid;
		string _token;
	};
}

