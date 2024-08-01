
#ifndef H264_NALU_INCLUDED
#define H264_NALU_INCLUDED


#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C"{
#endif


/************************************************************************************************************
**                                        nalu header: 负责将VCL产生的比特字符串适配到各种各样的网络和多元环境中,
                                                       覆盖了所有片级以上的语法级别(NALU的作用, 方便网络传输)
**
-------------------------------------------------------------------------------------------------------------
**        字段名称            　　 |    长度(bits)    |        有关描述
-------------------------------------------------------------------------------------------------------------
**        forbidden_bit          |    1             |        编码中默认值为0, 当网络识别此单元中存在比特错误时, 可将其设为1, 以便接收方丢掉该单元
**        nal_reference_idc      |    2             |        0~3标识这个NALU的重要级别
**        nal_unit_type          |    5             |          NALU的类型(类型1~12是H.264定义的, 类型24~31是用于H.264以外的,
                                                             RTP负荷规范使用这其中的一些值来定义包聚合和分裂, 其他值为H.264保留)

** nal_unit_type:
    0                未使用
    1                未使用Data Partitioning, 非IDR图像的Slice
    2                使用Data Partitioning且为Slice A
    3                使用Data Partitioning且为Slice B
    4                使用Data Partitioning且为Slice C
    5                IDR图像的Slice(立即刷新)
    6                补充增强信息(SEI)
    7                序列参数集(sequence parameter set, SPS)
    8                图像参数集(picture parameter set, PPS)
    9                分界符
    10                序列结束
    11                码流结束
    12                填充
    13...23            保留
    24...31            未使用

** SPS, PPS. SLICE等信息就不解析了. 为了减少bits, 用了哥伦布编码(自己解析比较麻烦, 但是网上有很多).

** SPS信息说明:
        1. 视频宽高, 帧率等信息;
        2. seq_parameter_set_id, 指明本序列参数集的id号, 这个id号将被picture参数集引用;
        3. pic_width_in_mbs_minus1, 加1指定以宏块(16*16)为单位的每个解码图像的宽度, 即width = (pic_width_in_mbs_minus1 + 1) * 16
        4. pic_height_in_map_units_minus1;
        5. pic_order_cnt_type, 视频的播放顺序序号叫做POC(picture order count), 取值0,1,2;
        6. time_scale, fixed_frame_rate_flag, 计算帧率(fps).
           视频帧率信息在SPS的VUI parameters syntax中, 需要根据time_scale, fixed_frame_rate_flag计算得到: fps = time_scale / num_units_in_tick.
           但是需要判断参数timing_info_present_flag是否存在, 若不存在表示FPS在信息流中无法获取.
           同时还存在另外一种情况: fixed_frame_rate_flag为1时, 两个连续图像的HDR输出时间频率为单位, 获取的fps是实际的2倍.

** PPS信息说明:
        1. pic_parameter_set_id, 用以指定本参数集的序号, 该序号在各片的片头被引用;
        2. seq_parameter_set_id, 指明本图像参数集所引用的序列参数集的序号;
        3. 其他高深的暂时还不理解, 指明参考帧队列等.

** SLICE信息说明:
        1. slice_type, 片的类型;
        2. pic_parameter_set_id, 引用的图像索引;
        3. frame_num, 每个参考帧都有一个连续的frame_num作为它们的标识, 它指明了各图像的解码顺序. 非参考帧也有，但没有意义;
        4. least significant bits;
        5. 综合三种poc(pic_order_cnt_type), 类型2应该是最省bit的, 因为直接从frame_num获得, 但是序列方式限制最大;
           类型1, 只需要一定的bit量在sps标志出一些信息还在slice header中表示poc的变化, 但是比类型0要节省bit, 但是其序列并不是随意的, 要周期变化;
           对于类型0因为要对poc的lsb(pic_order_cnt_lsb, last bit)进行编码所以用到的bit最多, 优点是序列可以随意.
           ** 自我理解, 不一定准确(这边算显示顺序, 要根据SPS中的pic_order_cnt_type, 为2, 意味着码流中没有B帧, frame_num即为显示顺序;
              为1, 依赖frame_num求解POC; 为0, 把POC的低位编进码流内, 但这只是低位, 而POC的高位PicOrderCntMsb则要求解码器自行计数,
              计数方式依赖于前一编码帧(PrevPicOrderCntMsb与PrevPicOrderCntLsb.

           ** 一般的码流分析所见(未仔细证实): pic_order_cnt_type=2, 只有frame_num(无B帧);
              pic_order_cnt_type=1, 暂未分析到;
              pic_order_cnt_type=0, pic_order_cnt_lsb指示显示顺序, 一般为偶数增长(0, 2, 4, 6, 据说是什么场方式和帧方式, 场时其实是0 0 2 2 4 4).

           ** 编码与显示的原因: 视频编码顺序与视频的播放顺序, 并不完全相同, 视频编码时, 如果采用了B帧编码, 由于B帧很多时候都是双向预测得来的,
              这时会先编码B帧的后向预测图像(P帧), 然后再进行B帧编码, 因此会把视频原来的播放顺序打乱, 以新的编码顺序输出码流,
              而在解码断接收到码流后, 需要把顺序还原成原本的播放顺序, 以输出正确的视频. 在编解码中, 视频的播放顺序序号叫做POC(picture order count).

** 总结: 1. 码流中有很多SPS(序列), 一个序列中有多个图像, 一个图像中有多个片, 一个片中有多个块;
         2. SPS中有seq_parameter_set_id. PPS中有pic_parameter_set_id, 并通过seq_parameter_set_id指明关联的序列.
            SLICE中有pic_parameter_set_id, 指明关联的图像;
         3. SPS中可计算宽高以及帧率, pic_order_cnt_type(显示顺序的类型);
            SLICE HEADER中可算出解码的顺序, 以及根据pic_order_cnt_type算出显示顺序.
************************************************************************************************************/
enum {
    NALU_PRIORITY_DISPOSABLE     = 0,
    NALU_PRIORITY_LOW             = 1,
    NALU_PRIORITY_HIGH           = 2,
    NALU_PRIORITY_HIGHEST        = 3,
};

