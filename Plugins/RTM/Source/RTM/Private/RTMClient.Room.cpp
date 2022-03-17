#include "RTMClient.h"

namespace rtm
{
	using namespace std;
	using namespace fpnn;

	int32_t RTMClient::EnterRoom(int64_t roomID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(1, "enterroom");
		qw.param("rid", roomID);
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

	bool RTMClient::EnterRoom(DoneCallback callback, int64_t roomID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(1, "enterroom");
		qw.param("rid", roomID);
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

	int32_t RTMClient::LeaveRoom(int64_t roomID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(1, "leaveroom");
		qw.param("rid", roomID);
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

	bool RTMClient::LeaveRoom(DoneCallback callback, int64_t roomID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(1, "leaveroom");
		qw.param("rid", roomID);
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

	int32_t RTMClient::GetUserRooms(set<int64_t>& roomIDs, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQuestPtr quest = FPQWriter::emptyQuest("getuserrooms");
		FPAnswerPtr answer = client->sendQuest(quest, timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}
		roomIDs = ar.want("rooms", roomIDs);

		return FPNN_EC_OK;
	}

	bool RTMClient::GetUserRooms(function<void(set<int64_t>, int32_t)> callback, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(set<int64_t>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQuestPtr quest = FPQWriter::emptyQuest("getuserrooms");
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
			set<int64_t> roomIDs = ar.want("rooms", set<int64_t>());
			callback(roomIDs, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(set<int64_t>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	int32_t RTMClient::GetRoomMembers(set<int64_t>& uids, int64_t roomID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(1, "getroommembers");
		qw.param("rid", roomID);
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

	bool RTMClient::GetRoomMembers(function<void(set<int64_t>, int32_t)> callback, int64_t roomID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(set<int64_t>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(1, "getroommembers");
		qw.param("rid", roomID);
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

	int32_t RTMClient::GetRoomMemberCount(map<int64_t, int32_t>& counts, const set<int64_t>& roomIDs, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(1, "getroomcount");
		qw.param("rids", roomIDs);
		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}
		map<string, int32_t> countMap;
		countMap = ar.want("cn", countMap);

		for (auto iter = countMap.begin(); iter != countMap.end(); ++iter)
			counts.insert(make_pair(atoll(iter->first.c_str()), iter->second));

		return FPNN_EC_OK;
	}

	bool RTMClient::GetRoomMemberCount(function<void(map<int64_t, int32_t>, int32_t)> callback, const set<int64_t>& roomIDs, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(map<int64_t, int32_t>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(1, "getroomcount");
		qw.param("rids", roomIDs);
		bool status = client->sendQuest(qw.take(), [callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(map<int64_t, int32_t>(), FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(map<int64_t, int32_t>(), errorCode);
				return;
			}
			map<string, int32_t> countMap;
			map<int64_t, int32_t> counts;
			countMap = ar.want("cn", countMap);

			for (auto iter = countMap.begin(); iter != countMap.end(); ++iter)
				counts.insert(make_pair(atoll(iter->first.c_str()), iter->second));
			callback(counts, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(map<int64_t, int32_t>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	int32_t RTMClient::SetRoomInfo(int64_t roomID, const string& publicInfo /* = "" */, const string& privateInfo /* = "" */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		int32_t paramCount = 3;
		if (publicInfo.empty())
			paramCount -= 1;
		if (privateInfo.empty())
			paramCount -= 1;
		FPQWriter qw(paramCount, "setroominfo");
		qw.param("rid", roomID);
		if (!publicInfo.empty())
			qw.param("oinfo", publicInfo);
		if (!privateInfo.empty())
			qw.param("pinfo", privateInfo);

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

	bool RTMClient::SetRoomInfo(DoneCallback callback, int64_t roomID, const string& publicInfo /* = "" */, const string& privateInfo /* = "" */, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		int32_t paramCount = 3;
		if (publicInfo.empty())
			paramCount -= 1;
		if (privateInfo.empty())
			paramCount -= 1;
		FPQWriter qw(paramCount, "setroominfo");
		qw.param("rid", roomID);
		if (!publicInfo.empty())
			qw.param("oinfo", publicInfo);
		if (!privateInfo.empty())
			qw.param("pinfo", privateInfo);
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

	int32_t RTMClient::GetRoomInfo(string& publicInfo, string& privateInfo, int64_t roomID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(1, "getroominfo");
		qw.param("rid", roomID);
		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}

		publicInfo = ar.getString("oinfo");
		privateInfo = ar.getString("pinfo");
		return FPNN_EC_OK;
	}

	bool RTMClient::GetRoomInfo(function<void(string, string, int32_t)> callback, int64_t roomID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback("", "", FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(1, "getroominfo");
		qw.param("rid", roomID);
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
			string publicInfo = ar.getString("oinfo");
			string privateInfo = ar.getString("pinfo");
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

	int32_t RTMClient::GetRoomPublicInfo(string& publicInfo, int64_t roomID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(1, "getroomopeninfo");
		qw.param("rid", roomID);
		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}

		publicInfo = ar.getString("oinfo");
		return FPNN_EC_OK;
	}

	bool RTMClient::GetRoomPublicInfo(function<void(string, int32_t)> callback, int64_t roomID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback("", FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(1, "getroomopeninfo");
		qw.param("rid", roomID);
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
			string publicInfo = ar.getString("oinfo");
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

	int32_t RTMClient::GetRoomsPublicInfo(map<int64_t, string>& publicInfos, const set<int64_t>& roomIDs, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(1, "getroomsopeninfo");
		qw.param("rids", roomIDs);
		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}

		map<string, string> info;
		info = ar.get("info", info);

		for (auto iter = info.begin(); iter != info.end(); ++iter)
			publicInfos.insert(make_pair(atoll(iter->first.c_str()), iter->second));
		return FPNN_EC_OK;
	}

	bool RTMClient::GetRoomsPublicInfo(function<void(map<int64_t, string>, int32_t)> callback, const set<int64_t>& roomIDs, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(map<int64_t, string>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(1, "getroomsopeninfo");
		qw.param("rids", roomIDs);
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
			info = ar.get("info", info);
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