#include <stdio.h>

short int decode(char src)
{
    src ^= 0xD5;
    int sign = src & 0x80;
    int Intensity_3bit = (src & 0x70) >> 4;
    int data = src & 0x0f;
    data <<= 4;
    data += 8;         // a补1
    if (Intensity_3bit != 0) // 将wxyz前面的1补上
        data += 0x100;
    if (Intensity_3bit > 1)
        data <<= (Intensity_3bit - 1);

    short int dst = (short)(sign == 0 ? data : -data);
    return dst;
}

int main(int argc, char *argv[])
{
    FILE *fpin = NULL;
    FILE *fout = NULL;

    char tmp;
    short int pcm_val;

    fpin = fopen("./8bitalaw.g711a", "rb");
    fout = fopen("./16bitalawdec.pcm", "w+b");

    while (1 == fread(&tmp, sizeof(char), 1, fpin))
    {
        pcm_val = decode(tmp); // 8bit in, 16bit out
        fwrite(&pcm_val, sizeof(short int), 1, fout);
    }
    fclose(fpin);
    fclose(fout);
    return 0;
}
