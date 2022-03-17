# RTM Client UE4 SDK Value-Added API Docs

# Index

[TOC]

### Set Translated Language

	//-- Async Method
	bool SetTranslatedLanguage(DoneCallback callback, const string& targetLanguage, int32_t timeout = 0);
	
	//-- Sync Method
	int32_t SetTranslatedLanguage(const string& targetLanguage, int32_t timeout = 0);

Set target language to enable auto-translating.

Parameters:

+ `DoneCallback callback`

		typedef function<void(int32_t errorCode)> DoneCallback;

	Callabck for async method. Please refer [DoneCallback](Delegates.md#DoneCallback).

+ `const string& targetLanguage`

	Target language. Please refer the 'Language support' section in [document](https://docs.ilivedata.com/stt/production/) for language value.

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async calling is start.
	* false: Start async calling is failed.

+ int for Sync

	0 or FPNN_EC_OK means calling successed.

	Others are the reason for calling failed.

### Translate

	//-- Async Method
    bool Translate(function<void(TranslatedInfo, int32_t)> callback, const string& text, const string& destinationLanguage, const string& sourceLanguage = "", TranslateType type = Chat, ProfanityType profanity = Off, int32_t timeout = 0);
	
	//-- Sync Method
    int32_t Translate(TranslatedInfo& translatedInfo, const string& text, const string& destinationLanguage, const string& sourceLanguage = "", TranslateType type = Chat, ProfanityType profanity = Off, int32_t timeout = 0);

Translate text to target language.

Parameters:

+ `function<void(TranslatedInfo, int32_t)> callback`

	Callabck for async method.  
	First `TranslatedInfo` is translation message result, please refer [TranslatedInfo](Structures.md#TranslatedInfo);  
	Second `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `TranslatedInfo& translatedInfo`

	The translation message result, please refer [TranslatedInfo](Structures.md#TranslatedInfo).

+ `const string& text`

	The text need to be translated.

+ `const string& destinationLanguage`

	Target language. Please refer the 'Language support' section in [document](https://docs.ilivedata.com/stt/production/) for language value.

+ `const string& sourceLanguage`

	Source language. Empty string means automatic recognition. Please refer the 'Language support' section in [document](https://docs.ilivedata.com/stt/production/) for language value.

+ `TranslateType type`

	TranslateType.Chat or TranslateType.Mail. Default is TranslateType.Chat.

+ `ProfanityType profanity`

	Profanity filter action.

	* ProfanityType.Off (**Default**)
	* ProfanityType.Stop
	* ProfanityType.Censor

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async calling is start.
	* false: Start async calling is failed.

+ int for Sync

	0 or FPNN_EC_OK means calling successed.

	Others are the reason for calling failed.


### SpeechToText

	//-- Async Method
	bool SpeechToText(function<void(string, string, int32_t)> callback, const string& audio, const string& language, const string& codec = "", int32_t sampleRate = 0, int32_t timeout = 120);
	
	//-- Sync Method
	int32_t SpeechToText(string& resultText, string& resultLanguage, const string& audio, const string& language, const string& codec = "", int32_t sampleRate = 0, int32_t timeout = 120);

Speech Recognition, convert speech to text.

Parameters:

+ `function<void(string, string, int32_t)> callback`

	Callabck for async method.  
	First `string` is the text converted from recognized speech;  
	Second `string` is the recognized language.  
	Thrid `int32_t` is the error code indicating the calling is successful or the failed reasons.

+ `string& resultText`

	The text converted from recognized speech.

+ `string& resultLanguage`

	The recognized language.

+ `const string& audio`

	Speech binary data.

+ `const string&`

	Speech language when recording. Available language please refer the documents in [https://www.ilivedata.com/](https://docs.ilivedata.com/stt/production/).

	[Current Chinese document](https://docs.ilivedata.com/stt/production/)

+ `const string& codec`

	Codec for speech binary. If codec is `null` means `AMR_WB`.

+ `int32_t sampleRate`

	Sample rate for speech binary. If `0` means 16000.

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async calling is start.
	* false: Start async calling is failed.

+ int for Sync

	0 or FPNN_EC_OK means calling successed.

	Others are the reason for calling failed.


### TextCheck

	//-- Async Method
	bool TextCheck(function<void(TextCheckResult, int32_t)> callback, const string& text, int32_t timeout = 120);
	
	//-- Sync Method
	int32_t TextCheck(TextCheckResult& result, const string& text, int32_t timeout = 120);

Text moderation.

Parameters:

+ `function<void(TextCheckResult, int32_t)> callback`

	Callabck for async method.  
	First `TextCheckResult` is the result for text moderation;  
	Second `int32_t` is the error code indicating the calling is successful or the failed reasons.  
	`TextCheckResult` can be refered [TextCheckResult](Structures.md#TextCheckResult).

+ `TextCheckResult& result`

	The result for text moderation. Please refer [TextCheckResult](Structures.md#TextCheckResult).

+ `const string& text`

	The text need to be audited.

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async calling is start.
	* false: Start async calling is failed.

+ int for Sync

	0 or FPNN_EC_OK means calling successed.

	Others are the reason for calling failed.


### ImageCheck

	//-- Async Method
	bool ImageCheck(function<void(CheckResult, int32_t)> callback, const string& image, CheckType type, int32_t timeout = 120);
	
	//-- Sync Method
	int32_t ImageCheck(CheckResult& result, const string& image, CheckType type, int32_t timeout = 120);

Image review.

Parameters:

+ `function<void(CheckResult, int32_t)> callback`

	Callabck for async method.  
	First `CheckResult` is the result for image review;  
	Second `int32_t` is the error code indicating the calling is successful or the failed reasons.  
	`CheckResult` can be refered [CheckResult](Structures.md#CheckResult).

+ `CheckResult& result`

	The result for image review. Please refer [CheckResult](Structures.md#CheckResult).

+ `const string& image`

	Image's http/https url or binary data for auditing.

+ `CheckType type`

	Check type of the image. CheckType.Url or CheckType.Content.

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async calling is start.
	* false: Start async calling is failed.

+ int for Sync

	0 or FPNN_EC_OK means calling successed.

	Others are the reason for calling failed.


### AudioCheck

	//-- Async Method
	bool AudioCheck(function<void(CheckResult, int32_t)> callback, const string& audio, CheckType type, const string& language, const string& codec = "", int32_t sampleRate = 0, int32_t timeout = 120);
	
	//-- Sync Method
	int32_t AudioCheck(CheckResult& result, const string& audio, CheckType type, const string& language, const string& codec = "", int32_t sampleRate = 0, int32_t timeout = 120);

Audio check.

Parameters:

+ `function<void(CheckResult, int32_t)> callback`

	Callabck for async method.  
	First `CheckResult` is the result for audio checking;  
	Second `int` is the error code indicating the calling is successful or the failed reasons.  
	`CheckResult` can be refered [CheckResult](Structures.md#CheckResult).

+ `CheckResult& result`

	The result for audio checking. Please refer [CheckResult](Structures.md#CheckResult).

+ `const string& audio`

	Http/https url or binary content for speech to be checking.

+ `CheckType type`

	Check type of the image. CheckType.Url or CheckType.Content.

+ `const string& language`

	Audio language when recording. Available language please refer the documents in [https://www.ilivedata.com/](https://docs.ilivedata.com/stt/production/).

	[Current Chinese document](https://docs.ilivedata.com/audiocheck/techdoc/submit/)  
	[Current Chinese document (live audio)](https://docs.ilivedata.com/audiocheck/livetechdoc/livesubmit/)

+ `const string& codec`

	Codec for audio content. If codec is `null` means `AMR_WB`.

+ `int32_t sampleRate`

	Sample rate for audio content. If `0` means 16000.

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async calling is start.
	* false: Start async calling is failed.

+ int for Sync

	0 or FPNN_EC_OK means calling successed.

	Others are the reason for calling failed.


### VideoCheck

	//-- Async Method
	bool VideoCheck(function<void(CheckResult, int32_t)> callback, const string& video, CheckType type, const string& videoName, int32_t timeout = 120);
	
	//-- Sync Method
	int32_t VideoCheck(CheckResult& result, const string& video, CheckType type, const string& videoName, int32_t timeout = 120);

Video review.

Parameters:

+ `function<void(CheckResult, int32_t)> callback`

	Callabck for async method.  
	First `CheckResult` is the result for video review;  
	Second `int32_t` is the error code indicating the calling is successful or the failed reasons.  
	`CheckResult` can be refered [CheckResult](Structures.md#CheckResult).

+ `CheckResult& result`

	The result for video review. Please refer [CheckResult](Structures.md#CheckResult).

+ `const string& video`

	Video's http/https url or binary content for auditing.

+ `const string& videoName`

	Video name.

+ `int32_t timeout`

	Timeout in second.

	0 means using default setting.


Return Values:

+ bool for Async

	* true: Async calling is start.
	* false: Start async calling is failed.

+ int for Sync

	0 or FPNN_EC_OK means calling successed.

	Others are the reason for calling failed.

