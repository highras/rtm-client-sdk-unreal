#include "RTMClient.h"

namespace rtm
{
	using namespace std;
	using namespace fpnn;

	int32_t RTMClient::SendChat(int64_t& messageID, int64_t uid, const string& message, const string& attrs /* = "" */, int32_t timeout /* = 0 */)
	{
		return SendMessage(messageID, uid, TextChatMType, message, attrs, timeout);
	}

	bool RTMClient::SendChat(MessageIDCallback callback, int64_t uid, const string& message, const string& attrs /* = "" */, int32_t timeout /* = 0 */)
	{
		return SendMessage(callback, uid, TextChatMType, message, attrs, timeout);
	}

	int32_t RTMClient::SendGroupChat(int64_t& messageID, int64_t groupID, const string& message, const string& attrs /* = "" */, int32_t timeout /* = 0 */)
	{
		return SendGroupMessage(messageID, groupID, TextChatMType, message, attrs, timeout);
	}

	bool RTMClient::SendGroupChat(MessageIDCallback callback, int64_t groupID, const string& message, const string& attrs /* = "" */, int32_t timeout /* = 0 */)
	{
		return SendGroupMessage(callback, groupID, TextChatMType, message, attrs, timeout);
	}

	int32_t RTMClient::SendRoomChat(int64_t& messageID, int64_t roomID, const string& message, const string& attrs /* = "" */, int32_t timeout /* = 0 */)
	{
		return SendRoomMessage(messageID, roomID, TextChatMType, message, attrs, timeout);
	}

	bool RTMClient::SendRoomChat(MessageIDCallback callback, int64_t roomID, const string& message, const string& attrs /* = "" */, int32_t timeout /* = 0 */)
	{
		return SendGroupMessage(callback, roomID, TextChatMType, message, attrs, timeout);
	}

	int32_t RTMClient::SendCommand(int64_t& messageID, int64_t uid, const string& message, const string& attrs /* = "" */, int32_t timeout /* = 0 */)
	{
		return SendMessage(messageID, uid, CmdChatMType, message, attrs, timeout);
	}

	bool RTMClient::SendCommand(MessageIDCallback callback, int64_t uid, const string& message, const string& attrs /* = "" */, int32_t timeout /* = 0 */)
	{
		return SendMessage(callback, uid, CmdChatMType, message, attrs, timeout);
	}

	int32_t RTMClient::SendGroupCommand(int64_t& messageID, int64_t groupID, const string& message, const string& attrs /* = "" */, int32_t timeout /* = 0 */)
	{
		return SendGroupMessage(messageID, groupID, CmdChatMType, message, attrs, timeout);
	}

	bool RTMClient::SendGroupCommand(MessageIDCallback callback, int64_t groupID, const string& message, const string& attrs /* = "" */, int32_t timeout /* = 0 */)
	{
		return SendGroupMessage(callback, groupID, CmdChatMType, message, attrs, timeout);
	}

	int32_t RTMClient::SendRoomCommand(int64_t& messageID, int64_t roomID, const string& message, const string& attrs /* = "" */, int32_t timeout /* = 0 */)
	{
		return SendRoomMessage(messageID, roomID, CmdChatMType, message, attrs, timeout);
	}

	bool RTMClient::SendRoomCommand(MessageIDCallback callback, int64_t roomID, const string& message, const string& attrs /* = "" */, int32_t timeout /* = 0 */)
	{
		return SendRoomMessage(callback, roomID, CmdChatMType, message, attrs, timeout);
	}

	int32_t RTMClient::GetGroupChat(HistoryMessageResult& result, int64_t groupID, bool desc, int32_t count, int64_t beginMsec /* = 0 */, int64_t endMsec /* = 0 */, int64_t lastID /* = 0 */, int32_t timeout /* = 0 */)
	{
		return GetGroupMessage(result, groupID, desc, count, beginMsec, endMsec, lastID, _chatMessageTypes, timeout);
	}

	bool RTMClient::GetGroupChat(HistoryMessageCallback callback, int64_t groupID, bool desc, int32_t count, int64_t beginMsec /* = 0 */, int64_t endMsec /* = 0 */, int64_t lastID /* = 0 */, int32_t timeout /* = 0 */)
	{
		return GetGroupMessage(callback, groupID, desc, count, beginMsec, endMsec, lastID, _chatMessageTypes, timeout);
	}

