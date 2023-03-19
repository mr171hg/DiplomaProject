#include "os.h"
#include "vpn.h"
#include "wintun.h"


#ifdef _WIN32
int poll(struct pollfd *fds, nfds_t nfds, int timeout){
    return WSAPoll(fds, nfds, timeout);
}
int close(int fd){
    return closesocket(fd);
}
#endif
 
ssize_t safe_read(const int fd, void *const buf_, size_t count, const int timeout)
{
    struct pollfd  pfd;
    unsigned char *buf    = (unsigned char *) buf_;
    ssize_t        readnb = (ssize_t) -1;

    while (readnb != 0 && count > (ssize_t) 0) {
        while ((readnb = read(fd, buf, count)) < (ssize_t) 0) {
            if (errno == EAGAIN) {
                pfd.fd     = fd;
                pfd.events = POLLIN;
                
                if (poll(&pfd, (nfds_t) 1, timeout) <= 0) {
                    return (ssize_t) -1;
                }
            } else if (errno != EINTR || exit_signal_received) {
                return (ssize_t) -1;
            }
        }
        count -= readnb;
        buf += readnb;
    }
    return (ssize_t)(buf - (unsigned char *) buf_);
}

ssize_t safe_write(const int fd, const void *const buf_, size_t count, const int timeout)
{
    struct pollfd pfd;
    const char *  buf = (const char *) buf_;
    ssize_t       written;

    while (count > (size_t) 0) {
        while ((written = write(fd, buf, count)) < (ssize_t) 0) {
            if (errno == EAGAIN) {
                pfd.fd     = fd;
                pfd.events = POLLOUT;
                if (poll(&pfd, (nfds_t) 1, timeout) <= 0) {
                    return (ssize_t) -1;
                }
            } else if (errno != EINTR || exit_signal_received) {
                return (ssize_t) -1;
            }
        }
        buf += written;
        count -= written;
    }
    return (ssize_t)(buf - (const char *) buf_);
}

ssize_t safe_read_partial(const int fd, void *const buf_, const size_t max_count)
{
    unsigned char *const buf = (unsigned char *) buf_;
    ssize_t              readnb;

    while ((readnb = read(fd, buf, max_count)) < (ssize_t) 0 && errno == EINTR &&
           !exit_signal_received)
        ;
    return readnb;
}

ssize_t safe_write_partial(const int fd, void *const buf_, const size_t max_count)
{
    unsigned char *const buf = (unsigned char *) buf_;
    ssize_t              writenb;

    while ((writenb = write(fd, buf, max_count)) < (ssize_t) 0 && errno == EINTR &&
           !exit_signal_received)
        ;
    return writenb;
}

