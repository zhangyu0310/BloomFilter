#ifndef BLOOMFILTER_BLOOM_FILTER_H
#define BLOOMFILTER_BLOOM_FILTER_H

typedef unsigned int (*HashFunc)(const char* str);
typedef unsigned int FilterType;

typedef struct BloomFilter {
  unsigned int filter_size;
  unsigned int hash_func_size;
  HashFunc* hash_func;
  FilterType filter[0];
} BloomFilter;

BloomFilter* CreateBloomFilter(unsigned int filter_size, unsigned int hash_func_size);

void DestroyBloomFilter(BloomFilter* filter);

void UpdateFilter(BloomFilter* filter, const char* str);

int CheckFilterItems(BloomFilter* filter, const char* str);

unsigned int BKDRHash(const char* str);

unsigned int SDBMHash(const char* str);

unsigned int RSHash(const char* str);

#endif//BLOOMFILTER_BLOOM_FILTER_H
