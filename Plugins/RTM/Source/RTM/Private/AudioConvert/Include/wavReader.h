#ifndef WAVREADER_H
#define WAVREADER_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void* wav_read_open_stream(const char *src, uint32_t srcSize);
void wav_read_close_stream(void* obj);
int wav_get_header_stream(void* obj, int* format, int* channels, int* sample_rate, int* bits_per_sample, unsigned int* data_length);
int wav_read_data_stream(void* obj, unsigned char* data, unsigned int length);

#ifdef __cplusplus
}
#endif

#endif
