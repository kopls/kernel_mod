#include "bitmap.h"

unsigned char *create_bitmap(size_t bytes)
{
    unsigned char *bitmap = vzalloc(bytes);
    if (!bitmap)
        return NULL;
    return bitmap;
}

void bitmap_destroy(unsigned char *bitmap)
{
    vfree(bitmap);
}

bool bitmap_is_free(const unsigned char *bitmap, size_t bit)
{
    return !((bitmap[bit / 8] >> (bit % 8)) & 1);
}

void my_bitmap_set(unsigned char *bitmap, size_t bit)
{
    bitmap[bit / 8] = bitmap[bit / 8] | (1 << (bit % 8));
}

void my_bitmap_clear(unsigned char *bitmap, size_t bit)
{
    bitmap[bit / 8] = bitmap[bit / 8] & ~(1 << (bit % 8));
}
