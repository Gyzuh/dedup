/***********************************
 * ESE-532 Deduplication Project   *
 * Encoder example                 *
 *                                 *
 * Encoder main header             *
 *                                 *
 * Copyright (c) 2023, Hans Giesen *
 * All Rights Reserved.            *
 ***********************************/

#ifndef DEDUP_ENC_ENCODER_H_
#define DEDUP_ENC_ENCODER_H_

#include <cstdint>
#include <map>
#include <vector>
#include <string>

#define CHUNK_PRIME (140737488355213ull)
#define CHUNK_WIN_SIZE (16)
// Minimum size is needed because signature of zeroes is always 0.
#define CHUNK_MIN_SIZE (2048)
#define CHUNK_MAX_SIZE (8192)
// 0xFFF = 2048 - 1 -> avg. size = min. size + 2048 = 4096
#define CHUNK_MASK (0x7FF)

typedef std::vector<std::string> ChunkList;

ChunkList * Chunk(const std::string & data);

typedef struct {
  uint8_t data[64];
  uint32_t datalen;
  uint32_t bitlen[2];
  uint32_t state[8];
} SHA256_CTX;

#define HASH_LEN (32)

void sha256_init(SHA256_CTX * ctx);
void sha256_update(SHA256_CTX * ctx, uint8_t * data, uint32_t len);
void sha256_final(SHA256_CTX * ctx, uint8_t * hash);

#define COMPRESS_BUF_SIZE (2 * CHUNK_MAX_SIZE)

std::string Compress(const std::string & data);

#endif  // DEDUP_ENC_ENCODER_H_
