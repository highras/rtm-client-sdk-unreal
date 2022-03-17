#ifndef voiceConverter_h
#define voiceConverter_h

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <map>
#include "interf_dec.h"
#include "interf_enc.h"
#include "dec_if.h"
#include "enc_if.h"
#include "wavWriter.h"
#include "wavReader.h"
#include "dynamicBuffer.h"

#ifdef WIN32

#define UNITY_CPP_DECL_SPEC    __declspec(dllexport)

#else
#define UNITY_CPP_DECL_SPEC
#endif

extern "C"
{
    UNITY_CPP_DECL_SPEC char* convert_wav_to_amrwb(const char* wavSrc, int wavSrcSize, int& status, int& amrSize);
    UNITY_CPP_DECL_SPEC char* convert_amrwb_to_wav(const char* amrSrc, int amrSrcSize, int& status, int& wavSize);
    UNITY_CPP_DECL_SPEC void free_memory(char* ptr);
}


/*
return value:
    0 success
    1 memory alloc fail
    2 voice header error
    3 voice format error
    4 unsupported sample depth
*/
int wav_to_amrwb(const char* wavSrc, uint32_t wavSrcSize, DYNAMIC_BUFFER_TYPE* amrBuffer);

/*
return value:
    0 success
    1 memory alloc fail
    2 voice header error
    3 voice format error
*/
int amrwb_to_wav(const char* amrwbSrc, uint32_t amrwbSrcSize, DYNAMIC_BUFFER_TYPE* wavBuffer);



int wav_to_amrwb_file(const char* inFile, const char* outFile);
int amrwb_to_wav_file(const char* inFile, const char* outFile);

#endif
