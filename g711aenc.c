#include<stdio.h>
   
unsigned char encode(short src)//-32768 ~ 32767
{  
        int sign = (src & 0x8000) >> 8; //1. 取符号位
        if(sign != 0)
            src = -src - 1;
        int Intensity_3bit = 7;
        int template = 0x4000; // 0100 0000 0000 0000

        for(; (src & template) == 0 && Intensity_3bit >0; Intensity_3bit--,template >>= 1){}
        int HighSample_4bit = (src >> ((Intensity_3bit == 0) ? 4 : (Intensity_3bit + 3))) & 0x0f;
        unsigned char alaw = (unsigned char)(sign | Intensity_3bit << 4 | HighSample_4bit);

        return (unsigned char)(alaw ^0xD5); //1101 0101      
} 

int main(int argc, char *argv[]) {
    FILE *fpin = NULL;
    FILE *fout = NULL;

    char tmp;
    short int pcm_val;

    fpin = fopen("./hsggx_44100_stereo.pcm", "rb");
    fout = fopen("./8bitalaw.g711a", "w+b");

    while (1 == fread(&pcm_val, sizeof(short int), 1, fpin)) {
        tmp = encode(pcm_val);  // 16bit in, 8bit out
        fputc(tmp, fout);
    }

    fclose(fpin);
    fclose(fout);
    return 0;
}