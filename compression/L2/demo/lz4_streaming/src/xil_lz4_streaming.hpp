/*
 * Copyright 2019 Xilinx, Inc.
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
 */
/**
 * @file xil_lz4.hpp
 * @brief Header for snappy host functionality
 *
 * This file is part of XF Compression Library host code for lz4 compression.
 */

#ifndef _XFCOMPRESSION_XIL_LZ4_STREAMING_HPP_
#define _XFCOMPRESSION_XIL_LZ4_STREAMING_HPP_

#include "defns.hpp"

// This extension file is required for stream APIs
#include <CL/cl_ext_xilinx.h>

/**
 * Maximum host buffer used to operate per kernel invocation
 */
#define HOST_BUFFER_SIZE (2 * 1024 * 1024)

/**
 * Default block size
 */
#define BLOCK_SIZE_IN_KB 64

/**
 * Maximum number of blocks based on host buffer size
 */
#define MAX_NUMBER_BLOCKS (HOST_BUFFER_SIZE / (BLOCK_SIZE_IN_KB * 1024))

/**
 * Below are the codes as per LZ4 standard for
 * various maximum block sizes supported.
 */
#define BSIZE_STD_64KB 0x40
#define BSIZE_STD_256KB 0x50
#define BSIZE_STD_1024KB 0x60
#define BSIZE_STD_4096KB 0x70

/**
 * Maximum block sizes supported by LZ4
 */
#define MAX_BSIZE_64KB (64 * 1024)
#define MAX_BSIZE_256KB (256 * 1024)
#define MAX_BSIZE_1024KB (1024 * 1024)
#define MAX_BSIZE_4096KB (4096 * 1024)

/**
 * This value is used to set
 * uncompressed block size value.
 * 4th byte is always set to below
 * and placed as uncompressed byte
 */
#define NO_COMPRESS_BIT 128

/**
 * In case of uncompressed block
 * Values below are used to set
 * 3rd byte to following values
 * w.r.t various maximum block sizes
 * supported by standard
 */
#define BSIZE_NCOMP_64 1
#define BSIZE_NCOMP_256 4
#define BSIZE_NCOMP_1024 16
#define BSIZE_NCOMP_4096 64

/**
 * @brief Validate the compressed file.
 *
 * @param inFile_name input file name
 * @param outFile_name output file name
 */
int validate(std::string& inFile_name, std::string& outFile_name);

static uint64_t getFileSize(std::ifstream& file) {
    file.seekg(0, file.end);
    uint64_t file_size = file.tellg();
    file.seekg(0, file.beg);
    return file_size;
}

/**
 *  xfLz4Streaming class. Class containing methods for snappy
 * compression and decompression to be executed on host side.
 */
class xfLz4Streaming {
   public:
    /**
     * @brief Initialize the class object.
     *
     * @param binaryFile file to be read
     */
    int init(const std::string& binaryFile);

    /**
     * @brief release
     *
     */
    int release();

    /**
     * @brief Compress.
     *
     * @param in input byte sequence
     * @param out output byte sequence
     * @param input_size input size
     * @param host_buffer_size host buffer size
     */
    uint64_t compress(uint8_t* in, uint8_t* out, uint64_t input_size);

    /**
     * @brief Compress the input file.
     *
     * @param inFile_name input file name
     * @param outFile_name output file name
     * @param actual_size input size
     */
    uint64_t compressFile(std::string& inFile_name, std::string& outFile_name, uint64_t input_size);

    /**
     * @brief Decompress sequential.
     *
     * @param in input byte sequence
     * @param out output byte sequence
     * @param input_size input size
     * @param original_size original size
     * @param host_buffer_size host buffer size
     */
    uint64_t decompress(uint8_t* in, uint8_t* out, uint64_t input_size, uint64_t original_size);

    /**
     * @brief Decompress the input file.
     *
     * @param inFile_name input file name
     * @param outFile_name output file name
     * @param actual_size input size
     */
    uint64_t decompressFile(std::string& inFile_name, std::string& outFile_name, uint64_t input_size);

    /**
     * @brief Get the duration of input event
     *
     * @param event event to get duration for
     */
    uint64_t getEventDurationNs(const cl::Event& event);

    /**
     * @brief Create and map streams to kernel inputs
     *
     * @param flow yes/no
     */
    void mapStreams(bool flow);

    /**
     * Binary flow compress/decompress
     */
    bool m_bin_flow;

    /**
     * Block Size
     */
    uint32_t m_block_size_in_kb;

    /**
     * Switch between FPGA/Standard flows
     */
    bool m_switch_flow;

    /**
     * @brief Class constructor
     *
     */
    xfLz4Streaming();

    /**
     * @brief Class destructor.
     */
    ~xfLz4Streaming();

   private:
    cl::Device m_device;
    cl::Program* m_program;
    cl::Context* m_context;
    cl::CommandQueue* m_q;
    cl::Kernel* compress_kernel_lz4;
    cl::Kernel* decompress_kernel_lz4;
    bool first_init;

    // Compression related
    std::vector<uint8_t, aligned_allocator<uint8_t> > h_buf_in;
    std::vector<uint8_t, aligned_allocator<uint8_t> > h_buf_out;
    std::vector<uint32_t, aligned_allocator<uint32_t> > h_compressSize;

    // Extension pointer for device connection specification of streams
    cl_mem_ext_ptr_t cExt;
    cl_int cRet;

    // Compression Streams
    cl_stream cpr_input_stream;
    cl_stream cpr_output_stream;
    cl_stream cpr_csize_stream;

    // Extension pointer for decompression
    cl_mem_ext_ptr_t dExt;
    cl_int dRet;

    // Decompression Streams
    cl_stream dcp_input_stream;
    cl_stream dcp_output_stream;

    // Kernel names
    std::string compress_kernel_name = "xilLz4CompressStream";
    std::string decompress_kernel_name = "xilLz4DecompressStream";
};

#endif // _XFCOMPRESSION_XIL_LZ4_STREAMING_HPP_
