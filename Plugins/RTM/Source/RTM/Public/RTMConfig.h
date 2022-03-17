#pragma once

#include <string>

namespace rtm
{
	using namespace std;

	class RegressiveStrategy
	{
	public:
		static int32_t startConnectFailedCount;	//-- 连接失败多少次后，开始退行性处理
		static int32_t maxIntervalSeconds;      //-- 退行性重连最大时间间隔
		static int32_t linearRegressiveCount;   //-- 从第一次退行性连接开始，到最大链接时间，允许尝试几次连接，每次时间间隔都会增大
		static int32_t maxRegressvieCount;      //-- 退行性重连最大次数，超出该次数则不再进行重连
	};

	class RTMConfig
	{
	public:
		const static string SDKVersion;
		const static string InterfaceVersion;

		static int32_t lostConnectionAfterLastPingInSeconds;
		static int32_t globalConnectTimeoutSeconds;
		static int32_t globalQuestTimeoutSeconds;
		static int32_t fileGateClientHoldingSeconds;
		//static common.ErrorRecorder errorRecorder = null;
		//static bool triggerCallbackIfAsyncMethodReturnFalse;
		//static RegressiveStrategy globalRegressiveStrategy = new RegressiveStrategy();

	};
}

