
#ifndef H265_NALU_INCLUDED
#define H265_NALU_INCLUDED


#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C"{
#endif


/**
 * Table 7-1 – NAL unit type codes and NAL unit type classes in
 * T-REC-H.265-201802
 */
enum HEVCNALUnitType {
    HEVC_NAL_TRAIL_N        = 0,
    HEVC_NAL_TRAIL_R        = 1,
    HEVC_NAL_TSA_N          = 2,
    HEVC_NAL_TSA_R          = 3,
    HEVC_NAL_STSA_N         = 4,
    HEVC_NAL_STSA_R         = 5,
    HEVC_NAL_RADL_N         = 6,
    HEVC_NAL_RADL_R         = 7,
    HEVC_NAL_RASL_N         = 8,
    HEVC_NAL_RASL_R         = 9,
    HEVC_NAL_VCL_N10        = 10,
    HEVC_NAL_VCL_R11        = 11,
    HEVC_NAL_VCL_N12        = 12,
    HEVC_NAL_VCL_R13        = 13,
    HEVC_NAL_VCL_N14        = 14,
    HEVC_NAL_VCL_R15        = 15,
    HEVC_NAL_BLA_W_LP       = 16,
    HEVC_NAL_BLA_W_RADL     = 17,
    HEVC_NAL_BLA_N_LP       = 18,
    HEVC_NAL_IDR_W_RADL     = 19,
    HEVC_NAL_IDR_N_LP       = 20,
    HEVC_NAL_CRA_NUT        = 21,
    HEVC_NAL_RSV_IRAP_VCL22 = 22,
    HEVC_NAL_RSV_IRAP_VCL23 = 23,
    HEVC_NAL_RSV_VCL24      = 24,
    HEVC_NAL_RSV_VCL25      = 25,
    HEVC_NAL_RSV_VCL26      = 26,
    HEVC_NAL_RSV_VCL27      = 27,
    HEVC_NAL_RSV_VCL28      = 28,
    HEVC_NAL_RSV_VCL29      = 29,
    HEVC_NAL_RSV_VCL30      = 30,
    HEVC_NAL_RSV_VCL31      = 31,
    HEVC_NAL_VPS            = 32,
    HEVC_NAL_SPS            = 33,
    HEVC_NAL_PPS            = 34,
    HEVC_NAL_AUD            = 35,
    HEVC_NAL_EOS_NUT        = 36,
    HEVC_NAL_EOB_NUT        = 37,
    HEVC_NAL_FD_NUT         = 38,
    HEVC_NAL_SEI_PREFIX     = 39,
    HEVC_NAL_SEI_SUFFIX     = 40,
    HEVC_NAL_RSV_NVCL41     = 41,
    HEVC_NAL_RSV_NVCL42     = 42,
    HEVC_NAL_RSV_NVCL43     = 43,
    HEVC_NAL_RSV_NVCL44     = 44,
    HEVC_NAL_RSV_NVCL45     = 45,
    HEVC_NAL_RSV_NVCL46     = 46,
    HEVC_NAL_RSV_NVCL47     = 47,
    HEVC_NAL_UNSPEC48       = 48,
    HEVC_NAL_UNSPEC49       = 49,
    HEVC_NAL_UNSPEC50       = 50,
    HEVC_NAL_UNSPEC51       = 51,
    HEVC_NAL_UNSPEC52       = 52,
    HEVC_NAL_UNSPEC53       = 53,
    HEVC_NAL_UNSPEC54       = 54,
    HEVC_NAL_UNSPEC55       = 55,
    HEVC_NAL_UNSPEC56       = 56,
    HEVC_NAL_UNSPEC57       = 57,
    HEVC_NAL_UNSPEC58       = 58,
    HEVC_NAL_UNSPEC59       = 59,
    HEVC_NAL_UNSPEC60       = 60,
    HEVC_NAL_UNSPEC61       = 61,
    HEVC_NAL_UNSPEC62       = 62,
    HEVC_NAL_UNSPEC63       = 63,
};

enum HEVCSliceType {
    HEVC_SLICE_B = 0,
    HEVC_SLICE_P = 1,
    HEVC_SLICE_I = 2,
};

typedef struct h265_nalu_header {
//   uint8_t forbidden_zero_bit;     // forbidden_zero_bit:1
   uint8_t nal_unit_type;            // nal_unit_type:6
   uint8_t nuh_layer_id;             // nuh_layer_id:6
   uint8_t nuh_temporal_id;          // nuh_temporal_id:3
} h265_nalu_header;


/* find raw h265 bs data startcode
 * 1. h265的起始码: 0x000001(3 Bytes)或0x00000001(4 Bytes);
 * 2. 文件流中用起始码来区分NALU.
 * rawbs -  input h265 bs data pointer
 * return  0x00  - no find startcode   0x03 - startcode 3 bytes  0x04 - startcode 4 bytes
 */ 
int FindStartCode(unsigned char *rawbs);

/* find raw h265 bs data nal data size, not include startcode
 * 
 * rawbs -  input h265 bs data pointer
 * size  -  input h265 bs data size
 * startPos - find nal data start position
 * return  nal data size
 */ 
int FindNalu(unsigned char *rawbs, int size, int startPos);


/* parsel nalu data get nalu header
 * 
 * nalu    -  input h265 nalu data pointer
 * size    -  input h265 nalu data size
 * header  -  output h265 nalu nalu header
 * return code   code >= 0 success
 */ 
int ParseNaluHeader(const unsigned char *nalu, int size, h265_nalu_header *header);


#ifdef __cplusplus
}
#endif

#endif //H264_NALU_INCLUDED