/*
Organization: Technical University of Kosice (TUKE),
Department: Department of Electronics and Multimedia Telecommunications (DEMT/KEMT), 
Faculties: Faculty of Electrical Engineering and Informatics (FEI),
Feld of study: 	Informatics,
Study program: Computer Networks,  
School year: 4., Ing., 2021/2022
Author of corrections:  Marek Rohac -- MR,
Last Update: 20.02.2022 -- add description
*/

#ifndef vpn_H
#define vpn_H 1

/*UNIX-like OS Dependent Libraries*/
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>

#include <net/if.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
/*End UNIX-like OS Dependent Libraries*/

/*%Standard C Libraries*/
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <netdb.h>
#include <poll.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
/*End of Standard C Libraries*/

/*OS setup dependencies*/
#ifdef __linux__
#include <linux/if_tun.h>
#endif

#ifdef __APPLE__
#include <net/if_utun.h>
#include <sys/kern_control.h>
#include <sys/sys_domain.h>
#endif

#define VERSION_STRING "0.1.4"

#ifdef __NetBSD__
#define DEFAULT_MTU 1500
#else
#define DEFAULT_MTU 9000
#endif
/*End of OS setup dependencies*/

/*Constants - system independent*/
#define RECONNECT_ATTEMPTS 100
#define TAG_LEN 6
#define MAX_PACKET_LEN 65536
#define TS_TOLERANCE 7200
#define TIMEOUT (60 * 1000)
#define ACCEPT_TIMEOUT (10 * 1000)
#define OUTER_CONGESTION_CONTROL_ALG "bbr"
#define BUFFERBLOAT_CONTROL 1    
#define NOTSENT_LOWAT (128 * 1024)
#define DEFAULT_CLIENT_IP "192.168.192.1"
#define DEFAULT_SERVER_IP "192.168.192.254"
#define DEFAULT_PORT "443"

/*Edianity check + setup*/
#if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && \
    __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ && !defined(NATIVE_BIG_ENDIAN)
#define NATIVE_BIG_ENDIAN
#endif

/*Big endian definition*/
#ifdef NATIVE_BIG_ENDIAN
#define endian_swap16(x) __builtin_bswap16(x)
#define endian_swap32(x) __builtin_bswap32(x)
#define endian_swap64(x) __builtin_bswap64(x)
#else
#define endian_swap16(x) (x)
#define endian_swap32(x) (x)
#define endian_swap64(x) (x)
#endif

//celociselny typ pre objekty, ktorych hodnota sa meni priradovacim operatorom
extern volatile sig_atomic_t exit_signal_received;
// atomova operacia -- vykonavanie nie je nikdy pozastavene, kym nie je ciastocne ukoncena operacia
//asi na spracovanie udajov zo sietovky ?? -- uvidime 
#endif
