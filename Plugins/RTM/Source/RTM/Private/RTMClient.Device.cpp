#include "RTMClient.h"

namespace rtm
{
	using namespace std;
	using namespace fpnn;

	int32_t RTMClient::AddDevice(const string& appType, const string& deviceToken, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(2, "adddevice");
		qw.param("apptype", appType);
		qw.param("devicetoken", deviceToken);
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

	bool RTMClient::AddDevice(DoneCallback callback, const string& appType, const string& deviceToken, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(2, "adddevice");
		qw.param("apptype", appType);
		qw.param("devicetoken", deviceToken);
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

	int32_t RTMClient::RemoveDevice(const string& deviceToken, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(1, "removedevice");
		qw.param("devicetoken", deviceToken);
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

	bool RTMClient::RemoveDevice(DoneCallback callback, const string& deviceToken, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(1, "adddevice");
		qw.param("devicetoken", deviceToken);
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
	int8_t RTMClient::GetPushOptionType(MessageCategory messageCategory)
	{
		switch (messageCategory)
		{
		case rtm::MessageCategory_P2PMessage:
			return 0;
		case rtm::MessageCategory_GroupMessage:
			return 1;
		case rtm::MessageCategory_RoomMessage:
		case rtm::MessageCategory_BroadcastMessage:
		default:
			return 99;
		}
	}

	int32_t RTMClient::AddDevicePushOption(MessageCategory messageCategory, int64_t targetID, set<int8_t> messageTypes /* = set<int8_t>() */, int32_t timeout /* = 0 */)
	{
		int8_t type = GetPushOptionType(messageCategory);
		if (type > 1)
			return RTM_EC_INVALID_PARAMETER;
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		int32_t paramCount = 3;
		if (messageTypes.empty())
			paramCount -= 1;
		FPQWriter qw(paramCount, "addoption");
		qw.param("type", type);
		qw.param("xid", targetID);
		if (!messageTypes.empty())
			qw.param("mtypes", messageTypes);

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

	bool RTMClient::AddDevicePushOption(DoneCallback callback, MessageCategory messageCategory, int64_t targetID, set<int8_t> messageTypes /* = set<int8_t>() */, int32_t timeout /* = 0 */)
	{
		int8_t type = GetPushOptionType(messageCategory);
		if (type > 1)
		{
			ClientEngine::runTask([callback]() {
				callback(RTM_EC_INVALID_PARAMETER);
				});
			return false;
		}
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		int32_t paramCount = 3;
		if (messageTypes.empty())
			paramCount -= 1;
		FPQWriter qw(paramCount, "addoption");
		qw.param("type", type);
		qw.param("xid", targetID);;
		if (!messageTypes.empty())
			qw.param("mtypes", messageTypes);
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

	int32_t RTMClient::RemoveDevicePushOption(MessageCategory messageCategory, int64_t targetID, set<int8_t> messageTypes /* = set<int8_t>() */, int32_t timeout /* = 0 */)
	{
		int8_t type = GetPushOptionType(messageCategory);
		if (type > 1)
			return RTM_EC_INVALID_PARAMETER;
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		int32_t paramCount = 3;
		if (messageTypes.empty())
			paramCount -= 1;
		FPQWriter qw(paramCount, "removeoption");
		qw.param("type", type);
		qw.param("xid", targetID);;
		if (!messageTypes.empty())
			qw.param("mtypes", messageTypes);

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

	bool RTMClient::RemoveDevicePushOption(DoneCallback callback, MessageCategory messageCategory, int64_t targetID, set<int8_t> messageTypes /* = set<int8_t>() */, int32_t timeout /* = 0 */)
	{
		int8_t type = GetPushOptionType(messageCategory);
		if (type > 1)
		{
			ClientEngine::runTask([callback]() {
				callback(RTM_EC_INVALID_PARAMETER);
				});
			return false;
		}
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		int32_t paramCount = 3;
		if (messageTypes.empty())
			paramCount -= 1;
		FPQWriter qw(paramCount, "removeoption");
		qw.param("type", type);
		qw.param("xid", targetID);;
		if (!messageTypes.empty())
			qw.param("mtypes", messageTypes);
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

	int32_t RTMClient::GetDevicePushOption(map<int64_t, set<int8_t>>& p2pMap, map<int64_t, set<int8_t>>& groupMap, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;

		FPQuestPtr quest = FPQWriter::emptyQuest("getoption");
		FPAnswerPtr answer = client->sendQuest(quest, timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
			return ar.getInt("code");

		map<string, set<int8_t>> p2p;
		map<string, set<int8_t>> group;
		p2p = ar.want("p2p", p2p);
		group = ar.want("group", group);

		for (auto iter = p2p.begin(); iter != p2p.end(); ++iter)
			p2pMap.insert(make_pair(atoll(iter->first.c_str()), move(iter->second)));
		for (auto iter = group.begin(); iter != group.end(); ++iter)
			groupMap.insert(make_pair(atoll(iter->first.c_str()), move(iter->second)));
		return FPNN_EC_OK;
	}

	bool RTMClient::GetDevicePushOption(function<void(map<int64_t, set<int8_t>>, map<int64_t, set<int8_t>>, int32_t)> callback, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(map<int64_t, set<int8_t>>(), map<int64_t, set<int8_t>>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}

		FPQuestPtr quest = FPQWriter::emptyQuest("getoption");
		bool status = client->sendQuest(quest, [callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(map<int64_t, set<int8_t>>(), map<int64_t, set<int8_t>>(), FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(map<int64_t, set<int8_t>>(), map<int64_t, set<int8_t>>(), errorCode);
				return;
			}
			map<string, set<int8_t>> p2p;
			map<string, set<int8_t>> group;
			map<int64_t, set<int8_t>> p2pMap;
			map<int64_t, set<int8_t>> groupMap;
			p2p = ar.want("p2p", p2p);
			group = ar.want("group", group);

			for (auto iter = p2p.begin(); iter != p2p.end(); ++iter)
				p2pMap.insert(make_pair(atoll(iter->first.c_str()), move(iter->second)));
			for (auto iter = group.begin(); iter != group.end(); ++iter)
				groupMap.insert(make_pair(atoll(iter->first.c_str()), move(iter->second)));

			callback(p2pMap, groupMap, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(map<int64_t, set<int8_t>>(), map<int64_t, set<int8_t>>(), FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}
}