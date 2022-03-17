#pragma once
#include "RTMTest.h"

namespace rtm
{
	using namespace std;

	class RTMHttp
	{
	public:
		void DownLoad(FString URL) 
		{
			TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
			HttpRequest->OnProcessRequestComplete().BindRaw(this, &RTMHttp::HandleDownloadRequest);
			//HttpRequest->OnRequestProgress().BindRaw(this, &RTMHttp::HandleHttpRequestProgress);
			HttpRequest->SetURL(URL);
			HttpRequest->SetVerb(TEXT("GET"));
			HttpRequest->ProcessRequest();
		}

		//下载完成调用
		void HandleDownloadRequest(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
		{
			if (!HttpRequest.IsValid() || !HttpResponse.IsValid()) {
				return;
			}

			if (bSucceeded && EHttpResponseCodes::IsOk(HttpResponse->GetResponseCode())) {
				UE_LOG(LogTemp, Warning, TEXT("HandleDownloadRequest"));
				const std::string audio(reinterpret_cast<const char*>(HttpResponse->GetContent().GetData()), HttpResponse->GetContent().Num());
				RTMAudioData data(audio, "zh-CN", 5000);
				AudioRecord::StartPlay(data);
			}
		}

		//处理下载进度
		void HandleHttpRequestProgress(FHttpRequestPtr HttpRequest, int32 BytesSent, int32 BytesReceived)
		{

		}
	};
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
	 		UE_LOG(LogTemp, Warning, TEXT("TestAudioRecordListener OnRecord"));
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
				}, audioData._audio, "zh-CN", "AMR", 8000);

			status = _client->SendFile([](int64_t messageID, int32_t errorCode) {
				UE_LOG(LogTemp, Warning, TEXT("SendFile %d"), errorCode);
				}, 1234567, audioData);

			//RTMHttp* http = new RTMHttp;
			//http->DownLoad("https://s3.cn-northwest-1.amazonaws.com.cn/rtm-filegated-cn-northwest-1/80000071/20220316/7654321/zh-CN-2af2650bba86fe074ebfdd6d7feb4775");
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

	class RTMAudio : public RTMTest
	{
	public:
		RTMAudio(const string& endpoint, int64_t pid, int64_t uid, const string& token)
		{
			_endpoint = endpoint;
			_pid = pid;
			_uid = uid;
			_token = token;
		}

		virtual void Start()
		{
			LoginRTM(_endpoint, _pid, _uid, _token);
			if (_client == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("User %lld login RTM failed"), _uid);
				return;
			}
			
			UE_LOG(LogTemp, Warning, TEXT("init"));
			AudioRecord::Init("zh-CN", make_shared<TestAudioRecordListener>(_client));
		}

		virtual void Stop()
		{
		}

		void Test()
		{
		}
		
	private:
		thread _thread;
		string _endpoint;
		int64_t _pid;
		int64_t _uid;
		string _token;
	};
}