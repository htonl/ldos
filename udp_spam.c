/*
 * =====================================================================================
 *
 *       Filename:  udp_spam.c
 *
 *    Description:  A traffic injection tool for testing. This tool SPAMS UDP to a 
 *                  specified host on a specified port
 *                  usage:
 *                          ./udp_spam -p <port> -h <hostname> -m <message>
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
 * original author. Do not use this for anything illegal.
 */

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Opens a socket for UDP
 * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
int get_sock_fd()
{
    int fd = socket(AF_INET,SOCK_DGRAM,0);

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
 * Sends the UDP packet
 * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
int udp_send(const char *msg, int fd, struct sockaddr_in servaddr)
{
    if (sendto(fd, msg, strlen(msg), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
        perror("cannot send message");
        return -1;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Parse Args and start the loop
 * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
int main(int argc, char* argv[])
{
    int port = 0, n = 1, fd;
    struct sockaddr_in servaddr;
    char hostname[1024];
    char  message[1024];
    short h;
    short m;

    //initialize the strings
    for (int i = 0; i <= 1024; i++) {
        hostname[i] = 0;
        message[i] = 0;
    }
    if (argc != 7) {
        printf ("usage: -p port -h hostname -m message0\n");
        return -1;
    }
    //Arg parsing
    for (int i = 0; i < argc; i++)
    {
        if (!strcmp(argv[i],"-p")) {
            port = atoi(argv[i + 1]);
        }
        else if (!strcmp(argv[i],"-h")) {
            for (int c = 0; c <= strlen(argv[i+1]); c++) {
                    hostname[c] = argv[i + 1][c];
            }
        }
        else if (!strcmp(argv[i], "-m")) {
            for (int c = 0; c <= strlen(argv[i+1]); c++) {
                    message[c] = argv[i + 1][c];
            }
        }
    }
    if (!*hostname) {
        printf ("usage: -p port -h hostname -m message1\n");
        return -1;
    }
    if (!*message) {
        printf ("usage: -p port -h hostname -m message2\n");
        return -1;
    }
    if (!port) {
        printf("hostname: %s message: %s\n",hostname,message);
        printf ("usage: -p port -h hostname -m message3 port: %d\n",port);
        return -1;
    }
    printf("Starting...\n");
    printf("Port: %d, Host: %s, Message: %s\n",port,hostname,message);
    
    //create the UDP socket
    fd = get_sock_fd();
    //configure the socket
    servaddr = configure_sock(port, hostname);
    //start spamming
    while(1) {
        udp_send(message, fd, servaddr);
        if (n++ == 1000000) {
            printf("1000000 more packets\n");
            n=0;
        }
    }
    close(fd);
    return 0;
}