/*
 * =====================================================================================
 *
 *       Filename:  sock_config.h
 *
 *    Description:  Shared file h file
 *
 *        Version:  1.0
 *        Created:  07/01/2018 12:09:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dr. Fritz Mehner (mn), mehner@fh-swf.de
 *        Company:  FH Südwestfalen, Iserlohn
 *
 * =====================================================================================
 */
#ifndef SOCK_CONFIG_H_
#define SOCK_CONFIG_H_

#include <sys/socket.h>

int get_sock_fd();

struct sockaddr_in configure_sock(int port, char *hostname);
int check_args(char* hostname, char* message, int port, char* source_ip, char* attack_type);

#endif //SOCK_CONFIG_H_
