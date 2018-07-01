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
 * original author. 
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
#include "sock_config.h"


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
    return 0;
}


