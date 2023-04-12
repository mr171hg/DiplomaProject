/*
#Organization: TUKE FEI KEMT 2023
#Feld of study: 9.2.4 Computer engineering
#Study program: Computer Networks, Bachelor Study 
#Author:  Marek Rohac
#Compiler: Winlibs GCC (MinGW-W64 x86_64-posix-seh, built by Brecht Sanders) 12.2.0 
#Description: simple demo of Wintun Adapter for OS Windows 
For run you have to run exe as administrator
For end of program press Ctrl-c
*/
#include <winsock2.h>
#include <Windows.h>
#include <ws2ipdef.h>
#include <iphlpapi.h>
#include <mstcpip.h>
#include <winternl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "wintun.h"

static WINTUN_CREATE_ADAPTER_FUNC *WintunCreateAdapter;
static WINTUN_CLOSE_ADAPTER_FUNC *WintunCloseAdapter;
static WINTUN_DELETE_DRIVER_FUNC *WintunDeleteDriver;
static WINTUN_START_SESSION_FUNC *WintunStartSession;
static WINTUN_END_SESSION_FUNC *WintunEndSession;
static WINTUN_GET_ADAPTER_LUID_FUNC *WintunGetAdapterLUID; 
static WINTUN_RECEIVE_PACKET_FUNC *WintunReceivePacket;
static WINTUN_RELEASE_RECEIVE_PACKET_FUNC *WintunReleaseReceivePacket;
static WINTUN_ALLOCATE_SEND_PACKET_FUNC *WintunAllocateSendPacket;
static WINTUN_SEND_PACKET_FUNC *WintunSendPacket;
static WINTUN_GET_READ_WAIT_EVENT_FUNC *WintunGetReadWaitEvent;

static volatile int keepRunning = 1;

void intHandler(int dummy) {
    keepRunning = 0;
}

int is_all_zeros(unsigned char* data, int data_size) {
    for (int i = 0; i < data_size; i++) {
        if (data[i] != 0) {
            return 0;
        }
    }
    return 1;
}

void print_packet(unsigned char *packet, int packet_size) {
    int f=0;
    if(!is_all_zeros(packet,packet_size)){
        printf("\nObsah prijateho paketu:\n");
        for (int i = 0; i < packet_size; i++) {
            printf("%02X", packet[i]);
            f++;
            if ((i+1) % 16 == 0) {
                printf("\n");
            }
        }
    }
    printf("\n");
    printf("Pocet bajtov: %d\n", f);
    if(packet[0] == 0x45) {
    printf("Prijate z: %d.%d.%d.%d\n", (int)packet[12],(int)packet[13],(int)packet[14],(int)packet[15]);
    printf("Odoslane z: %d.%d.%d.%d\n", (int)packet[16],(int)packet[17],(int)packet[18],(int)packet[19]);
    }
}

void tun_reader(WINTUN_SESSION_HANDLE session){ 
    BYTE *Packet;
    DWORD PacketSize;
    Packet = WintunReceivePacket(session, &PacketSize);
    if (Packet){   
    //here can be implemented another logic 
        print_packet(Packet,PacketSize);
        WintunReleaseReceivePacket(session, Packet);
    }else if (GetLastError() != ERROR_NO_MORE_ITEMS){
        printf("Tun reader error: %ld\n",GetLastError());
    }
}

static USHORT
IPChecksum(_In_reads_bytes_(Len) BYTE *Buffer, _In_ DWORD Len)
{
    ULONG Sum = 0;
    for (; Len > 1; Len -= 2, Buffer += 2)
        Sum += *(USHORT *)Buffer;
    if (Len)
        Sum += *Buffer;
    Sum = (Sum >> 16) + (Sum & 0xffff);
    Sum += (Sum >> 16);
    return (USHORT)(~Sum);
}

void tun_writer(WINTUN_SESSION_HANDLE session){     
   BYTE *OutgoingPacket = WintunAllocateSendPacket(session, 28);
    if (OutgoingPacket){    //formovanie icmp paketu pre ping request
        memset(OutgoingPacket, 0, 28);
        OutgoingPacket[0] = 0x45;
        *(USHORT *)&OutgoingPacket[2] = htons(28);
        OutgoingPacket[8] = 255;
        OutgoingPacket[9] = 1;
        *(ULONG *)&OutgoingPacket[12] = htonl((10 << 24) | (6 << 16) | (7 << 8) | (7 << 0)); /* 10.6.7.6 */ //cielova adresa
        *(ULONG *)&OutgoingPacket[16] = htonl((10 << 24) | (6 << 16) | (7 << 8) | (6 << 0)); /* 10.6.7.7 */ //destinacna addresa
        *(USHORT *)&OutgoingPacket[10] = IPChecksum(OutgoingPacket, 20);
        OutgoingPacket[20] = 8;
        *(USHORT *)&OutgoingPacket[22] = IPChecksum(&OutgoingPacket[20], 8);
        WintunSendPacket(session, OutgoingPacket);
        if(GetLastError() != 0 && GetLastError() != ERROR_NO_MORE_ITEMS) {
        printf("Tun writer error: %ld\n",GetLastError());
        }
    }
}

int main(){

    HMODULE Wintun = LoadLibraryExW(L"wintun.dll", NULL, LOAD_LIBRARY_SEARCH_APPLICATION_DIR | LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (!Wintun)
        return -1;
#define X(Name) ((*(FARPROC *)&Name = GetProcAddress(Wintun, #Name)) == NULL)
    if (X(WintunCreateAdapter) || X(WintunCloseAdapter) || X(WintunDeleteDriver) || 
        X(WintunStartSession) || X(WintunEndSession) || X(WintunGetAdapterLUID)|| 
        X(WintunReceivePacket) || X(WintunReleaseReceivePacket) ||
        X(WintunGetReadWaitEvent) || X(WintunAllocateSendPacket) ||
         X(WintunSendPacket))
#undef X
    {
        DWORD LastError = GetLastError();
        FreeLibrary(Wintun);
        SetLastError(LastError);
        return -1;
    }

    if (!Wintun){
        puts("Failed to initialize Wintun");
        FreeLibrary(Wintun);
        return -1;        
    }
    else{    
        puts("Wintun library loaded");
    }

    WINTUN_ADAPTER_HANDLE adapter  = WintunCreateAdapter(L"wtun2", L"Wintun2", NULL);
    if (!adapter){
        printf("Failed to create Wintun adapter: %ld\n", GetLastError());
        return -1;
    }
    else{
        printf("Adapter created\n");
    }

    MIB_UNICASTIPADDRESS_ROW AddressRow;
    InitializeUnicastIpAddressEntry(&AddressRow);
    WintunGetAdapterLUID(adapter, &AddressRow.InterfaceLuid);
    AddressRow.Address.Ipv4.sin_family = AF_INET;
    AddressRow.Address.Ipv4.sin_addr.S_un.S_addr =  htonl((10 << 24) | (6 << 16) | (7 << 8) | (7 << 0)); //moja ip na interfaci
    AddressRow.OnLinkPrefixLength = 24; /* This is a /24 network */     //maska
    AddressRow.DadState = IpDadStatePreferred;
    CreateUnicastIpAddressEntry(&AddressRow);
    WINTUN_SESSION_HANDLE session = WintunStartSession(adapter, 0x400000);
    signal(SIGINT, intHandler);
    while(keepRunning){
        tun_reader(session);
        tun_writer(session);
        Sleep(5000); //5sekund caka
    }

    WintunEndSession(session);
    WintunCloseAdapter(adapter);
    FreeLibrary(Wintun);
    printf("Adapter deleted\n");
    return 0;
}