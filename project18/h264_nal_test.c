#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "h264_nal.h"



int main(int argc, char *argv[]) {
    int fileLen = 0;
    int naluLen = 0;
    int h264BitsPos = 0;

    unsigned char *h264Bits = NULL;
    unsigned char *naluData = NULL;

    FILE *fp = NULL;

    if (2 != argc)
    {
        printf("Usage: flvparse **.flv\n");

        return -1;
    }

    fp = fopen(argv[1], "rb");
    if (!fp)
    {
        printf("open file[%s] error!\n", argv[1]);

        return -1;
    }

    fseek(fp, 0, SEEK_END);

    fileLen = ftell(fp);

    fseek(fp, 0, SEEK_SET);

    h264Bits = (unsigned char*)malloc(fileLen);
    if (!h264Bits)
    {
        printf("maybe file is too long, or memery is not enough!\n");

        fclose(fp);

        return -1;
    }

    memset(h264Bits, 0x0, fileLen);

    if (fread(h264Bits, 1, fileLen, fp) < 0)
    {
        printf("read file data to h264Bits error!\n");

        fclose(fp);
        free(h264Bits);

        h264Bits = NULL;

        return -1;
    }

    fclose(fp);

    printf("-----+-------- NALU Table ------+\n");
    printf(" NUM |    IDC |  TYPE |   LEN   |\n");
    printf("-----+--------+-------+---------+\n");

    while (h264BitsPos < (fileLen-4))
    {
        int startcodeLen = FindStartCode(&h264Bits[h264BitsPos]);
        if (startcodeLen > 0) {
            naluLen = FindNalu(h264Bits, fileLen, h264BitsPos + startcodeLen);

            naluData = (unsigned char*)malloc(naluLen);
            if (naluData) {
                h264_nalu_header header = {0};
                memset(naluData, 0x0, naluLen);

                memcpy(naluData, h264Bits + h264BitsPos + startcodeLen, naluLen);
                ParseNaluHeader(naluData, naluLen, &header);

                free(naluData);
                naluData = NULL;
            }

            h264BitsPos += (naluLen + startcodeLen);
        } else {
            ++h264BitsPos;
        }      
    }

    return 0;
}