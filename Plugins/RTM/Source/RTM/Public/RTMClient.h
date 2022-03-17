// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TCPClient.h"
#include "UDPClient.h"
#include "RTMStructure.h"
#include "RTMMidGenerator.h"
#include "RTMErrorCode.h"
#include "RTMAudioData.h"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include "md5.h"
#include "hex.h"


/**
 *
 */
namespace rtm
{
	using namespace fpnn;
	using namespace std;

	typedef function<void(int64_t, int64_t, bool, int32_t)> AuthCallback;
	typedef function<void(int64_t, int32_t)> MessageIDCallback;
	typedef function<void(HistoryMessageResult, int32_t)> HistoryMessageCallback;
	typedef function<void(int32_t)> DoneCallback;

	class RTMProcessor;
	class RTCProcessor;
	class RTMQuestProcessor;
	typedef shared_ptr<RTMQuestProcessor> RTMQuestProcessorPtr;

	enum class ClientStatus
	{
		Closed,
		Connecting,
		Connected
	};

	struct AuthStatusInfo
	{
		AuthStatusInfo()
		{
			remainedTimeout = 0;
			lastActionMsecTimeStamp = 0;
		}
		vector<AuthCallback> callbackList;
		string token;
		map<string, string> attrs;
		string language;
		int32_t remainedTimeout;
		int64_t lastActionMsecTimeStamp;

		void Clear()
		{
			callbackList.clear();
			token.clear();
			attrs.clear();
			language.clear();
			remainedTimeout = 0;
			lastActionMsecTimeStamp = 0;
		}
		
	};

	struct AutoReloginInfo
	{
		AutoReloginInfo()
		{
			disabled = true;
			canRelogin = false;
			reloginCount = 0;
			lastErrorCode = 0;
			lastReloginMS = 0;
		}

		bool disabled;
		bool canRelogin;
		int32_t reloginCount;
		int32_t lastErrorCode;
		int64_t lastReloginMS;
		string token;
		map<string, string> attrs;
		string language;

		void Login()
		{
			if (disabled)
			{
				disabled = false;
				reloginCount = 0;
				lastErrorCode = 0;
			}
			else if (canRelogin)
			{
				reloginCount += 1;
			}
			
			lastReloginMS = TimeUtil::curr_msec();
		}

		void LoginSuccessful()
		{
			canRelogin = true;
			reloginCount = 0;
			lastReloginMS = 0;
		}

		void Disable()
		{
			disabled = true;
			canRelogin = false;
		}
	};
	class RTMClient;
	typedef shared_ptr<RTMClient> RTMClientPtr;

	class RTM_API RTMClient : public enable_shared_from_this<RTMClient>
	{
	private:
		RTMClient(int64_t pid, int64_t uid, const string& endpoint, const string& rtcEndpoint = "", bool autoRelogin = true);
		void Init(RTMQuestProcessorPtr rtmQuestProcessor);
	public:
		static RTMClientPtr CreateRTMClient(int64_t pid, int64_t uid, const string& endpoint, RTMQuestProcessorPtr rtmQuestProcessor, bool autoRelogin = true);
		~RTMClient();
		int32_t Login(bool& ok, const string& token, const map<string, string>& attrs = map<string, string>(), const string& language = "", int32_t timeout = 0);
		bool Login(AuthCallback callback, const string& token, const map<string, string>& attrs = map<string, string>(), const string& language = "", int32_t timeout = 0);
		void StartRelogin();
		void StartNextRelogin();
		bool StopRelogin(int32_t errorCode);
		void Auth();
		void AuthFinish(bool successful, int32_t errorCode);
		void Bye(bool async = true);
		void Connected(uint64_t connectionID, bool connected);
		void Closed(uint64_t connectionID, bool closedByError);
		void Close(bool disableRelogin = true, bool waitConnectingCancelled = true);
		bool IsAlive();
		int64_t GetPid() { return _pid; }
		int64_t GetUid() { return _uid; }
		ClientStatus GetClientStatus() { return _clientStatus; }
		bool RequireClose() { return _requireClose; }

		TCPClientPtr GetRTMClient();
		UDPClientPtr GetRTCClient();

		//===============[ System ]===============

		int32_t AddAttributes(const map<string, string>& attrs, int32_t timeout = 0);
		bool AddAttributes(DoneCallback callback, const map<string, string>& attrs, int32_t timeout = 0);

		int32_t GetAttributes(map<string, string>& attrs, int32_t timeout = 0);
		bool GetAttributes(function<void(map<string, string>, int32_t)> callback, int32_t timeout = 0);

