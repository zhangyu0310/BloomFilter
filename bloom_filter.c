#include "bloom_filter.h"

#include <limits.h>
#include <stdlib.h>
#include <memory.h>

BloomFilter* CreateBloomFilter(unsigned int filter_size, unsigned int hash_func_size) {
  unsigned int size = filter_size / (sizeof(FilterType) * CHAR_BIT) + 1;
  BloomFilter* filter = malloc(sizeof(BloomFilter) + size * sizeof(FilterType));
  if (!filter) return NULL;
  memset(filter, 0, sizeof(BloomFilter) + size * sizeof(FilterType));
  filter->hash_func = malloc(sizeof(HashFunc) * hash_func_size);
  memset(filter->hash_func, 0, sizeof(HashFunc) * hash_func_size);
  filter->filter_size = filter_size;
  filter->hash_func_size = hash_func_size;
  return filter;
}

void DestroyBloomFilter(BloomFilter* filter) {
  if (!filter) return;
  free(filter->hash_func);
  filter->hash_func = NULL;
  free(filter);
}

void UpdateFilter(BloomFilter* filter, const char* str) {
  if (!filter || !str) return;
  for (unsigned int i = 0; i < filter->hash_func_size; ++i) {
    if (filter->hash_func[i]) {
      unsigned int h = filter->hash_func[i](str) % filter->filter_size;
      unsigned int bit_num = sizeof(FilterType) * CHAR_BIT;
      filter->filter[h / bit_num] |= (1 << (h % bit_num));
    }
  }
}

int CheckFilterItems(BloomFilter* filter, const char* str) {
  if (!filter || !str) return -1;
  for (unsigned int i = 0; i < filter->hash_func_size; ++i) {
    if (filter->hash_func[i]) {
      unsigned int h = filter->hash_func[i](str) % filter->filter_size;
      unsigned int bit_num = sizeof(FilterType) * CHAR_BIT;
      if (!(filter->filter[h / bit_num] & (1 << (h % bit_num)))) {
        return 0;
      }
    }
  }
  return 1;
}

unsigned int BKDRHash(const char* str) {
  register int hash = 0;
  char ch = 0;
  while (ch = *str++) {
    hash = hash * 131 + ch;
  }
  return hash;
}

unsigned int SDBMHash(const char* str) {
  register int hash = 0;
  char ch = 0;
  while (ch = *str++) {
    hash = hash * 65599 + ch;
  }
  return hash;
}

unsigned int RSHash(const char* str)
{
  register int hash = 0;
  int magic = 63689;
  char ch = 0;
  while (ch = *str++)
  {
    hash = hash * magic + ch;
    magic *= 378551;
  }
  return hash;
}