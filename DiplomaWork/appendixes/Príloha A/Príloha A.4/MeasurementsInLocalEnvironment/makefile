#Organization: TUKE FEI KEMT 2021
#Feld of study: 9.2.4 Computer engineering
#Study program: Computer Networks, Bachelor Study 
#Author:  Marek Rohac
#Compiler: Winlibs GCC (MinGW-W64 x86_64-posix-seh, built by Brecht Sanders) 12.2.0 
#Description: simply Makefile for OS Windows

#
CC=gcc
CFLAGS= -Wall -Wextra -g 
WINFLAGS = -lbcrypt 
INCLUDEPATH = 
SOURCES= test
CHARM=charm.c
OS=localMeasurement.c
all: $(SOURCES) 

$(SOURCES): %:
	$(CC) -o $< $(SOURCES) $(CHARM) $(OS) $(CFLAGS) $(INCLUDEPATH) $(WINFLAGS)

clean:
	$(RM) *.exe 