		int32_t AddDebugLog(const string& message, const string& attrs, int32_t timeout = 0);
		bool AddDebugLog(DoneCallback callback, const string& message, const string& attrs, int32_t timeout = 0);
		//===============[ Message ]===============
		int32_t SendMessage(int64_t& messageID, int64_t uid, int8_t messageType, const string& message, const string& attrs = "", int32_t timeout = 0);
		bool SendMessage(MessageIDCallback callback, int64_t uid, int8_t messageType, const string& message, const string& attrs = "", int32_t timeout = 0);

		int32_t SendGroupMessage(int64_t& messageID, int64_t groupID, int8_t messageType, const string& message, const string& attrs = "", int32_t timeout = 0);
		bool SendGroupMessage(MessageIDCallback callback, int64_t groupID, int8_t messageType, const string& message, const string& attrs = "", int32_t timeout = 0);

		int32_t SendRoomMessage(int64_t& messageID, int64_t roomID, int8_t messageType, const string& message, const string& attrs = "", int32_t timeout = 0);
		bool SendRoomMessage(MessageIDCallback callback, int64_t roomID, int8_t messageType, const string& message, const string& attrs = "", int32_t timeout = 0);
		
		int32_t GetGroupMessage(HistoryMessageResult& result, int64_t groupID, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, const vector<int8_t>& messageTypes = vector<int8_t>(), int32_t timeout = 0);
		bool GetGroupMessage(HistoryMessageCallback callback, int64_t groupID, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, const vector<int8_t>& messageTypes = vector<int8_t>(), int32_t timeout = 0);

		int32_t GetRoomMessage(HistoryMessageResult& result, int64_t roomID, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, const vector<int8_t>& messageTypes = vector<int8_t>(), int32_t timeout = 0);
		bool GetRoomMessage(HistoryMessageCallback callback, int64_t roomID, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, const vector<int8_t>& messageTypes = vector<int8_t>(), int32_t timeout = 0);

		int32_t GetBroadcastMessage(HistoryMessageResult& result, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, const vector<int8_t>& messageTypes = vector<int8_t>(), int32_t timeout = 0);
		bool GetBroadcastMessage(HistoryMessageCallback callback,bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, const vector<int8_t>& messageTypes = vector<int8_t>(), int32_t timeout = 0);

		int32_t GetP2PMessage(HistoryMessageResult& result, int64_t uid, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, const vector<int8_t>& messageTypes = vector<int8_t>(), int32_t timeout = 0);
		bool GetP2PMessage(HistoryMessageCallback callback, int64_t uid, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, const vector<int8_t>& messageTypes = vector<int8_t>(), int32_t timeout = 0);

		int32_t GetMessage(RetrievedMessage& result, int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout = 0);
		bool GetMessage(function<void(RetrievedMessage, int32_t)> callback, int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout = 0);

		int32_t DeleteMessage(int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout = 0);
		bool DeleteMessage(DoneCallback callback, int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout = 0);

		//===============[ Chat ]===============
		int32_t SendChat(int64_t& messageID, int64_t uid, const string& message, const string& attrs = "", int32_t timeout = 0);
		bool SendChat(MessageIDCallback callback, int64_t uid, const string& message, const string& attrs = "", int32_t timeout = 0);
		
		int32_t SendGroupChat(int64_t& messageID, int64_t groupID, const string& message, const string& attrs = "", int32_t timeout = 0);
		bool SendGroupChat(MessageIDCallback callback, int64_t groupID, const string& message, const string& attrs = "", int32_t timeout = 0);

		int32_t SendRoomChat(int64_t& messageID, int64_t roomID, const string& message, const string& attrs = "", int32_t timeout = 0);
		bool SendRoomChat(MessageIDCallback callback, int64_t roomID, const string& message, const string& attrs = "", int32_t timeout = 0);

		int32_t SendCommand(int64_t& messageID, int64_t uid, const string& message, const string& attrs = "", int32_t timeout = 0);
		bool SendCommand(MessageIDCallback callback, int64_t uid, const string& message, const string& attrs = "", int32_t timeout = 0);
		
		int32_t SendGroupCommand(int64_t& messageID, int64_t groupID, const string& message, const string& attrs = "", int32_t timeout = 0);
		bool SendGroupCommand(MessageIDCallback callback, int64_t groupID, const string& message, const string& attrs = "", int32_t timeout = 0);

		int32_t SendRoomCommand(int64_t& messageID, int64_t roomID, const string& message, const string& attrs = "", int32_t timeout = 0);
		bool SendRoomCommand(MessageIDCallback callback, int64_t roomID, const string& message, const string& attrs = "", int32_t timeout = 0);

