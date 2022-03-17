#include "RTMConfig.h"

namespace rtm
{
	using namespace std;

	int32_t RegressiveStrategy::startConnectFailedCount = 0;
	int32_t RegressiveStrategy::maxIntervalSeconds = 8;     
	int32_t RegressiveStrategy::linearRegressiveCount = 4;  
	int32_t RegressiveStrategy::maxRegressvieCount = 10;    


	const string RTMConfig::SDKVersion = "1.0.0";
	const string RTMConfig::InterfaceVersion = "2.7.0";

	int32_t RTMConfig::lostConnectionAfterLastPingInSeconds = 60;
	int32_t RTMConfig::globalConnectTimeoutSeconds = 30;
	int32_t RTMConfig::globalQuestTimeoutSeconds = 30;
	int32_t RTMConfig::fileGateClientHoldingSeconds = 150;
	//bool RTMConfig::triggerCallbackIfAsyncMethodReturnFalse = false;
}