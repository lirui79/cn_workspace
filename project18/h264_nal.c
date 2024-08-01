#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "h264_nal.h"


static const char *const h264_nal_type_name[32] = {
    "Unspecified 0", //H264_NAL_UNSPECIFIED
    "Coded slice of a non-IDR picture", // H264_NAL_SLICE
    "Coded slice data partition A", // H264_NAL_DPA
    "Coded slice data partition B", // H264_NAL_DPB
    "Coded slice data partition C", // H264_NAL_DPC
    "IDR", // H264_NAL_IDR_SLICE
    "SEI", // H264_NAL_SEI
    "SPS", // H264_NAL_SPS
    "PPS", // H264_NAL_PPS
    "AUD", // H264_NAL_AUD
    "End of sequence", // H264_NAL_END_SEQUENCE
    "End of stream", // H264_NAL_END_STREAM
    "Filler data", // H264_NAL_FILLER_DATA
    "SPS extension", // H264_NAL_SPS_EXT
    "Prefix", // H264_NAL_PREFIX
    "Subset SPS", // H264_NAL_SUB_SPS
    "Depth parameter set", // H264_NAL_DPS
    "Reserved 17", // H264_NAL_RESERVED17
    "Reserved 18", // H264_NAL_RESERVED18
    "Auxiliary coded picture without partitioning", // H264_NAL_AUXILIARY_SLICE
    "Slice extension", // H264_NAL_EXTEN_SLICE
    "Slice extension for a depth view or a 3D-AVC texture view", // H264_NAL_DEPTH_EXTEN_SLICE
    "Reserved 22", // H264_NAL_RESERVED22
    "Reserved 23", // H264_NAL_RESERVED23
    "Unspecified 24", // H264_NAL_UNSPECIFIED24
    "Unspecified 25", // H264_NAL_UNSPECIFIED25
    "Unspecified 26", // H264_NAL_UNSPECIFIED26
    "Unspecified 27", // H264_NAL_UNSPECIFIED27
    "Unspecified 28", // H264_NAL_UNSPECIFIED28
    "Unspecified 29", // H264_NAL_UNSPECIFIED29
    "Unspecified 30", // H264_NAL_UNSPECIFIED30
    "Unspecified 31", // H264_NAL_UNSPECIFIED31
};

/* 1. h264的起始码: 0x000001(3 Bytes)或0x00000001(4 Bytes);
 * 2. 文件流中用起始码来区分NALU.
 * nal -  input data pointer
 * return  0x00  - no find startcode   0x03 - startcode 3 bytes  0x04 - startcode 4 bytes
 */ 
int FindStartCode(unsigned char *rawbs) {
    if ((0x00 != rawbs[0]) || (0x00 != rawbs[1])) {
        return 0x00;
    }

    if (0x01 == rawbs[2]) {
        return 0x03;
    }

    if ((0x00 != rawbs[2]) || (0x01 != rawbs[3])) {
        return 0x00;
    }
    return 0x04;
}

/* find raw h264 bs data nal data size, not include startcode
 * 
 * rawbs -  input h264 bs data pointer
 * size  -  input h264 bs data size
 * startPos - find nal data start position
 * return  nal data size
 */ 
int FindNalu(unsigned char *rawbs, int size, int startPos) {
    int parsePos = startPos;
    while (parsePos < size) {
        if (FindStartCode(&rawbs[parsePos]) > 0) {
            return (parsePos - startPos);
        } else {
            ++parsePos;
        }
    }

    return (parsePos - startPos); // if file is end
}

/* parsel nalu data get nalu header
 * 
 * nalu    -  input h264 nalu data pointer
 * size    -  input h264 nalu data size
 * header  -  output h264 nalu nalu header
 * return code   code >= 0 success
 */ 
int ParseNaluHeader(const unsigned char *nalu, int size, h264_nalu_header *header) {
    memset(header, 0, sizeof(h264_nalu_header));
    if ((nalu[0] & 0x80) != 0) {
        printf("forbidden_zero_bit:%02x \n", nalu[0]);
        return -1;
    }
    header->nal_reference_idc =  (nalu[0] >> 5) & 0x3;
    header->nal_unit_type     =  nalu[0] & 0x1f;
#if 1
    static int naluNum = 0;
    printf("%5d| %04d| %6s| %8d|\n", naluNum, header->nal_reference_idc, h264_nal_type_name[header->nal_unit_type], size);
    ++naluNum;
#endif
    return 0x00;
}
