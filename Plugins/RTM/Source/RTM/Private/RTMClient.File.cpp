#include "RTMClient.h"
#include "RTMControlCenter.h"
#include "RTMQuestProcessor.h"
#include "StringUtil.h"
namespace rtm
{
	using namespace std;
	using namespace fpnn;

	bool RTMClient::CheckFileMessageType(int8_t messageType)
	{
		if (messageType >= ImageFileType && messageType <= NormalFileType)
			return true;
		return false;
	}

	void RTMClient::BuildFileInfo(RTMMessage& message)
	{
		message.fileInfo = make_shared<FileInfo>();
		ParseFileMessage(message);
		ParseFileAttrs(message);
	}

	void RTMClient::ParseFileMessage(RTMMessage& message)
	{
		rapidjson::Document document;
		if (document.Parse(message.message.c_str()).HasParseError())
			return;
		if (!document.IsObject())
			return;
		if (document.HasMember("url") && document["url"].IsString())
			message.fileInfo->url = document["url"].GetString();
		if (document.HasMember("size") && document["size"].IsInt())
			message.fileInfo->size = document["size"].GetInt();
		if (message.messageType == ImageFileType)
		{
			if (document.HasMember("surl") && document["surl"].IsString())
				message.fileInfo->surl = document["surl"].GetString();
		}
		message.message = "";
	}

	void RTMClient::ParseFileAttrs(RTMMessage& message)
	{
		rapidjson::Document document;
		if (document.Parse(message.attrs.c_str()).HasParseError())
			return;
		if (!document.IsObject())
			return;
		if (document.HasMember("rtm"))
		{
			const auto& rtm = document["rtm"];
			auto iter = rtm.FindMember("type");
			if (iter != rtm.MemberEnd() && iter->value.IsString())
			{
				string type = iter->value.GetString();
				if (type == "audiomsg")
					message.fileInfo->isRTMAudio = true;
			}
			if (message.fileInfo->isRTMAudio == true)
			{
				iter = rtm.FindMember("lang");
				if (iter != rtm.MemberEnd() && iter->value.IsString())
					message.fileInfo->language = iter->value.GetString();
				iter = rtm.FindMember("duration");
				if (iter != rtm.MemberEnd() && iter->value.IsInt())
					message.fileInfo->duration = iter->value.GetInt();
			}
		}
		if (document.HasMember("custom"))
		{
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			document["custom"].Accept(writer);
			message.attrs = buffer.GetString();
		}
	}

	void RTMClient::UpdateFileGateTimeout(int32_t& timeout, int64_t& lastActionTimestamp)
	{
		int64_t now = TimeUtil::curr_msec();

		timeout -= (int)((now - lastActionTimestamp) / 1000);

		lastActionTimestamp = now;
	}

	int32_t RTMClient::FileToken(string& token, string& endpoint, FileTokenType tokenType, int64_t xid, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(2, "filetoken");
		switch (tokenType)
		{
		case P2PFile:
			qw.param("cmd", "sendfile");
			qw.param("to", xid);
			break;
		case GroupFile:
			qw.param("cmd", "sendgroupfile");
			qw.param("gid", xid);
			break;
		case RoomFile:
			qw.param("cmd", "sendroomfile");
			qw.param("rid", xid);
			break;
		default:
			return RTM_EC_INVALID_PARAMETER;
		}

		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}

		token = ar.wantString("token");
		endpoint = ar.wantString("endpoint");

