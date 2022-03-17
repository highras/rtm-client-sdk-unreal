# RTM Client UE4 SDK Rooms API Docs

# Index

[TOC]

### Enter Room

	//-- Async Method
	bool EnterRoom(DoneCallback callback, int64_t roomID, int32_t timeout = 0);

	//-- Sync Method
	int32_t EnterRoom(int64_t roomID, int32_t timeout = 0);

Enter room.

Parameters:

+ `DoneCallback callback`

		typedef function<void(int32_t errorCode)> DoneCallback;

	Callabck for async method. Please refer [DoneCallback](Delegates.md#DoneCallback).

+ `int64_t roomID`

	Room id.

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

### Leave Room


	//-- Async Method
	bool LeaveRoom(DoneCallback callback, int64_t roomID, int32_t timeout = 0);

	//-- Sync Method
	int32_t LeaveRoom(int64_t roomID, int32_t timeout = 0);

Leave room.

Parameters:

+ `DoneCallback callback`

		typedef function<void(int32_t errorCode)> DoneCallback;

	Callabck for async method. Please refer [DoneCallback](Delegates.md#DoneCallback).

+ `int64_t roomID`

	Room id.

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



### Get User Rooms

	//-- Async Method
	bool GetUserRooms(function<void(set<int64_t>, int32_t)> callback, int32_t timeout = 0);

	//-- Sync Method
	int32_t GetUserRooms(set<int64_t>& roomIDs, int32_t timeout = 0);

Get current user's all groups.

+ `function<void(set<int64_t>, int32_t)> callback`

	Callabck for async method.  
	First `set<int64_t>` is gotten current user's room ids;  
	Second `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `set<int64_t>& roomIDs`

	The gotten current user's room ids.

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

### Set Room Info


	//-- Async Method
	bool SetRoomInfo(DoneCallback callback, int64_t roomID, const string& publicInfo = "", const string& privateInfo = "", int32_t timeout = 0);
	
	//-- Sync Method
	int32_t SetRoomInfo(int64_t roomID, const string& publicInfo = "", const string& privateInfo = "", int32_t timeout = 0);

Set room public info and private info. Note: Current user MUST in the room.

Parameters:

+ `DoneCallback callback`

		typedef function<void(int32_t errorCode)> DoneCallback;

	Callabck for async method. Please refer [DoneCallback](Delegates.md#DoneCallback).

+ `int64_t roomID`

	Room id.

+ `const string& publicInfo`

	New public info for room. Empty string means don't change the public info. Max length is 65535 bytes.

+ `const string& privateInfo`

	New private info for room. Empty string means don't change the private info. Max length is 65535 bytes.

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


### Get Room Info

	//-- Async Method
	bool GetRoomInfo(function<void(string, string, int32_t)> callback, int64_t roomID, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetRoomInfo(string& publicInfo, string& privateInfo, int64_t roomID, int32_t timeout = 0);

Get room public info and private info. Note: Current user MUST in the room.

Parameters:

+ `function<void(string, string, int32_t)> callback`

	Callabck for async method.  
	First `string` is gotten public info of this room;  
	Second `string` is gotten private info of this room;  
	Thrid `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `string& publicInfo`

	The gotten public info of this room.

+ `string& privateInfo`

	The gotten private info of this room.

+ `int64_t roomID`

	Room id.

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


### Get Room Public Info

	//-- Async Method
	bool GetRoomPublicInfo(function<void(string, int32_t)> callback, int64_t roomID, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetRoomPublicInfo(string& publicInfo, int64_t roomID, int32_t timeout = 0);

Get Room public info.

Parameters:

+ `function<void(string, int32_t)> callback`

	Callabck for async method.  
	First `string` is gotten public info of the room;  
	Second `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `string& publicInfo`

	The gotten public info of this room.

+ `int64_t roomID`

	Room id.

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


### Get Rooms Public Infos

	//-- Async Method
	bool GetRoomsPublicInfo(function<void(map<int64_t, string>, int32_t)> callback, const set<int64_t>& roomIDs, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetRoomsPublicInfo(map<int64_t, string>& publicInfos, const set<int64_t>& roomIDs, int32_t timeout = 0);

Get rooms' public infos.

Parameters:

+ `function<void(map<int64_t, string>, int32_t)> callback`

	Callabck for async method.  
	First `map<int64_t, string>` is gotten rooms' public infos. Key is room id, value is the public info;  
	Second `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `map<int64_t, string>& publicInfos`

	The gotten rooms' public infos. Key is room id, value is the public info.

+ `const set<int64_t>& roomIDs`

	Rooms' ids. Max 100 rooms for each calling.

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


### Get Room Members

	//-- Async Method
	bool GetRoomMembers(function<void(set<int64_t>, int32_t)> callback, int64_t roomID, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetRoomMembers(set<int64_t>& uids, int64_t roomID, int32_t timeout = 0);

Get room member list.

**IMPORTANT** The synchronous frequence of the statistic data is 5 seconds.

Parameters:

+ `function<void(set<int64_t>, int32_t)> callback`

	Callabck for async method.  
	First `set<int64_t>` is the member list of indicated room;  
	Second `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `set<int64_t>& uids`

	The member list of indicated room.

+ `int64_t roomID`

	Id of room which member list is wanted to be gotten.

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


### Get Room Member Count

	//-- Async Method
	bool GetRoomMemberCount(function<void(map<int64_t, int32_t>, int32_t)> callback, const set<int64_t>& roomIDs, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t GetRoomMemberCount(map<int64_t, int32_t>& counts, const set<int64_t>& roomIDs, int32_t timeout = 0);

Get rooms' member counts.

**IMPORTANT** The synchronous frequence of the statistic data is 5 seconds.

Parameters:

+ `function<void(map<int64_t, int32_t>, int32_t)> callback`

	Callabck for async method.  
	First `map<int64_t, int32_t>` is the directionary for room ids and member counts. Key is room id, value is member count of this room;  
	Second `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `map<int64_t, int32_t>& counts`

	The directionary for room ids and member counts. Key is room id, value is member count of this room.

+ `const set<int64_t>& roomIDs`

	Ids of rooms which member counts are wanted to be gotten.

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


