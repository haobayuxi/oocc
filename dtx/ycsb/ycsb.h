#pragma once

#include <cassert>
#include <cstdint>
#include <vector>

#include "../memstore.h"
#include "util/json_config.h"

static ALWAYS_INLINE uint32_t FastRand(uint64_t *seed) {
  *seed = *seed * 1103515245 + 12345;
  return (uint32_t)(*seed >> 32);
}