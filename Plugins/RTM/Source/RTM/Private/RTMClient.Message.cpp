#include "RTMClient.h"

namespace rtm
{

	FPQuestPtr RTMClient::GenSendMessageQuest(int64_t uid, int8_t messageType, const string& message, const string& attrs, int64_t& messageID)
	{
		messageID = RTMMidGenerator::Gen();

		FPQWriter qw(5, "sendmsg");
		qw.param("to", uid);
		qw.param("mid", messageID);
		qw.param("mtype", messageType);
		qw.param("msg", message);
		qw.param("attrs", attrs);
		return qw.take();
	}

	int32_t RTMClient::SendMessage(int64_t& messageID, int64_t uid, int8_t messageType, const string& message, const string& attrs /*= ""*/, int32_t timeout /*= 0*/)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQuestPtr quest = GenSendMessageQuest(uid, messageType, message, attrs, messageID);
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

	bool RTMClient::SendMessage(function<void(int64_t, int)> callback, int64_t uid, int8_t messageType, const string& message, const string& attrs /*= ""*/, int32_t timeout/* = 0*/)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(0, FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		int64_t messageID = 0;
		FPQuestPtr quest = GenSendMessageQuest(uid, messageType, message, attrs, messageID);
		bool status = client->sendQuest(quest, [messageID, callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(0, FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(0, errorCode);
				return;
			}
			callback(messageID, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(0, FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	FPQuestPtr RTMClient::GenSendGroupMessageQuest(int64_t groupID, int8_t messageType, const string& message, const string& attrs, int64_t& messageID)
	{
		messageID = RTMMidGenerator::Gen();

		FPQWriter qw(5, "sendgroupmsg");
		qw.param("gid", groupID);
		qw.param("mid", messageID);
		qw.param("mtype", messageType);
		qw.param("msg", message);
		qw.param("attrs", attrs);
		return qw.take();
	}

	int32_t RTMClient::SendGroupMessage(int64_t& messageID, int64_t groupID, int8_t messageType, const string& message, const string& attrs /* = "" */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQuestPtr quest = GenSendGroupMessageQuest(groupID, messageType, message, attrs, messageID);
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

	bool RTMClient::SendGroupMessage(MessageIDCallback callback, int64_t groupID, int8_t messageType, const string& message, const string& attrs /* = "" */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(0, FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		int64_t messageID = 0;
		FPQuestPtr quest = GenSendGroupMessageQuest(groupID, messageType, message, attrs, messageID);
		bool status = client->sendQuest(quest, [messageID, callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(0, FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(0, errorCode);
				return;
			}
			callback(messageID, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(0, FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	FPQuestPtr RTMClient::GenSendRoomMessageQuest(int64_t roomID, int8_t messageType, const string& message, const string& attrs, int64_t& messageID)
	{
		messageID = RTMMidGenerator::Gen();

		FPQWriter qw(5, "sendroommsg");
		qw.param("rid", roomID);
		qw.param("mid", messageID);
		qw.param("mtype", messageType);
		qw.param("msg", message);
		qw.param("attrs", attrs);
		return qw.take();
	}

	int32_t RTMClient::SendRoomMessage(int64_t& messageID, int64_t roomID, int8_t messageType, const string& message, const string& attrs /* = "" */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQuestPtr quest = GenSendRoomMessageQuest(roomID, messageType, message, attrs, messageID);
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

	bool RTMClient::SendRoomMessage(MessageIDCallback callback, int64_t roomID, int8_t messageType, const string& message, const string& attrs /* = "" */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(0, FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		int64_t messageID = 0;
		FPQuestPtr quest = GenSendRoomMessageQuest(roomID, messageType, message, attrs, messageID);
		bool status = client->sendQuest(quest, [messageID, callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(0, FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(0, errorCode);
				return;
			}
			callback(messageID, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(0, FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	void RTMClient::GetHistoryMessageResult(HistoryMessageResult& result, FPAnswerPtr answer, int64_t toID)
	{
		FPAReader ar(answer);
		result.count = ar.wantInt("num");
		result.lastCursorId = ar.wantInt("lastid");
		result.beginMsec = ar.wantInt("begin");
		result.endMsec = ar.wantInt("end");
		vector<CommonMessage> messages = ar.want("msgs", vector<CommonMessage>());
		for (auto iter = messages.begin(); iter != messages.end(); ++iter)
		{
			if (iter->deleted)
				continue;
			HistoryMessage message;
			message.cursorId = iter->id;
			message.fromUid = iter->from;
			message.toId = toID;
			message.messageType = iter->mtype;
			message.messageId = iter->mid;
			message.message = iter->msg;
			message.attrs = iter->attrs;
			message.modifiedTime = iter->mtime;

			if (CheckFileMessageType(message.messageType))
			{
				BuildFileInfo(message);
			}

			result.messages.push_back(move(message));
		}
		result.count = (int32_t)result.messages.size();
	}

	void RTMClient::AdjustHistoryMessageResultForP2PMessage(int64_t selfUid, int64_t peerUid, HistoryMessageResult& result)
	{
		for (auto iter = result.messages.begin(); iter != result.messages.end(); ++iter)
		{
			if (iter->fromUid == 1)
			{
				iter->fromUid = selfUid;
				iter->toId = peerUid;
			}
			else if (iter->fromUid == 2)
			{
				iter->fromUid = peerUid;
				iter->toId = selfUid;
			}
		}
	}

	FPQuestPtr RTMClient::GenGetGroupMessageQuest(int64_t groupID, bool desc, int32_t count, int64_t beginMsec, int64_t endMsec, int64_t lastID, vector<int8_t> messageTypes)
	{
		int paramCount = 6;
		if (!messageTypes.empty())
			paramCount += 1;
		FPQWriter qw(paramCount, "getgroupmsg");
		qw.param("gid", groupID);
		qw.param("desc", desc);
		qw.param("num", count);
		qw.param("begin", beginMsec);
		qw.param("end", endMsec);
		qw.param("lastid", lastID);
		if (!messageTypes.empty())
			qw.param("mtypes", messageTypes);
		return qw.take();
	}

	int32_t RTMClient::GetGroupMessage(HistoryMessageResult& result, int64_t groupID, bool desc, int32_t count, int64_t beginMsec /* = 0 */, int64_t endMsec /* = 0 */, int64_t lastID /* = 0 */, const vector<int8_t>& messageTypes /* = vector<int8_t>() */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQuestPtr quest = GenGetGroupMessageQuest(groupID, desc, count, beginMsec, endMsec, lastID, messageTypes);
		FPAnswerPtr answer = client->sendQuest(quest, timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		if (answer->status())
		{
			FPAReader ar(answer);
			return ar.getInt("code");
		}

		GetHistoryMessageResult(result, answer, groupID);

		return FPNN_EC_OK;
	}

	bool RTMClient::GetGroupMessage(HistoryMessageCallback callback, int64_t groupID, bool desc, int32_t count, int64_t beginMsec /* = 0 */, int64_t endMsec /* = 0 */, int64_t lastID /* = 0 */, const vector<int8_t>& messageTypes /* = vector<int8_t>() */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(HistoryMessageResult(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		int64_t messageID = 0;
		FPQuestPtr quest = GenGetGroupMessageQuest(groupID, desc, count, beginMsec, endMsec, lastID, messageTypes);
		bool status = client->sendQuest(quest, [this, groupID, messageID, callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(HistoryMessageResult(), FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(HistoryMessageResult(), errorCode);
				return;
			}

			HistoryMessageResult result;
			GetHistoryMessageResult(result, answer, groupID);
			callback(result, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(HistoryMessageResult(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	FPQuestPtr RTMClient::GenGetRoomMessageQuest(int64_t roomID, bool desc, int32_t count, int64_t beginMsec, int64_t endMsec, int64_t lastID, vector<int8_t> messageTypes)
	{
		int paramCount = 6;
		if (!messageTypes.empty())
			paramCount += 1;
		FPQWriter qw(paramCount, "getroommsg");
		qw.param("rid", roomID);
		qw.param("desc", desc);
		qw.param("num", count);
		qw.param("begin", beginMsec);
		qw.param("end", endMsec);
		qw.param("lastid", lastID);
		if (!messageTypes.empty())
			qw.param("mtypes", messageTypes);
		return qw.take();
	}

	int32_t RTMClient::GetRoomMessage(HistoryMessageResult& result, int64_t roomID, bool desc, int32_t count, int64_t beginMsec /* = 0 */, int64_t endMsec /* = 0 */, int64_t lastID /* = 0 */, const vector<int8_t>& messageTypes /* = vector<int8_t>() */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQuestPtr quest = GenGetRoomMessageQuest(roomID, desc, count, beginMsec, endMsec, lastID, messageTypes);
		FPAnswerPtr answer = client->sendQuest(quest, timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		if (answer->status())
		{
			FPAReader ar(answer);
			return ar.getInt("code");
		}

		GetHistoryMessageResult(result, answer, roomID);
		return FPNN_EC_OK;
	}

	bool RTMClient::GetRoomMessage(HistoryMessageCallback callback, int64_t roomID, bool desc, int32_t count, int64_t beginMsec /* = 0 */, int64_t endMsec /* = 0 */, int64_t lastID /* = 0 */, const vector<int8_t>& messageTypes /* = vector<int8_t>() */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(HistoryMessageResult(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		int64_t messageID = 0;
		FPQuestPtr quest = GenGetRoomMessageQuest(roomID, desc, count, beginMsec, endMsec, lastID, messageTypes);
		bool status = client->sendQuest(quest, [this, roomID, messageID, callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(HistoryMessageResult(), FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(HistoryMessageResult(), errorCode);
				return;
			}

			HistoryMessageResult result;
			GetHistoryMessageResult(result, answer, roomID);
			callback(result, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(HistoryMessageResult(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	FPQuestPtr RTMClient::GenGetBroadcastMessageQuest(bool desc, int32_t count, int64_t beginMsec, int64_t endMsec, int64_t lastID, vector<int8_t> messageTypes)
	{
		int paramCount = 5;
		if (!messageTypes.empty())
			paramCount += 1;
		FPQWriter qw(paramCount, "getbroadcastmsg");
		qw.param("desc", desc);
		qw.param("num", count);
		qw.param("begin", beginMsec);
		qw.param("end", endMsec);
		qw.param("lastid", lastID);
		if (!messageTypes.empty())
			qw.param("mtypes", messageTypes);
		return qw.take();
	}

	int32_t RTMClient::GetBroadcastMessage(HistoryMessageResult& result, bool desc, int32_t count, int64_t beginMsec /* = 0 */, int64_t endMsec /* = 0 */, int64_t lastID /* = 0 */, const vector<int8_t>& messageTypes /* = vector<int8_t>() */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQuestPtr quest = GenGetBroadcastMessageQuest(desc, count, beginMsec, endMsec, lastID, messageTypes);
		FPAnswerPtr answer = client->sendQuest(quest, timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		if (answer->status())
		{
			FPAReader ar(answer);
			return ar.getInt("code");
		}

		GetHistoryMessageResult(result, answer, 0);
		return FPNN_EC_OK;
	}

	bool RTMClient::GetBroadcastMessage(HistoryMessageCallback callback, bool desc, int32_t count, int64_t beginMsec /* = 0 */, int64_t endMsec /* = 0 */, int64_t lastID /* = 0 */, const vector<int8_t>& messageTypes /* = vector<int8_t>() */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(HistoryMessageResult(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		int64_t messageID = 0;
		FPQuestPtr quest = GenGetBroadcastMessageQuest(desc, count, beginMsec, endMsec, lastID, messageTypes);
		bool status = client->sendQuest(quest, [this, messageID, callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(HistoryMessageResult(), FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(HistoryMessageResult(), errorCode);
				return;
			}

			HistoryMessageResult result;
			GetHistoryMessageResult(result, answer, 0);
			callback(result, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(HistoryMessageResult(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	FPQuestPtr RTMClient::GenGetP2PMessageQuest(int64_t uid, bool desc, int32_t count, int64_t beginMsec, int64_t endMsec, int64_t lastID, vector<int8_t> messageTypes)
	{
		int paramCount = 6;
		if (!messageTypes.empty())
			paramCount += 1;
		FPQWriter qw(paramCount, "getp2pmsg");
		qw.param("ouid", uid);
		qw.param("desc", desc);
		qw.param("num", count);
		qw.param("begin", beginMsec);
		qw.param("end", endMsec);
		qw.param("lastid", lastID);
		if (!messageTypes.empty())
			qw.param("mtypes", messageTypes);
		return qw.take();
	}

	int32_t RTMClient::GetP2PMessage(HistoryMessageResult& result, int64_t uid, bool desc, int32_t count, int64_t beginMsec /* = 0 */, int64_t endMsec /* = 0 */, int64_t lastID /* = 0 */, const vector<int8_t>& messageTypes /* = vector<int8_t>() */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQuestPtr quest = GenGetP2PMessageQuest(uid, desc, count, beginMsec, endMsec, lastID, messageTypes);
		FPAnswerPtr answer = client->sendQuest(quest, timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		if (answer->status())
		{
			FPAReader ar(answer);
			return ar.getInt("code");
		}

		GetHistoryMessageResult(result, answer, 0);
		AdjustHistoryMessageResultForP2PMessage(_uid, uid, result);
		return FPNN_EC_OK;
	}

	bool RTMClient::GetP2PMessage(HistoryMessageCallback callback, int64_t uid, bool desc, int32_t count, int64_t beginMsec /* = 0 */, int64_t endMsec /* = 0 */, int64_t lastID /* = 0 */, const vector<int8_t>& messageTypes /* = vector<int8_t>() */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(HistoryMessageResult(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		int64_t messageID = 0;
		FPQuestPtr quest = GenGetP2PMessageQuest(uid, desc, count, beginMsec, endMsec, lastID, messageTypes);
		bool status = client->sendQuest(quest, [this, uid, messageID, callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(HistoryMessageResult(), FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(HistoryMessageResult(), errorCode);
				return;
			}

			HistoryMessageResult result;
			GetHistoryMessageResult(result, answer, 0);
			AdjustHistoryMessageResultForP2PMessage(_uid, uid, result);
			callback(result, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(HistoryMessageResult(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	FPQuestPtr RTMClient::GenGetMessageQuest(int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory)
	{
		FPQWriter qw(4, "getmsg");
		qw.param("from", fromUid);
		qw.param("mid", messageID);
		qw.param("xid", toId);
		qw.param("type", (int8_t)messageCategory);
		return qw.take();
	}

	void RTMClient::BuildRetrievedMessage(RetrievedMessage* message, FPAnswerPtr answer)
	{
		FPAReader ar(answer);
		message->cursorId = ar.wantInt("id");
		message->messageType = ar.wantInt("mtype");
		message->attrs = ar.wantString("attrs");
		message->modifiedTime = ar.wantInt("mtime");
		message->message = ar.wantString("msg");

		if (CheckFileMessageType(message->messageType))
			RTMClient::BuildFileInfo(*message);
	}

	int32_t RTMClient::GetMessage(RetrievedMessage& result, int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQuestPtr quest = GenGetMessageQuest(fromUid, toId, messageID, messageCategory);
		FPAnswerPtr answer = client->sendQuest(quest, timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		if (answer->status())
		{
			FPAReader ar(answer);
			return ar.getInt("code");
		}

		BuildRetrievedMessage(&result, answer);
		return FPNN_EC_OK;
	}

	bool RTMClient::GetMessage(function<void(RetrievedMessage, int32_t)> callback, int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(RetrievedMessage(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQuestPtr quest = GenGetMessageQuest(fromUid, toId, messageID, messageCategory);
		bool status = client->sendQuest(quest, [this, callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(RetrievedMessage(), FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(RetrievedMessage(), errorCode);
				return;
			}

			RetrievedMessage result;
			BuildRetrievedMessage(&result, answer);
			callback(result, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(RetrievedMessage(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	FPQuestPtr RTMClient::GenDeleteMessageQuest(int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory)
	{
		FPQWriter qw(4, "delmsg");
		qw.param("from", fromUid);
		qw.param("mid", messageID);
		qw.param("xid", toId);
		qw.param("type", (int8_t)messageCategory);
		return qw.take();
	}

	int32_t RTMClient::DeleteMessage(int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQuestPtr quest = GenGetMessageQuest(fromUid, toId, messageID, messageCategory);
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

	bool RTMClient::DeleteMessage(DoneCallback callback, int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQuestPtr quest = GenDeleteMessageQuest(fromUid, toId, messageID, messageCategory);
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
}
