#include "RTMQuestProcessor.h"
#include "RTMClient.h"
#include "RTMControlCenter.h"
#include "RTMConfig.h"

namespace rtm
{
	using namespace std;
	using namespace fpnn;


	RTMProcessor::RTMProcessor(ConnectCallback connectCallback, CloseCallback closeCallback)
	{
		_duplicatedMessageFilter = new DuplicatedMessageFilter();
		_lastPingTime = 0;
		//_client = client;
		_connectCallback = connectCallback;
		_closeCallback = closeCallback;

		registerMethod("ping", &RTMProcessor::Ping);
		registerMethod("kickout", &RTMProcessor::Kickout);
		registerMethod("kickoutroom", &RTMProcessor::KickoutRoom);
		registerMethod("pushmsg", &RTMProcessor::PushMessage);
		registerMethod("pushgroupmsg", &RTMProcessor::PushGroupMessage);
		registerMethod("pushroommsg", &RTMProcessor::PushRoomMessage);
		registerMethod("pushbroadcastmsg", &RTMProcessor::PushBroadcastMessage);
		registerMethod("pushEnterRTCRoom", &RTMProcessor::PushEnterRTCRoom);
		registerMethod("pushExitRTCRoom", &RTMProcessor::PushExitRTCRoom);
		registerMethod("pushRTCRoomClosed", &RTMProcessor::PushRTCRoomClosed);
		registerMethod("pushInviteIntoRTCRoom", &RTMProcessor::PushInviteIntoRTCRoom);
		registerMethod("pushKickOutRTCRoom", &RTMProcessor::PushKickoutRTCRoom);
		registerMethod("pushPullIntoRTCRoom", &RTMProcessor::PushPullIntoRTCRoom);
		registerMethod("pushAdminCommand", &RTMProcessor::PushAdminCommand);
	}

	RTMProcessor::~RTMProcessor()
	{
		if (_duplicatedMessageFilter != nullptr)
		{
			delete _duplicatedMessageFilter;
			_duplicatedMessageFilter = nullptr;
		}
		_rtmQuestProcessor = nullptr;
		//_client = nullptr;
	}

	void RTMProcessor::ClearClient()
	{
		//_client = nullptr;
	}

	void RTMProcessor::connected(const ConnectionInfo& connInfo, bool connected)
	{
		//if (_client == nullptr)
		//	return;
		//_client->Connected(connInfo.uniqueId(), connected);
		if (_connectCallback)
			_connectCallback(connInfo.uniqueId(), connected);
	}

	void RTMProcessor::connectionWillClose(const ConnectionInfo& connInfo, bool closeByError)
	{
		//if (_client == nullptr)
		//	return;
		//_client->Closed(connInfo.uniqueId(), closeByError);
		if (_closeCallback)
			_closeCallback(connInfo.uniqueId(), closeByError);
	}

	void RTMProcessor::SessionClosed(int32_t closedByErrorCode)
	{
		if (_rtmQuestProcessor == nullptr)
			return;
		_rtmQuestProcessor->SessionClosed(closedByErrorCode);
	}

	bool RTMProcessor::ReloginWillStart(int32_t lastErrorCode, int32_t retriedCount)
	{
		bool startRelogin = true;
		if (_rtmQuestProcessor != nullptr)
			startRelogin = _rtmQuestProcessor->ReloginWillStart(lastErrorCode, retriedCount);
		return startRelogin;
	}

	void RTMProcessor::ReloginCompleted(bool successful, bool retryAgain, int32_t errorCode, int32_t retriedCount)
	{
		if (_rtmQuestProcessor != nullptr)
			_rtmQuestProcessor->ReloginCompleted(successful, retryAgain, errorCode, retriedCount);
	}

	void RTMProcessor::SetQuestProcessor(RTMQuestProcessorPtr questProcessor)
	{
		_rtmQuestProcessor = questProcessor;
	}

	bool RTMProcessor::IsAlive()
	{
		if (_lastPingTime == 0)
			return true;
		if (TimeUtil::curr_sec() - _lastPingTime < RTMConfig::lostConnectionAfterLastPingInSeconds)
			return true;
		return false;
	}

	FPAnswerPtr RTMProcessor::Ping(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci)
	{
		sendAnswer(quest, FPAWriter::emptyAnswer(quest));
		UpdatePingTime();
		return nullptr;
	}

