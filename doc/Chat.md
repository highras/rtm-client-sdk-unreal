# RTM Client UE4 SDK Chat API Docs

# Index

[TOC]

### Send P2P Chat

	//-- Async Method
	bool SendChat(MessageIDCallback callback, int64_t uid, const string& message, const string& attrs = "", int32_t timeout = 0);
	
	//-- Sync Method
	int32_t SendChat(int64_t& messageID, int64_t uid, const string& message, const string& attrs = "", int32_t timeout = 0);

Send P2P text message.

Parameters:

+ `MessageIDCallback callback`

		typedef function<void(int64_t messageId, int32_t errorCode)> MessageIDCallback;

	Callabck for async method. Please refer [MessageIDCallback](Delegates.md#MessageIDCallback).

+ `int64_t& messageID`

	Sent message id.

+ `int64_t uid`

	Receiver user id.

+ `const string& message`

	Chat message.

+ `const string& attrs`

	Chat message attributes in Json.

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async sending is start.
	* false: Start async sending is failed.

+ int for Sync

	0 or FPNN_EC_OK means sending successed.

	Others are the reason for sending failed.


### Send Group Chat

	//-- Async Method
	bool SendGroupChat(MessageIDCallback callback, int64_t groupID, const string& message, const string& attrs = "", int32_t timeout = 0);
	
	//-- Sync Method
	int32_t SendGroupChat(int64_t& messageID, int64_t groupID, const string& message, const string& attrs = "", int32_t timeout = 0);

Send text message in group.

Parameters:

+ `MessageIDCallback callback`

		typedef function<void(int64_t messageId, int32_t errorCode)> MessageIDCallback;

	Callabck for async method. Please refer [MessageIDCallback](Delegates.md#MessageIDCallback).

+ `int64_t& messageID`

	Sent message id.

+ `int64_t groupID`

	Group id.

+ `const string& message`

	Chat message.

+ `const string& attrs`

	Chat message attributes in Json.

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async sending is start.
	* false: Start async sending is failed.

+ int for Sync

	0 or FPNN_EC_OK means sending successed.

	Others are the reason for sending failed.


### Send Room Chat

	//-- Async Method
	pbool SendRoomChat(MessageIDCallback callback, int64_t roomID, const string& message, const string& attrs = "", int32_t timeout = 0);
	
	//-- Sync Method
	int32_t SendRoomChat(int64_t& messageID, int64_t roomID, const string& message, const string& attrs = "", int32_t timeout = 0);

Send text message in room.

Parameters:

+ `MessageIDCallback callback`

		typedef function<void(int64_t messageId, int32_t errorCode)> MessageIDCallback;

	Callabck for async method. Please refer [MessageIDCallback](Delegates.md#MessageIDCallback).

+ `int64_t& messageID`

	Sent message id.

+ `int64_t roomId`

	Room id.

+ `const string& message`

	Chat message.

+ `const string& attrs`

	Chat message attributes in Json.

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async sending is start.
	* false: Start async sending is failed.

+ int for Sync

	0 or FPNN_EC_OK means sending successed.

	Others are the reason for sending failed.


### Send P2P Cmd

	//-- Async Method
	bool SendCommand(MessageIDCallback callback, int64_t uid, const string& message, const string& attrs = "", int32_t timeout = 0);
	
	//-- Sync Method
	int32_t SendCommand(int64_t& messageID, int64_t uid, const string& message, const string& attrs = "", int32_t timeout = 0);

Send P2P text cmd.

Parameters:

+ `MessageIDCallback callback`

		typedef function<void(int64_t messageId, int32_t errorCode)> MessageIDCallback;

	Callabck for async method. Please refer [MessageIDCallback](Delegates.md#MessageIDCallback).

+ `int64_t& messageID`

	Sent message id.

+ `int64_t uid`

	Receiver user id.

+ `const string& message`

	Text cmd.

+ `const string& attrs`

	Text cmd attributes in Json.

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async sending is start.
	* false: Start async sending is failed.

+ int for Sync

	0 or FPNN_EC_OK means sending successed.

	Others are the reason for sending failed.


### Send Group Cmd

	//-- Async Method
	bool SendGroupCommand(MessageIDCallback callback, int64_t groupID, const string& message, const string& attrs = "", int32_t timeout = 0);
	
	//-- Sync Method
	int32_t SendGroupCommand(int64_t& messageID, int64_t groupID, const string& message, const string& attrs = "", int32_t timeout = 0);

Send text cmd in group.

Parameters:

+ `MessageIDCallback callback`

		typedef function<void(int64_t messageId, int32_t errorCode)> MessageIDCallback;

	Callabck for async method. Please refer [MessageIDCallback](Delegates.md#MessageIDCallback).

+ `int64_t& messageID`

	Sent message id.

+ `int64_t groupID`

	Group id.

+ `const string& message`

	Text cmd.

+ `const string& attrs`

	Text cmd attributes in Json.

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async sending is start.
	* false: Start async sending is failed.

+ int for Sync

	0 or FPNN_EC_OK means sending successed.

	Others are the reason for sending failed.


### Send Room Cmd

	//-- Async Method
	bool SendRoomCommand(MessageIDCallback callback, int64_t roomID, const string& message, const string& attrs = "", int32_t timeout = 0);
	
	//-- Sync Method
	int32_t SendRoomCommand(int64_t& messageID, int64_t roomID, const string& message, const string& attrs = "", int32_t timeout = 0);

Send text cmd in room.

Parameters:

+ `MessageIDCallback callback`

		typedef function<void(int64_t messageId, int32_t errorCode)> MessageIDCallback;

	Callabck for async method. Please refer [MessageIDCallback](Delegates.md#MessageIDCallback).

+ `int64_t& messageID`

	Sent message id.

+ `int64_t roomID`

	Room id.

+ `const string& message`

	Text cmd.

+ `const string& attrs`

	Text cmd attributes in Json.

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async sending is start.
	* false: Start async sending is failed.

+ int for Sync

	0 or FPNN_EC_OK means sending successed.

	Others are the reason for sending failed.


### Get P2P Chat

	//-- Async Method
	bool GetP2PChat(HistoryMessageCallback callback, int64_t uid, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetP2PChat(HistoryMessageResult& result, int64_t uid, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, int32_t timeout = 0);

Get history data for P2P chat, including text chat, text cmd and file message.

Parameters:

+ `HistoryMessageCallback callback`

		typedef function<void(HistoryMessageResult result, int32_t errorCode)> HistoryMessageCallback;

	Callabck for async method. Please refer [HistoryMessageCallback](Delegates.md#HistoryMessageCallback).

+ `HistoryMessageResult& result`

	Fetched history data. Please refer [HistoryMessageResult](Structures.md#HistoryMessageResult).

+ `int64_t uid`

	Peer user id.

+ `bool desc`

	* true: desc order;
	* false: asc order.

+ `int32_t count`

	Count for retrieving. Max is 20 for each calling.

+ `int64_t beginMsec`

	Beginning timestamp in milliseconds.

+ `int64_t endMsec`

	Ending timestamp in milliseconds.

+ `int64_t lastId`

	Last data id returned when last calling. First calling using 0.

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async calling is start.
	* false: Start async calling is failed.

+ int for Sync

	0 or FPNN_EC_OK means calling successed.

	Others are the reason for calling failed.


### Get Group Chat


	//-- Async Method
	bool GetGroupChat(HistoryMessageCallback callback, int64_t groupID, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetGroupChat(HistoryMessageResult& result, int64_t groupID, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, int32_t timeout = 0);

Get history data for group chat, including text chat, text cmd and file message.

Parameters:

+ `HistoryMessageCallback callback`

		typedef function<void(HistoryMessageResult result, int32_t errorCode)> HistoryMessageCallback;

	Callabck for async method. Please refer [HistoryMessageCallback](Delegates.md#HistoryMessageCallback).

+ `HistoryMessageResult& result`

	Fetched history data. Please refer [HistoryMessageResult](Structures.md#HistoryMessageResult).

+ `int64_t groupID`

	Group id.

+ `bool desc`

	* true: desc order;
	* false: asc order.

+ `int32_t count`

	Count for retrieving. Max is 20 for each calling.

+ `int64_t beginMsec`

	Beginning timestamp in milliseconds.

+ `int64_t endMsec`

	Ending timestamp in milliseconds.

+ `int64_t lastId`

	Last data id returned when last calling. First calling using 0.

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async calling is start.
	* false: Start async calling is failed.

+ int for Sync

	0 or FPNN_EC_OK means calling successed.

	Others are the reason for calling failed.


### Get Room Chat

	//-- Async Method
	bool GetRoomChat(HistoryMessageCallback callback, int64_t roomID, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetRoomChat(HistoryMessageResult& result, int64_t roomID, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, int32_t timeout = 0);

Get history data for room chat, including text chat, text cmd and file message.

Parameters:

+ `HistoryMessageCallback callback`

		typedef function<void(HistoryMessageResult result, int32_t errorCode)> HistoryMessageCallback;

	Callabck for async method. Please refer [HistoryMessageCallback](Delegates.md#HistoryMessageCallback).

+ `HistoryMessageResult& result`

	Fetched history data. Please refer [HistoryMessageResult](Structures.md#HistoryMessageResult).

+ `int64_t roomID`

	Room id.

+ `bool desc`

	* true: desc order;
	* false: asc order.

+ `int32_t count`

	Count for retrieving. Max is 20 for each calling.

+ `int64_t beginMsec`

	Beginning timestamp in milliseconds.

+ `int64_t endMsec`

	Ending timestamp in milliseconds.

+ `int64_t lastId`

	Last data id returned when last calling. First calling using 0.

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async calling is start.
	* false: Start async calling is failed.

+ int for Sync

	0 or FPNN_EC_OK means calling successed.

	Others are the reason for calling failed.

### Get Broadcast Chat


	//-- Async Method
	bool GetBroadcastChat(HistoryMessageCallback callback,bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetBroadcastChat(HistoryMessageResult& result, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, int32_t timeout = 0);

Get history data for broadcast chat, including text chat, text cmd and file message.

Parameters:

+ `HistoryMessageCallback callback`

		typedef function<void(HistoryMessageResult result, int32_t errorCode)> HistoryMessageCallback;

	Callabck for async method. Please refer [HistoryMessageCallback](Delegates.md#HistoryMessageCallback).

+ `HistoryMessageResult& result`

	Fetched history data. Please refer [HistoryMessageResult](Structures.md#HistoryMessageResult).

+ `bool desc`

	* true: desc order;
	* false: asc order.

+ `int32_t count`

	Count for retrieving. Max is 20 for each calling.

+ `int64_t beginMsec`

	Beginning timestamp in milliseconds.

+ `int64_t endMsec`

	Ending timestamp in milliseconds.

+ `int64_t lastId`

	Last data id returned when last calling. First calling using 0.

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async calling is start.
	* false: Start async calling is failed.

+ int for Sync

	0 or FPNN_EC_OK means calling successed.

	Others are the reason for calling failed.


### Get Unread Chat Infos

	//-- Async Method
	bool GetUnread(function<void(vector<int64_t>, vector<int64_t>, int32_t)> callback, bool clear = false, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetUnread(vector<int64_t>& p2pList, vector<int64_t>& groupList, bool clear = false, int32_t timeout = 0);

Get unread infos about which P2P sessions and groups have unread chat messages.

Parameters:

+ `function<void(vector<int64_t>, vector<int64_t>, int32_t)> callback`

	Callabck for async method.  
	First `vector<int64_t>` is uids list which including the peer uids that have the unread chat messages;  
	Second `vector<int64_t>` is group ids list which including the groups that have the unread chat messages.  
	Thrid `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `vector<int64_t>& p2pList`

	User ids list including the peer uids that have the unread chat messages.

+ `vector<int64_t>& groupList`

	Group ids list including the groups that have the unread chat messages.

+ `bool clear`

	Whether clear the unread hint after calling.

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async calling is start.
	* false: Start async calling is failed.

+ int for Sync

	0 or FPNN_EC_OK means calling successed.

	Others are the reason for calling failed.


### Clear Unread infos

	//-- Async Method
	bool ClearUnread(DoneCallback callback, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t ClearUnread(int32_t timeout = 0);

Clear unread infos.

Parameters:

+ `DoneCallback callback`

		typedef function<void(int32_t errorCode)> DoneCallback;

	Callabck for async method. Please refer [DoneCallback](Delegates.md#DoneCallback).

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async calling is start.
	* false: Start async calling is failed.

+ int for Sync

	0 or FPNN_EC_OK means calling successed.

	Others are the reason for calling failed.


### Get P2P Unread Infos

	//-- Async Method
	bool GetP2PUnread(function<void(map<int64_t, int32_t>, map<int64_t, int64_t>, int32_t)> callback, const set<int64_t>& uids, int64_t startTime = 0, const set<int8_t>& messageTypes = set<int8_t>(), int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetP2PUnread(map<int64_t, int32_t>& unreadMap, map<int64_t, int64_t>& lastUnreadTimestampMap, const set<int64_t>& uids, int64_t startTime = 0, const set<int8_t>& messageTypes = set<int8_t>(), int32_t timeout = 0);

Get P2P unread infos when indicated P2P sessions have unread messages.

Parameters:

+ `function<void(map<int64_t, int32_t>, map<int64_t, int64_t>, int32_t)> callback`

	Callabck for async method.  
	First `map<int64_t, int32_t>` is the unread dictionary which key is the peer's uid who has some unread messages, and value is the count of the unread messages;  
	Second `map<int64_t, int64_t>` is the last unread message timestamp dictionary which key is the peer's uid who has some unread messages, and value is the timestamp in milliseconds of the latest unread message;  
	Thrid `int` is the error code indicating the calling is successful or the failed reasons.

+ `map<int64_t, int32_t>& unreadMap`

	Unread dictionary which key is the peer's uid who has some unread messages, and value is the count of the unread messages.

+ `map<int64_t, int64_t>& lastUnreadTimestampMap`

	Last unread message timestamp dictionary which key is the peer's uid who has some unread messages, and value is the timestamp in milliseconds of the latest unread message.

+ `const set<int64_t>& uids`

	The uids of the peer in P2P sessions which will be wanted to be checked.

+ `int64_t startTime`

	The timestamp in millisecond which indicated the start time to calculate the unread messages. `0` means using the last offline/logout time. 

+ `const set<int8_t>& messageTypes`

	Which message types will be checked. If set is null or empty, only chat messages, cmd messages and file messages will be checked.

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async calling is start.
	* false: Start async calling is failed.

+ int for Sync

	0 or FPNN_EC_OK means calling successed.

	Others are the reason for calling failed.


### Get Group Unread Infos

	//-- Async Method
	bool GetGroupUnread(function<void(map<int64_t, int32_t>, map<int64_t, int64_t>, int32_t)> callback, const set<int64_t>& groupIDs, int64_t startTime = 0, const set<int8_t>& messageTypes = set<int8_t>(), int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetGroupUnread(map<int64_t, int32_t>& unreadMap, map<int64_t, int64_t>& lastUnreadTimestampMap, const set<int64_t>& groupIDs, int64_t startTime = 0, const set<int8_t>& messageTypes = set<int8_t>(), int32_t timeout = 0);

Get group unread infos when indicated group sessions have unread messages.

Parameters:

+ `function<void(map<int64_t, int32_t>, map<int64_t, int64_t>, int32_t)> callback`

	Callabck for async method.  
	First `map<int64_t, int32_t>` is the unread dictionary which key is the group id which has some unread messages, and value is the count of the unread messages;  
	Second `map<int64_t, int64_t>` is the last unread message timestamp dictionary which key is the group id which has some unread messages, and value is the timestamp in milliseconds of the latest unread message;  
	Thrid `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `map<int64_t, int32_t>& unreadMap`

	Unread dictionary which key is the group id which has some unread messages, and value is the count of the unread messages.

+ `map<int64_t, int64_t>& lastUnreadTimestampMap`

	Last unread message timestamp dictionary which key is the group id which has some unread messages, and value is the timestamp in milliseconds of the latest unread message.

+ `const set<int64_t>& groupIDs`

	The ids of groups which want to be checked.

+ `int64_t startTime`

	The timestamp in millisecond which indicated the start time to calculate the unread messages. `0` means using the last offline/logout time. 

+ `const set<int8_t>& messageTypes`

	Which message types will be checked. If set is null or empty, only chat messages, cmd messages and file messages will be checked.

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async calling is start.
	* false: Start async calling is failed.

+ int for Sync

	0 or FPNN_EC_OK means calling successed.

	Others are the reason for calling failed.


### Get Session


	//-- Async Method
	bool GetSession(function<void(set<int64_t>, set<int64_t>, int32_t)> callback, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetSession(set<int64_t>& p2pSet, set<int64_t>& groupSet, int32_t timeout = 0);

Get chat sessions.

Parameters:

+ `function<void(set<int64_t>, set<int64_t>, int32_t)> callback`

	Callabck for async method.  
	First `set<int64_t>` is uids list which have chat sessions with current user;  
	Second `set<int64_t>` is group ids list which members including current user.  
	Thrid `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `set<int64_t>& p2pSet`

	User ids list which have chat sessions with current user.

+ `set<int64_t>& groupSet`

	Group ids list which members including current user.

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async calling is start.
	* false: Start async calling is failed.

+ int for Sync

	0 or FPNN_EC_OK means calling successed.

	Others are the reason for calling failed.



### Delete Chat

	//-- Async Method
	bool DeleteChat(DoneCallback callback, int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t DeleteChat(int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout = 0);

Delete a sent chat message.

Parameters:

+ `DoneCallback callback`

		typedef function<void(int32_t errorCode)> DoneCallback;

	Callabck for async method. Please refer [DoneCallback](Delegates.md#DoneCallback).

+ `int64_t fromUid`

	Uid of the chat sender, which chat is wanted to be deleted.

+ `int64_t toId`

	If the chat is P2P chat, `toId` means the uid of peer;  
	If the chat is group chat, `toId` means the `groupId`;  
	If the chat is room chat, `toId` means the `roomId`.

+ `int64_t messageID`

	Message id for the chat message which wanted to be deleted.

+ `MessageCategory messageCategory`

	MessageCategory enumeration.

	Can be MessageCategory.P2PMessage, MessageCategory.GroupMessage, MessageCategory.RoomMessage.


+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async calling is start.
	* false: Start async calling is failed.

+ int for Sync

	0 or FPNN_EC_OK means calling successed.

	Others are the reason for calling failed.


### Get Chat Message

	//-- Async Method
	bool GetChat(function<void(RetrievedMessage, int32_t)> callback, int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetChat(RetrievedMessage& result, int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout = 0);

Retrieve a sent chat message.

Parameters:

+ `function<void(RetrievedMessage, int32_t)> callback`

	Callabck for async method.  
	First `RetrievedMessage` is retrieved data, please refer [RetrievedMessage](Structures.md#RetrievedMessage);  
	Second `int` is the error code indicating the calling is successful or the failed reasons.

+ `RetrievedMessage& retrievedMessage`

	The retrieved data, please refer [RetrievedMessage](Structures.md#RetrievedMessage).

+ `int64_t fromUid`

	Uid of the chat sender, which chat is wanted to be retrieved.

+ `int64_t toId`

	If the chat is P2P chat, `toId` means the uid of peer;  
	If the chat is group chat, `toId` means the `groupId`;  
	If the chat is room chat, `toId` means the `roomId`;  
	If the chat is broadcast chat, `toId` is `0`.

+ `int64_t messageID`

	Message id for the chat message which wanted to be retrieved.

+ `MessageCategory messageCategory`

	MessageCategory enumeration.

	Can be MessageCategory.P2PMessage, MessageCategory.GroupMessage, MessageCategory.RoomMessage, MessageCategory.BroadcastMessage.

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async calling is start.
	* false: Start async calling is failed.

+ int for Sync

	0 or FPNN_EC_OK means calling successed.

	Others are the reason for calling failed.

