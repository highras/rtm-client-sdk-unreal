// Fill out your copyright notice in the Description page of Project Settings.
#include "RTMClient.h"
#include "RTMControlCenter.h"
#include "RTMQuestProcessor.h"
#include "RTMConfig.h"

namespace rtm
{
	RTMClient::RTMClient(int64_t pid, int64_t uid, const string& endpoint, const string& rtcEndpoint /* = "" */, bool autoRelogin /* = true */) : _pid(pid), _uid(uid), _endpoint(endpoint)
	{
		_requireClose = false;
		_rtmGateConnectionID = 0;
		_rtmGate = Client::createTCPClient(endpoint);
		if (autoRelogin)
			_autoReloginInfo = new AutoReloginInfo;
		else
			_autoReloginInfo = nullptr;
		_authStatusInfo = new AuthStatusInfo;
		_loginLocker = unique_lock<mutex>(_loginMutex);
		_rtcGate = nullptr;
		if (!rtcEndpoint.empty())
			_rtcGate = Client::createUDPClient(rtcEndpoint);
	}

	void RTMClient::Init(RTMQuestProcessorPtr rtmQuestProcessor)
	{
		RTMClientPtr client = shared_from_this();
		_rtmQuestProcessor = make_shared<RTMProcessor>([client](uint64_t connectionID, bool connected) {
			client->Connected(connectionID, connected);
			}, [client](uint64_t connectionID, bool closedByError) {
				client->Closed(connectionID, closedByError);
			});
		_rtmQuestProcessor->SetQuestProcessor(rtmQuestProcessor);
		_rtmGate->setQuestProcessor(_rtmQuestProcessor);
		if (_rtcGate != nullptr)
		{
			_rtcQuestProcessor = make_shared<RTCProcessor>();
			_rtcGate->setQuestProcessor(_rtcQuestProcessor);
		}
	}

	RTMClientPtr RTMClient::CreateRTMClient(int64_t pid, int64_t uid, const string& endpoint, RTMQuestProcessorPtr rtmQuestProcessor, bool autoRelogin /* = true */)
	{
		RTMClientPtr client = RTMClientPtr(new RTMClient(pid, uid, endpoint, "", autoRelogin));
		client->Init(rtmQuestProcessor);
		return client;
	}

	RTMClient::~RTMClient()
	{
		//Close();
		if (_autoReloginInfo)
		{
			delete _autoReloginInfo;
			_autoReloginInfo = nullptr;
		}
		if (_authStatusInfo)
		{
			delete _authStatusInfo;
			_authStatusInfo = nullptr;
		}
		if (_rtmQuestProcessor != nullptr)
		{
			_rtmQuestProcessor->ClearClient();
			_rtmQuestProcessor = nullptr;
		}
	}

	FPQuestPtr RTMClient::GenAuthQuest(const string& token)
	{
		int32_t paramCount = 4;
		if (!_authStatusInfo->language.empty())
			paramCount += 1;
		if (!_authStatusInfo->attrs.empty())
			paramCount += 1;

		FPQWriter qw(paramCount, "auth");
		qw.param("pid", _pid);
		qw.param("uid", _uid);
		qw.param("token", token);
		qw.param("version", "Unreal-" + RTMConfig::SDKVersion);
		if (!_authStatusInfo->language.empty())
			qw.param("lang", _authStatusInfo->language);
		if (!_authStatusInfo->attrs.empty())
			qw.param("attrs", _authStatusInfo->attrs);

		return qw.take();
	}

