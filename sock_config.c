/*
 * =====================================================================================
 *
 *       Filename:  sock_config.c
 *
 *    Description:  Configure the shared parts of the attacks
 *
 *        Version:  1.0
 *        Created:  07/01/2018 12:05:36 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dr. Fritz Mehner (mn), mehner@fh-swf.de
 *        Company:  FH Südwestfalen, Iserlohn
 *
 * =====================================================================================
 */
#include "sock_config.h"

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

int check_args(char* hostname, char* message, int port, char* source_ip, char* attack_type)
{
    if (!*hostname) {
        printf ("usage: -p port -h hostname -m message\n");
        return -1;
    }
    if (!*message && !strncmp(attack_type, "udp_spam", 8)) {
        printf ("usage: -p port -h hostname -m message\n");
        return -1;
    }
    if (!port) {
        printf ("usage: -p port -h hostname -m message\n");
        return -1;
    }
    if (!*source_ip && !strncmp(attack_type, "syn_flood", 9)) {
        printf ("usage: -p port -h hostname -s source_ip\n");
        return -1;
    }
    return 0;
}
