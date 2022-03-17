# RTM Client UE4 SDK API Docs: Event Process

# Index

[TOC]

## Class RTMQuestProcessor

    class RTM_API RTMQuestProcessor
	{
	public:
		// =============== [ System ] ===============
		virtual void SessionClosed(int32_t closedByErrorCode) {}

		// Return true for starting relogin, false for stopping relogin.
		virtual bool ReloginWillStart(int32_t lastErrorCode, int32_t retriedCount) { return true; }
		virtual void ReloginCompleted(bool successful, bool retryAgain, int32_t errorCode, int32_t retriedCount) {}

		virtual void Kickout() {}
		virtual void KickoutRoom(int64_t roomID) {}

		// =============== [ Message ] ===============
		virtual void PushMessage(const RTMMessage& message) {}
		virtual void PushGroupMessage(const RTMMessage& message) {}
		virtual void PushRoomMessage(const RTMMessage& message) {}
		virtual void PushBroadcastMessage(const RTMMessage& message) {}

		// =============== [ Chat ] ===============
		virtual void PushChat(const RTMMessage& message) {}
		virtual void PushGroupChat(const RTMMessage& message) {}
		virtual void PushRoomChat(const RTMMessage& message) {}
		virtual void PushBroadcastChat(const RTMMessage& message) {}

		// =============== [ Cmd ] ===============
		virtual void PushCmd(const RTMMessage& message) {}
		virtual void PushGroupCmd(const RTMMessage& message) {}
		virtual void PushRoomCmd(const RTMMessage& message) {}
		virtual void PushBroadcastCmd(const RTMMessage& message) {}

		// =============== [ Files ] ===============
		virtual void PushFile(const RTMMessage& message) {}
		virtual void PushGroupFile(const RTMMessage& message) {}
		virtual void PushRoomFile(const RTMMessage& message) {}
		virtual void PushBroadcastFile(const RTMMessage& message) {}
	};

### Session Close Event

	void SessionClosed(int32_t closedByErrorCode)

Parameters:

+ `int closedByErrorCode`

	0 or FPNN_EC_OK means closed by user or kickout cmd.

	Others are the reason for failed.

### Server Pushed Events

All methods in RTMQuestProcessor interface except for SessionClosed() are server pushed events.

#### Kickout

Current client is kicked.

The session is closed by RTM SDK automatically before the method is called.

#### Push Files Methods

The parameter `message` is the URL of file in CDN.

#### ReloginWillStart & ReloginCompleted

Will triggered when connection lost after **first successful login** if user's token is available and user isn't forbidden.