	int32_t RTMClient::Login(bool& ok, const string& token, const map<string, string>& attrs /* = map<string , string >() */, const string& language /* = "" */, int32_t timeout /* = 0 */)
	{
		int32_t error = FPNN_EC_CORE_TIMEOUT;
		if (timeout == 0)
			timeout = RTMConfig::globalQuestTimeoutSeconds;
		bool status = Login([this, &ok, &error](int64_t projectID, int64_t uid, bool authStatus, int32_t errorCode) {
			ok = authStatus;
			error = errorCode;
			_condition.notify_all();
			}, token, attrs, language, timeout);
		if (!status)
		{
			if (GetClientStatus() == ClientStatus::Connected)
			{
				ok = true;
				return FPNN_EC_OK;
			}
			if (GetClientStatus() == ClientStatus::Closed)
			{
				ok = false;
				return FPNN_EC_CORE_INVALID_CONNECTION;
			}
		}
		_condition.wait_for(_loginLocker, chrono::seconds(timeout));
		return error;
	}

	bool RTMClient::Login(AuthCallback callback, const string& token, const map<string, string>& attrs /* = map<string , string > () */, const string& language /* = "" */, int32_t timeout /* = 0 */)
	{
		{
			lock_guard<mutex> locker(_mutex);
			if (GetClientStatus() == ClientStatus::Connected)
			{
				ClientEngine::runTask([this, callback]() {
					callback(_pid, _uid, true, FPNN_EC_OK);
					});
				return true;
			}

			if (GetClientStatus() == ClientStatus::Connecting)
			{
				_authStatusInfo->callbackList.push_back(callback);
				return true;
			}

			_clientStatus = ClientStatus::Connecting;
			_requireClose = false;
			if (_autoReloginInfo)
				_autoReloginInfo->Login();
		}

		_authStatusInfo->callbackList.push_back(callback);
		_authStatusInfo->token = token;
		_authStatusInfo->attrs = attrs;
		_authStatusInfo->language = language;
		_authStatusInfo->lastActionMsecTimeStamp = TimeUtil::curr_msec();
		_authStatusInfo->remainedTimeout = timeout;

		if (_authStatusInfo->remainedTimeout == 0)
			_authStatusInfo->remainedTimeout = RTMConfig::globalConnectTimeoutSeconds;;
		_rtmGate->asyncConnect();
		return true;
	}

	void RTMClient::StartRelogin()
	{
		bool start = _rtmQuestProcessor->ReloginWillStart(_autoReloginInfo->lastErrorCode, _autoReloginInfo->reloginCount);
		if (!start)
		{
			_rtmQuestProcessor->SessionClosed(_autoReloginInfo->lastErrorCode);
			return;
		}

		RTMControlCenter::UnregisterSession(_rtmGateConnectionID);

		bool startLogin = Login([this](int64_t pid, int64_t uid, bool successful, int32_t errorCode) {
			if (successful)
			{
				_rtmQuestProcessor->ReloginCompleted(true, false, errorCode, _autoReloginInfo->reloginCount);
				_autoReloginInfo->LoginSuccessful();
				return;
			}
			bool connected = false;
			{
				lock_guard<mutex> locker(_mutex);
				if (GetClientStatus() == ClientStatus::Connected)
					connected = true;
			}

			if (connected || errorCode == RTM_EC_DUPLCATED_AUTH)
			{
				//重复认证
				_rtmQuestProcessor->ReloginCompleted(true, false, FPNN_EC_OK, _autoReloginInfo->reloginCount);
				_autoReloginInfo->LoginSuccessful();
				return;
			}

			if (errorCode == FPNN_EC_OK)
			{
				_rtmQuestProcessor->ReloginCompleted(false, false, RTM_EC_INVALID_AUTH_TOEKN, _autoReloginInfo->reloginCount);
				_autoReloginInfo->Disable();
				_rtmQuestProcessor->SessionClosed(RTM_EC_INVALID_AUTH_TOEKN);
				return;
			}

			bool stopRetry = StopRelogin(errorCode);
			if (stopRetry)
			{
				_autoReloginInfo->Disable();
				_rtmQuestProcessor->SessionClosed(errorCode);
				return;
			}
			else
			{
				_autoReloginInfo->lastErrorCode = errorCode;
			}

			StartNextRelogin();
			}, _autoReloginInfo->token, _autoReloginInfo->attrs, _autoReloginInfo->language);
		if (!startLogin)
		{
			ClientStatus status; 
			{
				lock_guard<mutex> locker(_mutex);
				status = GetClientStatus();
			}

			if (status == ClientStatus::Connected)
			{
				_rtmQuestProcessor->ReloginCompleted(true, false, FPNN_EC_OK, _autoReloginInfo->reloginCount);
				_autoReloginInfo->LoginSuccessful();
				return;
			}
			else
			{
				int32_t errorCode = FPNN_EC_CORE_CONNECTION_CLOSED;
				if (status == ClientStatus::Connecting)
					errorCode = FPNN_EC_CORE_UNKNOWN_ERROR;

				_rtmQuestProcessor->ReloginCompleted(false, true, errorCode, _autoReloginInfo->reloginCount);
				_autoReloginInfo->lastErrorCode = errorCode;
				StartNextRelogin();
			}
		}
	}

