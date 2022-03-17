#pragma once
#include "RTMClient.h"
#include "RTMQuestProcessor.h"
#include "AudioRecord.h"
#include "Http.h"

namespace rtm
{
	using namespace std;

	class RTMTestQuestProcessor : public RTMQuestProcessor
	{
	public:
		virtual ~RTMTestQuestProcessor(){}

		virtual void SessionClosed(int32_t closedByErrorCode)
		{
			UE_LOG(LogTemp, Warning, TEXT("Session Closed, errorCode = %d"), closedByErrorCode);
		}

		// Return true for starting relogin, false for stopping relogin.
		virtual bool ReloginWillStart(int32_t lastErrorCode, int32_t retriedCount) 
		{
			UE_LOG(LogTemp, Warning, TEXT("ReloginWillStart, lastErrorCode = %d, retriedCount = %d"), lastErrorCode, retriedCount);
			return true; 
		}
		virtual void ReloginCompleted(bool successful, bool retryAgain, int32_t errorCode, int32_t retriedCount) 
		{
			UE_LOG(LogTemp, Warning, TEXT("ReloginCompleted, successful = %d, retryAgain = %d, errorCode = %d, retriedCount = %d"), successful, retryAgain, errorCode, retriedCount);
		}

		virtual void Kickout() 
		{
			UE_LOG(LogTemp, Warning, TEXT("Kickout"));
		}

		virtual void KickoutRoom(int64_t roomID)
		{
			UE_LOG(LogTemp, Warning, TEXT("Kickout Room %lld"), roomID);
		}

		virtual void PushMessage(const RTMMessage& message)
		{
			UE_LOG(LogTemp, Warning, TEXT("PushMessage: from = %lld, type = %d, mid = %lld, attrs = %s, message = %s"), message.fromUid, message.messageType, message.messageId, UTF8_TO_TCHAR(message.attrs.c_str()), UTF8_TO_TCHAR(message.message.c_str()));
		}

		virtual void PushGroupMessage(const RTMMessage& message) 
		{
			UE_LOG(LogTemp, Warning, TEXT("PushGroupMessage: from = %lld, groupID = %lld, type = %d, mid = %lld, attrs = %s, message = %s"), message.fromUid, message.toId, message.messageType, message.messageId, UTF8_TO_TCHAR(message.attrs.c_str()), UTF8_TO_TCHAR(message.message.c_str()));
		}

		virtual void PushRoomMessage(const RTMMessage& message) 
		{
			UE_LOG(LogTemp, Warning, TEXT("PushRoomMessage: from = %lld, roomID = %lld, type = %d, mid = %lld, attrs = %s, message = %s"), message.fromUid, message.toId, message.messageType, message.messageId, UTF8_TO_TCHAR(message.attrs.c_str()), UTF8_TO_TCHAR(message.message.c_str()));
		}

		virtual void PushBroadcastMessage(const RTMMessage& message) 
		{
			UE_LOG(LogTemp, Warning, TEXT("PushBroadcastMessage: from = %lld, type = %d, mid = %lld, attrs = %s, message = %s"), message.fromUid, message.messageType, message.messageId, UTF8_TO_TCHAR(message.attrs.c_str()), UTF8_TO_TCHAR(message.message.c_str()));
		}

		// =============== [ Chat ] ===============
		virtual void PushChat(const RTMMessage& message)
		{
			UE_LOG(LogTemp, Warning, TEXT("PushChat: from = %lld, type = %d, mid = %lld, attrs = %s, message = %s"), message.fromUid, message.messageType, message.messageId, UTF8_TO_TCHAR(message.attrs.c_str()), UTF8_TO_TCHAR(message.message.c_str()));
		}

		virtual void PushGroupChat(const RTMMessage& message)
		{
			UE_LOG(LogTemp, Warning, TEXT("PushGroupChat: from = %lld, groupID = %lld, type = %d, mid = %lld, attrs = %s, message = %s"), message.fromUid, message.toId, message.messageType, message.messageId, UTF8_TO_TCHAR(message.attrs.c_str()), UTF8_TO_TCHAR(message.message.c_str()));
		}

		virtual void PushRoomChat(const RTMMessage& message)
		{
			UE_LOG(LogTemp, Warning, TEXT("PushRoomChat: from = %lld, roomID = %lld, type = %d, mid = %lld, attrs = %s, message = %s"), message.fromUid, message.toId, message.messageType, message.messageId, UTF8_TO_TCHAR(message.attrs.c_str()), UTF8_TO_TCHAR(message.message.c_str()));
		}

