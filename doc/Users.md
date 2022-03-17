# RTM Client UE4 SDK Users API Docs

# Index

[TOC]

### Get Online Users

	//-- Async Method
	bool GetOnlineUsers(function<void(set<int64_t>, int32_t)> callback, const set<int64_t>& uids, int32_t timeout = 0);

	//-- Sync Method
	int32_t GetOnlineUsers(set<int64_t>& onlineUIDs, const set<int64_t>& uids, int32_t timeout = 0);

Get online users.

+ `function<void(set<int64_t>, int32_t)> callback`

	Callabck for async method.  
	First `set<int64_t>` is the online users' ids;  
	Second `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `set<int64_t>& onlineUIDs`

	The online users' ids.

+ `const set<int64_t>& uids`

	The users' ids which want to be checked.

	Max 200 uids for each calling.

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

### Set User Info


	//-- Async Method
	bool SetUserInfo(DoneCallback callback, const string& publicInfo = "", const string& privateInfo = "", int32_t timeout = 0);
	
	//-- Sync Method
	int32_t SetUserInfo(const string& publicInfo = "", const string& privateInfo = "", int32_t timeout = 0);

Set user's public info and private info.

Parameters:

+ `DoneCallback callback`

		typedef function<void(int32_t errorCode)> DoneCallback;

	Callabck for async method. Please refer [DoneCallback](Delegates.md#DoneCallback).

+ `const string& publicInfo`

	New public info for group. Empty string means don't change the public info. Max length is 65535 bytes.

+ `const string& privateInfo = ""`

	New private info for group. Empty string means don't change the private info. Max length is 65535 bytes.

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


### Get User Info

	//-- Async Method
	bool GetUserInfo(function<void(string, string, int32_t)> callback, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetUserInfo(string& publicInfo, string& privateInfo, int32_t timeout = 0);

Get user's public info and private info.

Parameters:

+ `function<void(string, string, int32_t)> callback`

	Callabck for async method.  
	First `string` is gotten public info of current user;  
	Second `string` is gotten private info of current user;  
	Thrid `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `string& publicInfo`

	The gotten public info of current user.

+ `string& privateInfo`

	The gotten private info of current user.

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


### Get Users Public Infos

	//-- Async Method
	bool GetUserPublicInfo(function<void(map<int64_t, string>, int32_t)> callback, const set<int64_t>& uids, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetUserPublicInfo(map<int64_t, string>& publicInfos, const set<int64_t>& uids, int32_t timeout = 0);

Get users' public infos.

Parameters:

+ `function<void(map<int64_t, string>, int32_t)> callback`

	Callabck for async method.  
	First `map<int64_t, string>` is gotten users' public infos. Key is uid, value is the public info;  
	Second `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `map<int64_t, string>& publicInfos`

	The gotten users' public infos. Key is uid, value is the public info.

+ `const set<int64_t>& uids`

	Users' ids.	Max 100 users for each calling.

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


