#pragma once

#include "CoreMinimal.h"
#include "TCPClient.h"
#include "RTMStructure.h"
#include "RTMClient.h"
#include "DuplicatedMessageFilter.h"
#include "IQuestProcessor.h"

namespace rtm
{
	using namespace std;
	using namespace fpnn;

	//class RTMClient;
	//typedef shared_ptr<RTMClient> RTMClientPtr;
	typedef function<void(uint64_t, bool)> ConnectCallback;
	typedef function<void(uint64_t, bool)> CloseCallback;


	class RTM_API RTMQuestProcessor
	{
	public:
		// =============== [ System ] ===============
		virtual void SessionClosed(int32_t closedByErrorCode) {}

		// Return true for starting relogin, false for stopping relogin.
		virtual bool ReloginWillStart(int32_t lastErrorCode, int32_t retriedCount) { return true; }
		virtual void ReloginCompleted(bool successful, bool retryAgain, int32_t errorCode, int32_t retriedCount) {}

		virtual void Kickout() {}
		virtual void KickoutRoom(int64_t roomID) {}

		// =============== [ Message ] ===============
		virtual void PushMessage(const RTMMessage& message) {}
		virtual void PushGroupMessage(const RTMMessage& message) {}
		virtual void PushRoomMessage(const RTMMessage& message) {}
		virtual void PushBroadcastMessage(const RTMMessage& message) {}

		// =============== [ Chat ] ===============
		virtual void PushChat(const RTMMessage& message) {}
		virtual void PushGroupChat(const RTMMessage& message) {}
		virtual void PushRoomChat(const RTMMessage& message) {}
		virtual void PushBroadcastChat(const RTMMessage& message) {}

		// =============== [ Cmd ] ===============
		virtual void PushCmd(const RTMMessage& message) {}
		virtual void PushGroupCmd(const RTMMessage& message) {}
		virtual void PushRoomCmd(const RTMMessage& message) {}
		virtual void PushBroadcastCmd(const RTMMessage& message) {}

		// =============== [ Files ] ===============
		virtual void PushFile(const RTMMessage& message) {}
		virtual void PushGroupFile(const RTMMessage& message) {}
		virtual void PushRoomFile(const RTMMessage& message) {}
		virtual void PushBroadcastFile(const RTMMessage& message) {}

		// =============== [ RTC ] ===============
		virtual void PushEnterRTCRoom(int64_t roomID, int64_t uid, int64_t messageTime) {}
		virtual void PushExitRTCRoom(int64_t roomID, int64_t uid, int64_t messageTime) {}
		virtual void PushRTCRoomClosed(int64_t roomID) {}
		virtual void PushInviteIntoRTCRoom(int64_t fromUid, int64_t roomID) {}
		virtual void PushKickOutRTCRoom(int64_t fromUid, int64_t roomID) {}
		virtual void PushPullIntoRTCRoom(int64_t roomID, const string& token) {}
		virtual void PushAdminCommand(AdministratorCommand command, const set<int64_t>& uids) {}
		virtual void PushVoice(int64_t uid, int64_t roomID, int64_t seq, int64_t time, const string& data) {}
		virtual void PushVideo(int64_t uid, int64_t roomID, int64_t seq, int64_t flags, int64_t time, int64_t rotation, int64_t version, int32_t captureLevel, const string& data, const string& sps, const string& pps) {}
	};

	typedef shared_ptr<RTMQuestProcessor> RTMQuestProcessorPtr;

	typedef function<FPAnswerPtr(const FPReaderPtr, const FPQuestPtr, const ConnectionInfo&)> QuestCallback;

	class RTMProcessor : public IQuestProcessor
	{
		QuestProcessorClassPrivateFields(RTMProcessor)
	public:
		RTMProcessor(ConnectCallback connectCallback, CloseCallback closeCallback);
		virtual ~RTMProcessor();
		virtual void connected(const ConnectionInfo& connInfo, bool connected);
		virtual void connectionWillClose(const ConnectionInfo& connInfo, bool closeByError);
		void ClearClient();

		void SessionClosed(int32_t closedByErrorCode);
		bool ReloginWillStart(int32_t lastErrorCode, int32_t retriedCount);
		void ReloginCompleted(bool successful, bool retryAgain, int32_t errorCode, int32_t retriedCount);

		void SetQuestProcessor(RTMQuestProcessorPtr questProcessor);
		bool IsAlive();
		void UpdatePingTime();
		void ClearPingTime();
		QuestProcessorClassBasicPublicFuncs
	private:
		FPAnswerPtr Ping(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci);
		FPAnswerPtr Kickout(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci);
		FPAnswerPtr KickoutRoom(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci);

		FPAnswerPtr PushMessage(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci);
		FPAnswerPtr PushGroupMessage(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci);
		FPAnswerPtr PushRoomMessage(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci);
		FPAnswerPtr PushBroadcastMessage(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci);

		FPAnswerPtr PushEnterRTCRoom(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci);
		FPAnswerPtr PushExitRTCRoom(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci);
		FPAnswerPtr PushRTCRoomClosed(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci);
		FPAnswerPtr PushInviteIntoRTCRoom(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci);
		FPAnswerPtr PushKickoutRTCRoom(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci);
		FPAnswerPtr PushPullIntoRTCRoom(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci);
		FPAnswerPtr PushAdminCommand(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci);

		void BuildRTMMessage(RTMMessage& message, const FPReaderPtr args, int64_t from, int64_t to, int64_t mid);
		void BuildTranslateInfo(shared_ptr<TranslatedInfo> info, const FPReaderPtr args);
	private:
		DuplicatedMessageFilter* _duplicatedMessageFilter;
		atomic<int64_t> _lastPingTime;
		RTMQuestProcessorPtr _rtmQuestProcessor;
		//RTMClientPtr _client;
		ConnectCallback _connectCallback;
		CloseCallback _closeCallback;
	};

	class RTCProcessor : public IQuestProcessor
	{
		QuestProcessorClassPrivateFields(RTCProcessor)
	public:
		RTCProcessor();
		~RTCProcessor();

		QuestProcessorClassBasicPublicFuncs
	private:
		FPAnswerPtr Ping(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci);
		FPAnswerPtr PushVoice(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci);
		FPAnswerPtr PushVideo(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci);
	private:
		//atomic<int64_t> _lastPingTime;
		//RTMQuestProcessorPtr _rtmQuestProcessor;
	};

	class FileProcessor : public IQuestProcessor
	{
		QuestProcessorClassPrivateFields(FileProcessor)
	public:
		enum class ConnectStatus
		{
			IPV4,
			IPV6,
			Connected,
		};
		typedef function<void(const ConnectionInfo&, bool connected)> ConnectCallback;
		FileProcessor(TCPClientPtr client, ConnectCallback callback);
		virtual ~FileProcessor();
		virtual void connected(const ConnectionInfo& connInfo, bool connected);
		virtual void connectionWillClose(const ConnectionInfo& connInfo, bool closeByError);

		QuestProcessorClassBasicPublicFuncs
	private:
		ConnectStatus _connectStatus;
		TCPClientPtr _client;
		ConnectCallback _callback;
	};
}

