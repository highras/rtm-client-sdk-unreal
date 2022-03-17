#pragma once
#include "RTMTest.h"

namespace rtm
{
	using namespace std;

	class RTMHistories : public RTMTest
	{
	public:
		RTMHistories(const string& endpoint, int64_t pid, int64_t uid, const string& token)
		{
			_endpoint = endpoint;
			_pid = pid;
			_uid = uid;
			_token = token;
		}

		virtual void Start()
		{
			_thread = thread(&RTMHistories::Test, this);
		}
		virtual void Stop()
		{
			_thread.join();
		}
		void Test()
		{
			LoginRTM(_endpoint, _pid, _uid, _token);
			if (_client == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("User %lld login RTM failed"), _uid);
				return;
			}

			EnterRoom();

			int32_t fetchCount = 100;
			UE_LOG(LogTemp, Warning, TEXT("========== [Get P2P message] =========="));
			GetP2PMessageInSync(_uid + 1, fetchCount);
			UE_LOG(LogTemp, Warning, TEXT("========== [Get group message] =========="));
			GetGroupMessageInSync(_groupId, fetchCount);
			UE_LOG(LogTemp, Warning, TEXT("========== [Get room message] =========="));
			GetRoomMessageInSync(_roomId, fetchCount);
			UE_LOG(LogTemp, Warning, TEXT("========== [Get broadcast message] =========="));
			GetBroadcastMessageInSync(fetchCount);

			UE_LOG(LogTemp, Warning, TEXT("========== [Get P2P chat] =========="));
			GetP2PChatInSync(_uid + 1, fetchCount);
			UE_LOG(LogTemp, Warning, TEXT("========== [Get group chat] =========="));
			GetGroupChatInSync(_groupId, fetchCount);
			UE_LOG(LogTemp, Warning, TEXT("========== [Get room chat] =========="));
			GetRoomMessageInSync(_roomId, fetchCount);
			UE_LOG(LogTemp, Warning, TEXT("========== [Get broadcast chat] =========="));
			GetBroadcastChatInSync(fetchCount);
			UE_LOG(LogTemp, Warning, TEXT("========== [Demo completed] =========="));
		}

