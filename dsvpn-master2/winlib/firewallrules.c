#ifdef _WIN32
    static const char
        *set_cmds[] =
            { "powershell -Command Set-NetIPInterface -InterfaceAlias $IF_NAME -Forwarding Enabled",
              "netsh interface ipv4 add address $IF_NAME $LOCAL_TUN_IP $LOCAL_TUN_MASK",
              "netsh interface ipv6 add address $IF_NAME $LOCAL_TUN_IP6/$LOCAL_TUN_MASK6",
              "netsh interface set interface $IF_NAME admin=ENABLED",
              "netsh advfirewall firewall add rule name=\"Block $LOCAL_TUN_IP\" dir=in action=block "
              "remoteip=$LOCAL_TUN_IP protocol=any",
              "netsh interface ipv4 set interface $EXT_IF_NAME forwarding=enable",
              "netsh advfirewall firewall add rule name=\"Allow VPN Traffic In\" dir=in action=allow "
              "protocol=any enable=yes remoteip=$REMOTE_TUN_IP profile=private",
              "netsh advfirewall firewall add rule name=\"Allow VPN Traffic Out\" dir=out action=allow "
              "protocol=any enable=yes remoteip=$REMOTE_TUN_IP profile=private",
              NULL },
        *unset_cmds[] = {
            "netsh advfirewall firewall delete rule name=\"Allow VPN Traffic Out\"",
            "netsh advfirewall firewall delete rule name=\"Allow VPN Traffic In\"",
            "netsh interface ipv4 set interface $EXT_IF_NAME forwarding=disable",
            "netsh advfirewall firewall delete rule name=\"Block $LOCAL_TUN_IP\"",
            NULL
        };
#endif
//client
set_cmds = (
    "netsh interface ipv4 set subinterface \"{IF_NAME}\" admin=up",
    "netsh interface ipv4 add address \"{IF_NAME}\" {LOCAL_TUN_IP} {REMOTE_TUN_IP}",
    "netsh interface ipv6 add address \"{IF_NAME}\" {LOCAL_TUN_IP6}/{PREFIX_LENGTH} {REMOTE_TUN_IP6}",
    "netsh interface ipv4 set interface \"{IF_NAME}\" metric=10",
    "route -p add 0.0.0.0 mask 0.0.0.0 {LOCAL_TUN_IP} metric 20 if {IF_NAME}",
    "route -p add ::/0 {REMOTE_TUN_IP6} metric 20 if {IF_NAME}",
    "route -p add {REMOTE_NETWORK_IPV4}/{REMOTE_PREFIX_LENGTH} {REMOTE_TUN_IP} metric 20 if {IF_NAME}",
    "route -p add {REMOTE_NETWORK_IPV6}/{REMOTE_PREFIX_LENGTH} {REMOTE_TUN_IP6} metric 20 if {IF_NAME}",
    "netsh interface ipv4 add route {LOCAL_NETWORK_IPV4}/{LOCAL_PREFIX_LENGTH} \"{IF_NAME}\" {LOCAL_TUN_IP} metric 20",
    "netsh interface ipv6 add route {LOCAL_NETWORK_IPV6}/{LOCAL_PREFIX_LENGTH} \"{IF_NAME}\" {LOCAL_TUN_IP6} metric 20",
    "netsh interface ipv4 set interface \"{IF_NAME}\" forwarding=enabled",
    "netsh interface ipv4 set interface \"{IF_NAME}\" neighbor=enabled",
)

unset_cmds = (
    "netsh interface ipv4 delete route {LOCAL_NETWORK_IPV4}/{LOCAL_PREFIX_LENGTH} \"{IF_NAME}\" {LOCAL_TUN_IP}",
    "netsh interface ipv6 delete route {LOCAL_NETWORK_IPV6}/{LOCAL_PREFIX_LENGTH} \"{IF_NAME}\" {LOCAL_TUN_IP6}",
    "route -p delete 0.0.0.0 mask 0.0.0.0 {LOCAL_TUN_IP}",
    "route -p delete ::/0 {REMOTE_TUN_IP6}",
    "route -p delete {REMOTE_NETWORK_IPV4}/{REMOTE_PREFIX_LENGTH} {REMOTE_TUN_IP}",
    "route -p delete {REMOTE_NETWORK_IPV6}/{REMOTE_PREFIX_LENGTH} {REMOTE_TUN_IP6}",
    "netsh interface ipv4 set interface \"{IF_NAME}\" forwarding=disabled",
    "netsh interface ipv4 set interface \"{IF_NAME}\" neighbor=disabled",
)
