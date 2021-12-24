//
// Created by poppinzhang on 2021/12/23.
//

#include <bloom_filter.h>
#include <cJSON/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void printReturn(int ret) {
  if (ret == 0) {
    printf("Not Hit\n");
  }
  else if (ret == 1) {
    printf("Hit\n");
  } else {
    printf("Error\n");
  }
}

static void GetUTF8FirstWord(const char *str, char *first_word) {
  if (!str || !first_word) return;
  memset(first_word, 0, 4);
  int word_size = 0;
  unsigned char tmp = (unsigned char)str[0];
  if (tmp < 128) {
    word_size = 1;
  } else if (tmp > 191 && tmp < 224) {
    word_size = 2;
  } else if (tmp > 223 && tmp < 240) {
    word_size = 3;
  } else if (tmp > 239) {
    word_size = 4;
  }
  for (int i = 0; i < word_size; ++i) {
    first_word[i] = str[i];
  }
}

int main() {
  FILE* fp = NULL;
  fopen_s(&fp, "D:\\GitHub\\BloomFilter\\sensitive_word.json", "r");
  BloomFilter* bf = CreateBloomFilter(100000, 3);
  bf->hash_func[0] = BKDRHash;
  bf->hash_func[1] = SDBMHash;
  bf->hash_func[2] = RSHash;
  char* buff = malloc(999999);
  fread(buff, sizeof(char), 999999, fp);
  cJSON* json = cJSON_Parse(buff);
  int arr_size = cJSON_GetArraySize(json);
  for (int i = 0; i < arr_size; ++i) {
    cJSON* item = cJSON_GetArrayItem(json, i);
    char* str = cJSON_GetStringValue(item);
    char tmp[4];
    GetUTF8FirstWord(str, tmp);
    UpdateFilter(bf, tmp);
  }
  int ret = -1;
  char* str = "我不知道";
  char tmp[4];
  GetUTF8FirstWord(str, tmp);
  ret = CheckFilterItems(bf, tmp);
  printReturn(ret);
  str = "潮吹";
  GetUTF8FirstWord(str, tmp);
  ret = CheckFilterItems(bf, tmp);
  printReturn(ret);
  DestroyBloomFilter(bf);
  return 0;
}