		int32_t GetGroupChat(HistoryMessageResult& result, int64_t groupID, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, int32_t timeout = 0);
		bool GetGroupChat(HistoryMessageCallback callback, int64_t groupID, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, int32_t timeout = 0);

		int32_t GetRoomChat(HistoryMessageResult& result, int64_t roomID, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, int32_t timeout = 0);
		bool GetRoomChat(HistoryMessageCallback callback, int64_t roomID, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, int32_t timeout = 0);

		int32_t GetBroadcastChat(HistoryMessageResult& result, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, int32_t timeout = 0);
		bool GetBroadcastChat(HistoryMessageCallback callback,bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, int32_t timeout = 0);

		int32_t GetP2PChat(HistoryMessageResult& result, int64_t uid, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, int32_t timeout = 0);
		bool GetP2PChat(HistoryMessageCallback callback, int64_t uid, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, int32_t timeout = 0);

		int32_t GetChat(RetrievedMessage& result, int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout = 0);
		bool GetChat(function<void(RetrievedMessage, int32_t)> callback, int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout = 0);

		int32_t DeleteChat(int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout = 0);
		bool DeleteChat(DoneCallback callback, int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout = 0);

		int32_t GetUnread(vector<int64_t>& p2pList, vector<int64_t>& groupList, bool clear = false, int32_t timeout = 0);
		bool GetUnread(function<void(vector<int64_t>, vector<int64_t>, int32_t)> callback, bool clear = false, int32_t timeout = 0);

		int32_t ClearUnread(int32_t timeout = 0);
		bool ClearUnread(DoneCallback callback, int32_t timeout = 0);

		int32_t GetP2PUnread(map<int64_t, int32_t>& unreadMap, map<int64_t, int64_t>& lastUnreadTimestampMap, const set<int64_t>& uids, int64_t startTime = 0, const set<int8_t>& messageTypes = set<int8_t>(), int32_t timeout = 0);
		bool GetP2PUnread(function<void(map<int64_t, int32_t>, map<int64_t, int64_t>, int32_t)> callback, const set<int64_t>& uids, int64_t startTime = 0, const set<int8_t>& messageTypes = set<int8_t>(), int32_t timeout = 0);

		int32_t GetGroupUnread(map<int64_t, int32_t>& unreadMap, map<int64_t, int64_t>& lastUnreadTimestampMap, const set<int64_t>& groupIDs, int64_t startTime = 0, const set<int8_t>& messageTypes = set<int8_t>(), int32_t timeout = 0);
		bool GetGroupUnread(function<void(map<int64_t, int32_t>, map<int64_t, int64_t>, int32_t)> callback, const set<int64_t>& groupIDs, int64_t startTime = 0, const set<int8_t>& messageTypes = set<int8_t>(), int32_t timeout = 0);

		int32_t GetSession(set<int64_t>& p2pSet, set<int64_t>& groupSet, int32_t timeout = 0);
		bool GetSession(function<void(set<int64_t>, set<int64_t>, int32_t)> callback, int32_t timeout = 0);

		//===============[ Data ]===============
		int32_t DataGet(string& value, const string& key, int32_t timeout = 0);
		bool DataGet(function<void(string, int32_t)> callback, const string& key, int32_t timeout = 0);

		int32_t DataSet(const string& key, const string& value, int32_t timeout = 0);
		bool DataSet(DoneCallback callback, const string& key, const string& value, int32_t timeout = 0);

		int32_t DataDelete(const string& key, int32_t timeout = 0);
		bool DataDelete(DoneCallback callback, const string& key, int32_t timeout = 0);

		//===============[ Friend ]===============
		int32_t AddFriends(const set<int64_t>& uids, int32_t timeout = 0);
		bool AddFriends(DoneCallback callback, const set<int64_t>& uids, int32_t timeout = 0);

		int32_t DeleteFriends(const set<int64_t>& uids, int32_t timeout = 0);
		bool DeleteFriends(DoneCallback callback, const set<int64_t>& uids, int32_t timeout = 0);

		int32_t GetFriends(set<int64_t>& friends, int32_t timeout = 0);
		bool GetFriends(function<void(set<int64_t>, int32_t)> callback, int32_t timeout = 0);

		int32_t AddBlackList(const set<int64_t>& uids, int32_t timeout = 0);
		bool AddBlackList(DoneCallback callback, const set<int64_t>& uids, int32_t timeout = 0);

		int32_t DeleteBlackList(const set<int64_t>& uids, int32_t timeout = 0);
		bool DeleteBlackList(DoneCallback callback, const set<int64_t>& uids, int32_t timeout = 0);

