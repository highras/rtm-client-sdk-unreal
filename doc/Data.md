# RTM Client UE4 SDK Data API Docs

# Index

[TOC]

### Get Data

	//-- Async Method
	bool DataGet(function<void(string, int32_t)> callback, const string& key, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t DataGet(string& value, const string& key, int32_t timeout = 0);

Get user's data.

Parameters:

+ `function<void(string, int32_t)> callback`

	Callabck for async method.  
	First `string` is gotten data associated the inputted `string key`;  
	Second `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `string& value`

	The gotten data associated the inputted `string key`.

+ `const string& key`

	The key of wanted to be gotten data.

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

### Set Data

	//-- Async Method
	bool DataSet(DoneCallback callback, const string& key, const string& value, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t DataSet(const string& key, const string& value, int32_t timeout = 0);

Set user's data.

Parameters:

+ `DoneCallback callback`

		typedef function<void(int32_t errorCode)> DoneCallback;

	Callabck for async method. Please refer [DoneCallback](Delegates.md#DoneCallback).

+ `const string& key`

	The key of user's data. Max 128 bytes length.

+ `const string& value`

	The value of user's data. Max 65535 bytes length.

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

### Delete Data

	//-- Async Method
	bool DataDelete(DoneCallback callback, const string& key, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t DataDelete(const string& key, int32_t timeout = 0);

Delete user's data.

Parameters:

+ `DoneCallback callback`

		typedef function<void(int32_t errorCode)> DoneCallback;

	Callabck for async method. Please refer [DoneCallback](Delegates.md#DoneCallback).

+ `const string& key`

	The key of user's data.

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



