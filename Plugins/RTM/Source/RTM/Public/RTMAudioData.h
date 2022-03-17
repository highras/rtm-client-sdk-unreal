#pragma once

#include "RTMStructure.h"
#include <string>

namespace rtm
{
	using namespace std;

	class RTMAudioData
	{
	public:
		RTMAudioData(const string& audio, float* pcmData, const string& codecType, const string& lang, int64_t duration, int32_t lengthSamples, int32_t frequency);
		RTMAudioData(const string& audio, const FileInfo& fileInfo);
		RTMAudioData(const string& audio, const string& language, int64_t time);

		void ParseAudioData();
		static float ToFloat(char c1, char c2);
		static string ConvertToAmrwb(const char* wavData, int32_t wavSize);
		static string ConvertToWav(const string& amrBuffer);
	public:
		const static string DefaultCodec;
		const static int32_t DefaultSampleRate;
		string _codecType;
		string _lang;
		string _audio;  // Current is AMR-WB
		float* _pcmData;  // Original PCM data
		int64_t _duration;   // Duration in ms
		int32_t _frequency;
		int32_t _lengthSamples;
	};
}