	void RTMClient::StartNextRelogin()
	{
		if (_autoReloginInfo->reloginCount < RegressiveStrategy::startConnectFailedCount)
		{
			StartRelogin();
			return;
		}

		int32_t regressiveCount = _autoReloginInfo->reloginCount - RegressiveStrategy::startConnectFailedCount;
		if (regressiveCount > RegressiveStrategy::maxRegressvieCount)
			return;
		int64_t interval = RegressiveStrategy::maxIntervalSeconds * 1000;
		if (regressiveCount < RegressiveStrategy::linearRegressiveCount)
			interval = interval * regressiveCount / RegressiveStrategy::linearRegressiveCount;
		RTMControlCenter::DelayRelogin(shared_from_this(), interval);
	}

	bool RTMClient::StopRelogin(int32_t errorCode)
	{
		switch (errorCode)
		{
		case RTM_EC_INVALID_PID_OR_UID:
		case RTM_EC_INVALID_PID_OR_SIGN:
		case RTM_EC_PERMISSION_DENIED:
		case RTM_EC_AUTH_DENIED:
		case RTM_EC_ADMIN_LOGIN:
		case RTM_EC_ADMIN_ONLY:
		case RTM_EC_INVALID_AUTH_TOEKN:
		case RTM_EC_BLOCKED_USER:
			return true;
		default:
			return false;
		}
	}

	void RTMClient::Auth()
	{
//		TCPClientPtr client = GetRTMClient();
//		if (!client)
//			return;
		_rtmQuestProcessor->ClearPingTime();
		FPQuestPtr quest = GenAuthQuest(_authStatusInfo->token);
		bool status = _rtmGate->sendQuest(quest, [this](FPAnswerPtr answer, int errroCode) {
			if (RequireClose())
			{
				AuthFinish(false, FPNN_EC_CORE_CONNECTION_CLOSED);
				return;
			}
			if (errroCode != FPNN_EC_OK)
			{
				AuthFinish(false, errroCode);
				return;
			}
			FPAReader ar(answer);
			bool ok = ar.getBool("ok");
			AuthFinish(ok, FPNN_EC_OK);
			}, _authStatusInfo->remainedTimeout);
		if (!status)
			AuthFinish(false, FPNN_EC_CORE_INVALID_CONNECTION);
	}

	void RTMClient::AuthFinish(bool successful, int32_t errorCode)
	{
		{
			lock_guard<mutex> locker(_mutex);
			if (GetClientStatus() != ClientStatus::Connecting)
				return;
			if (successful)
				_clientStatus = ClientStatus::Connected;
			else
			{
				_clientStatus = ClientStatus::Closed;
				RTMControlCenter::UnregisterSession(_rtmGateConnectionID);
				_rtmGateConnectionID = 0;
			}

			if (_autoReloginInfo)
			{
				_autoReloginInfo->token = _authStatusInfo->token;
				_autoReloginInfo->attrs = _authStatusInfo->attrs;
				_autoReloginInfo->language = _authStatusInfo->language;

				if (successful && !_autoReloginInfo->canRelogin)
					_autoReloginInfo->LoginSuccessful();
			}
		}

		for (auto callback : _authStatusInfo->callbackList)
			callback(_pid, _uid, successful, errorCode);

		_authStatusInfo->Clear();
		if (_rtmQuestProcessor)
			_rtmQuestProcessor->UpdatePingTime();
	}

