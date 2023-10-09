#include <stdio.h>

short int decode(char src)
{
    src ^= 0xFF;
    int sign = src & 0x80;
    int Intensity_3bit = (src & 0x70) >> 4;
    int data = src & 0x0f;
    data += 0x10;        // 将wxyz前面的1补上  
    data <<= 1;
    data += 0x1;         // a补1
    data <<= (Intensity_3bit + 2);

    short int dst = (short)(sign == 0 ? data : -data);
    return dst;
}

int main(int argc, char *argv[])
{
    FILE *fpin = NULL;
    FILE *fout = NULL;

    char tmp;
    short int pcm_val;

    fpin = fopen("./outputfile/8bitulaw.g711u", "rb");
    fout = fopen("./outputfile/16bitulawdec.pcm", "w+b");

    if(fpin && fout)
    {
        while (1 == fread(&tmp, sizeof(char), 1, fpin))
        {
            pcm_val = decode(tmp); // 8bit in, 16bit out
            fwrite(&pcm_val, sizeof(short int), 1, fout);
        }
        fclose(fpin);
        fclose(fout);
    }
    return 0;
}