#ifdef __linux__
int tun_create(char if_name[IFNAMSIZ], const char *wanted_name)
{
    struct ifreq ifr;
    int          fd;
    int          err;

    fd = open("/dev/net/tun", O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "tun module not present. See https://sk.tl/2RdReigK\n");
        return -1;
    }
    ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
    snprintf(ifr.ifr_name, IFNAMSIZ, "%s", wanted_name == NULL ? "" : wanted_name);
    if (ioctl(fd, TUNSETIFF, &ifr) != 0) {
        err = errno;
        (void) close(fd);
        errno = err;
        return -1;
    }
    snprintf(if_name, IFNAMSIZ, "%s", ifr.ifr_name);

    return fd;
}
#elif defined(_WIN32)
HANDLE tun_create(char if_name[IFNAMSIZ], const char *wanted_name)
{   
    /*HMODULE Wintun = InitializeWintun();
    if (!Wintun){
        puts("Failed to initialize Wintun");
        return INVALID_HANDLE_VALUE;        
    }
    else{    
        puts("Wintun library loaded");
    }*/

    char adapterName = (wanted_name == NULL || strcmp(wanted_name, "auto") == 0) ? "wtun0" : wanted_name;
    //GUID MyGuid = { 0xdeadbabe, 0xcafe, 0xbeef, { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef } }; //moze byt null
    WINTUN_ADAPTER_HANDLE Adapter = WintunCreateAdapter(adapterName, "Wintun", NULL);
    if (!Adapter){
        puts("Failed to create Wintun adapter");
        return INVALID_HANDLE_VALUE;
    }
    snprintf(if_name, IFNAMSIZ, "%s", adapterName);

    /*DWORD Version = WintunGetRunningDriverVersion();
    puts("Wintun v%u.%u loaded", (Version >> 16) & 0xff, (Version >> 0) & 0xff);
    
    char* ipAddrStr = "192.168.1.1";
    unsigned long ipAddr = inet_addr(ipAddrStr);
    if (ipAddr == INADDR_NONE) {
        printf("Invalid IP address: %s\n", ipAddrStr);
    } else {
        printf("IP address in binary BE: %u\n", ipAddr);
    }
    
    */

   /* MIB_UNICASTIPADDRESS_ROW AddressRow;
    InitializeUnicastIpAddressEntry(&AddressRow);
    WintunGetAdapterLUID(Adapter, &AddressRow.InterfaceLuid);
    AddressRow.Address.Ipv4.sin_family = AF_INET;
    AddressRow.Address.Ipv4.sin_addr.S_un.S_addr = htonl((10 << 24) | (6 << 16) | (7 << 8) | (7 << 0)); // 10.6.7.7 //
    AddressRow.OnLinkPrefixLength = 24; // This is a /24 network //
    AddressRow.DadState = IpDadStatePreferred;
    DWORD LastError = CreateUnicastIpAddressEntry(&AddressRow);
    if (LastError != ERROR_SUCCESS && LastError != ERROR_OBJECT_ALREADY_EXISTS)
    {
        puts("Failed to set Wintun IP address: %u", LastError);
        WintunCloseAdapter(Adapter);
        return INVALID_HANDLE_VALUE;
    }*/
    return Adapter;
}
#elif defined(__APPLE__)
static int tun_create_by_id(char if_name[IFNAMSIZ], unsigned int id)
{
    struct ctl_info     ci;
    struct sockaddr_ctl sc;
    int                 err;
    int                 fd;

    if ((fd = socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL)) == -1) {
        return -1;
    }
    memset(&ci, 0, sizeof ci);
    snprintf(ci.ctl_name, sizeof ci.ctl_name, "%s", UTUN_CONTROL_NAME);
    if (ioctl(fd, CTLIOCGINFO, &ci)) {
        err = errno;
        (void) close(fd);
        errno = err;
        return -1;
    }
    memset(&sc, 0, sizeof sc);
    sc = (struct sockaddr_ctl){
        .sc_id      = ci.ctl_id,
        .sc_len     = sizeof sc,
        .sc_family  = AF_SYSTEM,
        .ss_sysaddr = AF_SYS_CONTROL,
        .sc_unit    = id + 1,
    };
    if (connect(fd, (struct sockaddr *) &sc, sizeof sc) != 0) {
        err = errno;
        (void) close(fd);
        errno = err;
        return -1;
    }
    snprintf(if_name, IFNAMSIZ, "utun%u", id);

    return fd;
}

int tun_create(char if_name[IFNAMSIZ], const char *wanted_name)
{
    unsigned int id;
    int          fd;

    if (wanted_name == NULL || *wanted_name == 0) {
        for (id = 0; id < 32; id++) {
            if ((fd = tun_create_by_id(if_name, id)) != -1) {
                return fd;
            }
        }
        return -1;
    }
    if (sscanf(wanted_name, "utun%u", &id) != 1) {
        errno = EINVAL;
        return -1;
    }
    return tun_create_by_id(if_name, id);
}
#elif defined(__OpenBSD__) || defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__)
int tun_create(char if_name[IFNAMSIZ], const char *wanted_name)
{
    char         path[64];
    unsigned int id;
    int          fd;

    if (wanted_name == NULL || *wanted_name == 0) {
        for (id = 0; id < 32; id++) {
            snprintf(if_name, IFNAMSIZ, "tun%u", id);
            snprintf(path, sizeof path, "/dev/%s", if_name);
            if ((fd = open(path, O_RDWR)) != -1) {
                return fd;
            }
        }
        return -1;
    }
    snprintf(if_name, IFNAMSIZ, "%s", wanted_name);
    snprintf(path, sizeof path, "/dev/%s", wanted_name);

    return open(path, O_RDWR);
}
#else
int tun_create(char if_name[IFNAMSIZ], const char *wanted_name)
{
    char path[64];

    if (wanted_name == NULL) {
        fprintf(stderr,
                "The tunnel device name must be specified on that platform "
                "(try 'tun0')\n");
        errno = EINVAL;
        return -1;
    }
    snprintf(if_name, IFNAMSIZ, "%s", wanted_name);
    snprintf(path, sizeof path, "/dev/%s", wanted_name);

    return open(path, O_RDWR);
}
#endif