		int32_t GetBlackList(set<int64_t>& uids, int32_t timeout = 0);
		bool GetBlackList(function<void(set<int64_t>, int32_t)> callback, int32_t timeout = 0);

		//===============[ Group ]===============
		int32_t AddGroupMembers(int64_t groupID, const set<int64_t>& uids, int32_t timeout = 0);
		bool AddGroupMembers(DoneCallback callback, int64_t groupID, const set<int64_t>& uids, int32_t timeout = 0);

		int32_t DeleteGroupMembers(int64_t groupID, const set<int64_t>& uids, int32_t timeout = 0);
		bool DeleteGroupMembers(DoneCallback callback, int64_t groupID, const set<int64_t>& uids, int32_t timeout = 0);

		int32_t GetGroupMembers(set<int64_t>& uids, int64_t groupID, int32_t timeout = 0);
		bool GetGroupMembers(function<void(set<int64_t>, int32_t)> callback, int64_t groupID, int32_t timeout = 0);

		int32_t GetGroupMembers(set<int64_t>& allUIDs, set<int64_t>& onlineUIDs, int64_t groupID, int32_t timeout = 0);
		bool GetGroupMembers(function<void(set<int64_t>, set<int64_t>, int32_t)> callback, int64_t groupID, int32_t timeout = 0);

		int32_t GetGroupCount(int32_t& memberCount, int64_t groupID, int32_t timeout = 0);
		bool GetGroupCount(function<void(int32_t, int32_t)> callback, int64_t groupID, int32_t timeout = 0);

		int32_t GetGroupCount(int32_t& memberCount, int32_t& onlineCount, int64_t groupID, int32_t timeout = 0);
		bool GetGroupCount(function<void(int32_t, int32_t, int32_t)> callback, int64_t groupID, int32_t timeout = 0);

		int32_t GetUserGroups(set<int64_t>& groupIDs, int32_t timeout = 0);
		bool GetUserGroups(function<void(set<int64_t>, int32_t)> callback, int32_t timeout = 0);

		int32_t SetGroupInfo(int64_t groupID, const string& publicInfo = "", const string& privateInfo = "", int32_t timeout = 0);
		bool SetGroupInfo(DoneCallback callback, int64_t groupID, const string& publicInfo = "", const string& privateInfo = "", int32_t timeout = 0);

		int32_t GetGroupInfo(string& publicInfo, string& privateInfo, int64_t groupID, int32_t timeout = 0);
		bool GetGroupInfo(function<void(string, string, int32_t)> callback, int64_t groupID, int32_t timeout = 0);

		int32_t GetGroupPublicInfo(string& publicInfo, int64_t groupID, int32_t timeout = 0);
		bool GetGroupPublicInfo(function<void(string, int32_t)> callback, int64_t groupID, int32_t timeout = 0);

		int32_t  GetGroupsPublicInfo(map<int64_t, string>& publicInfos, const set<int64_t>& groupIDs, int32_t timeout = 0);
		bool GetGroupsPublicInfo(function<void(map<int64_t, string>, int32_t)> callback, const set<int64_t>& groupIDs, int32_t timeout = 0);

		//===============[ Group ]===============
		int32_t EnterRoom(int64_t roomID, int32_t timeout = 0);
		bool EnterRoom(DoneCallback callback, int64_t roomID, int32_t timeout = 0);

		int32_t LeaveRoom(int64_t roomID, int32_t timeout = 0);
		bool LeaveRoom(DoneCallback callback, int64_t roomID, int32_t timeout = 0);

		int32_t GetUserRooms(set<int64_t>& roomIDs, int32_t timeout = 0);
		bool GetUserRooms(function<void(set<int64_t>, int32_t)> callback, int32_t timeout = 0);

		int32_t GetRoomMembers(set<int64_t>& uids, int64_t roomID, int32_t timeout = 0);
		bool GetRoomMembers(function<void(set<int64_t>, int32_t)> callback, int64_t roomID, int32_t timeout = 0);

		int32_t GetRoomMemberCount(map<int64_t, int32_t>& counts, const set<int64_t>& roomIDs, int32_t timeout = 0);
		bool GetRoomMemberCount(function<void(map<int64_t, int32_t>, int32_t)> callback, const set<int64_t>& roomIDs, int32_t timeout = 0);

		int32_t SetRoomInfo(int64_t roomID, const string& publicInfo = "", const string& privateInfo = "", int32_t timeout = 0);
		bool SetRoomInfo(DoneCallback callback, int64_t roomID, const string& publicInfo = "", const string& privateInfo = "", int32_t timeout = 0);

