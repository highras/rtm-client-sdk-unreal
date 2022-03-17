#ifndef WAVWRITER_H
#define WAVWRITER_H
#include "dynamicBuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

void* wav_write_open_stream(DYNAMIC_BUFFER_TYPE* buffer, int sample_rate, int bits_per_sample, int channels);
void wav_write_close_stream(void* obj);
void wav_write_data_stream(void* obj, const unsigned char* data, int length);

#ifdef __cplusplus
}
#endif

#endif
