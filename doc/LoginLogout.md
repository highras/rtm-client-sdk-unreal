# RTM Client UE4 SDK Login & Logout API Docs

# Index

[TOC]

### Login

	//-- Async Method
	bool Login(AuthCallback callback, const string& token, const map<string, string>& attrs = map<string, string>(), const string& language = "", int32_t timeout = 0);
	
	//-- Sync Method
	int32_t Login(bool& ok, const string& token, const map<string, string>& attrs = map<string, string>(), const string& language = "", int32_t timeout = 0);

User login.

Parameters:

+ `AuthCallback callback`

		typedef function<void(int64_t pid, int64_t uid, bool authStatus, int32_t errorCode)> AuthCallback;

	Callabck for async method. Please refer [AuthCallback](Delegates.md#AuthCallback).

+ `bool& ok`

	Login/auth success or failure.

+ `const string& token`

	Login/auth token, which can be gotten from your bueiness server or game server who quest for the current user from RTM Server-end endpoints.

+ `const map<string, string>& attrs`

	Session or connection attributes. That can be fetch by all sessions of the user.

+ `const string& language`

	The target language for enable auto-translating.

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

**Notice:**

If returned value is zero (FPNN_EC_OK) and the output parameter ok is false in sync calling, or errorCode is zero (FPNN_EC_OK) and authStatus is false in async calling, means the token is invalid, which need to be fetched again from RTM server by business server-end.

### Bye

	void Bye(bool async = true);

Logout and close the current session.

Parameters:

+ `bool async`

	In async way or sync way.

### Close

	void Close(bool disableRelogin = true, bool waitConnectingCancelled = true);

Without logout, close the current session directly.

Parameters:

+ `bool disableRelogin`

	Disable Relogin operation or not.

+ `bool waitConnectingCannelled`

	Waiting for current connecting has been cannelled or not.

