#include <iostream>
#include <pcap.h>
#include <net/ethernet.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>

#define EBUF_SIZE 64
#define MAC_SIZE 19

void handler(unsigned char*, const pcap_pkthdr*, const unsigned char*);

using namespace std;

int main(){
    char errbuf[EBUF_SIZE];
    pcap_t *p = pcap_open_offline("hw1.pcap",errbuf);
    if(!p){
        cout <<"packet open failed: " << errbuf << endl;
        return -1;
    }
    if(pcap_loop(p, 10, handler, NULL)<0){
        cout <<"pcap_loop fail: "<< pcap_geterr(p)<< endl;
    }
    return 0;
}

void handler(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {

    const struct ether_header* ethernetHeader;
    const struct ip* ipHeader;
    const struct tcphdr* tcpHeader;
    const struct udphdr* udpHeader;
    char srcMAC[MAC_SIZE];
    char dstMAC[MAC_SIZE];
    char sourceIp[INET_ADDRSTRLEN];
    char destIp[INET_ADDRSTRLEN];
    char timestamp[64], buf[64];
    struct tm *now;

    ethernetHeader = (struct ether_header*)packet;

    if (ntohs(ethernetHeader->ether_type) == ETHERTYPE_IP) {

        /*
        #TODO
        8. The captured length, actual length and the length in the IP header. 
        11. A protocol presented in the IP payload. 12. Identification and Flags in the IP header. 
        13. Identification in decimal and Flags in either DF or MF. 
        */

        ipHeader = (struct ip*)(packet + sizeof(struct ether_header));

        now = localtime(&pkthdr->ts.tv_sec);
        strftime(timestamp, sizeof timestamp, "%H:%M:%S", now);
        snprintf(buf, sizeof buf, "%s.%06ld", timestamp, pkthdr->ts.tv_usec);

        cout << "***********Packet Start***********" <<endl;
        cout << "TimeStamp:" << buf << endl;
        cout << "Length:" << pkthdr->caplen << endl << "Length in IP header:" << ntohs(ipHeader->ip_len) << endl<< endl;
        
        sprintf(dstMAC, " %02x:%02x:%02x:%02x:%02x:%02x", 
        (unsigned char)ethernetHeader->ether_dhost[0],
        (unsigned char)ethernetHeader->ether_dhost[1],
        (unsigned char)ethernetHeader->ether_dhost[2],
        (unsigned char)ethernetHeader->ether_dhost[3],
        (unsigned char)ethernetHeader->ether_dhost[4],
        (unsigned char)ethernetHeader->ether_dhost[5]);
        sprintf(srcMAC, " %02x:%02x:%02x:%02x:%02x:%02x", 
        (unsigned char)ethernetHeader->ether_shost[0],
        (unsigned char)ethernetHeader->ether_shost[1],
        (unsigned char)ethernetHeader->ether_shost[2],
        (unsigned char)ethernetHeader->ether_shost[3],
        (unsigned char)ethernetHeader->ether_shost[4],
        (unsigned char)ethernetHeader->ether_shost[5]);

        cout << "Source MAC:" << srcMAC << endl << "Destination MAC:" << dstMAC << endl << endl;
        
        inet_ntop(AF_INET, &(ipHeader->ip_src), sourceIp, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(ipHeader->ip_dst), destIp, INET_ADDRSTRLEN);

        cout << "Source IP:" << sourceIp << endl << "Destination IP:" << destIp <<  endl << endl;
        switch(ipHeader->ip_p) {
            case IPPROTO_TCP:
                cout << "Protocol:TCP" << endl;
                break;
            case IPPROTO_UDP:
                cout << "Protocol:UDP" << endl;
                break;
            case IPPROTO_ICMP:
                cout << "Protocol:ICMP" << endl;
                break;
            case IPPROTO_IGMP:
                cout << "Protocol:IGMP" << endl;
                break;
            default:
                cout << "Protocol:Unknown" << endl;
                break;
        }
        
        cout << "Identification:" << ntohs(ipHeader->ip_id) <<"(0x" << hex << ntohs(ipHeader->ip_id) << ")" << endl;
        cout << "Flag:0x" << hex << ntohs(ipHeader->ip_off) << endl;
        if(ntohs(ipHeader->ip_off) & IP_DF)
            cout <<"    -DF set" <<endl;
        if(ntohs(ipHeader->ip_off) & IP_MF)
            cout <<"    -MF set" <<endl;
        if(!ntohs(ipHeader->ip_off))
            cout <<"    -Flag not set" <<endl;
        cout << "TTL:" << dec << int(ipHeader->ip_ttl) << endl;
        cout << "-----------------------------------" <<endl << endl;
    }
}