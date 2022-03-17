#include "RTMClient.h"

namespace rtm
{
	using namespace std;
	using namespace fpnn;

	int32_t RTMClient::AddGroupMembers(int64_t groupID, const set<int64_t>& uids, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(2, "addgroupmembers");
		qw.param("gid", groupID);
		qw.param("uids", uids);
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

	bool RTMClient::AddGroupMembers(DoneCallback callback, int64_t groupID, const set<int64_t>& uids, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(2, "addgroupmembers");
		qw.param("gid", groupID);
		qw.param("uids", uids);
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

	int32_t RTMClient::DeleteGroupMembers(int64_t groupID, const set<int64_t>& uids, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(2, "delgroupmembers");
		qw.param("gid", groupID);
		qw.param("uids", uids);
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

	bool RTMClient::DeleteGroupMembers(DoneCallback callback, int64_t groupID, const set<int64_t>& uids, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(2, "delgroupmembers");
		qw.param("gid", groupID);
		qw.param("uids", uids);
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

	int32_t RTMClient::GetGroupMembers(set<int64_t>& uids, int64_t groupID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(1, "getgroupmembers");
		qw.param("gid", groupID);
		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}
		uids = ar.want("uids", uids);

		return FPNN_EC_OK;
	}

	bool RTMClient::GetGroupMembers(function<void(set<int64_t>, int32_t)> callback, int64_t groupID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(set<int64_t>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(1, "getgroupmembers");
		qw.param("gid", groupID);
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
			set<int64_t> uids = ar.want("uids", set<int64_t>());
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

	int32_t RTMClient::GetGroupMembers(set<int64_t>& allUIDs, set<int64_t>& onlineUIDs, int64_t groupID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(2, "getgroupmembers");
		qw.param("gid", groupID);
		qw.param("online", true);
		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}
		allUIDs = ar.want("uids", allUIDs);
		onlineUIDs = ar.want("onlines", onlineUIDs);

		return FPNN_EC_OK;
	}

	bool RTMClient::GetGroupMembers(function<void(set<int64_t>, set<int64_t>, int32_t)> callback, int64_t groupID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(set<int64_t>(), set<int64_t>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(2, "getgroupmembers");
		qw.param("gid", groupID);
		qw.param("online", true);
		bool status = client->sendQuest(qw.take(), [callback](FPAnswerPtr answer, int errorCode) {
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
			set<int64_t> uids = ar.want("uids", set<int64_t>());
			set<int64_t> onlineUids = ar.want("onlines", set<int64_t>());
			callback(uids, onlineUids, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(set<int64_t>(), set<int64_t>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	int32_t RTMClient::GetGroupCount(int32_t& memberCount, int64_t groupID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(1, "getgroupcount");
		qw.param("gid", groupID);
		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}
		memberCount = ar.wantInt("cn");

		return FPNN_EC_OK;
	}

	bool RTMClient::GetGroupCount(function<void(int32_t, int32_t)> callback, int64_t groupID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(0, FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(1, "getgroupcount");
		qw.param("gid", groupID);
		bool status = client->sendQuest(qw.take(), [callback](FPAnswerPtr answer, int errorCode) {
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
			int32_t count = ar.wantInt("cn");
			callback(count, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(0, FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	int32_t RTMClient::GetGroupCount(int32_t& memberCount, int32_t& onlineCount, int64_t groupID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(2, "getgroupcount");
		qw.param("gid", groupID);
		qw.param("online", true);
		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}
		memberCount = ar.wantInt("cn");
		onlineCount = ar.wantInt("online");

		return FPNN_EC_OK;
	}

	bool RTMClient::GetGroupCount(function<void(int32_t, int32_t, int32_t)> callback, int64_t groupID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(0, 0, FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(2, "getgroupcount");
		qw.param("gid", groupID);
		qw.param("online", true);
		bool status = client->sendQuest(qw.take(), [callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(0, 0, FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(0, 0, errorCode);
				return;
			}
			int32_t memberCount = ar.wantInt("cn");
			int32_t onlineCount = ar.wantInt("online");
			callback(memberCount, onlineCount, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(0, 0, FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	int32_t RTMClient::GetUserGroups(set<int64_t>& groupIDs, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQuestPtr quest = FPQWriter::emptyQuest("getusergroups");
		FPAnswerPtr answer = client->sendQuest(quest, timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}
		groupIDs = ar.want("gids", groupIDs);

		return FPNN_EC_OK;
	}

	bool RTMClient::GetUserGroups(function<void(set<int64_t>, int32_t)> callback, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(set<int64_t>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQuestPtr quest = FPQWriter::emptyQuest("getusergroups");
		bool status = client->sendQuest(quest, [callback](FPAnswerPtr answer, int errorCode) {
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
			set<int64_t> groupIDs = ar.want("gids", set<int64_t>());
			callback(groupIDs, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(set<int64_t>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	int32_t RTMClient::SetGroupInfo(int64_t groupID, const string& publicInfo /* = "" */, const string& privateInfo /* = "" */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;

		FPQuestPtr quest = nullptr;
		if (publicInfo == "" && privateInfo == "")
			quest = FPQWriter::emptyQuest("setgroupinfo");
		else
		{
			int32_t paramCount = 3;
			if (publicInfo == "")
				paramCount -= 1;
			if (privateInfo == "")
				paramCount -= 1;
			FPQWriter qw(paramCount, "setgroupinfo");
			qw.param("gid", groupID);
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

	bool RTMClient::SetGroupInfo(DoneCallback callback, int64_t groupID, const string& publicInfo /* = "" */, const string& privateInfo /* = "" */, int32_t timeout /* = 0 */)
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
			quest = FPQWriter::emptyQuest("setgroupinfo");
		else
		{
			int32_t paramCount = 3;
			if (publicInfo == "")
				paramCount -= 1;
			if (privateInfo == "")
				paramCount -= 1;
			FPQWriter qw(paramCount, "setgroupinfo");
			qw.param("gid", groupID);
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

	int32_t RTMClient::GetGroupInfo(string& publicInfo, string& privateInfo, int64_t groupID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(1, "getgroupinfo");
		qw.param("gid", groupID);
		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}
		publicInfo = ar.wantString("oinfo");
		privateInfo = ar.wantString("pinfo");

		return FPNN_EC_OK;
	}

	bool RTMClient::GetGroupInfo(function<void(string, string, int32_t)> callback, int64_t groupID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback("", "", FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(1, "getgroupinfo");
		qw.param("gid", groupID);
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

	int32_t RTMClient::GetGroupPublicInfo(string& publicInfo, int64_t groupID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(1, "getgroupopeninfo");
		qw.param("gid", groupID);
		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}
		publicInfo = ar.wantString("oinfo");

		return FPNN_EC_OK;
	}

	bool RTMClient::GetGroupPublicInfo(function<void(string, int32_t)> callback, int64_t groupID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback("", FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(1, "getgroupopeninfo");
		qw.param("gid", groupID);
		bool status = client->sendQuest(qw.take(), [callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback("", FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback("", errorCode);
				return;
			}
			string publicInfo = ar.wantString("oinfo");
			callback(publicInfo, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback("", FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	int32_t RTMClient::GetGroupsPublicInfo(map<int64_t, string>& publicInfos, const set<int64_t>& groupIDs, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(1, "getgroupsopeninfo");
		qw.param("gids", groupIDs);
		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}
		map<string, string> info;
		info = ar.want("info", info);
		for (auto iter = info.begin(); iter != info.end(); ++iter)
			publicInfos.insert(make_pair(atoll(iter->first.c_str()), iter->second));

		return FPNN_EC_OK;
	}

	bool RTMClient::GetGroupsPublicInfo(function<void(map<int64_t, string>, int32_t)> callback, const set<int64_t>& groupIDs, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(map<int64_t, string>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(1, "getgroupsopeninfo");
		qw.param("gids", groupIDs);
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
