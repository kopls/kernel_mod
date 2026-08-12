#ifndef BITMAP_H
#define BITMAP_H

#include <linux/vmalloc.h>
#include <linux/types.h>

unsigned char *create_bitmap(size_t bytes);
void bitmap_destroy(unsigned char *bitmap);
bool bitmap_is_free(const unsigned char *bitmap, size_t bit);
void my_bitmap_set(unsigned char *bitmap, size_t bit);
void my_bitmap_clear(unsigned char *bitmap, size_t bit);

#endif
