/*
 * (c) Copyright 2019 Xilinx, Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#ifndef _XFCOMPRESSION_SNAPPY_DECOMPRESS_KERNEL_HPP_
#define _XFCOMPRESSION_SNAPPY_DECOMPRESS_KERNEL_HPP_

/**
 * @file snappy_decompress_kernel.hpp
 * @brief C++ Header for snappy decompression kernel.
 *
 * This file is part of XF Compression Library.
 */
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "hls_stream.h"
#include <ap_int.h>

#include "lz_decompress.hpp"
#include "lz_optional.hpp"
#include "mm2s.hpp"
#include "s2mm.hpp"
#include "stream_downsizer.hpp"
#include "stream_upsizer.hpp"

#include "snappy_config.h"
#include "snappy_decompress.hpp"

extern "C" {
/**
 * @brief Snappy decompression kernel top.
 *
 * @param in input stream width
 * @param out output stream width
 * @param in_block_size intput size
 * @param in_compressd_size output size
 * @param block_size_in_kb intput size
 * @param no_blocks input size
 */
void xilSnappyDecompress(const xf::compression::uintMemWidth_t* in,
                         xf::compression::uintMemWidth_t* out,
                         uint32_t* in_block_size,
                         uint32_t* in_compress_size,
                         uint32_t block_size_in_kb,
                         uint32_t no_blocks);
}
#endif