enum {
    NALU_TYPE_SLICE        = 1,
    NALU_TYPE_DPA          = 2,
    NALU_TYPE_DPB          = 3,
    NALU_TYPE_DPC          = 4,
    NALU_TYPE_IDR          = 5,
    NALU_TYPE_SEI          = 6,
    NALU_TYPE_SPS          = 7,
    NALU_TYPE_PPS          = 8,
    NALU_TYPE_AUD          = 9,
    NALU_TYPE_EOSEQ        = 10,
    NALU_TYPE_EOSTREAM     = 11,
    NALU_TYPE_FILL         = 12,
};

typedef struct h264_nalu_header {
//   uint8_t forbidden_zero_bit;     // forbidden_zero_bit:1
   uint8_t nal_reference_idc;      // nal_reference_idc:2
   uint8_t nal_unit_type;          // nal_unit_type:5
} h264_nalu_header;


/* find raw h264 bs data startcode
 * 1. h264的起始码: 0x000001(3 Bytes)或0x00000001(4 Bytes);
 * 2. 文件流中用起始码来区分NALU.
 * rawbs -  input h264 bs data pointer
 * return  0x00  - no find startcode   0x03 - startcode 3 bytes  0x04 - startcode 4 bytes
 */ 
int FindStartCode(unsigned char *rawbs);

/* find raw h264 bs data nal data size, not include startcode
 * 
 * rawbs -  input h264 bs data pointer
 * size  -  input h264 bs data size
 * startPos - find nal data start position
 * return  nal data size
 */ 
int FindNalu(unsigned char *rawbs, int size, int startPos);


/* parsel nalu data get nalu header
 * 
 * nalu    -  input h264 nalu data pointer
 * size    -  input h264 nalu data size
 * header  -  output h264 nalu nalu header
 * return code   code >= 0 success
 */ 
int ParseNaluHeader(const unsigned char *nalu, int size, h264_nalu_header *header);


#ifdef __cplusplus
}
#endif

#endif //H264_NALU_INCLUDED