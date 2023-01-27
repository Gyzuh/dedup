/***********************************
 * ESE-532 Deduplication Project   *
 * Encoder example                 *
 *                                 *
 * Chunking engine                 *
 *                                 *
 * Copyright (c) 2023, Hans Giesen *
 * All Rights Reserved.            *
 ***********************************/

#include <stdint.h>

#include "encoder.h"

ChunkList * Chunk(const std::string & data) {
  ChunkList * chunks = new ChunkList();

  uint64_t power = 1;
  for (int i = 1; i < CHUNK_WIN_SIZE; i++)
    power = (power * 256) % CHUNK_PRIME;

  uint64_t sig = 0;
  for (int i = 0; i < CHUNK_WIN_SIZE; i++)
    sig = (sig * 256 + (unsigned char) data[i]) % CHUNK_PRIME;

  unsigned int last_idx = 0;
  for (unsigned int i = 1; i <= data.length() - CHUNK_WIN_SIZE; i++) {
    sig = (sig + CHUNK_PRIME - power * (unsigned char) data[i - 1] %
           CHUNK_PRIME) % CHUNK_PRIME;
    sig = (sig * 256 + (unsigned char) data[i + CHUNK_WIN_SIZE - 1]) %
           CHUNK_PRIME;

    if ((sig & CHUNK_MASK) == 0) {
      if (i + 1 - last_idx >= CHUNK_MIN_SIZE) {
        chunks->push_back(data.substr(last_idx, i + 1 - last_idx));
        last_idx = i + 1;
      }
    } else if (i + 1 - last_idx >= CHUNK_MAX_SIZE) {
      chunks->push_back(data.substr(last_idx, i + 1 - last_idx));
      last_idx = i + 1;
    }
  }

  if (last_idx < data.length() - 1)
    chunks->push_back(data.substr(last_idx));

  return chunks;
}
