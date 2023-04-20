/*
#Organization: TUKE FEI KEMT 2021
#Feld of study: 9.2.4 Computer engineering
#Study program: Computer Networks, Bachelor Study 
#Author:  Marek Rohac
#Compiler: Winlibs GCC (MinGW-W64 x86_64-posix-seh, built by Brecht Sanders) 12.2.0 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#include "charm.h"


//MACROS FOR EASIER TIMER MEASURMENT, VIA WINAPI QueryPerformanceCounter
#define TIMER_INIT \
    LARGE_INTEGER frequency; \
    LARGE_INTEGER t1,t2; \
    double elapsedtime; \
    QueryPerformanceFrequency(&frequency);


/** Use to start the performance timer */
#define TIMER_START QueryPerformanceCounter(&t1);

/** Use to stop the performance timer and output the result to the standard stream. Less verbose than \c TIMER_STOP_VERBOSE */
#define TIMER_STOP \
    QueryPerformanceCounter(&t2); \
    elapsedtime=1000*(double)(t2.QuadPart-t1.QuadPart)/frequency.QuadPart; //ms

static __inline__ uint64_t cpucyclesS(){
     unsigned cycles_low, cycles_high;
__asm__ volatile ("CPUID\n\t"
 "RDTSC\n\t"
 "mov %%edx, %0\n\t"
 "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
"%rax", "%rbx", "%rcx", "%rdx");
return  (((uint64_t)cycles_high << 32) | cycles_low );
}
//cpucyclesE -- end measurment
static __inline__ uint64_t  cpucyclesE(){
     unsigned cycles_low, cycles_high;
__asm__ volatile ("RDTSCP\n\t"
 "mov %%edx, %0\n\t"
 "mov %%eax, %1\n\t"
 "CPUID\n\t": "=r" (cycles_high), "=r" (cycles_low)::
"%rax", "%rbx", "%rcx", "%rdx");
return (((uint64_t)cycles_high << 32) | cycles_low );
}

