# RTM Client UE4 SDK Native Audio & Record Docs

# Index

[TOC]

## Audio Record Example



```cpp
#include "RTMClient.h"
#include "AudioRecord.h"

class TestAudioRecordListener : public AudioRecordListener
	{
	private:
		RTMClientPtr _client;
	public:
		TestAudioRecordListener(RTMClientPtr client) : _client(client){}
		virtual ~TestAudioRecordListener() {}
        virtual void RecordStart()
		{
	 		UE_LOG(LogTemp, Warning, TEXT("TestAudioRecordListener RecordStart"));
		}
        virtual void RecordEnd()
		{
	 		UE_LOG(LogTemp, Warning, TEXT("TestAudioRecordListener RecordEnd"));
		}
        virtual void OnRecord(const RTMAudioData& audioData)
		{
			AudioRecord::StartPlay(audioData);
			int64_t messageID = 0;

			bool status = _client->SpeechToText([this](string resultText, string resultLanguage, int32_t errorCode) {

				UE_LOG(LogTemp, Warning, TEXT("SpeechToText %d, resultText = %s, resultLanguage = %s"), errorCode, UTF8_TO_TCHAR(resultText.c_str()), UTF8_TO_TCHAR(resultLanguage.c_str()));
				if (errorCode == 0)
				{
					_client->Translate([this](TranslatedInfo info, int32_t errorCode) {
						UE_LOG(LogTemp, Warning, TEXT("srcLanguage = %s, targetLanguage = %s, srcText = %s, targetText = %s"), UTF8_TO_TCHAR(info.sourceLanguage.c_str()), UTF8_TO_TCHAR(info.targetLanguage.c_str()), UTF8_TO_TCHAR(info.sourceText.c_str()), UTF8_TO_TCHAR(info.targetText.c_str()));
						}, resultText, "en");
				}
				}, audioData._audio, Content, "zh-CN");

			status = _client->SendFile([](int64_t messageID, int32_t errorCode) {
				UE_LOG(LogTemp, Warning, TEXT("SendFile %d"), errorCode);
				}, 1234567, audioData);
		}
        virtual void OnVolumn(float db)
		{
	 		UE_LOG(LogTemp, Warning, TEXT("TestAudioRecordListener OnVolumn %f"), db);
		}
        virtual void PlayEnd()
		{
	 		UE_LOG(LogTemp, Warning, TEXT("TestAudioRecordListener PlayEnd"));
		}
	};

```

## Audio Push Handler Example


```cpp

// in QuestProcessor

void PushFile(const RTMMessage& message) 
{
	if (message.messageType == MessageType.AudioFileType && message.fileInfo != nullptr && message.fileInfo->isRTMAudio) 
	{
		string audioUrl = message.fileInfo->url;
		
		string audioContent = Download_From_Url(audioUrl); // here is a fake code, means to download the audio file from url

		RTMAudioData audioData(audioContent, *message.fileInfo);  // create the RTMAudioData instance


		// you can do the speech to text like this:
		bool status = _client->SpeechToText([this](string resultText, string resultLanguage, int32_t errorCode) {
				if (errorCode == FPNN_EC_OK)
				{
					UE_LOG(LogTemp, Warning, TEXT("SpeechToText resultText = %s, resultLanguage = %s"), UTF8_TO_TCHAR(resultText.c_str()), UTF8_TO_TCHAR(resultLanguage.c_str()));
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("SpeechToText errorCode = %d"), errorCode);
				}
				}, audioData._audio, audioData._lang);


		// you can play the audio like this:
		AudioRecord::StartPlay(audioData);;
	}
}


```

## Api

### AudioRecorderNative Init

	 void Init(const string& language, AudioRecordListenerPtr listener);

Init the Singleton AudioRecorderNative.

Parameters:

+ `const string& language`

	Supported language . Please refer [Supported Language](https://docs.ilivedata.com/stt/production/).

+ `AudioRecordListenerPtr listener`

	Instance of AudioRecordListener.

### Start Record

	public void StartRecord()

Start Record.


### Stop Record

	public void StopRecord()

Stop Record.

### Start Play Audio

	public void StartPlay(RTMAudioData data)

Start Play Aduio.


Parameters:

+ `RTMAudioData data`

	Audio Data.

### Stop Play Audio

    public void StopPlay()

Stop Play Audio.