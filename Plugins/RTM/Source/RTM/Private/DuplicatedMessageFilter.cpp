#include "DuplicatedMessageFilter.h"
#include "TimeUtil.h"
#include "jenkins.h"

namespace rtm
{
	using namespace std;
	using namespace fpnn;

	unordered_map<MessageDuplicateKey, int64_t, MessageDuplicateKeyHash, MessageDuplicateKeyEqual> DuplicatedMessageFilter::_cacheMap;
	mutex DuplicatedMessageFilter::_mutex;
	const int32_t DuplicatedMessageFilter::_expireSeconds = 60 * 20;

	size_t MessageDuplicateKeyHash::operator()(const MessageDuplicateKey& k) const
	{
		return (size_t)(hash<int32_t>()(k.messageCategory) << 24 || hash<int64_t>()(k.from) << 16 
			|| hash<int64_t>()(k.to) << 8 || hash<int64_t>()(k.mid));
	}

	bool MessageDuplicateKeyEqual::operator()(const struct MessageDuplicateKey& key1, const struct MessageDuplicateKey& key2) const
	{
		return key1.messageCategory == key2.messageCategory && key1.from == key2.from && key1.to == key2.to && key1.mid == key2.mid;
	}

	bool DuplicatedMessageFilter::CheckP2PMessage(int64_t from, int64_t mid)
	{
		MessageDuplicateKey key(from, 0, mid, MessageCategory_P2PMessage);
		return CheckMessage(key);
	}

	bool DuplicatedMessageFilter::CheckGroupMessage(int64_t from, int64_t to, int64_t mid)
	{
		MessageDuplicateKey key(from, to, mid, MessageCategory_GroupMessage);
		return CheckMessage(key);
	}

	bool DuplicatedMessageFilter::CheckRoomMessage(int64_t from, int64_t to, int64_t mid)
	{
		MessageDuplicateKey key(from, to, mid, MessageCategory_RoomMessage);
		return CheckMessage(key);
	}

	bool DuplicatedMessageFilter::CheckBroadcastMessage(int64_t from, int64_t mid)
	{
		MessageDuplicateKey key(from, 0, mid, MessageCategory_BroadcastMessage);
		return CheckMessage(key);
	}

	void DuplicatedMessageFilter::CheckExpiredMessage()
	{
		int64_t time = TimeUtil::curr_sec() - _expireSeconds;
		{
			lock_guard<mutex> locker(_mutex);
			for (auto iter = _cacheMap.begin(); iter != _cacheMap.end();)
			{
				if (iter->second <= time)
					iter = _cacheMap.erase(iter);
				else
					++iter;
			}
		}
	}

	bool DuplicatedMessageFilter::CheckMessage(const MessageDuplicateKey& key)
	{
		int64_t time = TimeUtil::curr_sec();

		{
			lock_guard<mutex> locker(_mutex);
			auto iter = _cacheMap.find(key);
			if (iter == _cacheMap.end())
			{
				CheckExpiredMessage();
				_cacheMap.insert(make_pair(key, time));
				return true;
			}
			iter->second = time;
			return false;
		}
	}
}