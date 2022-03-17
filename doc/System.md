# RTM Client UE4 SDK System API Docs

# Index

[TOC]

### Add Attributes

	//-- Async Method
	bool AddAttributes(DoneCallback callback, const map<string, string>& attrs, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t AddAttributes(const map<string, string>& attrs, int32_t timeout = 0);

Add session or connection attributes. That can be fetch by all sessions of the user.

Parameters:

+ `DoneCallback callback`

		public delegate void DoneCallback(int32_t errorCode);

	Callabck for async method. Please refer [DoneCallback](Delegates.md#DoneCallback).

+ `const map<string, string>& attrs`

	Session or connection attributes. That can be fetch by all sessions of the user.

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

### Get Attributes

	//-- Async Method
	bool GetAttributes(function<void(map<string, string>, int32_t)> callback, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetAttributes(map<string, string>& attrs, int32_t timeout = 0);

Get session or connection attributes.

Parameters:

+ `function<void(map<string, string>, int32_t)> callback`

	Callabck for async method.  
	First `map<string, string>` is the attributes dictionary for current session;  
	Second `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `map<string, string>& attrs attributes`

	Attributes dictionary for current session.

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

### Add Debug Log

	//-- Async Method
	bool AddDebugLog(DoneCallback callback, const string& message, const string& attrs, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t AddDebugLog(const string& message, const string& attrs, int32_t timeout = 0);

Add debug log.

Parameters:

+ `DoneCallback callback`

		public delegate void DoneCallback(int32_t errorCode);

	Callabck for async method. Please refer [DoneCallback](Delegates.md#DoneCallback).

+ `const string& message`

	log text.

+ `const string& attrs`

	Attributes for log text.

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


### Add Device

	//-- Async Method
	bool AddDevice(DoneCallback callback, const string& appType, const string& deviceToken, int32_t timeout = 0);

	//-- Sync Method
	int32_t AddDevice(const string& appType, const string& deviceToken, int32_t timeout = 0);

Add device infos.

Parameters:

+ `DoneCallback callback`

		public delegate void DoneCallback(int32_t errorCode);

	Callabck for async method. Please refer [DoneCallback](Delegates.md#DoneCallback).

+ `const string& appType`

	Application information. apns or fcm

+ `const string& deviceToke`

	Device Token

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


### Remove Device

	//-- Async Method
	bool RemoveDevice(DoneCallback callback, const string& deviceToken, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t RemoveDevice(const string& deviceToken, int32_t timeout = 0);

Remove device infos.

Parameters:

+ `DoneCallback callback`

		public delegate void DoneCallback(int32_t errorCode);

	Callabck for async method. Please refer [DoneCallback](Delegates.md#DoneCallback).

+ `const string& deviceToke`

	Device Token

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


### Add Device Push Option

	//-- Async Method
	bool AddDevicePushOption(DoneCallback callback, MessageCategory messageCategory, int64_t targetID, set<int8_t> messageTypes = set<int8_t>(), int32_t timeout = 0);
	
	//-- Sync Method
	int32_t AddDevicePushOption(MessageCategory messageCategory, int64_t targetID, set<int8_t> messageTypes = set<int8_t>(), int32_t timeout = 0);

Set disabled session for chat & messages push.

Parameters:

+ `DoneCallback callback`

		public delegate void DoneCallback(int32_t errorCode);

	Callabck for async method. Please refer [DoneCallback](Delegates.md#DoneCallback).

+ `MessageCategory messageCategory`

	Only `MessageCategory.P2PMessage` & `MessageCategory.GroupMessage` can be used.

+ `int64_t targetID`

	Peer uid for `MessageCategory.P2PMessage` and group id for `MessageCategory.GroupMessage`.

+ `set<int8_t> messageTypes`

	Disabled message types. If `messageTypes` is empty, means all message types are disalbed for push.

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


### Remove Device Push Option

	//-- Async Method
	bool RemoveDevicePushOption(DoneCallback callback, MessageCategory messageCategory, int64_t targetID, set<int8_t> messageTypes = set<int8_t>(), int32_t timeout = 0);
	
	//-- Sync Method
	int32_t RemoveDevicePushOption(MessageCategory messageCategory, int64_t targetID, set<int8_t> messageTypes = set<int8_t>(), int32_t timeout = 0);

Remove disabled option for chat & messages push.

Parameters:

+ `DoneCallback callback`

		public delegate void DoneCallback(int32_t errorCode);

	Callabck for async method. Please refer [DoneCallback](Delegates.md#DoneCallback).

+ `MessageCategory messageCategory`

	Only `MessageCategory.P2PMessage` & `MessageCategory.GroupMessage` can be used.

+ `int64_t targetID`

	Peer uid for `MessageCategory.P2PMessage` and group id for `MessageCategory.GroupMessage`.

+ `set<int8_t> messageTypes`

	Disabled message types. If `messageTypes` is empty, means all message types are removed disalbe attributes for push.

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


### Get Device Push Option

	//-- Async Method
	bool GetDevicePushOption(function<void(map<int64_t, set<int8_t>>, map<int64_t, set<int8_t>>, int32_t)> callback, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetDevicePushOption(map<int64_t, set<int8_t>>& p2pMap, map<int64_t, set<int8_t>>& groupMap, int32_t timeout = 0);

Get disabled option for chat & messages push.

Parameters:

+ `function<void(map<int64_t, set<int8_t>>, map<int64_t, set<int8_t>>, int32_t)> callback`

	Callabck for async method.  
	First `map<int64_t, set<int8_t>>` is peer user id with associated disabled message types set for P2P sessions;  
	Second `map<int64_t, set<int8_t>>` is group id with associated disabled message types set for group sessions;  
	Thrid `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `map<int64_t, set<int8_t>>& p2pMap`

	Peer user id with associated disabled message types set dictionary.

+ `map<int64_t, set<int8_t>>& groupMap`

	Group id with associated disabled message types set dictionary.

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

	