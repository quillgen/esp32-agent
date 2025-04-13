#ifndef __WAV_H_
#define __WAV_H_

#include <cstdint>

namespace agent {
// WAV header structure (44 bytes total)
#pragma pack(push, 1) // Disable struct padding to match binary layout
typedef struct {
  // RIFF chunk
  char chunk_id[4];    // "RIFF"
  uint32_t chunk_size; // File size - 8 bytes
  char format[4];      // "WAVE"

  // fmt subchunk
  char fmt_subchunk_id[4];    // "fmt "
  uint32_t fmt_subchunk_size; // Size of fmt chunk (16 bytes for PCM)
  uint16_t audio_format;      // 1 = PCM (uncompressed)
  uint16_t num_channels;      // 1 = mono, 2 = stereo
  uint32_t sample_rate;       // e.g., 44100 Hz
  uint32_t byte_rate;         // sample_rate * num_channels * bits_per_sample/8
  uint16_t block_align;       // num_channels * bits_per_sample/8
  uint16_t bits_per_sample;   // 16 for 16-bit PCM

  // data subchunk
  char data_subchunk_id[4];    // "data"
  uint32_t data_subchunk_size; // Size of raw audio data
} wav_header_t;
#pragma pack(pop) // Restore default struct padding
} // namespace agent

#endif