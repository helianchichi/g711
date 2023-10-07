#include<stdio.h>
   
unsigned char encode(short src)
{  

        short pcm  = src;
        int sign = (pcm & 0x8000) >> 8; 
        if(sign != 0)
            pcm = -pcm;
        if(pcm > 32635)   pcm = 32635;
        int exponent = 7;
        int expMask;
        for(expMask = 0x4000; (pcm & expMask) == 0 && exponent >0; exponent--,expMask >>= 1){}
        int mantissa = (pcm >> ((exponent == 0) ? 4 : (exponent + 3))) & 0x0f;
        unsigned char alaw = (unsigned char)(sign | exponent << 4 | mantissa);
        return (unsigned char)(alaw ^0xD5);   
        
} 

int main()
{
    short src = 0x0C8A;
    unsigned char dst = encode(src);

    printf("%x\n", dst);
}