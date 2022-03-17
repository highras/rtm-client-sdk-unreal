#pragma once
#include "RTMTest.h"

namespace rtm
{
	using namespace std;
	class RTMChat : public RTMTest
	{
	public:
		RTMChat(const string& endpoint, int64_t pid, int64_t uid, const string& token)
		{
			_endpoint = endpoint;
			_pid = pid;
			_uid = uid;
			_token = token;
		}

		virtual void Start()
		{
			_thread = thread(&RTMChat::Test, this);
		}
		virtual void Stop()
		{
			//_thread.join();
		}
		void Test()
		{
			LoginRTM(_endpoint, _pid, _uid, _token);
			if (_client == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("User %lld login RTM failed"), _uid);
				return;
			}
			
			SendP2PChat();
			SendP2PCmd();

			SendGroupChat();
			SendGroupCmd();

			if (EnterRoom())
			{
				SendRoomChat();
				SendRoomCmd();
			}

			GetP2PUnread();
			GetGroupUnread();

			TextCheck("sdaada asdasd asdasd asdas dds");
			TextCheck("ssds 他妈的， 去你妈逼，操你妈的");
			TextCheck("sdaada fuck you mother dds");

			ImageCheck("https://box.bdimg.com/static/fisp_static/common/img/searchbox/logo_news_276_88_1f9876a.png");
			AudioCheck("https://opus-codec.org/static/examples/samples/speech_orig.wav");
			VideoCheck("http://vfx.mtime.cn/Video/2019/02/04/mp4/190204084208765161.mp4");

			UE_LOG(LogTemp, Warning, TEXT("Running for receiving server pushed messsage if those are being demoed ..."));
		}
		
