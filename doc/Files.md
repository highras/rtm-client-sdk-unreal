# RTM Client UE4 SDK Files API Docs

# Index

[TOC]

### Send P2P File

	//-- Async Method
	bool SendFile(MessageIDCallback callback, int64_t uid, ChatMessageType type, const string& fileContent, const string& fileName, const string& fileExtension = "", const string& attrs = "", int32_t timeout = 120);
	
	//-- Sync Method
	int32_t SendFile(int64_t& messageID, int64_t uid, ChatMessageType type, const string& fileContent, const string& fileName, const string& fileExtension = "", const string& attrs = "", int32_t timeout = 120);

Send P2P file.

Parameters:

+ `MessageIDCallback callback`

		typedef function<void(int64_t messageId, int32_t errorCode)> MessageIDCallback;

	Callabck for async method. Please refer [MessageIDCallback](Delegates.md#MessageIDCallback).

+ `int64_t& messageID`

	Sent message id.

+ `int64_t uid`

	Receiver user id.

+ `ChatMessageType type`

	Message type for file.

+ `const string& fileContent`

	File content.

+ `const string& filename`

	File name.

+ `const string& fileExtension`

	File extension.

+ `const string& attrs`

	Text file attributes in Json.

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


### Send Group File

	//-- Async Method
	bool SendGroupFile(MessageIDCallback callback, int64_t groupID, ChatMessageType type, const string& fileContent, const string& fileName, const string& fileExtension = "", const string& attrs = "", int32_t timeout = 120);
	
	//-- Sync Method
	int32_t SendGroupFile(int64_t& messageID, int64_t groupID, ChatMessageType type, const string& fileContent, const string& fileName, const string& fileExtension = "", const string& attrs = "", int32_t timeout = 120);

Send file in group.

Parameters:

+ `MessageIDCallback callback`

		typedef function<void(int64_t messageId, int32_t errorCode)> MessageIDCallback;

	Callabck for async method. Please refer [MessageIDCallback](Delegates.md#MessageIDCallback).

+ `int64_t& messageID`

	Sent message id.

+ `int64_t groupID`

	Group id.

+ `ChatMessageType type`

	Message type for file.

+ `const string& fileContent`

	File content.

+ `const string& filename`

	File name.

+ `const string& fileExtension`

	File extension.

+ `const string& attrs`

	Text file attributes in Json.

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


### Send Room File

	//-- Async Method
	bool SendRoomFile(MessageIDCallback callback, int64_t roomID, ChatMessageType type, const string& fileContent, const string& fileName, const string& fileExtension = "", const string& attrs = "", int32_t timeout = 120);
	
	//-- Sync Method
	int32_t SendRoomFile(int64_t& messageID, int64_t roomID, ChatMessageType type, const string& fileContent, const string& fileName, const string& fileExtension = "", const string& attrs = "", int32_t timeout = 120);

Send file in room.

Parameters:

+ `MessageIDCallback callback`

		typedef function<void(int64_t messageId, int32_t errorCode)> MessageIDCallback;

	Callabck for async method. Please refer [MessageIDCallback](Delegates.md#MessageIDCallback).

+ `int64_t& messageID`

	Sent message id.

+ `int64_t roomID`

	Room id.

+ `ChatMessageType type`

	Message type for file.

+ `const string& fileContent`

	File content.

+ `const string& filename`

	File name.

+ `const string& fileExtension`

	File extension.

+ `const string& attrs`

	Text file attributes in Json.

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

