#pragma once
#include <vector>
#include <msgpack/msgpack.hpp>
#include "FPJson.h"

namespace rtm
{
	using namespace std;
	using namespace fpnn;

	struct QuestResult
	{
		int32_t errorCode;
		string errorInfo;

		QuestResult()
		{
			errorCode = 0;
			errorInfo = "";
		}

		bool isError()
		{
			return errorCode > 0 || !errorInfo.empty();
		}
	};

	struct CommonMessage
	{
		int64_t id;
		int64_t from;
		int8_t mtype;
		int64_t mid;
		bool deleted;
		string msg;
		string attrs;
		int64_t mtime;
		MSGPACK_DEFINE(id, from, mtype, mid, deleted, msg, attrs, mtime);
	};

	struct P2PMessage
	{
		int64_t id;
		int8_t direction;
		int8_t mtype;
		int64_t mid;
		bool deleted;
		string msg;
		string attrs;
		int64_t mtime;
		MSGPACK_DEFINE(id, direction, mtype, mid, deleted, msg, attrs, mtime);
	};

	enum ChatMessageType
	{
		TextChatMType = 30,
		AudioChatMType = 31,
		CmdChatMType = 32,
		RealAudioType = 35,
		RealVideoType = 36,
		ImageFileType = 40,
		AudioFileType = 41,
		VideoFileType = 42,
		NormalFileType = 50
	};

	enum FileTokenType
	{
		P2PFile = 1,
		MultiFiles,
		RoomFile,
		GroupFile,
		BroadcastFile
	};

	enum MessageCategory
	{
		MessageCategory_P2PMessage = 1,
		MessageCategory_GroupMessage = 2,
		MessageCategory_RoomMessage = 3,
		MessageCategory_BroadcastMessage = 4
	};

	enum RTCRoomType
	{
		InvalidRoom = 0,
		VoiceRoom = 1,
		VideoRoom = 2,
	};

	struct SendFileInfo
	{
		FileTokenType type;
		int64_t from;
		int64_t toId;
		set<int64_t> toIds;
	};

	struct TranslatedInfo
	{
		string sourceLanguage;
		string targetLanguage;
		string sourceText;
		string targetText;
	};

	struct FileInfo
	{
		FileInfo() : url(""), surl(""), language(""), size(0), duration(0), isRTMAudio(false) {}
		string url;
		string surl;
		string language;
		int32_t size;
		int32_t duration;
		bool isRTMAudio;
	};

	struct CheckResult
	{
		CheckResult() : result(0) {}
		int32_t result;
		vector<int32_t> tags;
	};

	struct TextCheckResult
	{
		TextCheckResult() : result(0) {}
		string text;
		int32_t result;
		vector<string> wlist;
		vector<int32_t> tags;
	};

	struct RTMMessage
	{
		int64_t fromUid;
		int64_t toId;                   //-- xid
		int8_t messageType;
		int64_t messageId;
		string message;
		string attrs;
		int64_t modifiedTime;
		shared_ptr<TranslatedInfo> translatedInfo;
		shared_ptr<FileInfo> fileInfo;
	};

	struct HistoryMessage : RTMMessage
	{
		int64_t cursorId;
	};

	struct RetrievedMessage : RTMMessage
	{
		int64_t cursorId;
	};

	struct HistoryMessageResult
	{
		int32_t count;
		int64_t lastCursorId;
		int64_t beginMsec;
		int64_t endMsec;
		vector<HistoryMessage> messages;
	};

	enum TranslateType
	{
		Chat,
		Mail
	};

	enum ProfanityType
	{
		Off,
		Censor
	};

	enum CheckType
	{
		Url = 1,
		Content = 2
	};

	enum TranslateLanguage
	{
		ar,             //????????????
		nl,             //?????????
		en,             //??????
		fr,             //??????
		de,             //??????
		el,             //?????????
		id,             //??????????????????
		it,             //????????????
		ja,             //??????
		ko,             //??????
		no,             //?????????
		pl,             //?????????
		pt,             //????????????
		ru,             //??????
		es,             //????????????
		sv,             //?????????
		tl,             //??????????????????????????????
		th,             //??????
		tr,             //????????????
		vi,             //?????????
		zh_cn,       //??????????????????
		zh_tw,       //??????????????????
		None
	};

	inline string getTranslatedLanguage(TranslateLanguage language)
	{
		switch (language)
		{
		case ar:   return "ar";
		case nl:   return "nl";
		case en:   return "en";
		case fr:   return "fr";
		case de:   return "de";
		case el:   return "el";
		case id:   return "id";
		case it:   return "it";
		case ja:   return "ja";
		case ko:   return "ko";
		case no:   return "no";
		case pl:   return "pl";
		case pt:   return "pt";
		case ru:   return "ru";
		case es:   return "es";
		case sv:   return "sv";
		case tl:   return "tl";
		case th:   return "th";
		case tr:   return "tr";
		case vi:   return "vi";
		case zh_cn:   return "zh-CN";
		case zh_tw:   return "zh-TW";
		default:      return "";
		}
	}

	enum AdministratorCommand
	{
		AppointAdministrator = 0, //?????????????????????
		DismissAdministrator = 1, //?????????????????????
		ForbidSendingAudio = 2, //????????????????????????
		AllowSendingAudio = 3, //????????????????????????
		ForbidSendingVideo = 4, //????????????????????????
		AllowSendingVideo = 5, //????????????????????????
		CloseOthersMicroPhone = 6, //?????????????????????
		CloseOthersMicroCamera = 7, //?????????????????????
	};

}