		virtual void PushBroadcastChat(const RTMMessage& message)
		{
			UE_LOG(LogTemp, Warning, TEXT("PushBroadcastChat: from = %lld, type = %d, mid = %lld, attrs = %s, message = %s"), message.fromUid, message.messageType, message.messageId, UTF8_TO_TCHAR(message.attrs.c_str()), UTF8_TO_TCHAR(message.message.c_str()));
		}

		// =============== [ Cmd ] ===============
		virtual void PushCmd(const RTMMessage& message)
		{
			UE_LOG(LogTemp, Warning, TEXT("PushCmd: from = %lld, type = %d, mid = %lld, attrs = %s, message = %s"), message.fromUid, message.messageType, message.messageId, UTF8_TO_TCHAR(message.attrs.c_str()), UTF8_TO_TCHAR(message.message.c_str()));
		}

		virtual void PushGroupCmd(const RTMMessage& message)
		{
			UE_LOG(LogTemp, Warning, TEXT("PushGroupCmd: from = %lld, groupID = %lld, type = %d, mid = %lld, attrs = %s, message = %s"), message.fromUid, message.toId, message.messageType, message.messageId, UTF8_TO_TCHAR(message.attrs.c_str()), UTF8_TO_TCHAR(message.message.c_str()));
		}

		virtual void PushRoomCmd(const RTMMessage& message)
		{
			UE_LOG(LogTemp, Warning, TEXT("PushRoomCmd: from = %lld, roomID = %lld, type = %d, mid = %lld, attrs = %s, message = %s"), message.fromUid, message.toId, message.messageType, message.messageId, UTF8_TO_TCHAR(message.attrs.c_str()), UTF8_TO_TCHAR(message.message.c_str()));
		}

		virtual void PushBroadcastCmd(const RTMMessage& message)
		{
			UE_LOG(LogTemp, Warning, TEXT("PushBroadcastCmd: from = %lld, type = %d, mid = %lld, attrs = %s, message = %s"), message.fromUid, message.messageType, message.messageId, UTF8_TO_TCHAR(message.attrs.c_str()), UTF8_TO_TCHAR(message.message.c_str()));
		}

		// =============== [ Files ] ===============
		virtual void PushFile(const RTMMessage& message)
		{
			UE_LOG(LogTemp, Warning, TEXT("PushFile: from = %lld, type = %d, mid = %lld, attrs = %s, url = %s"), message.fromUid, message.messageType, message.messageId, UTF8_TO_TCHAR(message.attrs.c_str()), UTF8_TO_TCHAR(message.fileInfo->url.c_str()));
			if (message.fileInfo->isRTMAudio)
				UE_LOG(LogTemp, Warning, TEXT("--[RTM Audio] language = %s, duration = %d"), UTF8_TO_TCHAR(message.fileInfo->language.c_str()), message.fileInfo->duration);
		}

		virtual void PushGroupFile(const RTMMessage& message)
		{
			UE_LOG(LogTemp, Warning, TEXT("PushGroupFile: from = %lld, type = %d, mid = %lld, attrs = %s, url = %s"), message.fromUid, message.messageType, message.messageId, UTF8_TO_TCHAR(message.attrs.c_str()), UTF8_TO_TCHAR(message.fileInfo->url.c_str()));
			if (message.fileInfo->isRTMAudio)
				UE_LOG(LogTemp, Warning, TEXT("--[RTM Audio] language = %s, duration = %d"), UTF8_TO_TCHAR(message.fileInfo->language.c_str()), message.fileInfo->duration);
		}

		virtual void PushRoomFile(const RTMMessage& message)
		{
			UE_LOG(LogTemp, Warning, TEXT("PushRoomFile: from = %lld, type = %d, mid = %lld, attrs = %s, url = %s"), message.fromUid, message.messageType, message.messageId, UTF8_TO_TCHAR(message.attrs.c_str()), UTF8_TO_TCHAR(message.fileInfo->url.c_str()));
			if (message.fileInfo->isRTMAudio)
				UE_LOG(LogTemp, Warning, TEXT("--[RTM Audio] language = %s, duration = %d"), UTF8_TO_TCHAR(message.fileInfo->language.c_str()), message.fileInfo->duration);
		}

