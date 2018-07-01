/*
 * =====================================================================================
 *
 *       Filename:  udp_spam.h
 *
 *    Description:  H file for udp_spam
 *
 *        Version:  1.0
 *        Created:  07/01/2018 12:16:57 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dr. Fritz Mehner (mn), mehner@fh-swf.de
 *        Company:  FH Südwestfalen, Iserlohn
 *
 * =====================================================================================
 */
#include <sys/socket.h>
#include <netinet/in.h>

int udp_send(const char *msg, int fd, struct sockaddr_in servaddr);
void udp_spam(char* hostname, int port, char* message);

