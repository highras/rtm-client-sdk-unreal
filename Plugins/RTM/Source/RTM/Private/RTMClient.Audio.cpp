#include "RTMClient.h"

namespace rtm
{
	map<string, string> RTMClient::BuildAudioMessageAttrs(const RTMAudioData& audioData)
	{
		map<string, string> attrs;
		attrs["type"] = "audiomsg";
		attrs["codec"] = audioData._codecType;
		attrs["srate"] = to_string(audioData._frequency);
		attrs["lang"] = audioData._lang;
		attrs["duration"] = to_string(audioData._duration);
		return attrs;
	}

	int32_t RTMClient::SendFile(int64_t& messageID, int64_t uid, const RTMAudioData& audioData, const string& attrs /* = "" */, int32_t timeout /* = 120 */)
	{
		return RealSendFile(messageID, P2PFile, uid, AudioFileType, audioData._audio, "", "", attrs, BuildAudioMessageAttrs(audioData), timeout);
	}

	bool RTMClient::SendFile(MessageIDCallback callback, int64_t uid, const RTMAudioData& audioData, const string& attrs /* = "" */, int32_t timeout /* = 120 */)
	{
		return RealSendFile(callback, P2PFile, uid, AudioFileType, audioData._audio, "", "", attrs, BuildAudioMessageAttrs(audioData), timeout);
	}

	int32_t RTMClient::SendGroupFile(int64_t& messageID, int64_t groupID, const RTMAudioData& audioData, const string& attrs /* = "" */, int32_t timeout /* = 120 */)
	{
		return RealSendFile(messageID, GroupFile, groupID, AudioFileType, audioData._audio, "", "", attrs, BuildAudioMessageAttrs(audioData), timeout);
	}

	bool RTMClient::SendGroupFile(MessageIDCallback callback, int64_t groupID, const RTMAudioData& audioData, const string& attrs /* = "" */, int32_t timeout /* = 120 */)
	{
		return RealSendFile(callback, GroupFile, groupID, AudioFileType, audioData._audio, "", "", attrs, BuildAudioMessageAttrs(audioData), timeout);
	}

	int32_t RTMClient::SendRoomFile(int64_t& messageID, int64_t roomID, const RTMAudioData& audioData, const string& attrs /* = "" */, int32_t timeout /* = 120 */)
	{
		return RealSendFile(messageID, RoomFile, roomID, AudioFileType, audioData._audio, "", "", attrs, BuildAudioMessageAttrs(audioData), timeout);
	}

	bool RTMClient::SendRoomFile(MessageIDCallback callback, int64_t roomID, const RTMAudioData& audioData, const string& attrs /* = "" */, int32_t timeout /* = 120 */)
	{
		return RealSendFile(callback, RoomFile, roomID, AudioFileType, audioData._audio, "", "", attrs, BuildAudioMessageAttrs(audioData), timeout);
	}
}