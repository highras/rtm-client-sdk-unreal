#include "RTMClient.h"

namespace rtm
{
	using namespace std;
	using namespace fpnn;

	int32_t RTMClient::DataGet(string& value, const string& key, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(1, "dataget");
		qw.param("key", key);
		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		FPAReader ar(answer);
		if (answer->status())
		{
			return ar.getInt("code");
		}
		value = ar.getString("val");

		return FPNN_EC_OK;
	}

	bool RTMClient::DataGet(function<void(string, int32_t)> callback, const string& key, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback("", FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(1, "dataget");
		qw.param("key", key);
		bool status = client->sendQuest(qw.take(), [callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback("", FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback("", errorCode);
				return;
			}
			string value = ar.getString("val");
			callback(value, errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback("", FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	int32_t RTMClient::DataSet(const string& key, const string& value, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(2, "dataset");
		qw.param("key", key);
		qw.param("val", value);
		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		if (answer->status())
		{
			FPAReader ar(answer);
			return ar.getInt("code");
		}

		return FPNN_EC_OK;
	}

	bool RTMClient::DataSet(DoneCallback callback, const string& key, const string& value, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(2, "dataset");
		qw.param("key", key);
		qw.param("val", value);
		bool status = client->sendQuest(qw.take(), [callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(errorCode);
				return;
			}

			callback(errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}

	int32_t RTMClient::DataDelete(const string& key, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
			return FPNN_EC_CORE_INVALID_CONNECTION;
		FPQWriter qw(1, "datadel");
		qw.param("key", key);
		FPAnswerPtr answer = client->sendQuest(qw.take(), timeout);
		if (!answer)
			return FPNN_EC_CORE_UNKNOWN_ERROR;

		if (answer->status())
		{
			FPAReader ar(answer);
			return ar.getInt("code");
		}

		return FPNN_EC_OK;
	}

	bool RTMClient::DataDelete(DoneCallback callback, const string& key, int32_t timeout /* = 0 */)
	{
		TCPClientPtr client = GetRTMClient();
		if (!client)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
			return false;
		}
		FPQWriter qw(1, "datadel");
		qw.param("key", key);
		bool status = client->sendQuest(qw.take(), [callback](FPAnswerPtr answer, int errorCode) {
			if (!answer)
			{
				callback(FPNN_EC_CORE_UNKNOWN_ERROR);
				return;
			}
			FPAReader ar(answer);
			if (answer->status())
			{
				callback(errorCode);
				return;
			}

			callback(errorCode);
			}, timeout);
		if (!status)
		{
			ClientEngine::runTask([callback]() {
				callback(FPNN_EC_CORE_INVALID_CONNECTION);
				});
		}
		return true;
	}
}