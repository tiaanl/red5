#pragma once

#include <spdlog/spdlog.h>

#include <cstddef>
#include <cstdint>

using U8 = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;

using I8 = std::int8_t;
using I16 = std::int16_t;
using I32 = std::int32_t;
using I64 = std::int64_t;

using F32 = float;
using F64 = double;

using MemSize = std::size_t;
using PtrDiff = std::ptrdiff_t;

#define COPY_WITH(ClassName, X)                                                                    \
  ClassName(const ClassName&) = X;                                                                 \
  ClassName& operator=(const ClassName&) = X

#define MOVE_WITH(ClassName, X)                                                                    \
  ClassName(ClassName&&) = X;                                                                      \
  ClassName& operator=(ClassName&&) = X

#define NO_COPY(ClassName) COPY_WITH(ClassName, delete)
#define DEFAULT_MOVE(ClassName) MOVE_WITH(ClassName, default)

#include "base/dimensions.h"
