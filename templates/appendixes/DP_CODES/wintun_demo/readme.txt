#Organization: TUKE FEI KEMT 2023
#Feld of study: 9.2.4 Computer engineering
#Study program: Computer Networks, Bachelor Study 
#Author:  Marek Rohac
#Description: Readme to simple Makefile for OS Windows

Program realizuje jednoduchu komunikaciu medzi wintun adapterom. 
V pripade ak ideme vytvorit komunikaciu medzi dvoma zariadeniami, tak treba 
skompilovat jeden program. Nasledne upravit zdrojovy kod a spustat s admin pravami. Upravit treba:

1. cielovu a destinacnu adresu --> tun_writer() --> vid kod
    --> *(ULONG *)&OutgoingPacket[12] = htonl((10 << 24) | (6 << 16) | (7 << 8) | (7 << 0)); /* 10.6.7.6 */ //cielova adresa
    -->*(ULONG *)&OutgoingPacket[16] = htonl((10 << 24) | (6 << 16) | (7 << 8) | (6 << 0)); /* 10.6.7.7 */ //destinacna addresa

2. riadok 138
    --> WINTUN_ADAPTER_HANDLE adapter  = WintunCreateAdapter(L"wtun1", L"Wintun1", NULL); 
        --> zmenit nazvy v uvodzovkach 
            --> napr na WintunCreateAdapter(L"wtun", L"Wintun", NULL);

3. riaadok 150 
    -->AddressRow.Address.Ipv4.sin_addr.S_un.S_addr =  htonl((10 << 24) | (6 << 16) | (7 << 8) | (7 << 0)); 
            --> adresa nami vytvoreneho wintun adapteru 
                --> 10.6.7.7

Compiling --> make all
                --> trreba zmenit aj SOURCES, aby sa neprepisal povodny program
turn on --> demo.exe

CMD prikazy na overenie:

--> route print
    --> vypise zoznam smerovacich pravidiel v os Windows


P.S.: napadlo mi este ze existuje prikaz 
    --> tracert 10.8.7.7
        --> vypisuje hopy az k ceste. 
        --> toto bol moj vystup v pripade tohto 

Tracing route to 10.8.7.6 over a maximum of 30 hops

  1    <1 ms    <1 ms    <1 ms  router.lan [192.168.88.1]
  2     4 ms     3 ms     3 ms  10.1.255.255
  3     *        *        *     Request timed out.
  4     *        *     

        -->overim ako to ide pri dsvpn naa linuxoch 

P.S.: velkost masky ovplyvnuje kolko a ake ruty budu pridane to route table (to co vidime v route print)
    --> cize preto tam su tri tak ako som ukazoval..
    --> ak by bola maska 32 tak sa pridava len jedna routa