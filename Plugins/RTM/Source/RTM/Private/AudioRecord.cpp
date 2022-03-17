#include "AudioRecord.h"
#include "AudioMixerDevice.h"
#include "ClientEngine.h"
#include <cstring>


namespace rtm {
	using namespace std;
	using namespace fpnn;

	string AudioRecord::_language("");
	AudioRecordListenerPtr AudioRecord::_audioRecordListener(nullptr);

#if (PLATFORM_IOS) || (PLATFORM_MAC)
	void AudioRecord::VolumnCallback(float db)
	{
		if (_audioRecordListener != nullptr)
		{
			float minValue = -60;
            float range = 60;
            float outRange = 100;
            if (db < minValue)
                db = minValue;

            db = (db + range) / range * outRange;
			_audioRecordListener->OnVolumn(db);
		}
	}

	void AudioRecord::StartRecordCallback()
	{
		if (_audioRecordListener != nullptr)
			_audioRecordListener->RecordStart();
	}

	void AudioRecord::StopRecordCallback(const char* data, int32_t length, int64_t time)
	{
		if (_audioRecordListener != nullptr)
		{
			_audioRecordListener->RecordEnd();
			string audio(data, length);
			RTMAudioData audioData(audio, _language, time);
			_audioRecordListener->OnRecord(audioData);
		}
	}

	void AudioRecord::PlayFinishCallback()
	{
		if (_audioRecordListener != nullptr)
			_audioRecordListener->PlayEnd();
	}
#elif (PLATFORM_ANDROID)
	void AudioRecord::VolumnCallback(JNIEnv* env, jobject clazz, jdouble db)
	{
		if (_audioRecordListener != nullptr)
			_audioRecordListener->OnVolumn((float)db);
	}

	void AudioRecord::StartRecordCallback(JNIEnv* env, jobject clazz, jboolean success, jstring errorMessage)
	{
		if (!success)
		{
			jboolean copy;
			const char* str = env->GetStringUTFChars(errorMessage, &copy);
			string error(str);
			env->ReleaseStringUTFChars(errorMessage, str);
		}
		if (_audioRecordListener != nullptr)
			_audioRecordListener->RecordStart();
	}

	void AudioRecord::StopRecordCallback(JNIEnv* env, jobject clazz, jbyteArray byteArray, jint time)
	{
		if (_audioRecordListener != nullptr)
		{
			_audioRecordListener->RecordEnd();

			jbyte* bytes = env->GetByteArrayElements(byteArray, 0);
			int length = env->GetArrayLength(byteArray);

			string audio((char*)bytes, length);
			env->ReleaseByteArrayElements(byteArray, bytes, 0);
			RTMAudioData audioData(audio, _language, time);
			_audioRecordListener->OnRecord(audioData);
		}
	}

	void AudioRecord::PlayFinishCallback(JNIEnv* env, jobject clazz)
	{
		if (_audioRecordListener != nullptr)
			_audioRecordListener->PlayEnd();
	}
#endif

	void AudioRecord::Init(const string& language, AudioRecordListenerPtr listener)
	{
		_language = language;
		_audioRecordListener = listener;
#if (PLATFORM_ANDROID)
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
    	{
			jclass rtmAudioClass = FAndroidApplication::FindJavaClass("com/NetForUnity/RTMAudio");
			JNINativeMethod methods[]{{"StartRecord", "(ZLjava/lang/String;)V", (void *)&AudioRecord::StartRecordCallback},
			{"BroadFinish", "()V", (void *)&AudioRecord::PlayFinishCallback},
			{"StopRecord", "([BI)V", (void *)&AudioRecord::StopRecordCallback},
			{"ListenVolume", "(D)V", (void *)&AudioRecord::VolumnCallback}};
			if (Env->RegisterNatives(rtmAudioClass, methods, 4) < 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("RegisterNatives failed"));
				return;
			}

			jmethodID method = Env->GetStaticMethodID(rtmAudioClass, "Init", "(Landroid/content/Context;Ljava/lang/String;)V");
			Env->CallStaticVoidMethod(rtmAudioClass, method, FAndroidApplication::GetGameActivityThis(), Env->NewStringUTF(_language.c_str()));
		}
#endif
	}

	void AudioRecord::StartRecord()
	{
#if (PLATFORM_IOS) || (PLATFORM_MAC)
		UE_LOG(LogTemp, Warning, TEXT("StartRecord"));
		startRecord(VolumnCallback, StartRecordCallback);
#elif (PLATFORM_ANDROID)
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
    	{
			jclass rtmAudioClass = FAndroidApplication::FindJavaClass("com/NetForUnity/RTMAudio");
			jmethodID method = Env->GetStaticMethodID(rtmAudioClass, "StartRecord", "()V");
			Env->CallStaticVoidMethod(rtmAudioClass, method);
		}
#endif
	}

	void AudioRecord::StopRecord()
	{
#if (PLATFORM_IOS) || (PLATFORM_MAC)
		stopRecord(StopRecordCallback);
#elif (PLATFORM_ANDROID)
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
    	{
			jclass rtmAudioClass = FAndroidApplication::FindJavaClass("com/NetForUnity/RTMAudio");
			jmethodID method = Env->GetStaticMethodID(rtmAudioClass, "StopRecord", "()V");
			Env->CallStaticVoidMethod(rtmAudioClass, method);
		}
#endif
	}

	void AudioRecord::StartPlay(const RTMAudioData& audioData)
	{
#if (PLATFORM_IOS) || (PLATFORM_MAC)
		startPlay(audioData._audio.c_str(), (int32_t)audioData._audio.size(), PlayFinishCallback);
#elif (PLATFORM_ANDROID)
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
    	{
			jclass rtmAudioClass = FAndroidApplication::FindJavaClass("com/NetForUnity/RTMAudio");
			jmethodID method = Env->GetStaticMethodID(rtmAudioClass, "StartPlay", "([B)V");

			string wavBuffer = RTMAudioData::ConvertToWav(audioData._audio);
			jbyte* bytes = (jbyte*)(wavBuffer.c_str());
			jbyteArray byteArray = Env->NewByteArray(wavBuffer.size());
			Env->SetByteArrayRegion(byteArray, 0, wavBuffer.size(), bytes);

			Env->CallStaticVoidMethod(rtmAudioClass, method, byteArray);
		}
#endif
	}

	void AudioRecord::StopPlay()
	{
#if (PLATFORM_IOS) || (PLATFORM_MAC)
		stopPlay();
#elif (PLATFORM_ANDROID)
 		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
    	{
			jclass rtmAudioClass = FAndroidApplication::FindJavaClass("com/NetForUnity/RTMAudio");
			jmethodID method = Env->GetStaticMethodID(rtmAudioClass, "StopPlay", "()V");
			Env->CallStaticVoidMethod(rtmAudioClass, method);
		}
#endif
	}
}