		void SendP2PChat()
		{
			int64_t messageID = 0;
			int32_t errorCode = _client->SendChat(messageID, _peerUid, _textMessage);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send chat message to user %lld in sync successed, messageId is %ldd"), _peerUid, messageID);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Send chat message to user %lld in sync failed, errorCode = %d"), _peerUid, errorCode);
			}

			bool status = _client->SendChat([this](int64_t messageID, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Send chat message to user %lld in async successed, messageId is %ldd"), _peerUid, messageID);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Send chat message to user %lld in async failed, errorCode = %d"), _peerUid, errorCode);
				}
				}, _peerUid, _textMessage);
			if (!status)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send chat message to user %lld in async failed"), _peerUid);
			}
		}

		void SendP2PCmd()
		{
			int64_t messageID = 0;
			int32_t errorCode = _client->SendCommand(messageID, _peerUid, _textMessage);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send cmd message to user %lld in sync successed, messageId is %ldd"), _peerUid, messageID);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Send cmd message to user %lld in sync failed"), _peerUid);
			}

			bool status = _client->SendCommand([this](int64_t messageID, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Send cmd message to user %lld in async successed, messageId is %ldd"), _peerUid, messageID);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Send cmd message to user %lld in async failed"), _peerUid);
				}
				}, _peerUid, _textMessage);
			if (!status)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send cmd message to user %lld in async failed"), _peerUid);
			}
		}

		void SendGroupChat()
		{
			int64_t messageID = 0;
			int32_t errorCode = _client->SendGroupChat(messageID, _groupId, _textMessage);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send group message to group %lld in sync successed, messageId is %ldd"), _groupId, messageID);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Send group message to group %lld in sync failed"), _groupId);
			}

			bool status = _client->SendGroupChat([this](int64_t messageID, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Send group message to group %lld in async successed, messageId is %ldd"), _groupId, messageID);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Send group message to group %lld in async failed"), _groupId);
				}
				}, _groupId, _textMessage);
			if (!status)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send group message to group %lld in async failed"), _groupId);
			}
		}

		void SendGroupCmd()
		{
			int64_t messageID = 0;
			int32_t errorCode = _client->SendGroupCommand(messageID, _groupId, _textMessage);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send group cmd to group %lld in sync successed, messageId is %ldd"), _groupId, messageID);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Send group cmd to group %lld in sync failed"), _groupId);
			}

			bool status = _client->SendGroupCommand([this](int64_t messageID, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Send group cmd to group %lld in async successed, messageId is %ldd"), _groupId, messageID);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Send group cmd to group %lld in async failed"), _groupId);
				}
				}, _groupId, _textMessage);
			if (!status)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send group cmd to group %lld in async failed"), _groupId);
			}
		}

		bool EnterRoom()
		{
			int32_t errorCode = _client->EnterRoom(_roomId);
			if (errorCode != FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Enter room  %lld in sync failed."), _roomId);
				return false;
			}
			else
				return true;
		}

		void SendRoomChat()
		{
			int64_t messageID = 0;
			int32_t errorCode = _client->SendRoomChat(messageID, _roomId, _textMessage);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send room message to room %lld in sync successed, messageId is %ldd"), _roomId, messageID);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Send room message to room %lld in sync failed"), _roomId);
			}
			bool status = _client->SendRoomChat([this](int64_t messageID, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Send room message to room %lld in async successed, messageId is %ldd"), _roomId, messageID);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Send room message to room %lld in async failed"), _roomId);
				}
				}, _groupId, _textMessage);
			if (!status)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send room message to room %lld in async failed"), _roomId);
			}
		}

		void SendRoomCmd()
		{
			int64_t messageID = 0;
			int32_t errorCode = _client->SendRoomCommand(messageID, _roomId, _textMessage);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send room cmd to room %lld in sync successed, messageId is %ldd"), _roomId, messageID);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Send room cmd to room%lld in sync failed"), _roomId);
			}

			bool status = _client->SendRoomCommand([this](int64_t messageID, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Send room cmd to room %lld in async successed, messageId is %ldd"), _roomId, messageID);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Send room cmd to room %lld in async failed"), _roomId);
				}
				}, _roomId, _textMessage);
			if (!status)
			{
				UE_LOG(LogTemp, Warning, TEXT("Send room cmd to room %lld in async failed"), _roomId);
			}
		}

		void GetP2PUnread()
		{
			map<int64_t, int32_t> unreadMap;
			map<int64_t, int64_t> lastTimestampMap;
			int32_t errorCode = _client->GetP2PUnread(unreadMap, lastTimestampMap, { _peerUid, _peerUid + 1, _peerUid + 2 });
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Fetch P2P unread in sync succeed."));
				for (auto kv : unreadMap)
				{
					UE_LOG(LogTemp, Warning, TEXT(" -- peer: %lld, unread message %d"), kv.first, kv.second);
				}
				for (auto kv : lastTimestampMap)
				{
					UE_LOG(LogTemp, Warning, TEXT(" -- peer: %lld, last timestamp %lld"), kv.first, kv.second);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Fetch P2P unread in sync failed, errorCode = %d."), errorCode);
			}

			errorCode = _client->GetP2PUnread(unreadMap, lastTimestampMap, { _peerUid, _peerUid + 1, _peerUid + 2 }, 0, {30, 40, 50});
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Fetch P2P unread with mtypes in sync succeed."));
				for (auto kv : unreadMap)
				{
					UE_LOG(LogTemp, Warning, TEXT(" -- peer: %lld, unread message %d"), kv.first, kv.second);
				}
				for (auto kv : lastTimestampMap)
				{
					UE_LOG(LogTemp, Warning, TEXT(" -- peer: %lld, last timestamp %lld"), kv.first, kv.second);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Fetch P2P unread with mtypes in sync failed, errorCode = %d"), errorCode);
			}

			bool status = _client->GetP2PUnread([this](map<int64_t, int32_t> unreadMap, map<int64_t, int64_t> lastTimestampMap, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Fetch P2P unread in async succeed."));
					for (auto kv : unreadMap)
					{
						UE_LOG(LogTemp, Warning, TEXT(" -- peer: %lld, unread message %d"), kv.first, kv.second);
					}
					for (auto kv : lastTimestampMap)
					{
						UE_LOG(LogTemp, Warning, TEXT(" -- peer: %lld, last timestamp %lld"), kv.first, kv.second);
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Fetch P2P unread in async failed, errorCode = %d"), errorCode);
				}
				}, { _peerUid, _peerUid + 1, _peerUid + 2 });
			if (!status)
			{
				UE_LOG(LogTemp, Warning, TEXT("Fetch P2P unread in async failed."));
			}

			status = _client->GetP2PUnread([this](map<int64_t, int32_t> unreadMap, map<int64_t, int64_t> lastTimestampMap, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Fetch P2P unread with mtypes in async succeed."));
					for (auto kv : unreadMap)
					{
						UE_LOG(LogTemp, Warning, TEXT(" -- peer: %lld, unread message %d"), kv.first, kv.second);
					}
					for (auto kv : lastTimestampMap)
					{
						UE_LOG(LogTemp, Warning, TEXT(" -- peer: %lld, last timestamp %lld"), kv.first, kv.second);
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Fetch P2P unread with mtypes in async failed, errorCode = %d."), errorCode);
				}
				}, { _peerUid, _peerUid + 1, _peerUid + 2 }, 0, { 30, 40, 50 });
			if (!status)
			{
				UE_LOG(LogTemp, Warning, TEXT("Fetch P2P unread with mtypes in async failed."));
			}
		}

		void GetGroupUnread()
		{
			map<int64_t, int32_t> unreadMap;
			map<int64_t, int64_t> lastTimestampMap;
			int32_t errorCode = _client->GetGroupUnread(unreadMap, lastTimestampMap, {_groupId});
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Fetch group unread in sync succeed."));
				for (auto kv : unreadMap)
				{
					UE_LOG(LogTemp, Warning, TEXT(" -- peer: %lld, unread message %d"), kv.first, kv.second);
				}
				for (auto kv : lastTimestampMap)
				{
					UE_LOG(LogTemp, Warning, TEXT(" -- peer: %lld, last timestamp %lld"), kv.first, kv.second);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Fetch group unread in sync failed, errorCode = %d."), errorCode);
			}

			errorCode = _client->GetGroupUnread(unreadMap, lastTimestampMap, {_groupId}, 0, {30, 40, 50});
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Fetch group unread with mtypes in sync succeed."));
				for (auto kv : unreadMap)
				{
					UE_LOG(LogTemp, Warning, TEXT(" -- peer: %lld, unread message %d"), kv.first, kv.second);
				}
				for (auto kv : lastTimestampMap)
				{
					UE_LOG(LogTemp, Warning, TEXT(" -- peer: %lld, last timestamp %lld"), kv.first, kv.second);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Fetch group unread with mtypes in sync failed, errorCode = %d"), errorCode);
			}

			bool status = _client->GetGroupUnread([this](map<int64_t, int32_t> unreadMap, map<int64_t, int64_t> lastTimestampMap, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Fetch group unread in async succeed."));
					for (auto kv : unreadMap)
					{
						UE_LOG(LogTemp, Warning, TEXT(" -- peer: %lld, unread message %d"), kv.first, kv.second);
					}
					for (auto kv : lastTimestampMap)
					{
						UE_LOG(LogTemp, Warning, TEXT(" -- peer: %lld, last timestamp %lld"), kv.first, kv.second);
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Fetch group unread in async failed, errorCode = %d"), errorCode);
				}
				}, {_groupId});
			if (!status)
			{
				UE_LOG(LogTemp, Warning, TEXT("Fetch group unread in async failed."));
			}

			status = _client->GetGroupUnread([this](map<int64_t, int32_t> unreadMap, map<int64_t, int64_t> lastTimestampMap, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Fetch group unread with mtypes in async succeed."));
					for (auto kv : unreadMap)
					{
						UE_LOG(LogTemp, Warning, TEXT(" -- peer: %lld, unread message %d"), kv.first, kv.second);
					}
					for (auto kv : lastTimestampMap)
					{
						UE_LOG(LogTemp, Warning, TEXT(" -- peer: %lld, last timestamp %lld"), kv.first, kv.second);
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Fetch group unread with mtypes in async failed, errorCode = %d."), errorCode);
				}
				}, {_groupId}, 0, { 30, 40, 50 });
			if (!status)
			{
				UE_LOG(LogTemp, Warning, TEXT("Fetch group unread with mtypes in async failed."));
			}
		}

		void TextCheck(const string& text)
		{
			TextCheckResult result;
			int32_t errorCode = _client->TextCheck(result, text);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("TextCheck in sync succeed"));
				UE_LOG(LogTemp, Warning, TEXT(" -- result %d"), result.result);
				UE_LOG(LogTemp, Warning, TEXT(" -- text %s"), UTF8_TO_TCHAR(result.text.c_str()));
				if (!result.tags.empty())
					UE_LOG(LogTemp, Warning, TEXT(" -- tags.size %u"), result.tags.size());
				if (!result.wlist.empty())
					UE_LOG(LogTemp, Warning, TEXT(" -- wlist.size %u"), result.wlist.size());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("TextCheck in sync failed, errorCode = %d"), errorCode);
			}

			bool status = _client->TextCheck([this](TextCheckResult result, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("TextCheck in async succeed"));
					UE_LOG(LogTemp, Warning, TEXT(" -- result %d"), result.result);
					UE_LOG(LogTemp, Warning, TEXT(" -- text %s"), UTF8_TO_TCHAR(result.text.c_str()));
					if (!result.tags.empty())
						UE_LOG(LogTemp, Warning, TEXT(" -- tags.size %u"), result.tags.size());
					if (!result.wlist.empty())
						UE_LOG(LogTemp, Warning, TEXT(" -- wlist.size %u"), result.wlist.size());
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("TextCheck in async failed, errorCode = %d"), errorCode);
				}
				}, text);
			if (!status)
				UE_LOG(LogTemp, Warning, TEXT("TextCheck in async failed"));
		}

		void ImageCheck(const string& url)
		{
			CheckResult result;
			int32_t errorCode = _client->ImageCheck(result, url, Url);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("ImageCheck in sync succeed"));
				UE_LOG(LogTemp, Warning, TEXT(" -- result %d"), result.result);
				if (!result.tags.empty())
					UE_LOG(LogTemp, Warning, TEXT(" -- tags.size %u"), result.tags.size());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("ImageCheck in sync failed, errorCode = %d"), errorCode);
			}

			bool status = _client->ImageCheck([this](CheckResult result, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("ImageCheck in async succeed"));
					UE_LOG(LogTemp, Warning, TEXT(" -- result %d"), result.result);
					if (!result.tags.empty())
						UE_LOG(LogTemp, Warning, TEXT(" -- tags.size %u"), result.tags.size());
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("ImageCheck in async failed, errorCode = %d"), errorCode);
				}
				}, url, Url);
			if (!status)
			{
				UE_LOG(LogTemp, Warning, TEXT("ImageCheck in async failed"));
			}
		}

		void AudioCheck(const string& url)
		{
			CheckResult result;
			int32_t errorCode = _client->AudioCheck(result, url, Url, "zh-CN");
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("AudioCheck in sync succeed"));
				UE_LOG(LogTemp, Warning, TEXT(" -- result %d"), result.result);
				if (!result.tags.empty())
					UE_LOG(LogTemp, Warning, TEXT(" -- tags.size %u"), result.tags.size());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("AudioCheck in sync failed, errorCode = %d"), errorCode);
			}

			bool status = _client->AudioCheck([this](CheckResult result, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("AudioCheck in async succeed"));
					UE_LOG(LogTemp, Warning, TEXT(" -- result %d"), result.result);
					if (!result.tags.empty())
						UE_LOG(LogTemp, Warning, TEXT(" -- tags.size %u"), result.tags.size());
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("AudioCheck in async failed, errorCode = %d"), errorCode);
				}
				}, url, Url, "zh-CN");
			if (!status)
			{
				UE_LOG(LogTemp, Warning, TEXT("AudioCheck in async failed"));
			}
		}

		void VideoCheck(const string& url)
		{
			CheckResult result;
			int32_t errorCode = _client->VideoCheck(result, url, Url, "testVideo");
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("VideoCheck in sync succeed"));
				UE_LOG(LogTemp, Warning, TEXT(" -- result %d"), result.result);
				if (!result.tags.empty())
					UE_LOG(LogTemp, Warning, TEXT(" -- tags.size %u"), result.tags.size());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("VideoCheck in sync failed, errorCode = %d"), errorCode);
			}

			bool status = _client->VideoCheck([this](CheckResult result, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("VideoCheck in async succeed"));
					UE_LOG(LogTemp, Warning, TEXT(" -- result %d"), result.result);
					if (!result.tags.empty())
						UE_LOG(LogTemp, Warning, TEXT(" -- tags.size %u"), result.tags.size());
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("VideoCheck in async failed, errorCode = %d"), errorCode);
				}
				}, url, Url, "testVideo");
			if (!status)
			{
				UE_LOG(LogTemp, Warning, TEXT("VideoCheck in async failed"));
			}
		}

	private:
		thread _thread;
		string _endpoint;
		int64_t _pid;
		int64_t _uid;
		string _token;
	};
}