	FPAnswerPtr RTMProcessor::Kickout(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci)
	{
		bool closed = RTMControlCenter::GetClientStatus(ci.uniqueId()) == ClientStatus::Closed;
		RTMControlCenter::CloseSession(ci.uniqueId());

		if (_rtmQuestProcessor && closed == false)
			_rtmQuestProcessor->Kickout();

		return nullptr;
	}

	FPAnswerPtr RTMProcessor::KickoutRoom(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci)
	{
		if (_rtmQuestProcessor == nullptr)
			return nullptr;
		int64_t roomID = args->wantInt("rid");
		_rtmQuestProcessor->KickoutRoom(roomID);
		return nullptr;
	}

	FPAnswerPtr RTMProcessor::PushMessage(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci)
	{
		sendAnswer(quest, FPAWriter::emptyAnswer(quest));

		if (_rtmQuestProcessor == nullptr)
			return nullptr;

		int64_t from = args->wantInt("from");
		int64_t to = args->wantInt("to");
		int64_t mid = args->wantInt("mid");

		if (_duplicatedMessageFilter == nullptr)
			return nullptr;

		if (_duplicatedMessageFilter->CheckP2PMessage(from, mid) == false)
			return nullptr;

		RTMMessage message;
		BuildRTMMessage(message, args, from, to, mid);
		if (message.messageType == TextChatMType)
		{
			if (message.translatedInfo != nullptr)
				_rtmQuestProcessor->PushChat(message);
		}
		else if (message.messageType == CmdChatMType)
		{
			_rtmQuestProcessor->PushCmd(message);
		}
		else if (RTMClient::CheckFileMessageType(message.messageType))
		{
			_rtmQuestProcessor->PushFile(message);
		}
		else
		{
			_rtmQuestProcessor->PushMessage(message);
		}

		return nullptr;
	}

	FPAnswerPtr RTMProcessor::PushGroupMessage(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci)
	{
		sendAnswer(quest, FPAWriter::emptyAnswer(quest));

		if (_rtmQuestProcessor == nullptr)
			return nullptr;

		int64_t groupID = args->wantInt("gid");
		int64_t from = args->wantInt("from");
		int64_t mid = args->wantInt("mid");

		if (_duplicatedMessageFilter == nullptr)
			return nullptr;

		if (_duplicatedMessageFilter->CheckGroupMessage(from, groupID, mid) == false)
			return nullptr;

		RTMMessage message;
		BuildRTMMessage(message, args, from, groupID, mid);
		if (message.messageType == TextChatMType)
		{
			if (message.translatedInfo != nullptr)
				_rtmQuestProcessor->PushGroupChat(message);
		}
		else if (message.messageType == CmdChatMType)
		{
			_rtmQuestProcessor->PushGroupCmd(message);
		}
		else if (RTMClient::CheckFileMessageType(message.messageType))
		{
			_rtmQuestProcessor->PushGroupFile(message);
		}
		else
		{
			_rtmQuestProcessor->PushGroupMessage(message);
		}

		return nullptr;
	}

	FPAnswerPtr RTMProcessor::PushRoomMessage(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci)
	{
		sendAnswer(quest, FPAWriter::emptyAnswer(quest));

		if (_rtmQuestProcessor == nullptr)
			return nullptr;

		int64_t roomID = args->wantInt("rid");
		int64_t from = args->wantInt("from");
		int64_t mid = args->wantInt("mid");

		if (_duplicatedMessageFilter == nullptr)
			return nullptr;

		if (_duplicatedMessageFilter->CheckRoomMessage(from, roomID, mid) == false)
			return nullptr;

		RTMMessage message;
		BuildRTMMessage(message, args, from, roomID, mid);
		if (message.messageType == TextChatMType)
		{
			if (message.translatedInfo != nullptr)
				_rtmQuestProcessor->PushRoomChat(message);
		}
		else if (message.messageType == CmdChatMType)
		{
			_rtmQuestProcessor->PushRoomCmd(message);
		}
		else if (RTMClient::CheckFileMessageType(message.messageType))
		{
			_rtmQuestProcessor->PushRoomFile(message);
		}
		else
		{
			_rtmQuestProcessor->PushRoomMessage(message);
		}

		return nullptr;
	}