	int32_t RTMClient::GetRoomChat(HistoryMessageResult& result, int64_t roomID, bool desc, int32_t count, int64_t beginMsec /* = 0 */, int64_t endMsec /* = 0 */, int64_t lastID /* = 0 */, int32_t timeout /* = 0 */)
	{
		return GetRoomMessage(result, roomID, desc, count, beginMsec, endMsec, lastID, _chatMessageTypes, timeout);
	}

	bool RTMClient::GetRoomChat(HistoryMessageCallback callback, int64_t roomID, bool desc, int32_t count, int64_t beginMsec /* = 0 */, int64_t endMsec /* = 0 */, int64_t lastID /* = 0 */, int32_t timeout /* = 0 */)
	{
		return GetGroupMessage(callback, lastID, desc, count, beginMsec, endMsec, lastID, _chatMessageTypes, timeout);
	}

	int32_t RTMClient::GetBroadcastChat(HistoryMessageResult& result, bool desc, int32_t count, int64_t beginMsec /* = 0 */, int64_t endMsec /* = 0 */, int64_t lastID /* = 0 */, int32_t timeout /* = 0 */)
	{
		return GetBroadcastMessage(result, desc, count, beginMsec, endMsec, lastID, _chatMessageTypes, timeout);
	}

	bool RTMClient::GetBroadcastChat(HistoryMessageCallback callback, bool desc, int32_t count, int64_t beginMsec /* = 0 */, int64_t endMsec /* = 0 */, int64_t lastID /* = 0 */, int32_t timeout /* = 0 */)
	{
		return GetBroadcastMessage(callback, desc, count, beginMsec, endMsec, lastID, _chatMessageTypes, timeout);
	}

	int32_t RTMClient::GetP2PChat(HistoryMessageResult& result, int64_t uid, bool desc, int32_t count, int64_t beginMsec /* = 0 */, int64_t endMsec /* = 0 */, int64_t lastID /* = 0 */, int32_t timeout /* = 0 */)
	{
		return GetP2PMessage(result, uid, desc, count, beginMsec, endMsec, lastID, _chatMessageTypes, timeout);
	}

	bool RTMClient::GetP2PChat(HistoryMessageCallback callback, int64_t uid, bool desc, int32_t count, int64_t beginMsec /* = 0 */, int64_t endMsec /* = 0 */, int64_t lastID /* = 0 */, int32_t timeout /* = 0 */)
	{
		return GetP2PMessage(callback, uid, desc, count, beginMsec, endMsec, lastID, _chatMessageTypes, timeout);
	}

	int32_t RTMClient::GetChat(RetrievedMessage& result, int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout /* = 0 */)
	{
		return GetMessage(result, fromUid, toId, messageID, messageCategory, timeout);
	}

	bool RTMClient::GetChat(function<void(RetrievedMessage, int32_t)> callback, int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout /* = 0 */)
	{
		return GetMessage(callback, fromUid, toId, messageID, messageCategory, timeout);
	}

	int32_t RTMClient::DeleteChat(int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout /* = 0 */)
	{
		return DeleteMessage(fromUid, toId, messageID, messageCategory, timeout);
	}

	bool RTMClient::DeleteChat(DoneCallback callback, int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout /* = 0 */)
	{
		return DeleteMessage(callback, fromUid, toId, messageID, messageCategory, timeout);
	}

	int32_t RTMClient::GetUnread(vector<int64_t>& p2pList, vector<int64_t>& groupList, bool clear /* = false */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;

		FPQWriter qw(1, "getunread");
		qw.param("clear", clear);
		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		if (answer->status())
		{
			FPAReader ar(answer);
			return ar.getInt("code");
		}

		FPAReader ar(answer);
		p2pList = ar.get("p2p", vector<int64_t>());
		groupList = ar.get("group", vector<int64_t>());

		return FPNN_EC_OK;
	}

