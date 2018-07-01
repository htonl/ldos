This software was designed for test purposes. It is the responsibility of this
software's operator to use this in a responsible fashion. The creators recommend
that the operator not break the law.

This software can implement a TCP SYN flood attack. More info on this attack can
be found in RFC 4987 - http://www.ietf.org/rfc/rfc4987

This software can implement a UDP packet flood (big or small packet sizes).
More information on these types of attack can be found here - 
https://www.incapsula.com/ddos/attack-glossary/ip-fragmentation-attack-teardrop


TODO:
 * main, in ldos, probably does more than it should
 * declare variables in the syn_flood and udp_spam funcs
 * Organize includes
 * Fix check_args -- the algorithm is currently wrong
 * Makefile
 * build the damn thing

We are almost there, another hour or two of work for this.


