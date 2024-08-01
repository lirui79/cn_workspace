#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "h265_nal.h"


static const char *const hevc_nal_type_name[64] = {
    "TRAIL_N", // HEVC_NAL_TRAIL_N
    "TRAIL_R", // HEVC_NAL_TRAIL_R
    "TSA_N", // HEVC_NAL_TSA_N
    "TSA_R", // HEVC_NAL_TSA_R
    "STSA_N", // HEVC_NAL_STSA_N
    "STSA_R", // HEVC_NAL_STSA_R
    "RADL_N", // HEVC_NAL_RADL_N
    "RADL_R", // HEVC_NAL_RADL_R
    "RASL_N", // HEVC_NAL_RASL_N
    "RASL_R", // HEVC_NAL_RASL_R
    "RSV_VCL_N10", // HEVC_NAL_VCL_N10
    "RSV_VCL_R11", // HEVC_NAL_VCL_R11
    "RSV_VCL_N12", // HEVC_NAL_VCL_N12
    "RSV_VLC_R13", // HEVC_NAL_VCL_R13
    "RSV_VCL_N14", // HEVC_NAL_VCL_N14
    "RSV_VCL_R15", // HEVC_NAL_VCL_R15
    "BLA_W_LP", // HEVC_NAL_BLA_W_LP
    "BLA_W_RADL", // HEVC_NAL_BLA_W_RADL
    "BLA_N_LP", // HEVC_NAL_BLA_N_LP
    "IDR_W_RADL", // HEVC_NAL_IDR_W_RADL
    "IDR_N_LP", // HEVC_NAL_IDR_N_LP
    "CRA_NUT", // HEVC_NAL_CRA_NUT
    "RSV_IRAP_VCL22", // HEVC_NAL_RSV_IRAP_VCL22
    "RSV_IRAP_VCL23", // HEVC_NAL_RSV_IRAP_VCL23
    "RSV_VCL24", // HEVC_NAL_RSV_VCL24
    "RSV_VCL25", // HEVC_NAL_RSV_VCL25
    "RSV_VCL26", // HEVC_NAL_RSV_VCL26
    "RSV_VCL27", // HEVC_NAL_RSV_VCL27
    "RSV_VCL28", // HEVC_NAL_RSV_VCL28
    "RSV_VCL29", // HEVC_NAL_RSV_VCL29
    "RSV_VCL30", // HEVC_NAL_RSV_VCL30
    "RSV_VCL31", // HEVC_NAL_RSV_VCL31
    "VPS", // HEVC_NAL_VPS
    "SPS", // HEVC_NAL_SPS
    "PPS", // HEVC_NAL_PPS
    "AUD", // HEVC_NAL_AUD
    "EOS_NUT", // HEVC_NAL_EOS_NUT
    "EOB_NUT", // HEVC_NAL_EOB_NUT
    "FD_NUT", // HEVC_NAL_FD_NUT
    "SEI_PREFIX", // HEVC_NAL_SEI_PREFIX
    "SEI_SUFFIX", // HEVC_NAL_SEI_SUFFIX
    "RSV_NVCL41", // HEVC_NAL_RSV_NVCL41
    "RSV_NVCL42", // HEVC_NAL_RSV_NVCL42
    "RSV_NVCL43", // HEVC_NAL_RSV_NVCL43
    "RSV_NVCL44", // HEVC_NAL_RSV_NVCL44
    "RSV_NVCL45", // HEVC_NAL_RSV_NVCL45
    "RSV_NVCL46", // HEVC_NAL_RSV_NVCL46
    "RSV_NVCL47", // HEVC_NAL_RSV_NVCL47
    "UNSPEC48", // HEVC_NAL_UNSPEC48
    "UNSPEC49", // HEVC_NAL_UNSPEC49
    "UNSPEC50", // HEVC_NAL_UNSPEC50
    "UNSPEC51", // HEVC_NAL_UNSPEC51
    "UNSPEC52", // HEVC_NAL_UNSPEC52
    "UNSPEC53", // HEVC_NAL_UNSPEC53
    "UNSPEC54", // HEVC_NAL_UNSPEC54
    "UNSPEC55", // HEVC_NAL_UNSPEC55
    "UNSPEC56", // HEVC_NAL_UNSPEC56
    "UNSPEC57", // HEVC_NAL_UNSPEC57
    "UNSPEC58", // HEVC_NAL_UNSPEC58
    "UNSPEC59", // HEVC_NAL_UNSPEC59
    "UNSPEC60", // HEVC_NAL_UNSPEC60
    "UNSPEC61", // HEVC_NAL_UNSPEC61
    "UNSPEC62", // HEVC_NAL_UNSPEC62
    "UNSPEC63", // HEVC_NAL_UNSPEC63
};

/* find raw h265 bs data startcode
 * 1. h265的起始码: 0x000001(3 Bytes)或0x00000001(4 Bytes);
 * 2. 文件流中用起始码来区分NALU.
 * rawbs -  input h265 bs data pointer
 * size  -  input h265 bs data size
 * return  0x00  - no find startcode   0x03 - startcode 3 bytes  0x04 - startcode 4 bytes
 */ 
int FindStartCode(unsigned char *rawbs) {
    uint32_t code = 0x00, i = 0;
    for (i = 0; i < 3; ++i) {
        code = (code << 8) + rawbs[i];
    }

    if (code == 0x00000001) {
        return 0x03;
    }
    
    if (code != 0x00000000) {
        return 0x00;
    }

    code = (code << 8) + rawbs[3];
    if (code == 0x00000001) {
        return 0x04;
    }
    
    return 0x00;
#if 0
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
#endif
}

/* find raw h265 bs data nal data size, not include startcode
 * 
 * rawbs -  input h265 bs data pointer
 * size  -  input h265 bs data size
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
 * nalu    -  input h265 nalu data pointer
 * size    -  input h265 nalu data size
 * header  -  output h265 nalu nalu header
 * return code   code >= 0 success
 */ 
int ParseNaluHeader(const unsigned char *nalu, int size, h265_nalu_header *header) {
    uint16_t nalheader = nalu[0];
    memset(header, 0, sizeof(h265_nalu_header));
    if ((nalheader & 0x80) != 0) {
        printf("forbidden_zero_bit:%02x \n", nalu[0]);
        return -1;
    }
    nalheader = (nalheader << 8) + nalu[1];
    header->nal_unit_type    =  (nalheader & 0x7E00) >> 9;
    header->nuh_layer_id     =  (nalheader & 0x01f8) >> 3;
    header->nuh_temporal_id  =  nalheader & 0x07;
#if 1
    static int naluNum = 0;
    printf("%5d| %6s| %8d| %8d| %8d|\n", naluNum, hevc_nal_type_name[header->nal_unit_type], header->nuh_layer_id, header->nuh_temporal_id, size);
    ++naluNum;
#endif
    return 0x00;
}