int tun_set_mtu(const char *if_name, int mtu)
{
    struct ifreq ifr;
    int          fd;

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        return -1;
    }
    ifr.ifr_mtu = mtu;
    snprintf(ifr.ifr_name, IFNAMSIZ, "%s", if_name);
    if (ioctl(fd, SIOCSIFMTU, &ifr) != 0) {
        close(fd);
        return -1;
    }
    return close(fd);
}
#if defined(_WIN32)
ssize_t tun_read(int fd, void *data, size_t size)
{
    
}

ssize_t tun_write(int fd, const void *data, size_t size)
{
    
}
#elif !defined(__APPLE__) && !defined(__OpenBSD__)
ssize_t tun_read(int fd, void *data, size_t size)
{
    return safe_read_partial(fd, data, size);
}

ssize_t tun_write(int fd, const void *data, size_t size)
{
    return safe_write(fd, data, size, TIMEOUT);
}
#else
ssize_t tun_read(int fd, void *data, size_t size)
{
    ssize_t  ret;
    uint32_t family;

    struct iovec iov[2] = {
        {
            .iov_base = &family,
            .iov_len  = sizeof family,
        },
        {
            .iov_base = data,
            .iov_len  = size,
        },
    };

    ret = readv(fd, iov, 2);
    if (ret <= (ssize_t) 0) {
        return -1;
    }
    if (ret <= (ssize_t) sizeof family) {
        return 0;
    }
    return ret - sizeof family;
}

ssize_t tun_write(int fd, const void *data, size_t size)
{
    uint32_t family;
    ssize_t  ret;

    if (size < 20) {
        return 0;
    }
    switch (*(const uint8_t *) data >> 4) {
    case 4:
        family = htonl(AF_INET);
        break;
    case 6:
        family = htonl(AF_INET6);
        break;
    default:
        errno = EINVAL;
        return -1;
    }
    struct iovec iov[2] = {
        {
            .iov_base = &family,
            .iov_len  = sizeof family,
        },
        {
            .iov_base = (void *) data,
            .iov_len  = size,
        },
    };
    ret = writev(fd, iov, 2);
    if (ret <= (ssize_t) 0) {
        return ret;
    }
    if (ret <= (ssize_t) sizeof family) {
        return 0;
    }
    return ret - sizeof family;
}
#endif

static char *read_from_shell_command(char *result, size_t sizeof_result, const char *command)
{
    FILE *fp;
    char *pnt;

    if ((fp = popen(command, "r")) == NULL) {
        return NULL;
    }
    if (fgets(result, (int) sizeof_result, fp) == NULL) {
        pclose(fp);
        fprintf(stderr, "Command [%s] failed]\n", command);
        return NULL;
    }
    if ((pnt = strrchr(result, '\n')) != NULL) {
        *pnt = 0;
    }
    (void) pclose(fp);

    return *result == 0 ? NULL : result;
}

const char *get_default_gw_ip(void)
{
    static char gw[64];
#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || \
    defined(__DragonFly__) || defined(__NetBSD__)
    return read_from_shell_command(
        gw, sizeof gw, "route -n get default 2>/dev/null|awk '/gateway:/{print $2;exit}'");
#elif defined(__linux__)
    return read_from_shell_command(gw, sizeof gw,
                                   "ip route show default 2>/dev/null|awk '/default/{print $3}'");
/*#elif defined(_WIN)
    return read_from_shell_command(gw, sizeof gw,
                                   "ip route show default 2>/dev/null|awk '/default/{print $3}'"); 
                                   route print 0.0.0.0 | findstr "0.0.0.0" | for /F "tokens=4" %i in ('more') do @echo %i  
                                   */   
#else
    return NULL;
#endif
}