		int32_t GetRoomInfo(string& publicInfo, string& privateInfo, int64_t roomID, int32_t timeout = 0);
		bool GetRoomInfo(function<void(string, string, int32_t)> callback, int64_t roomID, int32_t timeout = 0);

		int32_t GetRoomPublicInfo(string& publicInfo, int64_t roomID, int32_t timeout = 0);
		bool GetRoomPublicInfo(function<void(string, int32_t)> callback, int64_t roomID, int32_t timeout = 0);

		int32_t GetRoomsPublicInfo(map<int64_t, string>& publicInfos, const set<int64_t>& roomIDs, int32_t timeout = 0);
		bool GetRoomsPublicInfo(function<void(map<int64_t, string>, int32_t)> callback, const set<int64_t>& roomIDs, int32_t timeout = 0);
		//===============[ Device ]===============
		int32_t AddDevice(const string& appType, const string& deviceToken, int32_t timeout = 0);
		bool AddDevice(DoneCallback callback, const string& appType, const string& deviceToken, int32_t timeout = 0);

		int32_t RemoveDevice(const string& deviceToken, int32_t timeout = 0);
		bool RemoveDevice(DoneCallback callback, const string& deviceToken, int32_t timeout = 0);
	
		int32_t AddDevicePushOption(MessageCategory messageCategory, int64_t targetID, set<int8_t> messageTypes = set<int8_t>(), int32_t timeout = 0);
		bool AddDevicePushOption(DoneCallback callback, MessageCategory messageCategory, int64_t targetID, set<int8_t> messageTypes = set<int8_t>(), int32_t timeout = 0);

		int32_t RemoveDevicePushOption(MessageCategory messageCategory, int64_t targetID, set<int8_t> messageTypes = set<int8_t>(), int32_t timeout = 0);
		bool RemoveDevicePushOption(DoneCallback callback, MessageCategory messageCategory, int64_t targetID, set<int8_t> messageTypes = set<int8_t>(), int32_t timeout = 0);

		int32_t GetDevicePushOption(map<int64_t, set<int8_t>>& p2pMap, map<int64_t, set<int8_t>>& groupMap, int32_t timeout = 0);
		bool GetDevicePushOption(function<void(map<int64_t, set<int8_t>>, map<int64_t, set<int8_t>>, int32_t)> callback, int32_t timeout = 0);

		//===============[ User ]===============
		int32_t GetOnlineUsers(set<int64_t>& onlineUIDs, const set<int64_t>& uids, int32_t timeout = 0);
		bool GetOnlineUsers(function<void(set<int64_t>, int32_t)> callback, const set<int64_t>& uids, int32_t timeout = 0);

		int32_t SetUserInfo(const string& publicInfo = "", const string& privateInfo = "", int32_t timeout = 0);
		bool SetUserInfo(DoneCallback callback, const string& publicInfo = "", const string& privateInfo = "", int32_t timeout = 0);

		int32_t GetUserInfo(string& publicInfo, string& privateInfo, int32_t timeout = 0);
		bool GetUserInfo(function<void(string, string, int32_t)> callback, int32_t timeout = 0);

		int32_t GetUserPublicInfo(map<int64_t, string>& publicInfos, const set<int64_t>& uids, int32_t timeout = 0);
		bool GetUserPublicInfo(function<void(map<int64_t, string>, int32_t)> callback, const set<int64_t>& uids, int32_t timeout = 0);

		//===============[ Value Added ]===============
		int32_t SetTranslatedLanguage(const string& targetLanguage, int32_t timeout = 0);
		bool SetTranslatedLanguage(DoneCallback callback, const string& targetLanguage, int32_t timeout = 0);

		int32_t Translate(TranslatedInfo& translatedInfo, const string& text, const string& destinationLanguage, const string& sourceLanguage = "", TranslateType type = Chat, ProfanityType profanity = Off, int32_t timeout = 0);
		bool Translate(function<void(TranslatedInfo, int32_t)> callback, const string& text, const string& destinationLanguage, const string& sourceLanguage = "", TranslateType type = Chat, ProfanityType profanity = Off, int32_t timeout = 0);

		int32_t SpeechToText(string& resultText, string& resultLanguage, const string& audio, const string& language, const string& codec = "", int32_t sampleRate = 0, int32_t timeout = 120);
		bool SpeechToText(function<void(string, string, int32_t)> callback, const string& audio, const string& language, const string& codec = "", int32_t sampleRate = 0, int32_t timeout = 120);

		int32_t TextCheck(TextCheckResult& result, const string& text, int32_t timeout = 120);
		bool TextCheck(function<void(TextCheckResult, int32_t)> callback, const string& text, int32_t timeout = 120);