int main(){

    unsigned char key[52] = {"totoje32bajtoveheslonaoverenieau"};
    unsigned char tag_full[16];
    unsigned char data1[52];  
    unsigned char data2[250];
    unsigned char data3[500];
    unsigned char data4[1000];
    unsigned char data5[1500];
    unsigned char data6[3000];
    unsigned char data7[4500];
    unsigned char data8[6000];
    unsigned char data9[7500];
    unsigned char data10[9000];
    uint32_t      uc_kx_st[12];
    uint32_t      uc_st[2][12];
    uint32_t      st[12];
    uint8_t       pkt1[32 + 8 + 32], pkt2[32 + 32];
    uint8_t       h[32];
    uint8_t       k[32];
    uint8_t       iv[16] = { 0 };

    uc_randombytes_buf(data1, 52);
    uc_randombytes_buf(data2, 250);
    uc_randombytes_buf(data3, 500);
    uc_randombytes_buf(data4, 1000);
    uc_randombytes_buf(data5, 1500);
    uc_randombytes_buf(data6, 3000);
    uc_randombytes_buf(data7, 4500);
    uc_randombytes_buf(data8, 6000);
    uc_randombytes_buf(data9, 7500);
    uc_randombytes_buf(data10, 9000);

    uc_state_init(uc_kx_st, key, (const unsigned char *) "VPN Key Exchange");
    uc_memzero(key, sizeof(key));



    memcpy(st, uc_kx_st, sizeof st);
    uc_randombytes_buf(pkt2, 64);
    uc_randombytes_buf(pkt1, 72);
    uc_hash(st, h, pkt1, 32 + 8+32);
    uc_hash(st, pkt2 + 32, pkt2, 32);
    uc_hash(st, k, NULL, 0);
    iv[0] = 0;
    uc_state_init(uc_st[0], k, iv);
    iv[0] ^= 1;
    uc_state_init(uc_st[1], k, iv);

    TIMER_INIT
    uint64_t time=0,tick=0;

    {TIMER_START
    tick = cpucyclesS();
	uc_encrypt(uc_st[0], data1, 52, tag_full);
	time = cpucyclesE() - tick;
	TIMER_STOP}
    printf("C:  %llu cycles in %f ms\n",time,elapsedtime);

    {TIMER_START
    tick = cpucyclesS();
	uc_decrypt(uc_st[1], data1, 52, tag_full, 16);
	time = cpucyclesE() - tick;
	TIMER_STOP}
    printf("D:  %llu cycles in %f ms\n",time,elapsedtime);
    printf("---------------------------------------------\n");

        {TIMER_START
    tick = cpucyclesS();
    uc_encrypt(uc_st[0], data2, 250, tag_full);
	time = cpucyclesE() - tick;
	TIMER_STOP}
    printf("C:  %llu cycles in %f ms\n",time,elapsedtime);


    {TIMER_START
    tick = cpucyclesS();
    uc_decrypt(uc_st[1], data2, 250, tag_full, 16);
	time = cpucyclesE() - tick;
	TIMER_STOP}
    printf("D:  %llu cycles in %f ms\n",time,elapsedtime);
    printf("---------------------------------------------\n");


    {TIMER_START
    tick = cpucyclesS();
    uc_encrypt(uc_st[0], data3, 500, tag_full);
	time = cpucyclesE() - tick;
	TIMER_STOP}
    printf("C:  %llu cycles in %f ms\n",time,elapsedtime);
    {TIMER_START
    tick = cpucyclesS();
    uc_decrypt(uc_st[1], data3, 500, tag_full, 16);
	time = cpucyclesE() - tick;
	TIMER_STOP}
    printf("D:  %llu cycles in %f ms\n",time,elapsedtime);
    printf("---------------------------------------------\n");

    {TIMER_START
    tick = cpucyclesS();
    uc_encrypt(uc_st[0], data4, 1000, tag_full);
	time = cpucyclesE() - tick;
	TIMER_STOP}
    printf("C:  %llu cycles in %f ms\n",time,elapsedtime);

    {TIMER_START
    tick = cpucyclesS();
    uc_decrypt(uc_st[1], data4, 1000, tag_full, 16);
	time = cpucyclesE() - tick;
	TIMER_STOP}
    printf("D:  %llu cycles in %f ms\n",time,elapsedtime);
    printf("---------------------------------------------\n");

    {TIMER_START
    tick = cpucyclesS();
    uc_encrypt(uc_st[0], data5, 1500, tag_full);
	time = cpucyclesE() - tick;
	TIMER_STOP}
    printf("C:  %llu cycles in %f ms\n",time,elapsedtime);

    {TIMER_START
    tick = cpucyclesS();
    uc_decrypt(uc_st[1], data5, 1500, tag_full, 16);
	time = cpucyclesE() - tick;
	TIMER_STOP}
    printf("D:  %llu cycles in %f ms\n",time,elapsedtime);
    printf("---------------------------------------------\n");

    {TIMER_START
    tick = cpucyclesS();
    uc_encrypt(uc_st[0], data6, 3000, tag_full);
	time = cpucyclesE() - tick;
	TIMER_STOP}
    printf("C:  %llu cycles in %f ms\n",time,elapsedtime);

    {TIMER_START
    tick = cpucyclesS();
    uc_decrypt(uc_st[1], data6, 3000, tag_full, 16);
	time = cpucyclesE() - tick;
	TIMER_STOP}
    printf("D:  %llu cycles in %f ms\n",time,elapsedtime);
    printf("---------------------------------------------\n");

    {TIMER_START
    tick = cpucyclesS();
    uc_encrypt(uc_st[0], data7, 4500, tag_full);
	time = cpucyclesE() - tick;
	TIMER_STOP}
    printf("C:  %llu cycles in %f ms\n",time,elapsedtime);

    {TIMER_START
    tick = cpucyclesS();
    uc_decrypt(uc_st[1], data7, 4500, tag_full, 16);
	time = cpucyclesE() - tick;
	TIMER_STOP}
    printf("D:  %llu cycles in %f ms\n",time,elapsedtime);
    printf("---------------------------------------------\n");

    {TIMER_START
    tick = cpucyclesS();
    uc_encrypt(uc_st[0], data8, 6000, tag_full);
	time = cpucyclesE() - tick;
	TIMER_STOP}
    printf("C:  %llu cycles in %f ms\n",time,elapsedtime);

    {TIMER_START
    tick = cpucyclesS();
    uc_decrypt(uc_st[1], data8, 6000, tag_full, 16);
	time = cpucyclesE() - tick;
	TIMER_STOP}
    printf("D:  %llu cycles in %f ms\n",time,elapsedtime);
    printf("---------------------------------------------\n");

    {TIMER_START
    tick = cpucyclesS();
    uc_encrypt(uc_st[0], data9, 7500, tag_full);
	time = cpucyclesE() - tick;
	TIMER_STOP}
    printf("C:  %llu cycles in %f ms\n",time,elapsedtime);

    {TIMER_START
    tick = cpucyclesS();
    uc_decrypt(uc_st[1], data9, 7500, tag_full, 16);
	time = cpucyclesE() - tick;
	TIMER_STOP}
    printf("D:  %llu cycles in %f ms\n",time,elapsedtime);
    printf("---------------------------------------------\n");

    {TIMER_START
    tick = cpucyclesS();
    uc_encrypt(uc_st[0], data10, 9000, tag_full);
	time = cpucyclesE() - tick;
	TIMER_STOP}
    printf("C:  %llu cycles in %f ms\n",time,elapsedtime);

    {TIMER_START
    tick = cpucyclesS();
    uc_decrypt(uc_st[1], data10, 9000, tag_full, 16);
	time = cpucyclesE() - tick;
	TIMER_STOP}
    printf("D:  %llu cycles in %f ms\n",time,elapsedtime);
    printf("---------------------------------------------\n");

    return 0;
}