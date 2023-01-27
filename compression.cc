/***********************************
 * ESE-532 Deduplication Project   *
 * Encoder example                 *
 *                                 *
 * Compression engine              *
 *                                 *
 * Copyright (c) 2023, Hans Giesen *
 * All Rights Reserved.            *
 ***********************************/

#include "encoder.h"

typedef std::map<std::string, int> CodeMap;

static CodeMap codes;

static char buf_out[COMPRESS_BUF_SIZE];
static int buf_out_len;
static int byte;

static int get_len(int last_code) {
  int len = 0;
  while (last_code > 0) {
    last_code >>= 1;
    len++;
  }
  return len;
}

static void Write_code(int code, int len) {
  for (int i = 0; i < len; i++) {
    byte = (byte << 1) | ((code >> (len - 1 - i)) & 1);

    if (++buf_out_len % 8 == 0) {
      buf_out[buf_out_len / 8 - 1] = byte;
      byte = 0;
    }
  }
}

static void Finish_output(void) {
  if (buf_out_len % 8 > 0) {
    byte <<= 8 - buf_out_len % 8;
    buf_out[buf_out_len / 8] = byte;
  }
}

std::string Compress(const std::string & data) {
  buf_out_len = 0;
  byte = 0;

  codes.clear();
  for (int i = 0; i < 256; i++) {
    CodeMap::value_type val(std::string(1, static_cast<char>(i)), i);
    codes.insert(val);
  }

  if (data.length() == 0)
    return std::string();

  size_t pos = 1;
  std::string prefix(&data[0], 1);
  int code = (unsigned char) data[0];
  while (pos < data.length()) {
    std::string sym(&data[pos], 1);
    std::string new_prefix = prefix + sym;
    CodeMap::value_type Value(new_prefix, codes.size());
    std::pair<CodeMap::iterator, bool> result = codes.insert(Value);
    if (!result.second) {
      prefix = new_prefix;
      code = result.first->second;
    } else {
      Write_code(code, get_len(codes.size() - 1));
      prefix = sym;
      code = (unsigned char) sym[0];
    }
    pos++;
  }
  Write_code(code, get_len(codes.size()));

  Finish_output();

  return std::string(buf_out, (buf_out_len + 7) / 8);
}
