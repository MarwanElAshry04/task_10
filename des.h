#ifndef DES_H
#define DES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	uint64_t des(uint64_t input, uint64_t key, char mode);

#ifdef __cplusplus
}
#endif

#endif
