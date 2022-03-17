#pragma once

namespace rtm
{
	enum {
		RTM_EC_INVALID_PID_OR_UID = 200001,
		RTM_EC_INVALID_PID_OR_SIGN = 200002,
		RTM_EC_INVALID_FILE_OR_SIGN_OR_TOKEN = 200003,
		RTM_EC_ATTRS_WITHOUT_SIGN_OR_EXT = 200004,
		RTM_EC_INVALID_MTYPE = 200005,
		RTM_EC_SAME_SIGN = 200006,
		RTM_EC_INVALID_FILE_MTYPE = 200007,
		RTM_EC_INVALID_SERFVER_TIME = 200008,

		RTM_EC_FREQUENCY_LIMITED = 200010,
		RTM_EC_REFRESH_SCREEN_LIMITED = 200011,
		RTM_EC_KICKOUT_SELF = 200012,

		RTM_EC_FORBIDDEN_METHOD = 200020,
		RTM_EC_PERMISSION_DENIED = 200021,
		RTM_EC_UNAUTHORIZED = 200022,
		RTM_EC_DUPLCATED_AUTH = 200023,
		RTM_EC_AUTH_DENIED = 200024,
		RTM_EC_ADMIN_LOGIN = 200025,
		RTM_EC_ADMIN_ONLY = 200026,
		RTM_EC_INVALID_AUTH_TOEKN = 200027,

		RTM_EC_LARGE_MESSAGE_OR_ATTRS = 200030,
		RTM_EC_LARGE_FILE_OR_ATTRS = 200031,
		RTM_EC_TOO_MANY_ITEMS_IN_PARAMETERS = 200032,
		RTM_EC_EMPTY_PARAMETER = 200033,
		RTM_EC_INVALID_PARAMETER = 200034,

		RTM_EC_NOT_IN_ROOM = 200040,
		RTM_EC_NOT_GROUP_MEMBER = 200041,
		RTM_EC_MAX_GROUP_MEMBER_COUNT = 200042,
		RTM_EC_NOT_FRIEND = 200043,
		RTM_EC_BANNED_IN_GROUP = 200044,
		RTM_EC_BANNED_IN_ROOM = 200045,
		RTM_EC_EMPTY_GROUP = 200046,
		RTM_EC_MAX_ROOM_COUNT = 200047,
		RTM_EC_MAX_FRIEND_COUNT = 200048,
		RTM_EC_BLOCKED_USER = 200049,

		RTM_EC_UNSUPPORTED_LANGUAGE = 200050,
		RTM_EC_EMPTY_TRANSLATION = 200051,
		RTM_EC_SEND_TO_SELF = 200052,
		RTM_EC_DUPLCATED_MID = 200053,
		RTM_EC_SENSITIVE_WORDS = 200054,
		RTM_EC_NOT_ONLINE = 200055,
		RTM_EC_TRANSLATION_ERROR = 200056,
		RTM_EC_PROFANITY_STOP = 200057,
		RTM_EC_NO_CONFIG_IN_CONSOLE = 200060,
		RTM_EC_UNSUPPORTED_TRASNCRIBE_TYPE = 200061,

		RTM_EC_MESSAGE_NOT_FOUND = 200070,

		RTM_EC_UNKNOWN_ERROR = 200999,
	};
}
