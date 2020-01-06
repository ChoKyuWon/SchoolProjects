#include <iostream>
#include <pcap.h>
#include <net/ethernet.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

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
    if(pcap_loop(p, 100, handler, NULL)<0){
        cout <<"pcap_loop fail: "<< pcap_geterr(p)<< endl;
    }
    return 0;
}

void handler(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    const struct ether_header* ethernetHeader;
    const struct ip* ipHeader;
    const struct tcphdr* tcpHeader;
    char timestamp[64], buf[64];
    struct tm *now;
    uint payloadSize;

    ethernetHeader = (struct ether_header*)packet;

    if (ntohs(ethernetHeader->ether_type) == ETHERTYPE_IP) {

        ipHeader = (struct ip*)(packet + sizeof(struct ether_header));

        now = localtime(&pkthdr->ts.tv_sec);
        strftime(timestamp, sizeof timestamp, "%H:%M:%S", now);
        snprintf(buf, sizeof buf, "%s.%06ld", timestamp, pkthdr->ts.tv_usec);
        
        /*
        11. Starting sequence number and ending sequence number if the TCP payload
        exist
        12. Acknowledgement number
        13. TCP payload size
        14. TCP header size
        15. Advertising window size
        16. TCP segment type (S F R F A U)
        17. TCP options if exists
        18. Total length
        19. Checksum value
        */
        switch(ipHeader->ip_p) {
            case IPPROTO_TCP:
                cout << "***********Packet Start***********" <<endl;
                cout << "Localtime: " << buf << endl;
                cout << "Length:" << dec << pkthdr->caplen << endl << "Length in IP header:" << ntohs(ipHeader->ip_len) << endl<< endl;

                tcpHeader = (tcphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip));
                payloadSize = ntohs(ipHeader->ip_len) - (ipHeader->ip_hl + tcpHeader->doff)*4;
                cout << "Source Port: " << ntohs(tcpHeader->source) << ", Destination Port: " << ntohs(tcpHeader->dest) << endl;
                cout << "Sequence Number: 0x" << hex << ntohl(tcpHeader->seq) << endl;
                if(payloadSize > 0)
                    cout << "Ending Sequence Number: 0x" << hex << ntohl(tcpHeader->seq) + payloadSize -1 << endl;
                cout << "ACK Number: 0x" << hex << ntohl(tcpHeader->ack_seq) << endl;
                cout << "Payload Size: " << dec << payloadSize << endl;
                cout << "TCP Header size: " << tcpHeader->doff * 4 << endl;
                cout << "Window Size: " << ntohs(tcpHeader->window) << endl;
                cout << "Segment Type: " ;

                if(tcpHeader->fin)
                    cout << "F ";
                if(tcpHeader->syn)
                    cout << "S ";
                if(tcpHeader->rst)
                    cout << "R " ;
                if(tcpHeader->psh)
                    cout << "P ";
                if(tcpHeader->ack)
                    cout << "A ";
                if(tcpHeader->urg)
                    cout << "U";
                cout << endl;
                if(tcpHeader->doff > 5){
                    cout << "Option Exists." << endl;
                    //Option Parsing
                }
                cout << "Checksum: 0x" << hex << ntohs(tcpHeader->check) << endl;
                cout << "-----------------------------------" <<endl << endl;
                break;
            /*
            case IPPROTO_UDP:
                //cout << "Protocol:UDP" << endl;
                break;
            case IPPROTO_ICMP:
                //cout << "Protocol:ICMP" << endl;
                break;
            case IPPROTO_IGMP:
                //cout << "Protocol:IGMP" << endl;
                break;
                */
            default:
                //cout << "Protocol:Unknown" << endl;
                break;
        }
    }
}