		return FPNN_EC_OK;
	}

	bool RTMClient::FileToken(function<void(string, string, int32_t)> callback, FileTokenType tokenType, int64_t xid, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback("", "", FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(2, "filetoken");
		switch (tokenType)
		{
		case P2PFile:
			qw.param("cmd", "sendfile");
			qw.param("to", xid);
			break;

		case GroupFile:
			qw.param("cmd", "sendgroupfile");
			qw.param("gid", xid);
			break;

		case RoomFile:
			qw.param("cmd", "sendroomfile");
			qw.param("rid", xid);
			break;
		default:
			ClientEngine::runTask([callback]() {
				callback("", "", RTM_EC_INVALID_PARAMETER);
				});
			return false;
		}
		bool status = client->sendQuest(qw.take(), [callback](FPAnswerPtr answer, int errorCode) {
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
			string token = ar.wantString("token");
			string endpoint = ar.wantString("endpoint");
			callback(token, endpoint, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback("", "", FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	bool RTMClient::ipv4EndpointToipv6(const string& endpoint, string& ip, int32_t& port)
	{
		// https://tools.ietf.org/html/rfc6052
		vector<string> endpoints;
		StringUtil::split(endpoint, ":", endpoints);
		if (endpoints.size() < 2)
			return false;
		vector<string> parts;
		port = atoi(endpoints[1].c_str());
		StringUtil::split(endpoints[0], ".", parts);
		if (parts.size() != 4)
			return false;
		for (auto& part : parts) {
			int32_t p = atoi(part.c_str());
			if (p < 0 || p > 255)
				return false;
		}
		int32_t part7 = atoi(parts[0].c_str()) * 256 + atoi(parts[1].c_str());
		int32_t part8 = atoi(parts[2].c_str()) * 256 + atoi(parts[3].c_str());
		std::stringstream ss;
		ss << "64:ff9b::" << hex << part7 << ":" << hex << part8;
		ip = ss.str();
		return true;
	}

	string RTMClient::CalcMD5(const string& content)
	{
		unsigned char digest[16];
		md5_checksum(digest, content.c_str(), content.size());
		char hexstr[32 + 1];
		Hexlify(hexstr, digest, sizeof(digest));
		return string(hexstr);
	}

	FPQuestPtr RTMClient::GenSendFileQuest(int64_t& messageID, const string& token, FileTokenType tokenType, int64_t targetID, int8_t messageType, const string& fileContent, const string& fileName, const string& fileExtension, const string attrs, const map<string, string>& rtmAttrs)
	{
		shared_ptr<FPQWriter> qw;
		switch (tokenType)
		{
		case P2PFile:
		{
			qw.reset(new FPQWriter(8, "sendfile"));
			qw->param("to", targetID);
			break;
		}
		case RoomFile:
		{
			qw.reset(new FPQWriter(8, "sendroomfile"));
			qw->param("rid", targetID);
			break;
		}
		case GroupFile:
		{
			qw.reset(new FPQWriter(8, "sendgroupfile"));
			qw->param("gid", targetID);
			break;
		}
		default:
			return nullptr;
		}

		qw->param("pid", _pid);
		qw->param("from", _uid);
		qw->param("token", token);
		qw->param("mtype", messageType);
		messageID = RTMMidGenerator::Gen();
		qw->param("mid", messageID);
		qw->param("file", fileContent);

		string fileMD5 = CalcMD5(fileContent);
		transform(fileMD5.begin(), fileMD5.end(), fileMD5.begin(), ::tolower);
		string sign = CalcMD5(fileMD5 + ":" + token);
		transform(sign.begin(), sign.end(), sign.begin(), ::tolower);
		Json attr;
		map<string, string> rtm = rtmAttrs;
		rtm["sign"] = sign;
		if (!fileName.empty())
			rtm["filename"] = fileName;
		if (!fileExtension.empty())
			rtm["ext"] = fileExtension;
		attr.add("rtm", rtm);
		if (attrs.empty())
			attr.add("custom", "");
		else
			attr.add("custom", attrs);
		qw->param("attrs", attr.str());
		return qw->take();
	}

	int32_t RTMClient::RealSendFile(int64_t& messageID, FileTokenType tokenType, int64_t targetID, int8_t messageType, const string& fileContent, const string& fileName, const string& fileExtension /* = "" */, const string& attrs /* = "" */, const map<string, string>& rtmAttrs /* = map<string , string >() */, int32_t timeout /* = 120 */)
	{
		if (!CheckFileMessageType(messageType))
			return RTM_EC_INVALID_MTYPE;
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;

		int64_t lastActionTimestamp = TimeUtil::curr_msec();

		string token, endpoint;
		int32_t errorCode = FileToken(token, endpoint, tokenType, targetID, timeout);
		if (errorCode != FPNN_EC_OK)
			return errorCode;
		
		UpdateFileGateTimeout(timeout, lastActionTimestamp);
		if (timeout <= 0)
			return FPNN_EC_CORE_TIMEOUT;

		TCPClientPtr fileGate = RTMControlCenter::GetFileGate(endpoint);
		if (fileGate == nullptr)
		{
			fileGate = TCPClient::createClient(endpoint, true);
			errorCode = FileGateConnectIPv4Sync(fileGate, endpoint, timeout, lastActionTimestamp);
			if (errorCode != FPNN_EC_OK)
				return errorCode;
			if (fileGate == nullptr)
				return FPNN_EC_CORE_INVALID_CONNECTION;


			RTMControlCenter::ActiveFileGate(endpoint, fileGate);
			UpdateFileGateTimeout(timeout, lastActionTimestamp);
			if (timeout <= 0)
				return FPNN_EC_CORE_TIMEOUT;
		}

		FPQuestPtr quest = GenSendFileQuest(messageID, token, tokenType, targetID, messageType, fileContent, fileName, fileExtension, attrs, rtmAttrs);
		FPAnswerPtr answer = fileGate->sendQuest(quest, timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		if (answer->status())
		{
			FPAReader ar(answer);
			return ar.getInt("code");
		}

		return FPNN_EC_OK;
	}

	bool RTMClient::RealSendFile(MessageIDCallback callback, FileTokenType tokenType, int64_t targetID, int8_t messageType, const string& fileContent, const string& fileName, const string& fileExtension /* = "" */, const string attrs /* = "" */, const map<string, string> rtmAttrs /* = map<string , string > ()*/, int32_t timeout /* = 120 */)
	{
		if (!CheckFileMessageType(messageType))
		{
			ClientEngine::runTask([callback]() {
				callback(0, RTM_EC_INVALID_MTYPE);
				});
			return false;
		}
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(0, FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}

		int64_t lastActionTimestamp = TimeUtil::curr_msec();
		bool status = FileToken([this, client, callback, tokenType, targetID, messageType, fileContent, fileName, fileExtension, attrs, rtmAttrs, timeout, lastActionTimestamp](string token, string endpoint, int32_t errorCode) {
			if (errorCode != FPNN_EC_OK)
			{
				callback(0, errorCode);
				return;
			}
			int32_t timeoutToken = timeout;
			int64_t lastActionTimestampToken = lastActionTimestamp;

			UpdateFileGateTimeout(timeoutToken, lastActionTimestampToken);
			if (timeout < 0)
			{
				callback(0, FPNN_EC_CORE_TIMEOUT);
				return;
			}
			int64_t messageID = 0;
			FPQuestPtr quest = GenSendFileQuest(messageID, token, tokenType, targetID, messageType, fileContent, fileName, fileExtension, attrs, rtmAttrs);
			TCPClientPtr fileGate = RTMControlCenter::GetFileGate(endpoint);
			if (fileGate == nullptr)
			{
				fileGate = TCPClient::createClient(endpoint, true);
				FileGateConnectIPv4Async([callback, endpoint, quest, messageID, fileGate, timeoutToken, lastActionTimestampToken](int32_t errorCode) {
					if (errorCode != FPNN_EC_OK)
					{
						callback(0, errorCode);
						return;
					}
					if (fileGate == nullptr)
					{
						callback(0, FPNN_EC_CORE_INVALID_CONNECTION);
						return;
					}
					int32_t timeoutConnect = timeoutToken;
					int64_t lastActionTimestampConnect = lastActionTimestampToken;
					RTMControlCenter::ActiveFileGate(endpoint, fileGate);

					UpdateFileGateTimeout(timeoutConnect, lastActionTimestampConnect);
					bool status = fileGate->sendQuest(quest, [callback, messageID](FPAnswerPtr answer, int errorCode) {
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
						callback(messageID, FPNN_EC_OK);
						}, timeoutConnect);
					}, fileGate, endpoint, timeoutToken, lastActionTimestampToken);
			}
			else
			{
				bool status = fileGate->sendQuest(quest, [callback, messageID](FPAnswerPtr answer, int errorCode) {
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
					callback(messageID, FPNN_EC_OK);
					}, timeoutToken);
			}
			}, tokenType, targetID, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(0, FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	int32_t RTMClient::FileGateConnectIPv4Sync(TCPClientPtr fileGate, const string& endpoint, int32_t& timeout, int64_t& lastActionTimestamp)
	{
		mutex connectMutex;
		unique_lock<mutex> locker(connectMutex);
		condition_variable condition;
		int32_t errorCode = FPNN_EC_OK;
		fileGate->setQuestProcessor(shared_ptr<IQuestProcessor>(new FileProcessor(fileGate, [this, fileGate, &timeout, &lastActionTimestamp, endpoint, &errorCode, &condition](const ConnectionInfo& connInfo, bool connected) {
			if (!connected)
			{
				UpdateFileGateTimeout(timeout, lastActionTimestamp);
				if (timeout <= 0)
				{
					errorCode = FPNN_EC_CORE_TIMEOUT;
					condition.notify_all();
					return;
				}

				string ip;
				int32_t port = 0;
				if (!ipv4EndpointToipv6(endpoint, ip, port))
				{
					errorCode = FPNN_EC_CORE_INVALID_CONNECTION;
					condition.notify_all();
					return;
				}

				errorCode = FileGateConnectIPv6Sync(fileGate, ip, port, timeout, lastActionTimestamp);
			}
			condition.notify_all();
			})));
		fileGate->asyncConnect();
		condition.wait(locker);
		return errorCode;
	}

	int32_t RTMClient::FileGateConnectIPv6Sync(TCPClientPtr fileGate, const string& ip, int32_t port, int32_t& timeout, int64_t& lastActionTimestamp)
	{
		mutex connectMutex;
		unique_lock<mutex> locker(connectMutex);
		condition_variable condition;
		int32_t errorCode = FPNN_EC_OK;
		fileGate = TCPClient::createClient(ip, port, true);
		fileGate->setQuestProcessor(shared_ptr<IQuestProcessor>(new FileProcessor(fileGate, [&errorCode, &condition](const ConnectionInfo& connInfo, bool connected) {
			if (!connected)
				errorCode = FPNN_EC_CORE_INVALID_CONNECTION;
			condition.notify_all();
			})));
		fileGate->connect();
		condition.wait(locker);
		return errorCode;
	}

	void RTMClient::FileGateConnectIPv4Async(DoneCallback callback, TCPClientPtr fileGate, const string& endpoint, int32_t& timeout, int64_t& lastActionTimestamp)
	{
		fileGate->setQuestProcessor(shared_ptr<IQuestProcessor>(new FileProcessor(fileGate, [this, fileGate, &timeout, &lastActionTimestamp, endpoint, callback](const ConnectionInfo& connInfo, bool connected) {
			if (!connected)
			{
				UpdateFileGateTimeout(timeout, lastActionTimestamp);
				if (timeout <= 0)
				{
					callback(FPNN_EC_CORE_TIMEOUT);
					return;
				}

				string ip;
				int32_t port = 0;
				if (!ipv4EndpointToipv6(endpoint, ip, port))
				{
					callback(FPNN_EC_CORE_INVALID_CONNECTION);
					return;
				}

				FileGateConnectIPv6Async(callback, fileGate, ip, port, timeout, lastActionTimestamp);
			}
			else
				callback(FPNN_EC_OK);
			})));
		fileGate->asyncConnect();
	}

	void RTMClient::FileGateConnectIPv6Async(DoneCallback callback, TCPClientPtr fileGate, const string& ip, int32_t port, int32_t& timeout, int64_t& lastActionTimestamp)
	{
		fileGate = TCPClient::createClient(ip, port, true);
		fileGate->setQuestProcessor(shared_ptr<IQuestProcessor>(new FileProcessor(fileGate, [callback](const ConnectionInfo& connInfo, bool connected) {
			if (!connected)
			{
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				return;
			}
			callback(FPNN_EC_OK);
			})));
		fileGate->connect();
	}

	int32_t RTMClient::SendFile(int64_t& messageID, int64_t uid, ChatMessageType type, const string& fileContent, const string& fileName, const string& fileExtension /* = "" */, const string& attrs /* = "" */, int32_t timeout /* = 120 */)
	{
		return RealSendFile(messageID, P2PFile, uid, (int8_t)type, fileContent, fileName, fileExtension, attrs, map<string, string>(), timeout);
	}

	bool RTMClient::SendFile(MessageIDCallback callback, int64_t uid, ChatMessageType type, const string& fileContent, const string& fileName, const string& fileExtension /* = "" */, const string& attrs /* = "" */, int32_t timeout /* = 120 */)
	{
		return RealSendFile(callback, P2PFile, uid, (int8_t)type, fileContent, fileName, fileExtension, attrs, map<string, string>(), timeout);
	}

	int32_t RTMClient::SendGroupFile(int64_t& messageID, int64_t groupID, ChatMessageType type, const string& fileContent, const string& fileName, const string& fileExtension /* = "" */, const string& attrs /* = "" */, int32_t timeout /* = 120 */)
	{
		return RealSendFile(messageID, GroupFile, groupID, (int8_t)type, fileContent, fileName, fileExtension, attrs, map<string, string>(), timeout);
	}

	bool RTMClient::SendGroupFile(MessageIDCallback callback, int64_t groupID, ChatMessageType type, const string& fileContent, const string& fileName, const string& fileExtension /* = "" */, const string& attrs /* = "" */, int32_t timeout /* = 120 */)
	{
		return RealSendFile(callback, GroupFile, groupID, (int8_t)type, fileContent, fileName, fileExtension, attrs, map<string, string>(), timeout);
	}

	int32_t RTMClient::SendRoomFile(int64_t& messageID, int64_t roomID, ChatMessageType type, const string& fileContent, const string& fileName, const string& fileExtension /* = "" */, const string& attrs /* = "" */, int32_t timeout /* = 120 */)
	{
		return RealSendFile(messageID, RoomFile, roomID, (int8_t)type, fileContent, fileName, fileExtension, attrs, map<string, string>(), timeout);
	}

	bool RTMClient::SendRoomFile(MessageIDCallback callback, int64_t roomID, ChatMessageType type, const string& fileContent, const string& fileName, const string& fileExtension /* = "" */, const string& attrs /* = "" */, int32_t timeout /* = 120 */)
	{
		return RealSendFile(callback, RoomFile, roomID, (int8_t)type, fileContent, fileName, fileExtension, attrs, map<string, string>(), timeout);
	}
}
