#ifndef dynamicBuffer_H
#define dynamicBuffer_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#ifndef SET_BUFFER_SIZE
#define SET_BUFFER_SIZE 4096
#endif

#ifndef SET_INCREMENT_SIZE
#define SET_INCREMENT_SIZE 4096
#endif

typedef struct
{
    char *data;
	size_t len;
	size_t allocated;
} DYNAMIC_BUFFER_TYPE;

DYNAMIC_BUFFER_TYPE* dynamic_buffer_init();
char* dynamic_buffer_add(DYNAMIC_BUFFER_TYPE *buffer, const char *str, size_t n);
char* dynamic_buffer_rewrite(DYNAMIC_BUFFER_TYPE* buffer, size_t pos, const char *str, size_t n);
void dynamic_buffer_clear(DYNAMIC_BUFFER_TYPE *buffer);
void dynamic_buffer_destroy(DYNAMIC_BUFFER_TYPE **buffer);

#ifdef __cplusplus
}
#endif

#endif