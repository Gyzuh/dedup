/***********************************
 * ESE-532 Deduplication Project   *
 * Encoder example                 *
 *                                 *
 * Encoder main                    *
 *                                 *
 * Copyright (c) 2023, Hans Giesen *
 * All Rights Reserved.            *
 ***********************************/

#include "encoder.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <utility>


using namespace std;


typedef map<string, int> hash_map;


static size_t GetFileSize(istream & stream) {
  stream.seekg(0, ios::end);
  size_t size = stream.tellg();
  stream.seekg(0, ios::beg);
  return size;
}


static string LoadData(const char * filename) {

  ifstream stream_in(filename, ios::binary);
  if (!stream_in.good()) {
    cerr << "Could not open input file.\n";
    exit(EXIT_FAILURE);
  }
  
  size_t size = GetFileSize(stream_in);

  string data(size, 0);
  stream_in.read(&data[0], size);

  return data;
}


int main(int arg_cnt, char * args[]) {

  if (arg_cnt < 3) {
    cout << "Usage: " << args[0]
         << " <Uncompressed file> <Compressed file>" << endl;
    return EXIT_SUCCESS;
  }

  string data = LoadData(args[1]);

  ofstream stream_out(args[2], ios::binary);
  if (!stream_out.good()) {
    cerr << "Could not open stream_output file.\n";
    return EXIT_FAILURE;
  }

  ChunkList & chunks = *Chunk(data);

  hash_map hashes;
  size_t total_len = 0;
  unsigned int idx = 0;
  for (unsigned int i = 0; i < chunks.size(); i++) {

    SHA256_CTX ctx;
    unsigned char hash[HASH_LEN];

    sha256_init(&ctx);
    sha256_update(&ctx,
                  (unsigned char *) chunks[i].c_str(),
                  chunks[i].length());
    sha256_final(&ctx, hash);

    hash_map::value_type val(string(reinterpret_cast<char *>(hash),
                                    HASH_LEN), idx);
    pair<hash_map::iterator, bool> res = hashes.insert(val);
    uint32_t hdr;
//    stringstream Stream;
//    Stream << "Chunk_" << i << ".enc";
//    ofstream Chunk_file(Stream.str().c_str(), ios::binary);
    cout << "Position " << total_len << '\n';
    if (!res.second) {
      hdr = res.first->second;
      cout << "Chunk " << i << " of size " << chunks[i].length()
           << " is identical to chunk " << hdr << ".\n";
      hdr = (hdr << 1) | 1;
      stream_out.write(reinterpret_cast<char *>(&hdr), 4);
//      Chunk_file.write(reinterpret_cast<char *>(&hdr), 4);
    } else {
      cout << "Chunk " << i << " of size " << chunks[i].length()
           << " is new.\n";
      const string & compressed_data = Compress(chunks[i]);
      hdr = compressed_data.length() << 1;
      stream_out.write(reinterpret_cast<char *>(&hdr), 4);
      stream_out.write(&compressed_data[0], compressed_data.length());
      idx++;
//      Chunk_file.write(reinterpret_cast<char *>(&hdr), 4);
//      Chunk_file.write(&compressed_data[0], compressed_data.length());
    }
//    Chunk_file.close();
    total_len += chunks[i].length();
  }

  return EXIT_SUCCESS;
}