		int32_t ImageCheck(CheckResult& result, const string& image, CheckType type, int32_t timeout = 120);
		bool ImageCheck(function<void(CheckResult, int32_t)> callback, const string& image, CheckType type, int32_t timeout = 120);

		int32_t AudioCheck(CheckResult& result, const string& audio, CheckType type, const string& language, const string& codec = "", int32_t sampleRate = 0, int32_t timeout = 120);
		bool AudioCheck(function<void(CheckResult, int32_t)> callback, const string& audio, CheckType type, const string& language, const string& codec = "", int32_t sampleRate = 0, int32_t timeout = 120);

		int32_t VideoCheck(CheckResult& result, const string& video, CheckType type, const string& videoName, int32_t timeout = 120);
		bool VideoCheck(function<void(CheckResult, int32_t)> callback, const string& video, CheckType type, const string& videoName, int32_t timeout = 120);

		//===============[ File ]===============
		int32_t SendFile(int64_t& messageID, int64_t uid, ChatMessageType type, const string& fileContent, const string& fileName, const string& fileExtension = "", const string& attrs = "", int32_t timeout = 120);
		bool SendFile(MessageIDCallback callback, int64_t uid, ChatMessageType type, const string& fileContent, const string& fileName, const string& fileExtension = "", const string& attrs = "", int32_t timeout = 120);

		int32_t SendGroupFile(int64_t& messageID, int64_t groupID, ChatMessageType type, const string& fileContent, const string& fileName, const string& fileExtension = "", const string& attrs = "", int32_t timeout = 120);
		bool SendGroupFile(MessageIDCallback callback, int64_t groupID, ChatMessageType type, const string& fileContent, const string& fileName, const string& fileExtension = "", const string& attrs = "", int32_t timeout = 120);

		int32_t SendRoomFile(int64_t& messageID, int64_t roomID, ChatMessageType type, const string& fileContent, const string& fileName, const string& fileExtension = "", const string& attrs = "", int32_t timeout = 120);
		bool SendRoomFile(MessageIDCallback callback, int64_t roomID, ChatMessageType type, const string& fileContent, const string& fileName, const string& fileExtension = "", const string& attrs = "", int32_t timeout = 120);

		//===============[ Audio ]===============
		map<string, string> BuildAudioMessageAttrs(const RTMAudioData& audioData);

		int32_t SendFile(int64_t& messageID, int64_t uid, const RTMAudioData& audioData, const string& attrs = "", int32_t timeout = 120);
		bool SendFile(MessageIDCallback callback, int64_t uid, const RTMAudioData& audioData, const string& attrs = "", int32_t timeout = 120);

		int32_t SendGroupFile(int64_t& messageID, int64_t groupID, const RTMAudioData& audioData, const string& attrs = "", int32_t timeout = 120);
		bool SendGroupFile(MessageIDCallback callback, int64_t groupID, const RTMAudioData& audioData, const string& attrs = "", int32_t timeout = 120);

		int32_t SendRoomFile(int64_t& messageID, int64_t roomID, const RTMAudioData& audioData, const string& attrs = "", int32_t timeout = 120);
		bool SendRoomFile(MessageIDCallback callback, int64_t roomID, const RTMAudioData& audioData, const string& attrs = "", int32_t timeout = 120);

		//===============[ RTC ]===============
		int32_t CreateRTCRoom(int64_t roomID, RTCRoomType roomType, int32_t timeout = 0);
		bool CreateRTCRoom(function<void(int64_t, int32_t)> callback, int64_t roomID, RTCRoomType roomType, int32_t timeout = 0);

		int32_t EnterRTCRoom(int64_t roomID, int32_t timeout = 0);
		bool EnterRTCRoom(function<void(int64_t, RTCRoomType, int32_t)> callback, int64_t roomID, int32_t timeout = 0);

		int32_t ExitRTCRoom(int64_t roomID, int32_t timeout = 0);
		bool ExitRTCRoom(DoneCallback callback, int64_t roomID, int32_t timeout = 0);

		int32_t InviteUserIntoRTCRoom(int64_t roomID, const set<int64_t>& uids, int32_t timeout = 0);
		bool InviteUserIntoRTCRoom(DoneCallback callback, int64_t roomID, const set<int64_t>& uids, int32_t timeout = 0);

		int32_t GetRTCRoomMembers(set<int64_t>& uids, set<int64_t>& administrators, int64_t& owner, int64_t roomID, int32_t timeout = 0);
		bool GetRTCRoomMembers(function<void(set<int64_t>, set<int64_t>, int64_t, int32_t)> callback, int64_t roomID, int32_t timeout = 0);