		void EnterRoom()
		{
			int32_t errorCode = _client->EnterRoom(_roomId);
			if (errorCode == FPNN_EC_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Enter room %lld in sync succeed."), _roomId);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Enter room %lld in sync failed, errorCode = %d."), _roomId, errorCode);
			}
		}

		void DisplayHistoryMessages(const vector<HistoryMessage>& messages)
		{
			for (auto& message : messages)
			{
				if (RTMClient::CheckFileMessageType(message.messageType))
				{
					if (message.fileInfo == nullptr)
					{
						UE_LOG(LogTemp, Warning, TEXT("fileInfo == nullptr"));
						continue;
					}
					if (message.fileInfo->isRTMAudio)
					{
						UE_LOG(LogTemp, Warning, TEXT("-- [Audio message] id: %lld, from: %lld, mtype: %d, mid: %lld, url: %s, size: %d, attrs: %s, mtime: %lld, language: %s, duration: %d ms"), 
							message.cursorId, message.fromUid, message.messageType, message.messageId, UTF8_TO_TCHAR(message.fileInfo->url.c_str()), message.fileInfo->size, UTF8_TO_TCHAR(message.attrs.c_str()), message.modifiedTime, UTF8_TO_TCHAR(message.fileInfo->language.c_str()), message.fileInfo->duration);
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("-- [File message] id: %lld, from: %lld, mtype: %d, mid: %lld, url: %s, size: %u, attrs: %s, mtime: %lld"),
							message.cursorId, message.fromUid, message.messageType, message.messageId, UTF8_TO_TCHAR(message.fileInfo->url.c_str()), message.fileInfo->size, UTF8_TO_TCHAR(message.attrs.c_str()), message.modifiedTime);
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("-- [String message] id: %lld, from: %lld, mtype: %d, mid: %lld, attrs: %s, mtime: %lld"),
						message.cursorId, message.fromUid, message.messageType, message.messageId, UTF8_TO_TCHAR(message.attrs.c_str()), message.modifiedTime);

				}
			}
		}

		void GetP2PMessageInSync(int64_t uid, int32_t count)
		{
			int64_t begin = 0;
			int64_t end = 0;
			int64_t lastID = 0;
			int32_t fetchCount = 0;

			while (count > 0)
			{
				int32_t maxCount = (count > 20) ? 20 : count;
				count -= maxCount;

				HistoryMessageResult result;
				int32_t errorCode = _client->GetP2PMessage(result, uid, true, maxCount, begin, end, lastID);
				if (errorCode != FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Get P2P history message with user %lld in sync failed, errorCode = %d."), uid, errorCode);
					return;
				}

				fetchCount += result.count;
				DisplayHistoryMessages(result.messages);

				begin = result.beginMsec;
				end = result.endMsec;
				lastID = result.lastCursorId;
			}
			UE_LOG(LogTemp, Warning, TEXT("Get P2P histroy message total fetched %d items."), fetchCount);
		}

		void GetGroupMessageInSync(int64_t groupID, int32_t count)
		{
			int64_t begin = 0;
			int64_t end = 0;
			int64_t lastID = 0;
			int32_t fetchCount = 0;

			while (count > 0)
			{
				int32_t maxCount = (count > 20) ? 20 : count;
				count -= maxCount;

				HistoryMessageResult result;
				int32_t errorCode = _client->GetGroupMessage(result, groupID, true, maxCount, begin, end, lastID);
				if (errorCode != FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Get group history message in group %lld in sync failed, errorCode = %d."), groupID, errorCode);
					return;
				}

				fetchCount += result.count;
				DisplayHistoryMessages(result.messages);

				begin = result.beginMsec;
				end = result.endMsec;
				lastID = result.lastCursorId;
			}
			UE_LOG(LogTemp, Warning, TEXT("Get group histroy message total fetched %d items."), fetchCount);
		}

		void GetRoomMessageInSync(int64_t roomID, int32_t count)
		{
			int64_t begin = 0;
			int64_t end = 0;
			int64_t lastID = 0;
			int32_t fetchCount = 0;

			while (count > 0)
			{
				int32_t maxCount = (count > 20) ? 20 : count;
				count -= maxCount;

				HistoryMessageResult result;
				int32_t errorCode = _client->GetRoomMessage(result, roomID, true, maxCount, begin, end, lastID);
				if (errorCode != FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Get room history message in room %lld in sync failed, errorCode = %d."), roomID, errorCode);
					return;
				}

				fetchCount += result.count;
				DisplayHistoryMessages(result.messages);

				begin = result.beginMsec;
				end = result.endMsec;
				lastID = result.lastCursorId;
			}
			UE_LOG(LogTemp, Warning, TEXT("Get room histroy message total fetched %d items."), fetchCount);
		}

		void GetBroadcastMessageInSync(int32_t count)
		{
			int64_t begin = 0;
			int64_t end = 0;
			int64_t lastID = 0;
			int32_t fetchCount = 0;

			while (count > 0)
			{
				int32_t maxCount = (count > 20) ? 20 : count;
				count -= maxCount;

				HistoryMessageResult result;
				int32_t errorCode = _client->GetBroadcastMessage(result, true, maxCount, begin, end, lastID);
				if (errorCode != FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Get broadcast history message in sync failed, errorCode = %d."), errorCode);
					return;
				}

				fetchCount += result.count;
				DisplayHistoryMessages(result.messages);

				begin = result.beginMsec;
				end = result.endMsec;
				lastID = result.lastCursorId;
			}
			UE_LOG(LogTemp, Warning, TEXT("Get broadcast histroy message total fetched %d items."), fetchCount);
		}

		void GetP2PChatInSync(int64_t uid, int32_t count)
		{
			int64_t begin = 0;
			int64_t end = 0;
			int64_t lastID = 0;
			int32_t fetchCount = 0;

			while (count > 0)
			{
				int32_t maxCount = (count > 20) ? 20 : count;
				count -= maxCount;

				HistoryMessageResult result;
				int32_t errorCode = _client->GetP2PChat(result, uid, true, maxCount, begin, end, lastID);
				if (errorCode != FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Get P2P history chat with user %lld in sync failed, errorCode = %d."), uid, errorCode);
					return;
				}

				fetchCount += result.count;
				DisplayHistoryMessages(result.messages);

				begin = result.beginMsec;
				end = result.endMsec;
				lastID = result.lastCursorId;
			}
			UE_LOG(LogTemp, Warning, TEXT("Get P2P histroy chat total fetched %d items."), fetchCount);
		}

		void GetGroupChatInSync(int64_t groupID, int32_t count)
		{
			int64_t begin = 0;
			int64_t end = 0;
			int64_t lastID = 0;
			int32_t fetchCount = 0;

			while (count > 0)
			{
				int32_t maxCount = (count > 20) ? 20 : count;
				count -= maxCount;

				HistoryMessageResult result;
				int32_t errorCode = _client->GetGroupChat(result, groupID, true, maxCount, begin, end, lastID);
				if (errorCode != FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Get group history chat in group %lld in sync failed, errorCode = %d."), groupID, errorCode);
					return;
				}

				fetchCount += result.count;
				DisplayHistoryMessages(result.messages);

				begin = result.beginMsec;
				end = result.endMsec;
				lastID = result.lastCursorId;
			}
			UE_LOG(LogTemp, Warning, TEXT("Get group histroy chat total fetched %d items."), fetchCount);

		}

		void GetRoomChatInSync(int64_t roomID, int32_t count)
		{
			int64_t begin = 0;
			int64_t end = 0;
			int64_t lastID = 0;
			int32_t fetchCount = 0;

			while (count > 0)
			{
				int32_t maxCount = (count > 20) ? 20 : count;
				count -= maxCount;

				HistoryMessageResult result;
				int32_t errorCode = _client->GetRoomChat(result, roomID, true, maxCount, begin, end, lastID);
				if (errorCode != FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Get room chat message in room %lld in sync failed, errorCode = %d."), roomID, errorCode);
					return;
				}

				fetchCount += result.count;
				DisplayHistoryMessages(result.messages);

				begin = result.beginMsec;
				end = result.endMsec;
				lastID = result.lastCursorId;
			}
			UE_LOG(LogTemp, Warning, TEXT("Get room histroy chat total fetched %d items."), fetchCount);
		}

		void GetBroadcastChatInSync(int32_t count)
		{
			int64_t begin = 0;
			int64_t end = 0;
			int64_t lastID = 0;
			int32_t fetchCount = 0;

			while (count > 0)
			{
				int32_t maxCount = (count > 20) ? 20 : count;
				count -= maxCount;

				HistoryMessageResult result;
				int32_t errorCode = _client->GetBroadcastChat(result, true, maxCount, begin, end, lastID);
				if (errorCode != FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("Get broadcast history chat in sync failed, errorCode = %d."), errorCode);
					return;
				}

				fetchCount += result.count;
				DisplayHistoryMessages(result.messages);

				begin = result.beginMsec;
				end = result.endMsec;
				lastID = result.lastCursorId;
			}
			UE_LOG(LogTemp, Warning, TEXT("Get broadcast histroy chat total fetched %d items."), fetchCount);
		}

	private:
		thread _thread;
		string _endpoint;
		int64_t _pid;
		int64_t _uid;
		string _token;
	};
}
