#include "RTMClient.h"

namespace rtm
{
	using namespace std;
	using namespace fpnn;

	int32_t RTMClient::_CreateRTCRoom(string& token, int64_t roomID, RTCRoomType roomType, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		
		FPQWriter qw(3, "createRTCRoom");
		qw.param("rid", roomID);
		qw.param("type", (int32_t)roomType);
		qw.param("enableRecord", 0);

		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}
		token = ar.wantString("token");

		return FPNN_EC_OK;
	}

	bool RTMClient::_CreateRTCRoom(function<void(int64_t, string, int32_t)> callback, int64_t roomID, RTCRoomType roomType, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(0, "", FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}

		FPQWriter qw(3, "createRTCRoom");
		qw.param("rid", roomID);
		qw.param("type", (int32_t)roomType);
		qw.param("enableRecord", 0);

		bool status = client->sendQuest(qw.take(), [callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(0, "", FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(0, "", errorCode);
				return;
			}
			int64_t roomID = ar.getInt("rid");
			string token = ar.getString("token");
			callback(roomID, token, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(0, "", FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	int32_t RTMClient::_EnterRTCRoom(string& token, RTCRoomType& roomType, int64_t roomID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		
		FPQWriter qw(1, "enterRTCRoom");
		qw.param("rid", roomID);

		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}
		token = ar.wantString("token");
		roomType = (RTCRoomType)ar.getInt("type");

		return FPNN_EC_OK;
	}

	bool RTMClient::_EnterRTCRoom(function<void(string, RTCRoomType, int32_t)> callback, int64_t roomID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback("", InvalidRoom, FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}

		FPQWriter qw(1, "enterRTCRoom");
		qw.param("rid", roomID);

		bool status = client->sendQuest(qw.take(), [callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback("", InvalidRoom, FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback("", InvalidRoom, errorCode);
				return;
			}
			string token = ar.getString("token");
			RTCRoomType roomType = (RTCRoomType)ar.getInt("type");
			callback(token, roomType, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback("", InvalidRoom, FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	int32_t RTMClient::_EnterRTCRoom(int64_t roomID, const string& token, bool& microphone, set<int64_t>& uids, set<int64_t>& administrators, int64_t& owner, int32_t timeout /* = 0 */)
	{
		UDPClientPtr client = GetRTCClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;

		FPQWriter qw(4, "enterRTCRoom");
		qw.param("pid", (int32_t)_pid);
		qw.param("uid", _uid);
		qw.param("rid", roomID);
		qw.param("token", token);

		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}
		microphone = ar.wantBool("microphone");
		uids = ar.want("uids", uids);
		administrators = ar.want("administrators", administrators);
		owner = ar.wantInt("owner");

		return FPNN_EC_OK;
	}

	bool RTMClient::_EnterRTCRoom(function<void(bool, set<int64_t>, set<int64_t>, int64_t, int32_t)> callback, int64_t roomID, const string& token, int32_t timeout /* = 0 */)
	{
		UDPClientPtr client = GetRTCClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(false, set<int64_t>(), set<int64_t>(), 0, FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}

		FPQWriter qw(4, "enterRTCRoom");
		qw.param("pid", (int32_t)_pid);
		qw.param("uid", _uid);
		qw.param("rid", roomID);
		qw.param("token", token);

		bool status = client->sendQuest(qw.take(), [callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(false, set<int64_t>(), set<int64_t>(), 0, FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(false, set<int64_t>(), set<int64_t>(), 0, errorCode);
				return;
			}
			bool microphone = ar.wantBool("microphone");
			set<int64_t> uids = ar.want("uids", set<int64_t>());
			set<int64_t> administrators = ar.want("administrators", set<int64_t>());
			int64_t owner = ar.wantInt("owner");
			callback(microphone, uids, administrators, owner, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(false, set<int64_t>(), set<int64_t>(), 0, FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	int32_t RTMClient::CreateRTCRoom(int64_t roomID, RTCRoomType roomType, int32_t timeout /* = 0 */)
	{
		string token;
		int32_t errorCode = _CreateRTCRoom(token, roomID, roomType, timeout);
		if (errorCode != FPNN_EC_OK)
			return errorCode;
		bool microphone = false;
		set<int64_t> uids;
		set<int64_t> administrators;
		int64_t owner;
		errorCode = _EnterRTCRoom(roomID, token, microphone, uids, administrators, owner, timeout);

		if (errorCode != FPNN_EC_OK)
			return errorCode;

		//todo

		return FPNN_EC_OK;
	}

	bool RTMClient::CreateRTCRoom(function<void(int64_t, int32_t)> callback, int64_t roomID, RTCRoomType roomType, int32_t timeout /* = 0 */)
	{
		bool status = _CreateRTCRoom([this, callback, timeout](int64_t roomID, string token, int32_t errorCode) {
			if (errorCode != FPNN_EC_OK)
			{
				callback(0, errorCode);
				return;
			}

			_EnterRTCRoom([callback, roomID, token](bool microphone, set<int64_t> uids, set<int64_t> administrators, int64_t owner, int32_t errorCode) {
				if (errorCode != FPNN_EC_OK)
				{
					callback(0, errorCode);
					return;
				}
				//todo
				callback(roomID, FPNN_EC_OK);
				}, roomID, token, timeout);
			}, roomID, roomType, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(0, FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return status;
	}

	int32_t RTMClient::EnterRTCRoom(int64_t roomID, int32_t timeout /* = 0 */)
	{
		string token;
		RTCRoomType roomType;
		int32_t errorCode = _EnterRTCRoom(token, roomType, roomID, timeout);
		if (errorCode != FPNN_EC_OK)
			return errorCode;
		bool microphone = false;
		set<int64_t> uids;
		set<int64_t> administrators;
		int64_t owner;
		errorCode = _EnterRTCRoom(roomID, token, microphone, uids, administrators, owner, timeout);
		if (errorCode != FPNN_EC_OK)
			return errorCode;

		//todo
		return FPNN_EC_OK;
	}

	bool RTMClient::EnterRTCRoom(function<void(int64_t, RTCRoomType, int32_t)> callback, int64_t roomID, int32_t timeout /* = 0 */)
	{
		bool status = _EnterRTCRoom([this, callback, roomID, timeout](string token ,RTCRoomType roomType, int32_t errorCode) {
			if (errorCode != FPNN_EC_OK)
			{
				callback(0, InvalidRoom, errorCode);
				return;
			}
			_EnterRTCRoom([this, callback, roomID, roomType](bool microphone, set<int64_t> uids, set<int64_t> administrators, int64_t owner, int32_t errorCode) {
				if (errorCode != FPNN_EC_OK)
				{
					callback(0, InvalidRoom, errorCode);
					return;
				}
				//todo
				callback(roomID, roomType, errorCode);
				}, roomID, token, timeout);
			}, roomID, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(0, InvalidRoom, FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return status;
	}

	int32_t RTMClient::_ExitRTCRoom(int64_t roomID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		
		FPQWriter qw(1, "exitRTCRoom");
		qw.param("rid", roomID);

		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}

		return FPNN_EC_OK;
	}

	bool RTMClient::_ExitRTCRoom(DoneCallback callback, int64_t roomID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}

		FPQWriter qw(1, "exitRTCRoom");
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

	int32_t RTMClient::ExitRTCRoom(int64_t roomID, int32_t timeout /* = 0 */)
	{
		int32_t errorCode = _ExitRTCRoom(roomID, timeout);
		if (errorCode != FPNN_EC_OK)
			return errorCode;
		//todo
		return errorCode;
	}

	bool RTMClient::ExitRTCRoom(DoneCallback callback, int64_t roomID, int32_t timeout /* = 0 */)
	{
		bool status = _ExitRTCRoom([callback](int32_t errorCode) {
			if (errorCode != FPNN_EC_OK)
			{
				callback(errorCode);
				return;
			}
			//todo
			callback(errorCode);
			}, roomID, timeout);
		return status;
	}

	int32_t RTMClient::InviteUserIntoRTCRoom(int64_t roomID, const set<int64_t>& uids, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		
		FPQWriter qw(2, "inviteUserIntoRTCRoom");
		qw.param("rid", roomID);
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

	bool RTMClient::InviteUserIntoRTCRoom(DoneCallback callback, int64_t roomID, const set<int64_t>& uids, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}

		FPQWriter qw(2, "inviteUserIntoRTCRoom");
		qw.param("rid", roomID);
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

	int32_t RTMClient::GetRTCRoomMembers(set<int64_t>& uids, set<int64_t>& administrators, int64_t& owner, int64_t roomID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		
		FPQWriter qw(1, "getRTCRoomMembers");
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
		administrators = ar.want("administrators", administrators);
		owner = ar.wantInt("owner");

		return FPNN_EC_OK;
	}

	bool RTMClient::GetRTCRoomMembers(function<void(set<int64_t>, set<int64_t>, int64_t, int32_t)> callback, int64_t roomID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(set<int64_t>(), set<int64_t>(), 0, FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}

		FPQWriter qw(1, "getRTCRoomMembers");
		qw.param("rid", roomID);

		bool status = client->sendQuest(qw.take(), [callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(set<int64_t>(), set<int64_t>(), 0, FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(set<int64_t>(), set<int64_t>(), 0, errorCode);
				return;
			}
			set<int64_t> uids = ar.want("uids", set<int64_t>());
			set<int64_t> administrators = ar.want("administrators", set<int64_t>());
			int64_t owner = ar.wantInt("owner");
			callback(uids, administrators, owner, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(set<int64_t>(), set<int64_t>(), 0, FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	int32_t RTMClient::GetRTCRoomMemberCount(int32_t& count, int64_t roomID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		
		FPQWriter qw(1, "getRTCRoomMemberCount");
		qw.param("rid", roomID);

		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}
		count = ar.wantInt("count");

		return FPNN_EC_OK;
	}

	bool RTMClient::GetRTCRoomMemberCount(function<void(int32_t, int32_t)> callback, int64_t roomID, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(0, FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}

		FPQWriter qw(1, "getRTCRoomMemberCount");
		qw.param("rid", roomID);

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
			int32_t count = ar.wantInt("count");
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

	int32_t RTMClient::BlockUserVoiceInRTCRoom(int64_t roomID, const set<int64_t>& uids, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		
		FPQWriter qw(2, "blockUserVoiceInRTCRoom");
		qw.param("rid", roomID);
		qw.param("uids", uids);

		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}

		return FPNN_EC_OK;
	}

	bool RTMClient::BlockUserVoiceInRTCRoom(DoneCallback callback, int64_t roomID, const set<int64_t>& uids, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}

		FPQWriter qw(2, "blockUserVoiceInRTCRoom");
		qw.param("rid", roomID);
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

	int32_t RTMClient::UnblockUserVoiceInRTCRoom(int64_t roomID, const set<int64_t>& uids, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		
		FPQWriter qw(2, "unblockUserVoiceInRTCRoom");
		qw.param("rid", roomID);
		qw.param("uids", uids);

		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}

		return FPNN_EC_OK;
	}

	bool RTMClient::UnblockUserVoiceInRTCRoom(DoneCallback callback, int64_t roomID, const set<int64_t>& uids, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}

		FPQWriter qw(2, "unblockUserVoiceInRTCRoom");
		qw.param("rid", roomID);
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

	int32_t RTMClient::SubScribeVideo(int64_t roomID, const set<int64_t>& uids, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		
		FPQWriter qw(2, "subscribeVideo");
		qw.param("rid", roomID);
		qw.param("uids", uids);

		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}

		return FPNN_EC_OK;
	}

	bool RTMClient::SubScribeVideo(DoneCallback callback, int64_t roomID, const set<int64_t>& uids, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}

		FPQWriter qw(2, "subscribeVideo");
		qw.param("rid", roomID);
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

	int32_t RTMClient::UnsubScribeVideo(int64_t roomID, const set<int64_t>& uids, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		
		FPQWriter qw(2, "unsubscribeVideo");
		qw.param("rid", roomID);
		qw.param("uids", uids);

		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}

		return FPNN_EC_OK;
	}

	bool RTMClient::UnsubScribeVideo(DoneCallback callback, int64_t roomID, const set<int64_t>& uids, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}

		FPQWriter qw(2, "unsubscribeVideo");
		qw.param("rid", roomID);
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

	int32_t RTMClient::AdminCommand(int64_t roomID, const set<int64_t>& uids, AdministratorCommand command, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		
		FPQWriter qw(3, "adminCommand");
		qw.param("rid", roomID);
		qw.param("uids", uids);
		qw.param("command", (int32_t)command);

		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}

		return FPNN_EC_OK;
	}

	bool RTMClient::AdminCommand(DoneCallback callback, int64_t roomID, const set<int64_t>& uids, AdministratorCommand command, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}

		FPQWriter qw(3, "adminCommand");
		qw.param("rid", roomID);
		qw.param("uids", uids);
		qw.param("command", (int32_t)command);

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

	bool RTMClient::Voice(const vector<char>& data, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return false;
		
		//todo
		FPQWriter qw(4, "voice", true);
		qw.param("seq", 0); //todo
		qw.param("rid", 0); //todo
		qw.param("timestamp", 0); //todo
		qw.param("data", data);

		client->sendQuest(qw.take());
		return true;
	}

	bool RTMClient::Video(int64_t seq, int64_t roomID, int64_t flags, int64_t timestamp, int64_t rotation, int64_t version, int32_t facing, int32_t captureLevel, const vector<char>& data, const vector<char>& sps, const vector<char>& pps, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return false;
		
		FPQWriter qw(3, "video", true);
		qw.param("seq", seq);
		qw.param("rid", roomID);
		qw.param("flags", flags);
		qw.param("timestamp", timestamp);
		qw.param("rotation", rotation);
		qw.param("version", version);
		qw.param("facing", facing);
		qw.param("captureLevel", captureLevel);
		qw.param("data", data);
		qw.param("sps", sps);
		qw.param("pps", pps);

		client->sendQuest(qw.take());
		return true;
	}

	bool RTMClient::AdjustTime(function<void(int64_t, int32_t)> callback, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(0, FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}

		bool status = client->sendQuest(FPQWriter::emptyQuest("adjustTime"), [callback](FPAnswerPtr answer, int errorCode) {
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
			int64_t ts = ar.wantInt("ts");
			callback(ts, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(0, FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}
}