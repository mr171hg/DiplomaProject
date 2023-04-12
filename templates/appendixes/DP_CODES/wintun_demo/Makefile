#Organization: TUKE FEI KEMT 2023
#Feld of study: 9.2.4 Computer engineering
#Study program: Computer Networks, Bachelor Study 
#Author:  Marek Rohac
#Compiler: Winlibs GCC (MinGW-W64 x86_64-posix-seh, built by Brecht Sanders) 12.2.0 
#Description: simple Makefile for OS Windows

CC=gcc
CFLAGS= -Wall -Wextra -g 
WINFLAGS =  -liphlpapi -lws2_32
INCLUDEPATH = 
SOURCES= demo1
TUN= wtun_demo.c

all: $(SOURCES) 

$(SOURCES): %:
	$(CC) -o $< $(SOURCES) $(TUN) $(CFLAGS) $(INCLUDEPATH) $(WINFLAGS)

clean:
	$(RM) *.exe *.bin *.txt 