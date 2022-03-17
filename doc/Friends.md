# RTM Client UE4 SDK Friends API Docs

# Index

[TOC]

### Add Friends

	//-- Async Method
	bool AddFriends(DoneCallback callback, const set<int64_t>& uids, int32_t timeout = 0);

	//-- Sync Method
	int32_t AddFriends(const set<int64_t>& uids, int32_t timeout = 0);

Add friends.

Parameters:

+ `DoneCallback callback`

		typedef function<void(int32_t errorCode)> DoneCallback;

	Callabck for async method. Please refer [DoneCallback](Delegates.md#DoneCallback).

+ `const set<int64_t>& uids`

	Friends' uids set. Max 100 users for each calling.

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


### Delete Friends

	//-- Async Method
	bool DeleteFriends(DoneCallback callback, const set<int64_t>& uids, int32_t timeout = 0);

	//-- Sync Method
	int32_t DeleteFriends(const set<int64_t>& uids, int32_t timeout = 0);

Delete friends.

Parameters:

+ `DoneCallback callback`

		typedef function<void(int32_t errorCode)> DoneCallback;

	Callabck for async method. Please refer [DoneCallback](Delegates.md#DoneCallback).

+ `const set<int64_t>& uids`

	Friends' uids set. Max 100 users for each calling.

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


### Get Friends

	//-- Async Method
	bool GetFriends(function<void(set<int64_t>, int32_t)> callback, int32_t timeout = 0);

	//-- Sync Method
	int32_t GetFriends(set<int64_t>& friends, int32_t timeout = 0);

Get friends.

+ `function<void(set<int64_t>, int32_t)> callback`

	Callabck for async method.  
	First `set<int64_t>` is gotten friends' uids;  
	Second `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `set<int64_t>& friends`

	The gotten friends' uids.

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


### Add Blacklist

	//-- Async Method
	bool AddBlackList(DoneCallback callback, const set<int64_t>& uids, int32_t timeout = 0);

	//-- Sync Method
	int32_t AddBlackList(const set<int64_t>& uids, int32_t timeout = 0);

Add users to blacklist.

Parameters:

+ `DoneCallback callback`

		typedef function<void(int32_t errorCode)> DoneCallback;

	Callabck for async method. Please refer [DoneCallback](Delegates.md#DoneCallback).

+ `const set<int64_t>& uids`

	Uids set. Max 100 users for each calling.

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


### Delete Blacklist

	//-- Async Method
	bool DeleteBlackList(DoneCallback callback, const set<int64_t>& uids, int32_t timeout = 0);

	//-- Sync Method
	int32_t DeleteBlackList(const set<int64_t>& uids, int32_t timeout = 0);

Delete from blacklist.

Parameters:

+ `DoneCallback callback`

		typedef function<void(int32_t errorCode)> DoneCallback;

	Callabck for async method. Please refer [DoneCallback](Delegates.md#DoneCallback).

+ `const set<int64_t>& uids`

	Uids set. Max 100 users for each calling.

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


### Get Blacklist

	//-- Async Method
	bool GetBlackList(function<void(set<int64_t>, int32_t)> callback, int32_t timeout = 0);

	//-- Sync Method
	int32_t GetBlackList(set<int64_t>& uids, int32_t timeout = 0);

Get blocked uids from blacklist.

+ `function<void(set<int64_t>, int32_t)> callback`

	Callabck for async method.  
	First `set<int64_t>` is gotten uids;  
	Second `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `set<int64_t>& uids`

	The gotten uids.

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