	FPAnswerPtr RTMProcessor::PushBroadcastMessage(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci)
	{
		sendAnswer(quest, FPAWriter::emptyAnswer(quest));

		if (_rtmQuestProcessor == nullptr)
			return nullptr;

		int64_t from = args->wantInt("from");
		int64_t mid = args->wantInt("mid");

		if (_duplicatedMessageFilter == nullptr)
			return nullptr;

		if (_duplicatedMessageFilter->CheckBroadcastMessage(from, mid) == false)
			return nullptr;

		RTMMessage message;
		BuildRTMMessage(message, args, from, 0, mid);
		if (message.messageType == TextChatMType)
		{
			if (message.translatedInfo != nullptr)
				_rtmQuestProcessor->PushBroadcastChat(message);
		}
		else if (message.messageType == CmdChatMType)
		{
			_rtmQuestProcessor->PushBroadcastCmd(message);
		}
		else if (RTMClient::CheckFileMessageType(message.messageType))
		{
			_rtmQuestProcessor->PushBroadcastFile(message);
		}
		else
		{
			_rtmQuestProcessor->PushBroadcastMessage(message);
		}

		return nullptr;
	}

	FPAnswerPtr RTMProcessor::PushEnterRTCRoom(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci)
	{
		sendAnswer(quest, FPAWriter::emptyAnswer(quest));
		if (_rtmQuestProcessor == nullptr)
			return nullptr;
		int64_t roomID = args->wantInt("rid");
		int64_t uid = args->wantInt("uid");
		int64_t mtime = args->wantInt("mtime");

		_rtmQuestProcessor->PushEnterRTCRoom(roomID, uid, mtime);
		return nullptr;
	}

	FPAnswerPtr RTMProcessor::PushExitRTCRoom(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci)
	{
		sendAnswer(quest, FPAWriter::emptyAnswer(quest));
		if (_rtmQuestProcessor == nullptr)
			return nullptr;
		int64_t roomID = args->wantInt("rid");
		int64_t uid = args->wantInt("uid");
		int64_t mtime = args->wantInt("mtime");

		_rtmQuestProcessor->PushExitRTCRoom(roomID, uid, mtime);
		return nullptr;
	}

	FPAnswerPtr RTMProcessor::PushRTCRoomClosed(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci)
	{
		sendAnswer(quest, FPAWriter::emptyAnswer(quest));
		if (_rtmQuestProcessor == nullptr)
			return nullptr;

		int64_t roomID = args->wantInt("rid");
		_rtmQuestProcessor->PushRTCRoomClosed(roomID);
		return nullptr;
	}

	FPAnswerPtr RTMProcessor::PushInviteIntoRTCRoom(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci)
	{
		sendAnswer(quest, FPAWriter::emptyAnswer(quest));
		if (_rtmQuestProcessor == nullptr)
			return nullptr;

		int64_t fromUid = args->wantInt("fromUid");
		int64_t roomID = args->wantInt("rid");

		_rtmQuestProcessor->PushInviteIntoRTCRoom(fromUid, roomID);
		return nullptr;
	}

	FPAnswerPtr RTMProcessor::PushKickoutRTCRoom(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci)
	{
		sendAnswer(quest, FPAWriter::emptyAnswer(quest));
		if (_rtmQuestProcessor == nullptr)
			return nullptr;

		int64_t fromUid = args->wantInt("fromUid");
		int64_t roomID = args->wantInt("rid");

		_rtmQuestProcessor->PushKickOutRTCRoom(fromUid, roomID);
		return nullptr;
	}

	FPAnswerPtr RTMProcessor::PushPullIntoRTCRoom(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci)
	{
		sendAnswer(quest, FPAWriter::emptyAnswer(quest));
		if (_rtmQuestProcessor == nullptr)
			return nullptr;

		int64_t roomID = args->wantInt("rid");
		string token = args->wantString("token");

		RTMClientPtr client = RTMControlCenter::GetClient(ci.uniqueId());
		if (client != nullptr)
		{
			// todo
		}

		_rtmQuestProcessor->PushPullIntoRTCRoom(roomID, token);
		return nullptr;
	}

	FPAnswerPtr RTMProcessor::PushAdminCommand(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci)
	{
		sendAnswer(quest, FPAWriter::emptyAnswer(quest));
		if (_rtmQuestProcessor == nullptr)
			return nullptr;

		int32_t command = args->wantInt("command");
		set<int64_t> uids = args->want("uids", set<int64_t>());

		_rtmQuestProcessor->PushAdminCommand((AdministratorCommand)command, uids);
		return nullptr;
	}

