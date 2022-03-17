# RTM Client UE4 SDK Init API Docs

# Index

[TOC]

### RTM SDK Init (REQUIRED)

	#include "RTMControlCenter.h"
	...
	RTMControlCenter.Init();

**RTMConfig Fields:**

* RTMConfig::lostConnectionAfterLastPingInSeconds

	Max interval in seconds for pings from RTM servers. If the seconds are elapsed and client has not received any ping, RTMClient will consider the connection is broken.

	Default is 60 seconds.

* RTMConfig::globalConnectTimeoutSeconds

	Global RTMClient connecting timeout setting when no special connecting timeout are set for a RTMClient or login function.

	Default is 30 seconds.

* RTMConfig::globalQuestTimeoutSeconds
	
	Global quest timeout setting when no special quest timeout are set for a client or sendQuest function.

	Default is 30 seconds.

* RTMConfig::fileGateClientHoldingSeconds

	Cached seconds for file clients when the file transportation functions are used.

	Default is 150 seconds; 
	
* RegressiveStrategy::startConnectFailedCount

	How many failed connecting times before triggering the regressive reconnecting strategy.

* RegressiveStrategy::maxIntervalSeconds

	Max interval seconds between last failed relogin and next relogin starting when regressive reconnecting strategy acvived.

*  RegressiveStrategy::linearRegressiveCount

	Count of the relogin interval seconds increasing to the max value when regressive reconnecting strategy acvived.

*  RegressiveStrategy::maxRegressvieCount

	Count of the max relogin times.
