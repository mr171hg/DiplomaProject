#Organization: TUKE FEI KEMT 2021
#Feld of study: 9.2.4 Computer engineering
#Study program: Computer Networks, Bachelor Study 
#Author:  Marek Rohac
#Compiler: Winlibs GCC (MinGW-W64 x86_64-posix-seh, built by Brecht Sanders) 10.2.0 
#Description: simply Makefile for OS Windows

#
CC=gcc
CFLAGS= -Wall -Wextra -g 
WINFLAGS = -lbcrypt -liphlpapi -lws2_32
INCLUDEPATH = -I./include
SOURCES= dsvpn
CHARM=./src/charm.c
OS=./src/os.c
VPN=./src/vpn.c
all: $(SOURCES) 

$(SOURCES): %:
	$(CC) -o $< $(SOURCES) $(CHARM) $(OS) $(VPN) $(CFLAGS) $(INCLUDEPATH) $(WINFLAGS)

clean:
	$(RM) *.exe *.bin *.txt 