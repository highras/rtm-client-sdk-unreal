#include "RTMAudioData.h"
#include "voiceConverter.h"

namespace rtm
{
	const string RTMAudioData::DefaultCodec = "amr-wb";
	const int32_t RTMAudioData::DefaultSampleRate = 16000;
	RTMAudioData::RTMAudioData(const string& audio, float* pcmData, const string& codecType, const string& lang, int64_t duration, int32_t lengthSamples, int32_t frequency)
	{
		_audio = audio;
		_codecType = codecType;
		_lang = lang;
		_duration = duration;
		_lengthSamples = lengthSamples;
		_frequency = frequency;
		_pcmData = pcmData;
	}

	RTMAudioData::RTMAudioData(const string& audio, const FileInfo& fileInfo)
	{
		_codecType = DefaultCodec;
		_lang = fileInfo.language;
		_duration = fileInfo.duration;
		_audio = audio;
		_frequency = DefaultSampleRate;
		ParseAudioData();
	}

	RTMAudioData::RTMAudioData(const string& audio, const string& language, int64_t time)
	{
		_codecType = DefaultCodec;
		_lang = language;
		_duration = time;
		_audio = audio;
		_frequency = DefaultSampleRate;
		ParseAudioData();
	}

	void RTMAudioData::ParseAudioData()
	{
		string wavBuffer = ConvertToWav(_audio);

		int32_t channelCount = (int32_t)wavBuffer[22];
		int32_t pos = 12;

		while (!(wavBuffer[pos] == 100 && wavBuffer[pos + 1] == 97 && wavBuffer[pos + 2] == 116 && wavBuffer[pos + 3] == 97))
		{
			pos += 4;
			int32_t chunkSize = wavBuffer[pos] + wavBuffer[pos + 1] * 256 + wavBuffer[pos + 2] * 65536 + wavBuffer[pos + 3] * 16777216;
			pos += 4 + chunkSize;
		}
		pos += 8;

		_lengthSamples = (int32_t)(wavBuffer.size() - pos) / 2;
		_pcmData = new float[_lengthSamples];
		int32_t idx = 0;
		while (pos < (int32_t)wavBuffer.size())
		{
			*(_pcmData+idx) = ToFloat(wavBuffer[pos], wavBuffer[pos + 1]);
			pos += 2;
			idx += 1;
		}
	}

	float RTMAudioData::ToFloat(char c1, char c2)
	{
		short s = short(c2 << 8 | c1);
		return s / 32768.0f;
	}

	string RTMAudioData::ConvertToAmrwb(const char* wavData, int32_t wavSize)
	{
		int32_t status = 0;
		int32_t amrSize = 0;
		char* amrPtr = ::convert_wav_to_amrwb(wavData, wavSize, status, amrSize);
		if (amrPtr != nullptr && status == 0)
		{
			string res(amrPtr, amrSize);
			//char* amrBuffer = new char[amrSize];
			//memcpy(amrBuffer, amrPtr, amrSize);
			free_memory(amrPtr);
			
			return res;
		}

		if (amrPtr != nullptr)
			free_memory(amrPtr);

		return "";
	}

	string RTMAudioData::ConvertToWav(const string& amrBuffer)
	{
		int32_t status = 0;
		int32_t wavSize = 0;

		char* wavPtr = ::convert_amrwb_to_wav(amrBuffer.c_str(), amrBuffer.size(), status, wavSize);
		if (wavPtr != nullptr && status == 0)
		{
			char* wavBuffer = new char[wavSize];
			memcpy(wavBuffer, wavPtr, wavSize);
			free_memory(wavPtr);
			return string(wavBuffer, wavSize);
		}

		if (wavPtr != nullptr)
			free_memory(wavPtr);

		return "";
	}
}