	void RTMClient::Bye(bool async /* = true */)
	{
		TCPClientPtr client = GetRTMClient();
		if (client == nullptr)
			return;
		FPQuestPtr quest = FPQWriter::emptyQuest("bye");
		if (async)
		{
			bool success = client->sendQuest(quest, [this](FPAnswerPtr answer, int32_t errorCode) { Close(); });
			if (!success)
				Close();
		}
		else
		{
			client->sendQuest(quest);
			Close();
		}
	}

	void RTMClient::Connected(uint64_t connectionID, bool connected)
	{
		if (RequireClose())
		{
			AuthFinish(false, FPNN_EC_CORE_CONNECTION_CLOSED);
			return;
		}
		if (!connected)
		{
			AuthFinish(false, FPNN_EC_CORE_INVALID_CONNECTION);
			return;
		}

		_rtmGateConnectionID = connectionID;
		RTMControlCenter::RegisterSession(connectionID, shared_from_this());
		Auth();
	}

	void RTMClient::Closed(uint64_t connectionID, bool closedByError)
	{
		RTMControlCenter::UnregisterSession(connectionID);
		bool trigger = false;
		bool connecting = false;
		bool relogin = false;
		{
			lock_guard<mutex> locker(_mutex);
			trigger = (connectionID == _rtmGateConnectionID);
			if (trigger)
			{
				if (_clientStatus == ClientStatus::Connecting)
					connecting = true;
				else
					_clientStatus = ClientStatus::Closed;
			}
			if (_autoReloginInfo)
			{
				relogin = _autoReloginInfo->disabled == false && _autoReloginInfo->canRelogin;
				_autoReloginInfo->lastErrorCode = closedByError ? FPNN_EC_CORE_CONNECTION_CLOSED : FPNN_EC_OK;
			}
		}
		if (trigger)
		{
			if (connecting)
				AuthFinish(false, FPNN_EC_CORE_INVALID_CONNECTION);
			else
			{
				if (relogin)
				{
					StartRelogin();
				}
				else
				{
					_rtmQuestProcessor->SessionClosed(closedByError ? FPNN_EC_CORE_UNKNOWN_ERROR : FPNN_EC_OK);
				}
			}
		}
	}

	void RTMClient::Close(bool disableRelogin /* = true */, bool waitConnectingCancelled /* = true */)
	{
		bool connecting = false;

		{
			lock_guard<mutex> locker(_mutex);
			if (GetClientStatus() == ClientStatus::Closed)
				return;
			_requireClose = true;

			if (disableRelogin && _autoReloginInfo)
				_autoReloginInfo->Disable();

			if (GetClientStatus() == ClientStatus::Connecting)
				connecting = true;
			else
				_clientStatus = ClientStatus::Closed;
		}

		_rtmGate->close();
		if (_rtcGate)
			_rtcGate->close();

		if (connecting && waitConnectingCancelled)
			_condition.wait(_loginLocker);
	}

	bool RTMClient::IsAlive()
	{
		if (!_rtmQuestProcessor)
			return false;
		return _rtmQuestProcessor->IsAlive();
	}

	TCPClientPtr RTMClient::GetRTMClient()
	{
		{
			lock_guard<mutex> locker(_mutex);
			if (GetClientStatus() == ClientStatus::Connected)
				return _rtmGate;
			else
				return nullptr;
		}
	}

	UDPClientPtr RTMClient::GetRTCClient()
	{
		{
			lock_guard<mutex> locker(_mutex);
			if (GetClientStatus() == ClientStatus::Connected)
				return _rtcGate;
			else
				return nullptr;
		}
	}
}


