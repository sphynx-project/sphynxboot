/* Copyright (C) 2024 Leaf and contributors.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __SEIF_H__
#define __SEIF_H__

// Internal defines
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

// Seif Meta
typedef struct {
    u8 signature[8];  // Example: "LEAFSEIF". This indenticates the program the
                      // image was generated by.
    u32 width;
    u32 height;
} __attribute((packed)) SEIF_Meta;

// Seif Header
typedef struct {
    u8 magic[4];     // "SEIF".
    u8 flags;        // SIEF_FLAG_X ored together.
    u8 encoding;     // RGBA, RGB, ARGB, etc.
    SEIF_Meta meta;  // Meta data (signature, etc.).
    // Chunk Info
    u32 chunk_count;
    u32 chunk_size;
} __attribute((packed)) SEIF_Header;

// Image Chunk Header
typedef struct {
    u32 width;   //	Width and height have to multiply to chunk_size otherwise
                 // its an invalid image
    u32 height;  //
} __attribute((packed)) SEIF_ChunkHeader;

// Seif Flags
#define SEIF_FLAG_ALPHA (1 << 0)
#define SEIF_FLAG_COMPRESSED                                                   \
    (1 << 1)  // Indecates if the image is compressed. TODO

// Seif Encoding
#define SEIF_ENCODING_RGBA 0x01
#define SEIF_ENCODING_RGB 0x02
#define SEIF_ENCODING_ARGB 0x03

// Seif image layout:
//   [HEADER]:
//   	- magic: "SEIF"
//   	- flags
//   	- encoding
//   	- [META]:
//   		- signature: "LEAFSEIF"
//   	- chunk_count
//   	- chunk_size
//   [CHUNK]
//   	- [HEADER]:
//   		- width
//   		- height
//   	- [data]: The data can be represented in code as `uint8_t
//   data[chunk_size * encoding_size]`. Both gathered from the header
//
// Chunks are layed out like this on the image:
//
// 	 1  |  2  |  3
//  --------------
//   4  |  5  |  6
//  --------------
//   7  |  8  |  9
//
//   The data in the chunks are layed out from "left -> right" so a chunk would
//   look like this in the image:
//   ______________
//  | 1 2 3 4 ... |
//  |             |
//  |             |
//  |			  |
//  |_____________|
//
// 1 being some data and so on.

#endif  // __SEIF_H__