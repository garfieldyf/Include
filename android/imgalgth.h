/**
 * Bitmap algorithm in C
 */

#ifndef _IMAGEALGTH_INC_
#define _IMAGEALGTH_INC_

#include <stdint.h>

__BEGIN_DECLS
void    Android_grayBitmap(void* pixels, uint32_t width, uint32_t height);
void    Android_inverseBitmap(void* pixels, uint32_t width, uint32_t height);
void    Android_blurBitmap(void* pixels, uint32_t width, uint32_t height, uint32_t radius);
void    Android_binaryBitmap(void* pixels, uint32_t width, uint32_t height, uint32_t grayscale);
void    Android_mirrorBitmap(void* pixels, uint32_t width, uint32_t height, uint32_t horizontal);
void    Android_spreadBitmap(void* pixels, uint32_t width, uint32_t height, uint32_t spreadSize);
void    Android_mosaicBitmap(void* pixels, uint32_t width, uint32_t height, uint32_t mosaicSize);
__END_DECLS

#endif  /* _IMAGEALGTH_INC_ */