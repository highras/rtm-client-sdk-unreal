#pragma once

#include "AudioDevice.h"
#include "RTMAudioData.h"
#include "CoreMinimal.h"
#include "AudioCaptureCore.h"
#include <memory>
#include <queue>
#include <string>
#include <vector>
#if (PLATFORM_IOS) 
#import <RTMNative/FPNNBridge.h>
#elif (PLATFORM_MAC)
#include <FPNNBridge.h>
#elif (PLATFORM_ANDROID)
#include "Runtime/Launch/Public/Android/AndroidJNI.h"
#include "Runtime/ApplicationCore/Public/Android/AndroidApplication.h"
#endif


namespace rtm {
	using namespace std;

	class AudioRecordListener
	{
	public:
        virtual void RecordStart(){}
        virtual void RecordEnd(){}
        virtual void OnRecord(const RTMAudioData& audioData){}
        virtual void OnVolumn(float db){}
        virtual void PlayEnd(){}
	};
	typedef shared_ptr<AudioRecordListener> AudioRecordListenerPtr;
	
	class RTM_API AudioRecord
	{
	public:
		static void Init(const string& language, AudioRecordListenerPtr listener);
		static void StartRecord();
		static void StopRecord();
		static void StartPlay(const RTMAudioData& audioData);
		static void StopPlay();
	private:

#if (PLATFORM_IOS) || (PLATFORM_MAC)
		static void VolumnCallback(float db);
		static void StartRecordCallback();
		static void StopRecordCallback(const char* data, int32_t length, int64_t time);
		static void PlayFinishCallback();
#elif (PLATFORM_ANDROID)
		static void VolumnCallback(JNIEnv* env, jobject clazz, jdouble db);
		static void StartRecordCallback(JNIEnv* env, jobject clazz, jboolean success, jstring errorMessage);
		static void StopRecordCallback(JNIEnv* env, jobject clazz, jbyteArray byteArray, jint time);
		static void PlayFinishCallback(JNIEnv* env, jobject clazz);
#endif

		static string _language;
		static AudioRecordListenerPtr _audioRecordListener;
	};

} //End namespace UE4Capture