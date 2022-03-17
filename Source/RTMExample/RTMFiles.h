#pragma once
#include "RTMTest.h"

namespace rtm
{
	using namespace std;

	class RTMFiles : public RTMTest
	{
	public:
		RTMFiles(const string& endpoint, int64_t pid, int64_t uid, const string& token)
		{
			_endpoint = endpoint;
			_pid = pid;
			_uid = uid;
			_token = token;
		}

		virtual ~RTMFiles()
		{
			UE_LOG(LogTemp, Warning, TEXT("~RTMFiles"));
		}

		virtual void Start()
		{
			_thread = thread(&RTMFiles::Test, this);
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

			SendP2PFileInSync(_uid + 1, NormalFileType);
			SendP2PFileInAsync(_uid + 1, NormalFileType);

			SendGroupFileInSync(_groupId, NormalFileType);
			SendGroupFileInAsync(_groupId, NormalFileType);

			EnterRoom();

			SendRoomFileInSync(_roomId, NormalFileType);
			SendRoomFileInAsync(_roomId, NormalFileType);

			//_client->Close();
			_client->Bye();
			UE_LOG(LogTemp, Warning, TEXT("========== Demo completed =========="));
		}

		void SendP2PFileInSync(int64_t peerUid, ChatMessageType type)
		{
			int64_t messageID = 0;
			int32_t errorCode = _client->SendFile(messageID, peerUid, type, _fileContent, _fileName);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send file to user %lld in sync succeed, messageID = %lld."), peerUid, messageID);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Send file to user %lld in sync failed, errorCode = %d."), peerUid, errorCode);
			}
		}

		void SendP2PFileInAsync(int64_t peerUid, ChatMessageType type)
		{
			bool status = _client->SendFile([this, peerUid](int64_t messageID, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Send file to user %lld in async succeed, messageID = %lld."), peerUid, messageID);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Send file to user %lld in async failed, errorCode = %d."), peerUid, errorCode);
				}
				}, peerUid, type, _fileContent, _fileName);
			if (!status)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send file to user %lld in async failed."), peerUid);
			}
			else
				FPlatformProcess::Sleep(3.0f);
		}

		void SendGroupFileInSync(int64_t groupID, ChatMessageType type)
		{
			int64_t messageID = 0;
			int32_t errorCode = _client->SendGroupFile(messageID, groupID, type, _fileContent, _fileName);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send file to group %lld in sync succeed, messageID = %lld."), groupID, messageID);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Send file to group %lld in sync failed, errorCode = %d."), groupID, errorCode);
			}

		}

		void SendGroupFileInAsync(int64_t groupID, ChatMessageType type)
		{
			bool status = _client->SendGroupFile([this, groupID](int64_t messageID, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Send file to group %lld in async succeed, messageID = %lld."), groupID, messageID);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Send file to group %lld in async failed, errorCode = %d."), groupID, errorCode);
				}
				}, groupID, type, _fileContent, _fileName);
			if (!status)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send file to group %lld in async failed."), groupID);
			}
			else
				FPlatformProcess::Sleep(3.0f);
		}

		void EnterRoom()
		{
			int32_t errorCode = _client->EnterRoom(_roomId);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Enter room %lld in sync succeed."), _roomId);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Enter room %lld in sync failed, errorCode = %d."), _roomId, errorCode);
			}
		}

		void SendRoomFileInSync(int64_t roomID, ChatMessageType type)
		{
			int64_t messageID = 0;
			int32_t errorCode = _client->SendRoomFile(messageID, roomID, type, _fileContent, _fileName);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send file to room %lld in sync succeed, messageID = %lld."), roomID, messageID);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Send file to room %lld in sync failed, errorCode = %d."), roomID, errorCode);
			}
		}

		void SendRoomFileInAsync(int64_t roomID, ChatMessageType type)
		{
			bool status = _client->SendRoomFile([this, roomID](int64_t messageID, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Send file to room %lld in async succeed, messageID = %lld."), roomID, messageID);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Send file to room %lld in async failed, errorCode = %d."), roomID, errorCode);
				}
				}, roomID, type, _fileContent, _fileName);
			if (!status)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send file to room %lld in async failed."), roomID);
			}
			else
				FPlatformProcess::Sleep(3.0f);

		}
	private:
		thread _thread;
		string _endpoint;
		int64_t _pid;
		int64_t _uid;
		string _token;
		const string _fileName = "demo.txt";
		const string _fileContent = "Hello, RTM!";
	};
}

