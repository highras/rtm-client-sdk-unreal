#pragma once

#include "RTMStructure.h"
#include <mutex>

namespace rtm
{
	using namespace std;
	using namespace fpnn;
	struct MessageDuplicateKey
	{
		int64_t from;
		int64_t to;
		int64_t mid;
		MessageCategory messageCategory;

		MessageDuplicateKey(int64_t from, int64_t to, int64_t mid, MessageCategory messageCategory) : from(from), to(to), mid(mid), messageCategory(messageCategory) {}
	};

	struct MessageDuplicateKeyHash
	{
		size_t operator()(const MessageDuplicateKey& k) const;
	};

	struct MessageDuplicateKeyEqual
	{
		bool operator()(const struct MessageDuplicateKey& key1, const struct MessageDuplicateKey& key2) const;
	};
	class DuplicatedMessageFilter
	{
	public:
		static bool CheckP2PMessage(int64_t from, int64_t mid);
		static bool CheckGroupMessage(int64_t from, int64_t to, int64_t mid);
		static bool CheckRoomMessage(int64_t from, int64_t to, int64_t mid);
		static bool CheckBroadcastMessage(int64_t from, int64_t mid);
	private:
		static bool CheckMessage(const MessageDuplicateKey& key);
		static void CheckExpiredMessage();
	private:
		static unordered_map<MessageDuplicateKey, int64_t, MessageDuplicateKeyHash, MessageDuplicateKeyEqual> _cacheMap;
		const static int32_t _expireSeconds;
		static mutex _mutex;
	};
}