	bool RTMClient::GetUnread(function<void(vector<int64_t>, vector<int64_t>, int32_t)> callback, bool clear /* = false */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(vector<int64_t>(), vector<int64_t>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}

		FPQWriter qw(1, "getunread");
		qw.param("clear", clear);
		bool status = client->sendQuest(qw.take(), [this, callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(vector<int64_t>(), vector<int64_t>(), FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(vector<int64_t>(), vector<int64_t>(), errorCode);
				return;
			}
			vector<int64_t> p2pList = ar.get("p2p", vector<int64_t>());
			vector<int64_t> groupList = ar.get("group", vector<int64_t>());

			callback(p2pList, groupList, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(vector<int64_t>(), vector<int64_t>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	int32_t RTMClient::ClearUnread(int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQuestPtr quest = FPQWriter::emptyQuest(("cleanunread"));
		FPAnswerPtr answer = client->sendQuest(quest, timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		if (answer->status())
		{
			FPAReader ar(answer);
			return ar.getInt("code");
		}

		return FPNN_EC_OK;
	}

	bool RTMClient::ClearUnread(DoneCallback callback, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQuestPtr quest = FPQWriter::emptyQuest("cleanunread");
		bool status = client->sendQuest(quest, [this, callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(errorCode);
				return;
			}

			callback(errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	FPQuestPtr RTMClient::GenGetP2PUnreadQuest(const set<int64_t>& uids, int64_t startTime, const set<int8_t>& messageTypes)
	{
		int32_t paramCount = 3;
		if (startTime == 0)
			paramCount -= 1;
		if (messageTypes.empty())
			paramCount -= 1;
		FPQWriter qw(paramCount, "getp2punread");
		qw.param("uids", uids);
		if (startTime != 0)
			qw.param("mtime", startTime);
		if (!messageTypes.empty())
			qw.param("mtypes", messageTypes);
		return qw.take();
	}

	int32_t RTMClient::GetP2PUnread(map<int64_t, int32_t>& unreadMap, map<int64_t, int64_t>& lastUnreadTimestampMap, const set<int64_t>& uids, int64_t startTime /* = 0 */, const set<int8_t>& messageTypes /* = set<int8_t>() */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQuestPtr quest = GenGetP2PUnreadQuest(uids, startTime, messageTypes);
		FPAnswerPtr answer = client->sendQuest(quest, timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
			return ar.getInt("code");

		map<string, int32_t> unread = ar.want("p2p", map<string, int32_t>());
		map<string, int64_t> lastTimestamp = ar.want("ltime", map<string, int64_t>());

		for (auto iter = unread.begin(); iter != unread.end(); ++iter)
			unreadMap.insert(make_pair(atol(iter->first.c_str()), iter->second));
		for (auto iter = lastTimestamp.begin(); iter != lastTimestamp.end(); ++iter)
			lastUnreadTimestampMap.insert(make_pair(atol(iter->first.c_str()), iter->second));

		return FPNN_EC_OK;
	}

	bool RTMClient::GetP2PUnread(function<void(map<int64_t, int32_t>, map<int64_t, int64_t>, int32_t)> callback, const set<int64_t>& uids, int64_t startTime /* = 0 */, const set<int8_t>& messageTypes /* = set<int8_t>() */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(map<int64_t, int32_t>(), map<int64_t, int64_t>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQuestPtr quest = GenGetP2PUnreadQuest(uids, startTime, messageTypes);
		bool status = client->sendQuest(quest, [callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(map<int64_t, int32_t>(), map<int64_t, int64_t>(), FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(map<int64_t, int32_t>(), map<int64_t, int64_t>(), errorCode);
				return;
			}
			map<int64_t, int32_t> unreadMap;
			map<int64_t, int64_t> lastUnreadTimestampMap;
			map<string, int32_t> unread = ar.want("p2p", map<string, int32_t>());
			map<string, int64_t> lastTimestamp = ar.want("ltime", map<string, int64_t>());

			for (auto iter = unread.begin(); iter != unread.end(); ++iter)
				unreadMap.insert(make_pair(atol(iter->first.c_str()), iter->second));
			for (auto iter = lastTimestamp.begin(); iter != lastTimestamp.end(); ++iter)
				lastUnreadTimestampMap.insert(make_pair(atol(iter->first.c_str()), iter->second));
			callback(unreadMap, lastUnreadTimestampMap, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(map<int64_t, int32_t>(), map<int64_t, int64_t>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	FPQuestPtr RTMClient::GenGetGroupUnreadQuest(const set<int64_t>& groupIDs, int64_t startTime, const set<int8_t>& messageTypes)
	{
		int32_t paramCount = 3;
		if (startTime == 0)
			paramCount -= 1;
		if (messageTypes.empty())
			paramCount -= 1;
		FPQWriter qw(paramCount, "getgroupunread");
		qw.param("gids", groupIDs);
		if (startTime != 0)
			qw.param("mtime", startTime);
		if (!messageTypes.empty())
			qw.param("mtypes", messageTypes);
		return qw.take();
	}

	int32_t RTMClient::GetGroupUnread(map<int64_t, int32_t>& unreadMap, map<int64_t, int64_t>& lastUnreadTimestampMap, const set<int64_t>& groupIDs, int64_t startTime /* = 0 */, const set<int8_t>& messageTypes /* = set<int8_t>() */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQuestPtr quest = GenGetGroupUnreadQuest(groupIDs, startTime, messageTypes);
		FPAnswerPtr answer = client->sendQuest(quest, timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
			return ar.getInt("code");

		map<string, int32_t> unread = ar.want("group", map<string, int32_t>());
		map<string, int64_t> lastTimestamp = ar.want("ltime", map<string, int64_t>());

		for (auto iter = unread.begin(); iter != unread.end(); ++iter)
			unreadMap.insert(make_pair(atol(iter->first.c_str()), iter->second));
		for (auto iter = lastTimestamp.begin(); iter != lastTimestamp.end(); ++iter)
			lastUnreadTimestampMap.insert(make_pair(atol(iter->first.c_str()), iter->second));

		return FPNN_EC_OK;
	}

	bool RTMClient::GetGroupUnread(function<void(map<int64_t, int32_t>, map<int64_t, int64_t>, int32_t)> callback, const set<int64_t>& groupIDs, int64_t startTime /* = 0 */, const set<int8_t>& messageTypes /* = set<int8_t>() */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(map<int64_t, int32_t>(), map<int64_t, int64_t>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQuestPtr quest = GenGetGroupUnreadQuest(groupIDs, startTime, messageTypes);
		bool status = client->sendQuest(quest, [callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(map<int64_t, int32_t>(), map<int64_t, int64_t>(), FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(map<int64_t, int32_t>(), map<int64_t, int64_t>(), errorCode);
				return;
			}
			map<int64_t, int32_t> unreadMap;
			map<int64_t, int64_t> lastUnreadTimestampMap;
			map<string, int32_t> unread = ar.want("group", map<string, int32_t>());
			map<string, int64_t> lastTimestamp = ar.want("ltime", map<string, int64_t>());

			for (auto iter = unread.begin(); iter != unread.end(); ++iter)
				unreadMap.insert(make_pair(atol(iter->first.c_str()), iter->second));
			for (auto iter = lastTimestamp.begin(); iter != lastTimestamp.end(); ++iter)
				lastUnreadTimestampMap.insert(make_pair(atol(iter->first.c_str()), iter->second));
			callback(unreadMap, lastUnreadTimestampMap, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(map<int64_t, int32_t>(), map<int64_t, int64_t>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}
	int32_t RTMClient::GetSession(set<int64_t>& p2pSet, set<int64_t>& groupSet, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQuestPtr quest = FPQWriter::emptyQuest(("getsession"));
		FPAnswerPtr answer = client->sendQuest(quest, timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}
		p2pSet = ar.want("p2p", p2pSet);
		groupSet = ar.want("group", groupSet);
		return FPNN_EC_OK;
	}

	bool RTMClient::GetSession(function<void(set<int64_t>, set<int64_t>, int32_t)> callback, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(set<int64_t>(), set<int64_t>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQuestPtr quest = FPQWriter::emptyQuest("getsession");
		bool status = client->sendQuest(quest, [this, callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(set<int64_t>(), set<int64_t>(), FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(set<int64_t>(), set<int64_t>(), errorCode);
				return;
			}
			set<int64_t> p2pSet = ar.want("p2p", set<int64_t>());
			set<int64_t> groupSet = ar.want("group", set<int64_t>());
			callback(p2pSet, groupSet, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(set<int64_t>(), set<int64_t>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	int32_t RTMClient::SetTranslatedLanguage(const string& targetLanguage, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(1, "setlang");
		qw.param("lang", targetLanguage);
		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		if (answer->status())
		{
			FPAReader ar(answer);
			return ar.getInt("code");
		}

		return FPNN_EC_OK;
	}

	bool RTMClient::SetTranslatedLanguage(DoneCallback callback, const string& targetLanguage, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(1, "setlang");
		qw.param("lang", targetLanguage);
		bool status = client->sendQuest(qw.take(), [callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(errorCode);
				return;
			}

			callback(errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	FPQuestPtr RTMClient::GenTranslateQuest(const string& text, const string& destinationLanguage, const string& sourceLanguage /* = "" */, TranslateType type /* = Chat */, ProfanityType profanity /* = Off */)
	{
		int32_t paramCount = 5;
		if (sourceLanguage.empty())
			paramCount -= 1;
		FPQWriter qw(paramCount, "translate");
		qw.param("text", text);
		qw.param("dst", destinationLanguage);
		if (!sourceLanguage.empty())
			qw.param("src", sourceLanguage);
		if (type == Mail)
			qw.param("type", "mail");
		else
			qw.param("type", "chat");
		switch (profanity)
		{
			case Censor: qw.param("profanity", "censor"); break;
			case Off: qw.param("profanity", "off"); break;
		}

		return qw.take();
	}

	int32_t RTMClient::Translate(TranslatedInfo& translatedInfo, const string& text, const string& destinationLanguage, const string& sourceLanguage /* = "" */, TranslateType type /* = Chat */, ProfanityType profanity /* = Off */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQuestPtr quest = GenTranslateQuest(text, destinationLanguage, sourceLanguage, type, profanity);
		FPAnswerPtr answer = client->sendQuest(quest, timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}

		translatedInfo.sourceLanguage = ar.wantString("source");
		translatedInfo.targetLanguage = ar.wantString("target");
		translatedInfo.sourceText = ar.wantString("sourceText");
		translatedInfo.targetText = ar.wantString("targetText");

		return FPNN_EC_OK;
	}

	bool RTMClient::Translate(function<void(TranslatedInfo, int32_t)> callback, const string& text, const string& destinationLanguage, const string& sourceLanguage /* = "" */, TranslateType type /* = Chat */, ProfanityType profanity /* = Off */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(TranslatedInfo(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQuestPtr quest = GenTranslateQuest(text, destinationLanguage, sourceLanguage, type, profanity);
		bool status = client->sendQuest(quest, [callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(TranslatedInfo(), FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(TranslatedInfo(), errorCode);
				return;
			}
			TranslatedInfo translatedInfo;
			translatedInfo.sourceLanguage = ar.wantString("source");
			translatedInfo.targetLanguage = ar.wantString("target");
			translatedInfo.sourceText = ar.wantString("sourceText");
			translatedInfo.targetText = ar.wantString("targetText");
			callback(translatedInfo, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(TranslatedInfo(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	FPQuestPtr RTMClient::GenSpeechToTextQuest(const string& audio, CheckType type, const string& language, const string& codec /* = "" */, int32_t sampleRate /* = 0 */)
	{
		int32_t paramCount = 5;
		if (codec.empty())
			paramCount -= 1;
		if (sampleRate <= 0)
			paramCount -= 1;
		FPQWriter qw(paramCount, "speech2text");
		qw.param("audio", audio);
		qw.param("type", int32_t(type));
		qw.param("lang", language);
		if (!codec.empty())
			qw.param("codec", codec);
		if (sampleRate > 0)
			qw.param("srate", sampleRate);
		return qw.take();
	}

	int32_t RTMClient::SpeechToText(string& resultText, string& resultLanguage, const string& audio, const string& language, const string& codec /* = "" */, int32_t sampleRate /* = 0 */, int32_t timeout /* = 120 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQuestPtr quest = GenSpeechToTextQuest(audio, Content, language, codec, sampleRate);
		FPAnswerPtr answer = client->sendQuest(quest, timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}

		resultText = ar.wantString("text");
		resultLanguage = ar.wantString("lang");

		return FPNN_EC_OK;
	}

	bool RTMClient::SpeechToText(function<void(string, string, int32_t)> callback, const string& audio, const string& language, const string& codec /* = "" */, int32_t sampleRate /* = 0 */, int32_t timeout /* = 120 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback("", "", FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQuestPtr quest = GenSpeechToTextQuest(audio, Content, language, codec, sampleRate);
		bool status = client->sendQuest(quest, [callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback("", "", FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback("", "", errorCode);
				return;
			}

			string resultText = ar.wantString("text");
			string resultLanguage = ar.wantString("lang");

			callback(resultText, resultLanguage, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback("", "", FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	int32_t RTMClient::TextCheck(TextCheckResult& result, const string& text, int32_t timeout /* = 120 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(1, "tcheck");
		qw.param("text", text);
		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}

		result.result = ar.wantInt("result");
		result.text = ar.getString("text");
		result.tags = ar.get("tags", result.tags);
		result.wlist = ar.get("wlist", result.wlist);

		return FPNN_EC_OK;
	}

	bool RTMClient::TextCheck(function<void(TextCheckResult, int32_t)> callback, const string& text, int32_t timeout /* = 120 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(TextCheckResult(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(1, "tcheck");
		qw.param("text", text);
		bool status = client->sendQuest(qw.take(), [callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(TextCheckResult(), FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(TextCheckResult(), errorCode);
				return;
			}

			TextCheckResult result;
			result.result = ar.wantInt("result");
			result.text = ar.getString("text");
			result.tags = ar.get("tags", result.tags);
			result.wlist = ar.get("wlist", result.wlist);

			callback(result, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(TextCheckResult(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	int32_t RTMClient::ImageCheck(CheckResult& result, const string& image, CheckType type, int32_t timeout /* = 120 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(2, "icheck");
		qw.param("image", image);
		qw.param("type", (int32_t)type);
		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}

		result.result = ar.wantInt("result");
		result.tags = ar.get("tags", result.tags);

		return FPNN_EC_OK;
	}

	bool RTMClient::ImageCheck(function<void(CheckResult, int32_t)> callback, const string& image, CheckType type, int32_t timeout /* = 120 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(CheckResult(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(2, "icheck");
		qw.param("image", image);
		qw.param("type", (int32_t)type);

		bool status = client->sendQuest(qw.take(), [callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(CheckResult(), FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(CheckResult(), errorCode);
				return;
			}

			CheckResult result;
			result.result = ar.wantInt("result");
			result.tags = ar.get("tags", result.tags);

			callback(result, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(CheckResult(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	FPQuestPtr RTMClient::GenAudioCheckQuest(const string& audio, CheckType type, const string& language, const string& codec /* = "" */, int32_t sampleRate /* = 0 */)
	{
		int32_t paramCount = 5;
		if (codec.empty())
			paramCount -= 1;
		if (sampleRate <= 0)
			paramCount -= 1;
		FPQWriter qw(paramCount, "acheck");
		qw.param("audio", audio);
		qw.param("type", (int32_t)type);
		qw.param("lang", language);
		if (!codec.empty())
			qw.param("codec", codec);
		if (sampleRate > 0)
			qw.param("srate", sampleRate);
		return qw.take();
	}

	int32_t RTMClient::AudioCheck(CheckResult& result, const string& audio, CheckType type, const string& language, const string& codec /* = "" */, int32_t sampleRate /* = 0 */, int32_t timeout /* = 120 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQuestPtr quest = GenAudioCheckQuest(audio, type, language, codec, sampleRate);
		FPAnswerPtr answer = client->sendQuest(quest, timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}

		result.result = ar.wantInt("result");
		result.tags = ar.get("tags", result.tags);

		return FPNN_EC_OK;
	}

	bool RTMClient::AudioCheck(function<void(CheckResult, int32_t)> callback, const string& audio, CheckType type, const string& language, const string& codec /* = "" */, int32_t sampleRate /* = 0 */, int32_t timeout /* = 120 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(CheckResult(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQuestPtr quest = GenAudioCheckQuest(audio, type, language, codec, sampleRate);
		bool status = client->sendQuest(quest, [callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(CheckResult(), FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(CheckResult(), errorCode);
				return;
			}

			CheckResult result;
			result.result = ar.wantInt("result");
			result.tags = ar.get("tags", result.tags);

			callback(result, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(CheckResult(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	int32_t RTMClient::VideoCheck(CheckResult& result, const string& video, CheckType type, const string& videoName, int32_t timeout /* = 120 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;

		FPQWriter qw(3, "vcheck");
		qw.param("video", video);
		qw.param("type", (int32_t)type);
		qw.param("videoName", videoName);
		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}

		result.result = ar.wantInt("result");
		result.tags = ar.get("tags", result.tags);

		return FPNN_EC_OK;
	}

	bool RTMClient::VideoCheck(function<void(CheckResult, int32_t)> callback, const string& video, CheckType type, const string& videoName, int32_t timeout /* = 120 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(CheckResult(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}

		FPQWriter qw(3, "vcheck");
		qw.param("video", video);
		qw.param("type", (int32_t)type);
		qw.param("videoName", videoName);
		bool status = client->sendQuest(qw.take(), [callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(CheckResult(), FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(CheckResult(), errorCode);
				return;
			}

			CheckResult result;
			result.result = ar.wantInt("result");
			result.tags = ar.get("tags", result.tags);

			callback(result, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(CheckResult(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}
}