	void RTMProcessor::BuildRTMMessage(RTMMessage& message, const FPReaderPtr args, int64_t from, int64_t to, int64_t mid)
	{
		message.fromUid = from;
		message.toId = to;
		message.messageId = mid;
		message.messageType = args->wantInt("mtype");
		message.attrs = args->wantString("attrs");
		message.modifiedTime = args->wantInt("mtime");

		if (message.messageType == TextChatMType)
		{
			BuildTranslateInfo(message.translatedInfo, args);
			if (message.translatedInfo != nullptr)
			{
				if (message.translatedInfo->targetText.empty())
					message.message = message.translatedInfo->sourceText;
				else
					message.message = message.translatedInfo->targetText;
			}
		}
		else if (message.messageType == CmdChatMType)
		{
			message.message = args->wantString("msg");
		}
		else if (RTMClient::CheckFileMessageType(message.messageType))
		{
			message.message = args->wantString("msg");
			RTMClient::BuildFileInfo(message);
		}
		else
		{
			message.message = args->wantString("msg");
		}
	}

	void RTMProcessor::BuildTranslateInfo(shared_ptr<TranslatedInfo> info, const FPReaderPtr args)
	{
		string msg = args->wantString("msg");
		rapidjson::Document document;
		if (document.Parse(msg.c_str()).HasParseError())
			return;
		if (document.IsObject() == false)
			return;
		if (document.HasMember("source") && document["source"].IsString())
			info->sourceLanguage = document["source"].GetString();
		else
			info->sourceLanguage = "";
		if (document.HasMember("target") && document["target"].IsString())
			info->targetLanguage = document["target"].GetString();
		else
			info->targetLanguage = "";
		if (document.HasMember("sourceText") && document["sourceText"].IsString())
			info->sourceText = document["sourceText"].GetString();
		else
			info->sourceText = "";
		if (document.HasMember("targetText") && document["targetText"].IsString())
			info->targetText = document["targetText"].GetString();
		else
			info->targetText = "";
	}

	void RTMProcessor::UpdatePingTime()
	{
		_lastPingTime = TimeUtil::curr_sec();
	}

	void RTMProcessor::ClearPingTime()
	{
		_lastPingTime = 0;
	}

	FileProcessor::FileProcessor(TCPClientPtr client, ConnectCallback callback)
	{
		_connectStatus = ConnectStatus::IPV4;
		_client = client;
		_callback = callback;
	}

	FileProcessor::~FileProcessor()
	{

	}

	void FileProcessor::connected(const ConnectionInfo& connInfo, bool connected)
	{
		_callback(connInfo, connected);
		//if (_connectStatus == ConnectStatus::Connected)
		//	return;
		//if (connected)
		//{
		//	RTMControlCenter::ActiveFileGate(connInfo.endpoint(), _client);
		//	_connectStatus = ConnectStatus::Connected;
		//	return;
		//}

		//if (_connectStatus == ConnectStatus::IPV6)
		//{
		//	_callback(FPNN_EC_CORE_INVALID_CONNECTION);
		//	return;
		//}

		//// switch ipv4 to ipv6
		//string ip;
		//int32_t port = 0;
		//if (!RTMClient::ipv4EndpointToipv6(connInfo.endpoint(), ip, port))
		//{
		//	_callback(FPNN_EC_CORE_INVALID_CONNECTION);
		//	return;
		//}
		//_client = TCPClient::createTCPClient(ip, port);
		//_client->setQuestProcessor(std::make_shared<IQuestProcessor>(this));
		//_connectStatus = ConnectStatus::IPV6;
	}

	void FileProcessor::connectionWillClose(const ConnectionInfo& connInfo, bool closeByError)
	{

	}


	RTCProcessor::RTCProcessor()
	{
		//_lastPingTime = 0;

		registerMethod("ping", &RTCProcessor::Ping);
		registerMethod("pushVoice", &RTCProcessor::PushVoice);
		registerMethod("pushVideo", &RTCProcessor::PushVideo);
	}

	RTCProcessor::~RTCProcessor()
	{

	}

	FPAnswerPtr RTCProcessor::Ping(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci)
	{
		sendAnswer(FPAWriter::emptyAnswer(quest));
		return nullptr;
	}

	FPAnswerPtr RTCProcessor::PushVoice(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci)
	{
		int64_t uid = args->wantInt("uid");
		int64_t roomID = args->wantInt("rid");
		int64_t seq = args->wantInt("seq");
		int64_t timestamp = args->wantInt("timestamp");
		vector<char> data = args->want("data", vector<char>());

		RTMClientPtr client = RTMControlCenter::GetClient(ci.uniqueId());
		//todo
		return nullptr;
	}

	FPAnswerPtr RTCProcessor::PushVideo(const FPReaderPtr args, const FPQuestPtr quest, const ConnectionInfo& ci)
	{
		return nullptr;
	}
}