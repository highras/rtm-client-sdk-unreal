# RTM Client UE4 SDK API Docs: Delegates

# Index

[TOC]

### AuthCallback

	typedef function<void(int64_t pid, int64_t uid, bool authStatus, int32_t errorCode)> AuthCallback;

Parameters:

+ `int64_t pid`

	Project id.

+ `int64_t uid`

	User id.

+ `bool authStatus`

	* true: login success
	* false: login failed. Reason can be deduced from errorCode.

+ `int32_t errorCode`

	Reason for login failed.

### DoneCallback

	typedef function<void(int32_t errorCode)> DoneCallback;

Parameters:

+ `int errorCode`

	0 or FPNN_EC_OK means successed.

	Others are the reason for failed.

### MessageIDCallback

	typedef function<void(int64_t messageId, int32_t errorCode)> MessageIDCallback;

Parameters:

+ `int64_t messageId`

	If action is successful, `messageId` is the sent message id.

	If action is failed, `messageId` is 0.

+ `int32_t errorCode`

	0 or FPNN_EC_OK means successed.

	Others are the reason for failed.

### HistoryMessageCallback

	typedef function<void(HistoryMessageResult result, int32_t errorCode)> HistoryMessageCallback;

Parameters:

+ `HistoryMessageResult result`
	
	Result of the history message.

	Please prefer [HistoryMessageResult](Structures.md/#HistoryMessageResult)

+ `int errorCode`

	0 or FPNN_EC_OK means successed.

	Others are the reason for failed.