		virtual void PushBroadcastFile(const RTMMessage& message)
		{
			UE_LOG(LogTemp, Warning, TEXT("PushBroadcastFile: from = %lld, type = %d, mid = %lld, attrs = %s, url = %s"), message.fromUid, message.messageType, message.messageId, UTF8_TO_TCHAR(message.attrs.c_str()), UTF8_TO_TCHAR(message.fileInfo->url.c_str()));
			if (message.fileInfo->isRTMAudio)
				UE_LOG(LogTemp, Warning, TEXT("--[RTM Audio] language = %s, duration = %d"), UTF8_TO_TCHAR(message.fileInfo->language.c_str()), message.fileInfo->duration);
		}

		virtual void PushEnterRTCRoom(int64_t roomID, int64_t uid, int64_t messageTime) 
		{
			UE_LOG(LogTemp, Warning, TEXT("PushEnterRTCRoom roomID = %lld, uid = %lld, mtime = %lld"), roomID, uid, messageTime);
		}

		virtual void PushExitRTCRoom(int64_t roomID, int64_t uid, int64_t messageTime)
		{
			UE_LOG(LogTemp, Warning, TEXT("PushExitRTCRoom roomID = %lld, uid = %lld, mtime = %lld"), roomID, uid, messageTime);
		}

		virtual void PushRTCRoomClosed(int64_t roomID)
		{
			UE_LOG(LogTemp, Warning, TEXT("PushRTCRoomClosed roomID = %lld"), roomID);
		}

		virtual void PushInviteIntoRTCRoom(int64_t fromUid, int64_t roomID)
		{
			UE_LOG(LogTemp, Warning, TEXT("PushInviteIntoRTCRoom fromUid = %lld, roomID = %lld"), fromUid, roomID);
		}

		virtual void PushKickOutRTCRoom(int64_t fromUid, int64_t roomID)
		{
			UE_LOG(LogTemp, Warning, TEXT("PushKickOutRTCRoom fromUid = %lld, roomID = %lld"), fromUid, roomID);
		}

		virtual void PushPullIntoRTCRoom(int64_t roomID, const string& token)
		{
			UE_LOG(LogTemp, Warning, TEXT("PushPullIntoRTCRoom roomID = %lld, token = %s"), roomID, UTF8_TO_TCHAR(token.c_str()));
		}

		virtual void PushAdminCommand(AdministratorCommand command, const set<int64_t>& uids)
		{
			string str = "[";
			for (auto uid : uids)
				str += " " + to_string(uid);
			str += " ]";
			UE_LOG(LogTemp, Warning, TEXT("PushAdminCommand command = %d, uids = %s"), (int32_t)command, UTF8_TO_TCHAR(str.c_str()));
		}
	};
	
	
	class RTMTest
	{
	public:
		RTMTest()
		{
			_client = nullptr;
		}

		virtual ~RTMTest() {}
		virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual void Test() = 0;

		void LoginRTM(const string& rtmEndpoint, int64_t pid, int64_t uid, const string& token)
		{
			_client = RTMClient::CreateRTMClient(pid, uid, rtmEndpoint, make_shared<RTMTestQuestProcessor>());
			bool ok = false;
			int32_t errorCode = _client->Login(ok, token);
			if (ok)
			{
				UE_LOG(LogTemp, Warning, TEXT("login success"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("login failed, errorCode = %d"), errorCode);
			}
			//UE_LOG(LogTemp, Warning, TEXT("errorCode = %d, ok = %d"), errorCode, ok);
			//bool status = _client->Login([](int64_t pid, int64_t uid, bool ok, int32_t errorCode) {
				//UE_LOG(LogTemp, Warning, TEXT("errorCode = %d, ok = %d"), errorCode, ok);
				//}, "33C43A1787B2609513EB83CEC4BBC011");

		}

		RTMClientPtr _client;
		const int64_t _peerUid = 12345678;
		const int64_t _groupId = 223344;
		const int64_t _roomId = 556677;
		const int8_t _customMType = 60;
		const string _textMessage = "Hello, RTM!";
	};

	class RTMTestThread : public FRunnable
	{
	public:
		RTMTestThread(RTMTest* tester)
		{
			_tester = tester;
			_thread = FRunnableThread::Create(this, TEXT("Test Thread"));
		}
		virtual ~RTMTestThread()
		{
			if (_thread)
			{
				_thread->Kill();
				delete _thread;
			}
		}

		bool Init()
		{
			_run = true;
			UE_LOG(LogTemp, Warning, TEXT("Init"));
			return true;
		}

		uint32 Run()
		{
			if (_tester)
				_tester->Test();
			return 0;
		}

		void Stop()
		{
			_run = false;
			UE_LOG(LogTemp, Warning, TEXT("Stop"));
		}

	private:
		RTMTest* _tester;
		FRunnableThread* _thread;
		bool _run;
	};

	
}
