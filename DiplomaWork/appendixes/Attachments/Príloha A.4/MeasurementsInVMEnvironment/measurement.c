/*
#Organization: TUKE FEI KEMT 2021
#Feld of study: 9.2.4 Computer engineering
#Study program: Computer Networks, Bachelor Study 
#Author:  Marek Rohac
#Compiler: Winlibs GCC (MinGW-W64 x86_64-posix-seh, built by Brecht Sanders) 12.2.0 
#Description: simple program for reading results of measurements
Compilation: gcc measurement.c -g -Wall -Wextra
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define MAX_LINE_LEN 40

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int writeContent(int *array){
    int number=array[0];
    int counter =1;
    for (int i = 1; i < 2550; i++)
    {   
        if (array[i]==number)
        {
            counter++;
        }
        else{
            if (counter>=20)
            {
                printf("N:%d C:%d\n",number,counter);
            }
            counter=1;
            number=array[i];
        }      
    }
    return 0;
}

int main() {
    FILE *file = fopen("testingK.txt", "r");
    if (file == NULL) {
        printf("Failed to open file.\n");
        return 1;
    }
    int array[2550],j=0,k=0, num = 0;
    memset(array,0,sizeof(int)*2550);
    char line[MAX_LINE_LEN];
    memset(line,0,MAX_LINE_LEN);

    while (fgets(line, MAX_LINE_LEN, file) != NULL) {
        j=2;num=0;
        while (line[j] != 'B') {
        if (line[j] >= '0' && line[j] <= '9') {
            num = num * 10 + (line[j] - '0');
        }
        j++;
        }
        array[k]=num;
        k++;
    }
    file = fopen("testingK.txt", "r");
    if (file == NULL) {
        printf("Failed to open file.\n");
        return 1;
    }
    num = 0;
    memset(line,0,MAX_LINE_LEN);

    qsort(array,2500,sizeof(int),cmpfunc);
    writeContent(array);

    int i = 0, counter=0 ,c=0,t=0,zero=0;
    uint64_t sumc=0;
    double time=0;
    while (fgets(line, MAX_LINE_LEN, file) != NULL) {
        i=2;num=0; zero=0; c=0;t=0;
        while (line[i] != '\0') {
        if (line[i] >= '0' && line[i] <= '9') {
            num = num * 10 + (line[i] - '0');
            if(line[i]=='0') zero++;
        }else{
             if (line[i] == 'B'&&c!=1 &&t!=1) {
                 if(num==52){   //cislo na spocitanie 
                    counter++;
                     i=i+3;
                     num=0;
                     zero=0;
                     c=1;
                     t=0;
                 }else {
                    
                    break;
                    }
            }
            if(line[i]==' ' && num!=0 && t!=1){
             if (c==1){
                 i=i+12;
                 t=1;
                 if(num<3500){  //max odchylka
                 sumc+=(uint64_t) num;
                 }else
                 {counter--;t=0;}
                 num=0;
                 zero=0;
             }
            }
            if(line[i]==' ' && t==1 && c==1 && num !=0){
             if(line[i+1]=='s'){
                 time+=(double)num/pow(10,zero+ floor(log10(num)) + 1);
                 t=0;
                 c=0;
                 zero=0;
                 num=0;
                 break;
             }
            }
        }
        i++;
        //t=0;c=0;
    }
    }
    printf("KS: %d C: %lld T: %f\n",counter,sumc,time);
    printf("AC: %lld AT:%f\n",sumc/counter,time/counter);
    fclose(file);
    return 0;
}