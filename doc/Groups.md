# RTM Client UE4 SDK Groups API Docs

# Index

[TOC]

### Add Group Members

	//-- Async Method
	bool AddGroupMembers(DoneCallback callback, int64_t groupID, const set<int64_t>& uids, int32_t timeout = 0);

	//-- Sync Method
	int32_t AddGroupMembers(int64_t groupID, const set<int64_t>& uids, int32_t timeout = 0);

Add group members. Note: Current user MUST be the group member.

Parameters:

+ `DoneCallback callback`

		typedef function<void(int32_t errorCode)> DoneCallback;

	Callabck for async method. Please refer [DoneCallback](Delegates.md#DoneCallback).

+ `int64_t groupID`

	Group id.

+ `const set<int64_t>& uids`

	New members' uids set. Max 100 users for each calling.

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


### Delete Group Members

	//-- Async Method
	bool DeleteGroupMembers(DoneCallback callback, int64_t groupID, const set<int64_t>& uids, int32_t timeout = 0);

	//-- Sync Method
	int32_t DeleteGroupMembers(int64_t groupID, const set<int64_t>& uids, int32_t timeout = 0);

Delete group members. Note: Current user MUST be the group member.

Parameters:

+ `DoneCallback callback`

		typedef function<void(int32_t errorCode)> DoneCallback;

	Callabck for async method. Please refer [DoneCallback](Delegates.md#DoneCallback).

+ `int64_t groupID`

	Group id.

+ `const set<int64_t>& uids`

	The members' uids set. Max 100 users for each calling.

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


### Get Group Members

	//-- Async Method
	bool GetGroupMembers(function<void(set<int64_t>, int32_t)> callback, int64_t groupID, int32_t timeout = 0);
	bool GetGroupMembers(function<void(set<int64_t>, set<int64_t>, int32_t)> callback, int64_t groupID, int32_t timeout = 0);

	//-- Sync Method
	int32_t GetGroupMembers(set<int64_t>& uids, int64_t groupID, int32_t timeout = 0);
	int32_t GetGroupMembers(set<int64_t>& allUIDs, set<int64_t>& onlineUIDs, int64_t groupID, int32_t timeout = 0);

Get group members.

+ `function<void(set<int64_t>, int32_t)> callback`

	Callabck for async method.  
	First `set<int64_t>` is gotten group members' uids;  
	Second `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `function<void(set<int64_t>, set<int64_t>, int32_t)> callback`

	Callabck for async method.  
	First `set<int64_t>` is gotten group members' uids;  
	Second `set<int64_t>` is gotten group online members' uids;  
	Thrid `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `set<int64_t>& uids`

	The gotten group members' uids.

+ `set<int64_t>& allUIDs`

	The gotten group members' uids.

+ `set<int64_t>& onlineUIDs`

	The gotten group online members' uids.

+ `int64_t groupID`

	Group id.

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


### Get Group Count

	//-- Async Method
	bool GetGroupCount(function<void(int32_t, int32_t)> callback, int64_t groupID, int32_t timeout = 0);
	bool GetGroupCount(function<void(int32_t, int32_t, int32_t)> callback, int64_t groupID, int32_t timeout = 0);

	//-- Sync Method
	int32_t GetGroupCount(int32_t& memberCount, int64_t groupID, int32_t timeout = 0);
	int32_t GetGroupCount(int32_t& memberCount, int32_t& onlineCount, int64_t groupID, int32_t timeout = 0);

Get group member count.

+ `function<void(int32_t, int32_t)> callback`

	Callabck for async method.  
	First `int32_t` is group member count;  
	Second `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `function<void(int32_t, int32_t, int32_t)> callback`

	Callabck for async method.  
	First `int32_t` is group member count;  
	Second `int32_t` is group online member count;  
	Thrid `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `int32_t& memberCount`

	The group member count.

+ `int32_t& onlineCount`

	The group online member count.

+ `int64_t groupID`

	Group id.

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


### Get User Groups

	//-- Async Method
	bool GetUserGroups(function<void(set<int64_t>, int32_t)> callback, int32_t timeout = 0);

	//-- Sync Method
	int32_t GetUserGroups(set<int64_t>& groupIDs, int32_t timeout = 0);

Get current user's all groups.

+ `function<void(set<int64_t>, int32_t)> callback`

	Callabck for async method.  
	First `set<int64_t>` is gotten current user's group ids;  
	Second `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `set<int64_t>& groupIDs`

	The gotten current user's group ids.

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

### Set Group Info


	//-- Async Method
	bool SetGroupInfo(DoneCallback callback, int64_t groupID, const string& publicInfo = "", const string& privateInfo = "", int32_t timeout = 0);
	
	//-- Sync Method
	int32_t SetGroupInfo(int64_t groupID, const string& publicInfo = "", const string& privateInfo = "", int32_t timeout = 0);

Set group public info and private info. Note: Current user MUST be the group member.

Parameters:

+ `DoneCallback callback`

		typedef function<void(int32_t errorCode)> DoneCallback;

	Callabck for async method. Please refer [DoneCallback](Delegates.md#DoneCallback).

+ `int64_t groupID`

	Group id.

+ `const string& publicInfo`

	New public info for group. Empty string means don't change the public info. Max length is 65535 bytes.

+ `const string& privateInfo`

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


### Get Group Info

	//-- Async Method
	bool GetGroupInfo(function<void(string, string, int32_t)> callback, int64_t groupID, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetGroupInfo(string& publicInfo, string& privateInfo, int64_t groupID, int32_t timeout = 0);

Get group public info and private info. Note: Current user MUST be the group member.

Parameters:

+ `function<void(string, string, int32_t)> callback`

	Callabck for async method.  
	First `string` is gotten public info of this group;  
	Second `string` is gotten private info of this group;  
	Thrid `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `string& publicInfo`

	The gotten public info of this group.

+ `string& privateInfo`

	The gotten private info of this group.

+ `int64_t groupID`

	Group id.

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


### Get Group Public Info

	//-- Async Method
	bool GetGroupPublicInfo(function<void(string, int32_t)> callback, int64_t groupID, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetGroupPublicInfo(string& publicInfo, int64_t groupID, int32_t timeout = 0);

Get group public info.

Parameters:

+ `function<void(string, int32_t)> callback`

	Callabck for async method.  
	First `string` is gotten public info of the group;  
	Second `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `string& publicInfo`

	The gotten public info of this group.

+ `int64_t groupID`

	Group id.

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


### Get Groups Public Infos

	//-- Async Method
	bool GetGroupsPublicInfo(function<void(map<int64_t, string>, int32_t)> callback, const set<int64_t>& groupIDs, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t  GetGroupsPublicInfo(map<int64_t, string>& publicInfos, const set<int64_t>& groupIDs, int32_t timeout = 0);

Get groups' public infos.

Parameters:

+ `function<void(map<int64_t, string>, int32_t)> callback`

	Callabck for async method.  
	First `map<int64_t, string>` is gotten groups' public infos. Key is group id, value is the public info;  
	Second `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `map<int64_t, string>& publicInfos`

	The gotten groups' public infos. Key is group id, value is the public info.

+ `const set<int64_t>& groupIDs`

	Groups' ids. Max 100 groups for each calling.

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


