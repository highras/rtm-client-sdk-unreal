# RTM Client UE4 SDK API Docs: Structures

# Index

[TOC]

### CheckResult

	struct CheckResult
	{
		int32_t result;
		vector<int32_t> tags;
	};

* `result`

    Audit's result.

    Chinese documents:

    Image Review: [imageSpams -> result](https://docs.ilivedata.com/imagecheck/techdocs/respon/)

    Audio Checking: [result](https://docs.ilivedata.com/audiocheck/techdoc/callres/)

    Audio Checking (live): [audioSpams -> result](https://docs.ilivedata.com/audiocheck/livetechdoc/livecallres/)

    Video Review: [result](https://docs.ilivedata.com/videocheck/techdoc/callres/)

    Video Review (live): [result](https://docs.ilivedata.com/videocheck/livetechdoc/livecallres/)

* `tags`

    Classified information for found sensitive information.

    Chinese documents:

    Image Review: [tags](https://docs.ilivedata.com/imagecheck/techdocs/respon/)

    Audio Checking: [tags](https://docs.ilivedata.com/audiocheck/techdoc/callres/)

    Audio Checking (live): [tags](https://docs.ilivedata.com/audiocheck/livetechdoc/livecallres/)

    Video Review: [tags](https://docs.ilivedata.com/videocheck/techdoc/callres/)

    Video Review (live): [tags](https://docs.ilivedata.com/videocheck/livetechdoc/livecallres/)

### TextCheckResult

	struct TextCheckResult
	{
		string text;
		int32_t result;
		vector<string> wlist;
		vector<int32_t> tags;
	};

* `result`

    Audit's result.  
    Please refer [textSpams -> result](https://docs.ilivedata.com/textcheck/technologydocument/http/) (Chinese document).

* `tags`

    Classified information for found sensitive words.  
    Please refer [tags](https://docs.ilivedata.com/textcheck/technologydocument/http/) (Chinese document).

* `text`

    If all words is passed the audit, the field is null or empty.  
    If any sensitive word is found, this field is the filtered text. The sensitive words are repleaced by `*`.

* `wlist`

    Sensitive words list.

### FileInfo

	struct FileInfo
	{
		string url;
		string surl;
		string language;
		int32_t size;
		int32_t duration;
		bool isRTMAudio;
	};

* `url` & `size` are available for every FileInfo instance.
* `surl` is available for MessageType.ImageFile.
* `language` & `duration` are available when `isRTMAudio` is `true`.

### TranslatedInfo

	struct TranslatedInfo
	{
		string sourceLanguage;
		string targetLanguage;
		string sourceText;
		string targetText;
	};

### RTMMessage

	struct RTMMessage
	{
		int64_t fromUid;
		int64_t toId;
		int8_t messageType;
		int64_t messageId;
		string message;
		string attrs;
		int64_t modifiedTime;
		shared_ptr<TranslatedInfo> translatedInfo;
		shared_ptr<FileInfo> fileInfo;
	};

Using for receiving server pushed messages with interfaces of class RTMQuestProcessor.

When `messageType == (byte)MessageType.Chat`, `translatedInfo` will be assigned, `stringMessage` will be the translated messsge or original message if translation is not enabled, and `binaryMessage` will be null;   
When `messageType == (byte)MessageType.Cmd`, `stringMessage` will be assigned and `binaryMessage` will be null,  
When `messageType` is a kinds of File types, `fileInfo` will be assigned, and `binaryMessage` will be null.  
In other cases, if message data is text data, `stringMessage` will be assigned and `binaryMessage` will be null,  
If message data is binary data, `stringMessage` will be null and `binaryMessage` will be assigned.

### RetrievedMessage

	struct RetrievedMessage : RTMMessage
	{
		int64_t cursorId;
	};

Using for retrieving chat or messages.

The fields are same as the `class RTMMessage`.

### HistoryMessage

	struct HistoryMessage : RTMMessage
	{
		int64_t cursorId;
	};

Using for history message result.

The fields are same as the `class RTMMessage`.

### HistoryMessageResult

	struct HistoryMessageResult
	{
		int32_t count;
		int64_t lastCursorId;
		int64_t beginMsec;
		int64_t endMsec;
		vector<HistoryMessage> messages;
	};

Using as the result of history message & chat methods.

When calling history message or history chat methods for fetching subsequent message or chat data, please using the corresponding fields in the result.