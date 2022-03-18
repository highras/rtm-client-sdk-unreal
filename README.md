# RTM Client UE4 SDK

[TOC]

## Usage

### Integration

* Copy all the content in `Plugins` folder to `Plugins` folder in your project.

* Add the content below to [YourModule].Build.cs file. 
  >bEnableExceptions = true;  
  >bEnableUndefinedIdentifierWarnings = false;  
  >PrivateDependencyModuleNames.AddRange(new string[] { "FPNN", "RTM" });

### RTM SDK Init (REQUIRED)

	#include "RTMControlCenter.h"
	...
	RTMControlCenter::Init();

### Create

	#include "RTMClient.h"
	#include "RTMQuestProcessor.h"
	...
	RTMClientPtr client = RTMClient::CreateRTMClient(int64_t pid, int64_t uid, const string& endpoint, RTMQuestProcessorPtr rtmQuestProcessor, bool autoRelogin = true);
 
Please get your project params from RTM Console.

### RTMClient Instance Configure

#### Configure Properties:

	int32_t RTMConfig::globalConnectTimeoutSeconds;
	int32_t RTMConfig::globalQuestTimeoutSeconds;

### Login

	//-- Async interfaces
	bool Login(AuthCallback callback, const string& token, const map<string, string>& attrs = map<string, string>(), const string& language = "", int32_t timeout = 0);

	//-- Sync interfaces
	int32_t Login(bool& ok, const string& token, const map<string, string>& attrs = map<string, string>(), const string& language = "", int32_t timeout = 0);

### Send messages

* Send P2P Message

		//-- Async interface
		bool SendMessage(MessageIDCallback callback, int64_t uid, int8_t messageType, const string& message, const string& attrs = "", int32_t timeout = 0);

		//-- Sync interface
		int32_t SendMessage(int64_t& messageID, int64_t uid, int8_t messageType, const string& message, const string& attrs = "", int32_t timeout = 0);


* Send Group Message
	
		//-- Async interface
		bool SendGroupMessage(MessageIDCallback callback, int64_t groupID, int8_t messageType, const string& message, const string& attrs = "", int32_t timeout = 0);

		//-- Sync interface
		int32_t SendGroupMessage(int64_t& messageID, int64_t groupID, int8_t messageType, const string& message, const string& attrs = "", int32_t timeout = 0);


* Send Room Message

		//-- Async interface
		bool SendRoomMessage(MessageIDCallback callback, int64_t roomID, int8_t messageType, const string& message, const string& attrs = "", int32_t timeout = 0);

		//-- Sync interface
		int32_t SendRoomMessage(int64_t& messageID, int64_t roomID, int8_t messageType, const string& message, const string& attrs = "", int32_t timeout = 0);


### Send chat

* Send P2P Chat

		//-- Async interface
		bool SendChat(MessageIDCallback callback, int64_t uid, const string& message, const string& attrs = "", int32_t timeout = 0);

		//-- Sync interface
		int32_t SendChat(int64_t& messageID, int64_t uid, const string& message, const string& attrs = "", int32_t timeout = 0);


* Send Group Chat
	
		//-- Async interface
		bool SendGroupChat(MessageIDCallback callback, int64_t groupID, const string& message, const string& attrs = "", int timeout = 0);

		//-- Sync interface
		int32_t SendGroupChat(int64_t& messageID, int64_t groupID, const string& message, const string& attrs = "", int timeout = 0);

* Send Room Chat

		//-- Async interface
		bool SendRoomChat(MessageIDCallback callback, int64_t roomID, const string& message, const string& attrs = "", int timeout = 0);

		//-- Sync interface
		int32_t SendRoomChat(int64_t& messageID, int64_t roomID, const string& message, const string& attrs = "", int timeout = 0);

### SDK Version

	RTMConfig::SDKVersion;

## API docs

Please refer: [API docs](doc/API.md)


## Directory structure

* **\<rtm-client-sdk-unreal\>/Plugins/fpnn**

	Codes and libraries of FPNN SDK.

* **\<rtm-client-sdk-unreal\>/Plugins/rtm**

	Codes of RTM SDK.

* **\<rtm-client-sdk-unreal\>/RTMExample**

	Examples codes for using RTM SDK.

	* NewActorComponent.h/cpp:

		Entery of all examples.

	* RTMTest.h:

		RTMTest base class for all examples.
		RTMTestQuestProcessor for all examples.

* **\<rtm-client-sdk-unreal\>/doc**

	API documents in markdown format.