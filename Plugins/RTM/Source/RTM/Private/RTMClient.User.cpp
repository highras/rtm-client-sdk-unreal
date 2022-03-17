#include "RTMClient.h"

namespace rtm
{
	using namespace std;
	using namespace fpnn;

	int32_t RTMClient::GetOnlineUsers(set<int64_t>& onlineUIDs, const set<int64_t>& uids, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;

		FPQWriter qw(1, "getonlineusers");
		qw.param("uids", uids);
		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
			return ar.getInt("code");

		onlineUIDs = ar.want("uids", onlineUIDs);
		return FPNN_EC_OK;
	}

	bool RTMClient::GetOnlineUsers(function<void(set<int64_t>, int32_t)> callback, const set<int64_t>& uids, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(set<int64_t>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}

		FPQWriter qw(1, "getonlineusers");
		qw.param("uids", uids);
		bool status = client->sendQuest(qw.take(), [callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(set<int64_t>(), FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(set<int64_t>(), errorCode);
				return;
			}

			set<int64_t> uids;
			uids = ar.want("uids", uids);
			callback(uids, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(set<int64_t>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	int32_t RTMClient::SetUserInfo(const string& publicInfo /* = "" */, const string& privateInfo /* = "" */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;

		FPQuestPtr quest = nullptr;
		if (publicInfo == "" && privateInfo == "")
			quest = FPQWriter::emptyQuest("setuserinfo");
		else
		{
			int32_t paramCount = 2;
			if (publicInfo == "")
				paramCount -= 1;
			if (privateInfo == "")
				paramCount -= 1;
			FPQWriter qw(paramCount, "setuserinfo");
			if (publicInfo != "")
				qw.param("oinfo", publicInfo);
			if (privateInfo != "")
				qw.param("pinfo", privateInfo);
			quest = qw.take();
		}
		FPAnswerPtr answer = client->sendQuest(quest, timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
			return ar.getInt("code");

		return FPNN_EC_OK;
	}

	bool RTMClient::SetUserInfo(DoneCallback callback, const string& publicInfo /* = "" */, const string& privateInfo /* = "" */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQuestPtr quest = nullptr;
		if (publicInfo == "" && privateInfo == "")
			quest = FPQWriter::emptyQuest("setuserinfo");
		else
		{
			int32_t paramCount = 2;
			if (publicInfo == "")
				paramCount -= 1;
			if (privateInfo == "")
				paramCount -= 1;
			FPQWriter qw(paramCount, "setuserinfo");
			if (publicInfo != "")
				qw.param("oinfo", publicInfo);
			if (privateInfo != "")
				qw.param("pinfo", privateInfo);
			quest = qw.take();
		}
		bool status = client->sendQuest(quest, [callback](FPAnswerPtr answer, int errorCode) {
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

	int32_t RTMClient::GetUserInfo(string& publicInfo, string& privateInfo, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;

		FPQuestPtr quest = FPQWriter::emptyQuest("getuserinfo");
		FPAnswerPtr answer = client->sendQuest(quest, timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
			return ar.getInt("code");
		publicInfo = ar.wantString("oinfo");
		privateInfo = ar.wantString("pinfo");
		return FPNN_EC_OK;
	}

	bool RTMClient::GetUserInfo(function<void(string, string, int32_t)> callback, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback("", "", FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQuestPtr quest = FPQWriter::emptyQuest("setuserinfo");
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

			string publicInfo = ar.wantString("oinfo");
			string privateInfo = ar.wantString("pinfo");
			callback(publicInfo, privateInfo, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback("", "", FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	int32_t RTMClient::GetUserPublicInfo(map<int64_t, string>& publicInfos, const set<int64_t>& uids, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;

		FPQWriter qw(1, "getuseropeninfo");
		qw.param("uids", uids);
		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
			return ar.getInt("code");
		map<string, string> info;
		info = ar.want("info", info);
		for (auto iter = info.begin(); iter != info.end(); ++iter)
			publicInfos.insert(make_pair(atoll(iter->first.c_str()), iter->second));
		return FPNN_EC_OK;
	}

	bool RTMClient::GetUserPublicInfo(function<void(map<int64_t, string>, int32_t)> callback, const set<int64_t>& uids, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(map<int64_t, string>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(1, "getuseropeninfo");
		qw.param("uids", uids);
		bool status = client->sendQuest(qw.take(), [callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(map<int64_t, string>(), FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(map<int64_t, string>(), errorCode);
				return;
			}

			map<string, string> info;
			map<int64_t, string> publicInfos;
			info = ar.want("info", info);
			for (auto iter = info.begin(); iter != info.end(); ++iter)
				publicInfos.insert(make_pair(atoll(iter->first.c_str()), iter->second));
			callback(publicInfos, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(map<int64_t, string>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}
}