const char *get_default_ext_if_name(void)
{
    static char if_name[64];
#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || \
    defined(__DragonFly__) || defined(__NetBSD__)
    return read_from_shell_command(if_name, sizeof if_name,
                                   "route -n get default 2>/dev/null|awk "
                                   "'/interface:/{print $2;exit}'");
#elif defined(__linux__)
    return read_from_shell_command(if_name, sizeof if_name,
                                   "ip route show default 2>/dev/null|awk '/default/{print $5}'");
/*#elif defined(_WIN)
    return read_from_shell_command(if_name, sizeof if_name,
                                   "ip route show default 2>/dev/null|awk '/default/{print $5}'"); 
                                   route print 0.0.0.0 | findstr "0.0.0.0" | for /F "tokens=4" %i in ('more') do @echo %i //if asi vo win nema nazov 
                                   */                                  
#else
    return NULL;
#endif
}

int tcp_opts(int fd)
{
    int on = 1;

    (void) setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *) &on, sizeof on);
#ifdef TCP_QUICKACK
    (void) setsockopt(fd, IPPROTO_TCP, TCP_QUICKACK, (char *) &on, sizeof on);
#else
    (void) setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *) &on, sizeof on);
#endif
#ifdef TCP_CONGESTION
    (void) setsockopt(fd, IPPROTO_TCP, TCP_CONGESTION, OUTER_CONGESTION_CONTROL_ALG,
                      sizeof OUTER_CONGESTION_CONTROL_ALG - 1);
#endif
#if BUFFERBLOAT_CONTROL && defined(TCP_NOTSENT_LOWAT)
    (void) setsockopt(fd, IPPROTO_TCP, TCP_NOTSENT_LOWAT,
                      (char *) (unsigned int[]){ NOTSENT_LOWAT }, sizeof(unsigned int));
#endif
#ifdef TCP_USER_TIMEOUT
    (void) setsockopt(fd, IPPROTO_TCP, TCP_USER_TIMEOUT, (char *) (unsigned int[]){ TIMEOUT },
                      sizeof(unsigned int));
#endif
#ifdef SO_MARK
    (void) setsockopt(fd, SOL_SOCKET, SO_MARK, (char *) (unsigned int[]){ 42069U },
                      sizeof(unsigned int));
#endif
    return 0;
}

int shell_cmd(const char *substs[][2], const char *args_str, int silent)
{
    char * args[64];
    char   cmdbuf[4096];
    pid_t  child;
    size_t args_i = 0, cmdbuf_i = 0, args_str_i, i;
    int    c, exit_status, is_space = 1;

    errno = ENOSPC;
    for (args_str_i = 0; (c = args_str[args_str_i]) != 0; args_str_i++) {
        if (isspace((unsigned char) c)) {
            if (!is_space) {
                if (cmdbuf_i >= sizeof cmdbuf) {
                    return -1;
                }
                cmdbuf[cmdbuf_i++] = 0;
            }
            is_space = 1;
            continue;
        }
        if (is_space) {
            if (args_i >= sizeof args / sizeof args[0]) {
                return -1;
            }
            args[args_i++] = &cmdbuf[cmdbuf_i];
        }
        is_space = 0;
        for (i = 0; substs[i][0] != NULL; i++) {
            size_t pat_len = strlen(substs[i][0]), sub_len;
            if (!strncmp(substs[i][0], &args_str[args_str_i], pat_len)) {
                sub_len = strlen(substs[i][1]);
                if (sizeof cmdbuf - cmdbuf_i <= sub_len) {
                    return -1;
                }
                memcpy(&cmdbuf[cmdbuf_i], substs[i][1], sub_len);
                args_str_i += pat_len - 1;
                cmdbuf_i += sub_len;
                break;
            }
        }
        if (substs[i][0] == NULL) {
            if (cmdbuf_i >= sizeof cmdbuf) {
                return -1;
            }
            cmdbuf[cmdbuf_i++] = c;
        }
    }
    if (!is_space) {
        if (cmdbuf_i >= sizeof cmdbuf) {
            return -1;
        }
        cmdbuf[cmdbuf_i++] = 0;
    }
    if (args_i >= sizeof args / sizeof args[0] || args_i == 0) {
        return -1;
    }
    args[args_i] = NULL;
    if ((child = fork()) == (pid_t) -1) {
        return -1;
    } else if (child == (pid_t) 0) {
        if (silent) {
            dup2(dup2(open("/dev/null", O_WRONLY), 2), 1);
        }
        execvp(args[0], args);
        _exit(1);
    } else if (waitpid(child, &exit_status, 0) == (pid_t) -1 || !WIFEXITED(exit_status)) {
        return -1;
    }
    return 0;
}

