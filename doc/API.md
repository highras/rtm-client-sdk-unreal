# RTM Client UE4 SDK API Docs

# Index

[TOC]

## Current Version

	const string RTMConfig::SDKVersion = "1.0.0";

## Init & Config SDK

Initialize and configure RTM SDK, and dependent FPNN SDK. 

PLease refer：[Init & Config FPNN SDK & RTM SDK](Init.md)

## RTM Client

### Overview

* **Namespace**

		rtm

* **Declaration**

		class RTMClient

### Constructors

	static RTMClientPtr CreateRTMClient(int64_t pid, int64_t uid, const string& endpoint, RTMQuestProcessorPtr rtmQuestProcessor, bool autoRelogin = true);


* pid:

	Project ID. Please get your project id from RTM Console.

* uid:

	User id assigned by your project.

* endpoint:

	RTM servers endpoint. Please get your project endpoint from RTM Console.

* rtmQuestProcessor:

	Instance of events processor inheriting RTMQuestProcessor.

	Please refer [Event Process](EventProcess.md)

* autoRelogin:

	Enable auto-relogin when connection lost after **first successful login** if user's token is available and user isn't forbidden.

### Properties

* **int64_t GetPid**

		int64_t GetPid();
		

	Project ID.

* **int64_t GetUid**

		int64_t GetUid();

	User ID assigned by you project.

* **ClientStatus GetClientStatus()**

		ClientStatus GetClientStatus();

	RTM client current status.

	Values:

	+ ClientStatus.Closed
	+ ClientStatus.Connecting
	+ ClientStatus.Connected

### Structures

Please refer [RTM Structures](Structures.md)

### Delegates

Please refer [RTM Delegates](Delegates.md)

### Event Process

Please refer [Event Process](EventProcess.md)

### Methods

#### Login & Logout Functions

Please refer [Login & Logout Functions](LoginLogout.md)

#### RTM System Functions

Please refer [RTM System Functions](System.md)

#### Chat Functions

Please refer [Chat Functions](Chat.md)

#### Value-Added Functions

Please refer [Value-Added Functions](ValueAdded.md)

#### Messages Functions

Please refer [Messages Functions](Messages.md)

#### Audio Functions

Please refer [Audio Functions](Audio.md)

#### Files Functions

Please refer [Files Functions](Files.md)

#### Friends Functions

Please refer [Friends Functions](Friends.md)

#### Groups Functions

Please refer [Groups Functions](Groups.md)

#### Rooms Functions

Please refer [Rooms Functions](Rooms.md)

#### Users Functions

Please refer [Users Functions](Users.md)

#### Data Functions

Please refer [Data Functions](Data.md)

### Error Codes

[RTM SDK Error Codes](https://github.com/highras/rtm-client-sdk-unity/blob/master/Assets/Plugins/rtm/ErrorCode.cs)  
[FPNN SDK (Transport Layer) Error Codes](https://github.com/highras/rtm-client-sdk-unity/blob/master/Assets/Plugins/fpnn/ErrorCode.cs)