		int32_t GetRTCRoomMemberCount(int32_t& count, int64_t roomID, int32_t timeout = 0);
		bool GetRTCRoomMemberCount(function<void(int32_t, int32_t)> callback, int64_t roomID, int32_t timeout = 0);

		int32_t BlockUserVoiceInRTCRoom(int64_t roomID, const set<int64_t>& uids, int32_t timeout = 0);
		bool BlockUserVoiceInRTCRoom(DoneCallback callback, int64_t roomID, const set<int64_t>& uids, int32_t timeout = 0);

		int32_t UnblockUserVoiceInRTCRoom(int64_t roomID, const set<int64_t>& uids, int32_t timeout = 0);
		bool UnblockUserVoiceInRTCRoom(DoneCallback callback, int64_t roomID, const set<int64_t>& uids, int32_t timeout = 0);

		int32_t SubScribeVideo(int64_t roomID, const set<int64_t>& uids, int32_t timeout = 0);
		bool SubScribeVideo(DoneCallback callback, int64_t roomID, const set<int64_t>& uids, int32_t timeout = 0);

		int32_t UnsubScribeVideo(int64_t roomID, const set<int64_t>& uids, int32_t timeout = 0);
		bool UnsubScribeVideo(DoneCallback callback, int64_t roomID, const set<int64_t>& uids, int32_t timeout = 0);

		int32_t AdminCommand(int64_t roomID, const set<int64_t>& uids, AdministratorCommand command, int32_t timeout = 0);
		bool AdminCommand(DoneCallback callback, int64_t roomID, const set<int64_t>& uids, AdministratorCommand command, int32_t timeout = 0);

		bool Voice(const vector<char>& data, int32_t timeout = 0);
		bool Video(int64_t seq, int64_t roomID, int64_t flags, int64_t timestamp, int64_t rotation, int64_t version, int32_t facing, int32_t captureLevel, const vector<char>& data, const vector<char>& sps, const vector<char>& pps, int32_t timeout = 0);
		bool AdjustTime(function<void(int64_t, int32_t)> callback, int32_t timeout = 0);

		static bool CheckFileMessageType(int8_t messageType);
		static void BuildFileInfo(RTMMessage& message);
		static bool ipv4EndpointToipv6(const string& ipv4, string& ip, int32_t& port);
		static int8_t GetPushOptionType(MessageCategory messageCategory);
	private:
		int32_t FileToken(string& token, string& endpoint, FileTokenType tokenType, int64_t xid, int32_t timeout = 0);
		bool FileToken(function<void(string, string, int32_t)> callback, FileTokenType tokenType, int64_t xid, int32_t timeout = 0);

	private:
		FPQuestPtr GenAuthQuest(const string& token);
		FPQuestPtr GenSendMessageQuest(int64_t uid, int8_t messageType, const string& message, const string& attrs, int64_t& messageID);
		FPQuestPtr GenSendGroupMessageQuest(int64_t groupID, int8_t messageType, const string& message, const string& attrs, int64_t& messageID);
		FPQuestPtr GenSendRoomMessageQuest(int64_t roomID, int8_t messageType, const string& message, const string& attrs, int64_t& messageID);
		FPQuestPtr GenGetGroupMessageQuest(int64_t groupID, bool desc, int32_t count, int64_t beginMsec, int64_t endMsec, int64_t lastID, vector<int8_t> messageTypes);
		FPQuestPtr GenGetRoomMessageQuest(int64_t roomID, bool desc, int32_t count, int64_t beginMsec, int64_t endMsec, int64_t lastID, vector<int8_t> messageTypes);
		FPQuestPtr GenGetBroadcastMessageQuest(bool desc, int32_t count, int64_t beginMsec, int64_t endMsec, int64_t lastID, vector<int8_t> messageTypes);
		FPQuestPtr GenGetP2PMessageQuest(int64_t uid, bool desc, int32_t count, int64_t beginMsec, int64_t endMsec, int64_t lastID, vector<int8_t> messageTypes);
		FPQuestPtr GenGetMessageQuest(int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory);
		FPQuestPtr GenDeleteMessageQuest(int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory);
		FPQuestPtr GenGetP2PUnreadQuest(const set<int64_t>& uids, int64_t startTime, const set<int8_t>& messageTypes);
		FPQuestPtr GenGetGroupUnreadQuest(const set<int64_t>& groupIDs, int64_t startTime, const set<int8_t>& messageTypes);
		FPQuestPtr GenTranslateQuest(const string& text, const string& destinationLanguage, const string& sourceLanguage = "", TranslateType type = Chat, ProfanityType profanity = Off);
		FPQuestPtr GenSpeechToTextQuest(const string& audio, CheckType type, const string& language, const string& codec = "", int32_t sampleRate = 0);
		FPQuestPtr GenAudioCheckQuest(const string& audio, CheckType type, const string& language, const string& codec = "", int32_t sampleRate = 0);

