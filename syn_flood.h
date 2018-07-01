/*
 * =====================================================================================
 *
 *       Filename:  syn_flood.h
 *
 *    Description:  H file for syn_flood
 *
 *        Version:  1.0
 *        Created:  07/01/2018 12:17:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dr. Fritz Mehner (mn), mehner@fh-swf.de
 *        Company:  FH Südwestfalen, Iserlohn
 *
 * =====================================================================================
 */

/* * * * * * * * * * * * * * * * * * * * * * * * * * *
 * header for the checksum
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

unsigned short csum(unsigned short *ptr,int nbytes);
struct iphdr* configure_packet(char* source_ip, char* datagram, struct sockaddr_in sin);
int tcp_send(char *datagram, int sfd, struct sockaddr_in servaddr, struct iphdr* iph);
int check_args(char* hostname, int port, char* source_ip);//might move to shared h

