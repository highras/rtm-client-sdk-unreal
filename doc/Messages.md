# RTM Client UE4 SDK Messages API Docs

# Index

[TOC]

### Send P2P Message

	//-- Async Method
	bool SendMessage(MessageIDCallback callback, int64_t uid, int8_t messageType, const string& message, const string& attrs = "", int32_t timeout = 0);
	
	//-- Sync Method
	int32_t SendMessage(int64_t& messageID, int64_t uid, int64_t uid, const string& message, const string& attrs = "", int32_t timeout = 0);

Send P2P message.

Parameters:

+ `MessageIDCallback callback`

		typedef function<void(int64_t messageId, int32_t errorCode)> MessageIDCallback;

	Callabck for async method. Please refer [MessageIDCallback](Delegates.md#MessageIDCallback).

+ `int64_t& messageID`

	Sent message id.

+ `int64_t uid`

	Receiver user id.

+ `int64_t uid`

	Message type for message. MUST large than 50.

+ `const string& message`

	Text message.

+ `const string& attrs`

	Message attributes in Json.

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


### Send Group Messsage

	//-- Async Method
	bool SendGroupMessage(MessageIDCallback callback, int64_t groupID, int8_t messageType, const string& message, const string& attrs = "", int32_t timeout = 0);
	
	//-- Sync Method
	int32_t SendGroupMessage(int64_t& messageID, int64_t groupID, int8_t messageType, const string& message, const string& attrs = "", int32_t timeout = 0);

Send message in group.

Parameters:

+ `MessageIDCallback callback`

		typedef function<void(int64_t messageId, int32_t errorCode)> MessageIDCallback;

	Callabck for async method. Please refer [MessageIDCallback](Delegates.md#MessageIDCallback).

+ `int64_t& messageID`

	Sent message id.

+ `int64_t groupID`

	Group id.

+ `int8_t messageType`

	Message type for message. MUST large than 50.

+ `const string& message`

	Text message.

+ `const string& attrs`

	Message attributes in Json.

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


### Send Room Message

	//-- Async Method
	bool SendRoomMessage(MessageIDCallback callback, int64_t roomID, int8_t messageType, const string& message, const string& attrs = "", int32_t timeout = 0);
	
	//-- Sync Method
	int32_t SendRoomMessage(int64_t& messageID, int64_t roomID, int8_t messageType, const string& message, const string& attrs = "", int32_t timeout = 0);

Send message in room.

Parameters:

+ `MessageIDCallback callback`

		typedef function<void(int64_t messageId, int32_t errorCode)> MessageIDCallback;

	Callabck for async method. Please refer [MessageIDCallback](Delegates.md#MessageIDCallback).

+ `int64_t& messageID`

	Sent message id.

+ `int64_t roomID`

	Room id.

+ `int8_t messageType`

	Message type for message. MUST large than 50.

+ `const string& message`

	Text message.

+ `const string& attrs`

	Message attributes in Json.

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

### Get P2P Message

	//-- Async Method
	bool GetP2PMessage(HistoryMessageCallback callback, int64_t uid, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, const vector<int8_t>& messageTypes = vector<int8_t>(), int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetP2PMessage(HistoryMessageResult& result, int64_t uid, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, const vector<int8_t>& messageTypes = vector<int8_t>(), int32_t timeout = 0);

Get history data for P2P message.

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

+ `const vector<int8_t>& messageTypes`

	Message types for retrieved message. Empty vector means all types.

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


### Get Group Messsage


	//-- Async Method
	bool GetGroupMessage(HistoryMessageCallback callback, int64_t groupID, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, const vector<int8_t>& messageTypes = vector<int8_t>(), int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetGroupMessage(HistoryMessageResult& result, int64_t groupID, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, const vector<int8_t>& messageTypes = vector<int8_t>(), int32_t timeout = 0);

Get history data for group message.

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

+ `const vector<int8_t>& messageTypes`

	Message types for retrieved message. Empty vector means all types.

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


### Get Room Message

	//-- Async Method
	bool GetRoomMessage(HistoryMessageCallback callback, int64_t roomID, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, const vector<int8_t>& messageTypes = vector<int8_t>(), int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetRoomMessage(HistoryMessageResult& result, int64_t roomID, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, const vector<int8_t>& messageTypes = vector<int8_t>(), int32_t timeout = 0);

Get history data for room message.

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

+ `const vector<int8_t>& messageTypes`

	Message types for retrieved message. Empty vector means all types.

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async calling is start.
	* false: Start async calling is failed.

+ int for Sync

	0 or com.fpnn.ErrorCode.FPNN_EC_OK means calling successed.

	Others are the reason for calling failed.

### Get Broadcast Message


	//-- Async Method
	bool GetBroadcastMessage(HistoryMessageCallback callback,bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, const vector<int8_t>& messageTypes = vector<int8_t>(), int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetBroadcastMessage(HistoryMessageResult& result, bool desc, int32_t count, int64_t beginMsec = 0, int64_t endMsec = 0, int64_t lastID = 0, const vector<int8_t>& messageTypes = vector<int8_t>(), int32_t timeout = 0);

Get history data for broadcast message.

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

+ `const vector<int8_t>& messageTypes`

	Message types for retrieved message. Empty Vector means all types.

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

### Delete Message

	//-- Async Method
	bool DeleteMessage(DoneCallback callback, int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t DeleteMessage(int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout = 0);

Delete a sent message.

Parameters:

+ `DoneCallback callback`

		typedef function<void(int32_t errorCode)> DoneCallback;

	Callabck for async method. Please refer [DoneCallback](Delegates.md#DoneCallback).

+ `int64_t fromUid`

	Uid of the message sender, which message is wanted to be deleted.

+ `int64_t toId`

	If the message is P2P message, `toId` means the uid of peer;  
	If the message is group message, `toId` means the `groupId`;  
	If the message is room message, `toId` means the `roomId`.

+ `int64_t messageID`

	Message id for the message which wanted to be deleted.

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


### Get Message

	//-- Async Method
	bool GetMessage(function<void(RetrievedMessage, int32_t)> callback, int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetMessage(RetrievedMessage& result, int64_t fromUid, int64_t toId, int64_t messageID, MessageCategory messageCategory, int32_t timeout = 0);

Retrieve a sent message.

Parameters:

+ `function<void(RetrievedMessage, int32_t)> callback`

	Callabck for async method.  
	First `RetrievedMessage` is retrieved data, please refer [RetrievedMessage](Structures.md#RetrievedMessage);  
	Second `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `RetrievedMessage& result`

	The retrieved data, please refer [RetrievedMessage](Structures.md#RetrievedMessage).	

+ `int64_t fromUid`

	Uid of the message sender, which message is wanted to be retrieved.

+ `int64_t toId`

	If the message is P2P message, `toId` means the uid of peer;  
	If the message is group message, `toId` means the `groupId`;  
	If the message is room message, `toId` means the `roomId`;  
	If the message is broadcast message, `toId` is `0`.

+ `int64_t messageID`

	Message id for the message which wanted to be retrieved.

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

