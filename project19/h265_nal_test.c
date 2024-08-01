#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "h265_nal.h"



int main(int argc, char *argv[]) {
    int fileLen = 0;
    int naluLen = 0;
    int h265BitsPos = 0;

    unsigned char *h265Bits = NULL;
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

    h265Bits = (unsigned char*)malloc(fileLen);
    if (!h265Bits)
    {
        printf("maybe file is too long, or memery is not enough!\n");

        fclose(fp);

        return -1;
    }

    memset(h265Bits, 0x0, fileLen);

    if (fread(h265Bits, 1, fileLen, fp) < 0)
    {
        printf("read file data to h265Bits error!\n");

        fclose(fp);
        free(h265Bits);

        h265Bits = NULL;

        return -1;
    }

    fclose(fp);

    printf("-----+-------- NALU Table ------+\n");
    printf(" NUM |    TYPE | LAYER | TEMPORAL  |  LEN   |\n");
    printf("-----+--------+-------+---------+\n");

    while (h265BitsPos < (fileLen-4))
    {
        int startcodeLen = FindStartCode(&h265Bits[h265BitsPos]);
        if (startcodeLen > 0) {
            naluLen = FindNalu(h265Bits, fileLen, h265BitsPos + startcodeLen);

            naluData = (unsigned char*)malloc(naluLen);
            if (naluData) {
                h265_nalu_header header = {0};
                memset(naluData, 0x0, naluLen);

                memcpy(naluData, h265Bits + h265BitsPos + startcodeLen, naluLen);
                ParseNaluHeader(naluData, naluLen, &header);

                free(naluData);
                naluData = NULL;
            }

            h265BitsPos += (naluLen + startcodeLen);
        } else {
            ++h265BitsPos;
        }      
    }

    return 0;
}