		void GetHistoryMessageResult(HistoryMessageResult& result, FPAnswerPtr answer, int64_t toID);
		void AdjustHistoryMessageResultForP2PMessage(int64_t selfUid, int64_t peerUid, HistoryMessageResult& result);
		void BuildRetrievedMessage(RetrievedMessage* message, FPAnswerPtr answer);

		//=============== [File] ===============
		static void ParseFileMessage(RTMMessage& message);
		static void ParseFileAttrs(RTMMessage& message);
		static void UpdateFileGateTimeout(int32_t& timeout, int64_t& lastActionTimestamp);
		FPQuestPtr GenSendFileQuest(int64_t& messageID, const string& token, FileTokenType tokenType, int64_t targetID, int8_t messageType, const string& fileContent, const string& fileName, const string& fileExtension, const string attrs, const map<string, string>& rtmAttrs);
		int32_t RealSendFile(int64_t& messageID, FileTokenType tokenType, int64_t targetID, int8_t messageType, const string& fileContent, const string& fileName, const string& fileExtension = "", const string& attrs = "", const map<string, string>& rtmAttrs = map<string, string>(), int32_t timeout = 120);
		bool RealSendFile(MessageIDCallback callback, FileTokenType tokenType, int64_t targetID, int8_t messageType, const string& fileContent, const string& fileName, const string& fileExtension = "", const string attrs = "", const map<string, string> rtmAttrs = map<string, string>(), int32_t timeout = 120);

		int32_t FileGateConnectIPv4Sync(TCPClientPtr fileGate, const string& endpoint, int32_t& timeout, int64_t& lastActionTimestamp);
		int32_t FileGateConnectIPv6Sync(TCPClientPtr fileGate, const string& ip, int32_t port, int32_t& timeout, int64_t& lastActionTimestamp);
		void FileGateConnectIPv4Async(DoneCallback callback, TCPClientPtr fileGate, const string& endpoint, int32_t& timeout, int64_t& lastActionTimestamp);
		void FileGateConnectIPv6Async(DoneCallback callback, TCPClientPtr fileGate, const string& ip, int32_t port, int32_t& timeout, int64_t& lastActionTimestamp);

		string CalcMD5(const string& content);

		//=============== [RTC] ===============
		int32_t _CreateRTCRoom(string& token, int64_t roomID, RTCRoomType roomType, int32_t timeout = 0);
		bool _CreateRTCRoom(function<void(int64_t, string, int32_t)> callback, int64_t roomID, RTCRoomType roomType, int32_t timeout = 0);

		int32_t _EnterRTCRoom(string& token, RTCRoomType& roomType, int64_t roomID, int32_t timeout = 0);
		bool _EnterRTCRoom(function<void(string, RTCRoomType, int32_t)> callback, int64_t roomID, int32_t timeout = 0);

		int32_t _EnterRTCRoom(int64_t roomID, const string& token, bool& microphone, set<int64_t>& uids, set<int64_t>& administrators, int64_t& owner, int32_t timeout = 0);
		bool _EnterRTCRoom(function<void(bool, set<int64_t>, set<int64_t>, int64_t, int32_t)> callback, int64_t roomID, const string& token, int32_t timeout = 0);

		int32_t _ExitRTCRoom(int64_t roomID, int32_t timeout = 0);
		bool _ExitRTCRoom(DoneCallback callback, int64_t roomID, int32_t timeout = 0);
	private:
		TCPClientPtr _rtmGate;
		UDPClientPtr _rtcGate;
		atomic<uint64_t> _rtmGateConnectionID;
		int64_t _pid;
		int64_t _uid;
		string _endpoint;
		mutex _mutex;
		mutex _loginMutex;
		unique_lock<mutex> _loginLocker;
		ClientStatus _clientStatus;
		atomic<bool> _requireClose;
		shared_ptr<RTMProcessor> _rtmQuestProcessor;
		shared_ptr<RTCProcessor> _rtcQuestProcessor;
		AutoReloginInfo* _autoReloginInfo;
		AuthStatusInfo* _authStatusInfo;
		condition_variable _condition;

		const vector<int8_t> _chatMessageTypes = {TextChatMType, CmdChatMType, ImageFileType, AudioFileType, VideoFileType, NormalFileType};
	};
	typedef shared_ptr<RTMClient> RTMClientPtr;
}