Cmds firewall_rules_cmds(int is_server)
{
    if (is_server) {
#ifdef __linux__
        static const char
            *set_cmds[] =
                { "sysctl net.ipv4.ip_forward=1",
                  "ip addr add $LOCAL_TUN_IP peer $REMOTE_TUN_IP dev $IF_NAME",
                  "ip -6 addr add $LOCAL_TUN_IP6 peer $REMOTE_TUN_IP6/96 dev $IF_NAME",
                  "ip link set dev $IF_NAME up",
                  "iptables -t raw -I PREROUTING ! -i $IF_NAME -d $LOCAL_TUN_IP -m addrtype ! "
                  "--src-type LOCAL -j DROP",
                  "iptables -t nat -A POSTROUTING -o $EXT_IF_NAME -s $REMOTE_TUN_IP -j MASQUERADE",
                  "iptables -t filter -A FORWARD -i $EXT_IF_NAME -o $IF_NAME -m state --state "
                  "RELATED,ESTABLISHED -j ACCEPT",
                  "iptables -t filter -A FORWARD -i $IF_NAME -o $EXT_IF_NAME -j ACCEPT",
                  NULL },
            *unset_cmds[] = {
                "iptables -t nat -D POSTROUTING -o $EXT_IF_NAME -s $REMOTE_TUN_IP -j MASQUERADE",
                "iptables -t filter -D FORWARD -i $EXT_IF_NAME -o $IF_NAME -m state --state "
                "RELATED,ESTABLISHED -j ACCEPT",
                "iptables -t filter -D FORWARD -i $IF_NAME -o $EXT_IF_NAME -j ACCEPT",
                "iptables -t raw -D PREROUTING ! -i $IF_NAME -d $LOCAL_TUN_IP -m addrtype ! "
                "--src-type LOCAL -j DROP",
                NULL
            };
#elif defined(_WIN32)
    static const char
        *set_cmds[] =
            { "reg add HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters /v IPEnableRouter /t REG_DWORD /d 1 /f",
              "netsh interface ipv4 add address $IF_NAME $LOCAL_TUN_IP $REMOTE_TUN_IP",
              "netsh interface ipv6 add address $IF_NAME $LOCAL_TUN_IP6",
              "netsh interface ipv6 add route ::/96 $REMOTE_TUN_IP6 $LOCAL_TUN_IP6",
              "netsh interface set interface $IF_NAME admin=enable",
              "netsh advfirewall firewall add rule name=Drop traffic to local IP dir=in action=block protocol=any localip=$LOCAL_TUN_IP remoteip=!$LOCAL_TUN_IP",
              "netsh interface ipv4 set address $EXT_IF_NAME static $REMOTE_TUN_IP 255.255.255.255 $LOCAL_GATEWAY",
              "netsh routing ip nat add interface $EXT_IF_NAME full",
              "netsh routing ip nat add interface $IF_NAME private",
              "netsh advfirewall firewall add rule name=Allow RELATED and ESTABLISHED traffic from $EXT_IF_NAME to $IF_NAME dir=in action=allow protocol=any localip=any remoteip=me edge=yes enable=yes remoteport=any interfacetype=$EXT_IF_NAME",
              "netsh advfirewall firewall add rule name=Allow all traffic from $IF_NAME to $EXT_IF_NAME dir=out action=allow protocol=any localip=$LOCAL_TUN_IP remoteip=any interfacetype=$IF_NAME",
              "netsh advfirewall firewall add rule name=Allow traffic from $LOCAL_HOST to $REMOTE_TUN_IP dir=out action=allow protocol=any localip=$LOCAL_HOST remoteip=$REMOTE_TUN_IP interfacetype=any",
              "netsh advfirewall firewall add rule name=Allow traffic from $REMOTE_TUN_IP to $LOCAL_HOST dir=in action=allow protocol=any localip=$REMOTE_TUN_IP remoteip=$LOCAL_HOST interfacetype=any",
              NULL },
        *unset_cmds[] = {
            "reg delete HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters /v IPEnableRouter /f",
            "netsh interface ipv4 delete address $IF_NAME $LOCAL_TUN_IP",
            "netsh interface ipv6 delete address $IF_NAME $LOCAL_TUN_IP6",
            "netsh interface ipv6 delete route ::/96 $REMOTE_TUN_IP6 $LOCAL_TUN_IP6",
            "netsh interface set interface $IF_NAME admin=disable",
            "netsh advfirewall firewall delete rule name=Drop traffic to local IP"
            "netsh interface ipv4 delete address $EXT_IF_NAME $REMOTE_TUN_IP",
            "netsh routing ip nat delete interface $EXT_IF_NAME full",
            "netsh routing ip nat delete interface $IF_NAME private",
            "netsh advfirewall firewall delete rule name=Allow RELATED and ESTABLISHED traffic",
            "netsh advfirewall firewall delete rule name=Allow all traffic from interface to external interface",
            "netsh advfirewall firewall delete rule name=Allow traffic from local host to remote IP",
            "netsh advfirewall firewall delete rule name=Allow traffic from remote IP to local host",
            NULL
        };
#elif defined(__APPLE__) || defined(__OpenBSD__) || defined(__FreeBSD__) || \
    defined(__DragonFly__) || defined(__NetBSD__)
        static const char *set_cmds[] =
            { "sysctl -w net.inet.ip.forwarding=1",
              "ifconfig $IF_NAME $LOCAL_TUN_IP $REMOTE_TUN_IP up",
              "ifconfig $IF_NAME inet6 $LOCAL_TUN_IP6 $REMOTE_TUN_IP6 prefixlen 128 up", NULL },
                          *unset_cmds[] = { NULL, NULL };
#else
        static const char *const *set_cmds = NULL, *const *unset_cmds = NULL;
#endif
        return (Cmds){ set_cmds, unset_cmds };
    } else {
#if defined(__APPLE__) || defined(__OpenBSD__) || defined(__FreeBSD__) || \
    defined(__DragonFly__) || defined(__NetBSD__)
        static const char *set_cmds[] =
            { "ifconfig $IF_NAME $LOCAL_TUN_IP $REMOTE_TUN_IP up",
              "ifconfig $IF_NAME inet6 $LOCAL_TUN_IP6 $REMOTE_TUN_IP6 prefixlen 128 up",
#ifndef NO_DEFAULT_ROUTES
              "route add $EXT_IP $EXT_GW_IP",
              "route add 0/1 $REMOTE_TUN_IP",
              "route add 128/1 $REMOTE_TUN_IP",
              "route add -inet6 -blackhole 0000::/1 $REMOTE_TUN_IP6",
              "route add -inet6 -blackhole 8000::/1 $REMOTE_TUN_IP6",
#endif
              NULL },
                          *unset_cmds[] = {
#ifndef NO_DEFAULT_ROUTES
                              "route delete $EXT_IP",
                              "route delete 0/1",
                              "route delete 128/1",
                              "route delete -inet6 0000::/1",
                              "route delete -inet6 8000::/1",
#endif
                              NULL
                          };
#elif defined(__linux__)
        static const char
            *set_cmds[] =
                { "sysctl net.ipv4.tcp_congestion_control=bbr",
                  "ip link set dev $IF_NAME up",
                  "iptables -t raw -I PREROUTING ! -i $IF_NAME -d $LOCAL_TUN_IP -m addrtype ! "
                  "--src-type LOCAL -j DROP",
                  "ip addr add $LOCAL_TUN_IP peer $REMOTE_TUN_IP dev $IF_NAME",
                  "ip -6 addr add $LOCAL_TUN_IP6 peer $REMOTE_TUN_IP6/96 dev $IF_NAME",
#ifndef NO_DEFAULT_ROUTES
                  "ip route add default dev $IF_NAME table 42069",
                  "ip -6 route add default dev $IF_NAME table 42069",
                  "ip rule add not fwmark 42069 table 42069",
                  "ip -6 rule add not fwmark 42069 table 42069",
                  "ip rule add table main suppress_prefixlength 0",
                  "ip -6 rule add table main suppress_prefixlength 0",
#endif
                  NULL },
            *unset_cmds[] = {
#ifndef NO_DEFAULT_ROUTES
                "ip rule delete table 42069",
                "ip -6 rule delete table 42069",
                "ip rule delete table main suppress_prefixlength 0",
                "ip -6 rule delete table main suppress_prefixlength 0",
#endif
                "iptables -t raw -D PREROUTING ! -i $IF_NAME -d $LOCAL_TUN_IP -m addrtype ! "
                "--src-type LOCAL -j DROP",
                NULL
            };
#elif defined(_WIN32)
        static const char
    *set_cmds[] =
        { "reg add HKLM\\SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters /v EnableTCPA /t REG_DWORD /d 0x1 /f",
          "netsh interface set interface $IF_NAME admin=enable",
          "netsh advfirewall firewall add rule name=\"block traffic to LOCAL_TUN_IP\" dir=in action=block protocol=any localip=!$IF_NAME remoteip=$LOCAL_TUN_IP",
          "netsh interface ipv4 add address $IF_NAME $LOCAL_TUN_IP $REMOTE_TUN_IP",
          "netsh interface ipv6 add address $IF_NAME $LOCAL_TUN_IP6/$MASK_LENGTH",
#ifndef NO_DEFAULT_ROUTES
          "route add 0.0.0.0 mask 0.0.0.0 $LOCAL_TUN_IP metric 1 if $IF_NAME",
          "route add ::/0 $LOCAL_TUN_IP6 metric 1 if $IF_NAME",
          "route add 0.0.0.0 mask 0.0.0.0 $LOCAL_TUN_IP metric 1 table 42069",
          "route add ::/0 $LOCAL_TUN_IP6 metric 1 table 42069",
          "netsh interface ipv4 add route $LOCAL_TUN_IP/32 $IF_NAME metric=10 store=persistent",
          "netsh interface ipv6 add route $LOCAL_TUN_IP6/$MASK_LENGTH $IF_NAME metric=10 store=persistent",
#endif
          NULL },
    *unset_cmds[] = {
#ifndef NO_DEFAULT_ROUTES
        "route delete 0.0.0.0 mask 0.0.0.0 $LOCAL_TUN_IP metric 1 if $IF_NAME",
        "route delete ::/0 $LOCAL_TUN_IP6 metric 1 if $IF_NAME",
        "route delete 0.0.0.0 mask 0.0.0.0 $LOCAL_TUN_IP metric 1 table 42069",
        "route delete ::/0 $LOCAL_TUN_IP6 metric 1 table 42069",
        "netsh interface ipv4 delete route $LOCAL_TUN_IP/32 $IF_NAME",
        "netsh interface ipv6 delete route $LOCAL_TUN_IP6/$MASK_LENGTH $IF_NAME",
#endif
        "netsh advfirewall firewall delete rule name=\"block traffic to LOCAL_TUN_IP\"",
        "netsh interface set interface $IF_NAME admin=disable",
        NULL
    };     
#else
        static const char *const *set_cmds = NULL, *const *unset_cmds = NULL;
#endif
        return (Cmds){ set_cmds, unset_cmds };
    }
}
