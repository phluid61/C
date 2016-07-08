/*
 * hostlookup.c
 *
 * Prints a lot of information about a hostname.
 *
 * Author: Matthew Kerwin <matthew.kerwin@qut.edu.au>
 *
 * Copyright (c) 2012-2016, QUT Library eServices <libsys@qut.edu.au>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#ifndef   NI_MAXHOST
#define   NI_MAXHOST 1025
#endif

char* family(int af)
{
	switch (af) {
	case AF_UNSPEC: return "UNSPEC";
	case AF_LOCAL: return "LOCAL";
/*	case AF_UNIX: return "UNIX";*/
/*	case AF_FILE: return "FILE";*/
	case AF_INET: return "INET";
	case AF_AX25: return "AX25";
	case AF_IPX: return "IPX";
	case AF_APPLETALK: return "APPLETALK";
	case AF_NETROM: return "NETROM";
	case AF_BRIDGE: return "BRIDGE";
	case AF_ATMPVC: return "ATMPVC";
	case AF_X25: return "X25";
	case AF_INET6: return "INET6";
	case AF_ROSE: return "ROSE";
	case AF_DECnet: return "DECnet";
	case AF_NETBEUI: return "NETBEUI";
	case AF_SECURITY: return "SECURITY";
	case AF_KEY: return "KEY";
	case AF_NETLINK: return "NETLINK";
/*	case AF_ROUTE: return "ROUTE";*/
	case AF_PACKET: return "PACKET";
	case AF_ASH: return "ASH";
	case AF_ECONET: return "ECONET";
	case AF_ATMSVC: return "ATMSVC";
	case AF_SNA: return "SNA";
	case AF_IRDA: return "IRDA";
	case AF_PPPOX: return "PPPOX";
	case AF_WANPIPE: return "WANPIPE";
	case AF_BLUETOOTH: return "BLUETOOTH";
	case AF_MAX: return "MAX";
	}
	return "???";
}

char *stype(int st)
{
	switch (st) {
	case SOCK_STREAM: return "SOCK_STREAM";
	case SOCK_DGRAM: return "SOCK_DGRAM";
	case SOCK_RAW: return "SOCK_RAW";
	case SOCK_RDM: return "SOCK_RDM";
	case SOCK_SEQPACKET: return "SOCK_SEQPACKET";
	case SOCK_PACKET: return "SOCK_PACKET";
	}
	return "???";
}

char *sockop(int pf)
{
	switch (pf) {
	case 0: return "SOL_IP";
	case 1: return "SOL_ICMP";
	case 6: return "SOL_TCP";
	case 17: return "SOL_UDP";
	case 41: return "SOL_IPV6";
	case 58: return "SOL_ICMPV6";
	case 255: return "SOL_RAW";
	case 256: return "SOL_IPX";
/* etc. */
	case 263: return "SOL_PACKET";
	}
	return "???";
}

void printflags(unsigned int f)
{
	char c = '[';
	if (!f) { printf("%c", c); }
	if (f & AI_PASSIVE) { printf("%cAI_PASSIVE", c); c='|'; }
	if (f & AI_CANONNAME) { printf("%cAI_CANONNAME", c); c='|'; }
	if (f & AI_NUMERICHOST) { printf("%cAI_NUMERICHOST", c); c='|'; }
	if (f & AI_V4MAPPED) { printf("%cAI_V4MAPPED", c); c='|'; }
	if (f & AI_ALL) { printf("%cAI_ALL", c); c='|'; }
	if (f & AI_ADDRCONFIG) { printf("%cAI_ADDRCONFIG", c); c='|'; }
#ifdef __USE_GNU
	if (f & AI_IDN) { printf("%cAI_IDN", c); c='|'; }
	if (f & AI_CANONIDN) { printf("%cAI_CANONIDN", c); c='|'; }
	if (f & AI_IDN_ALLOW_UNASSIGNED) { printf("%cAI_IDN_ALLOW_UNASSIGNED", c); c='|'; }
	if (f & AI_IDN_USE_STD3_ASCII_RULES) { printf("%cAI_IDN_USE_STD3_ASCII_RULES", c); c='|'; }
#endif
	if (f & AI_NUMERICSERV) { printf("%cAI_NUMERICSERV", c); c='|'; }
	printf("]\n");
}

char *myerr(int e)
{
	switch (e) {
	case HOST_NOT_FOUND: return "HOST_NOT_FOUND";
	case NO_ADDRESS: return "NO_ADDRESS";
/*	case NO_DATA: return "NO_DATA";*/
	case TRY_AGAIN: return "TRY_AGAIN";
	default: return "???";
	}
}

int main(int argc, char *argv[])
{
	struct addrinfo  hints;
	struct addrinfo *result;
	struct addrinfo *res;
	struct sockaddr_in *in;
	struct hostent *host;
	struct in_addr *ad;
	int at;
	int error;
	int i;
	char hostname[NI_MAXHOST];
	char **alias;

	for (i = 1; i < argc; i++) {
		printf("***\n*** %s\n***\n\n", argv[i]);

		/* super awesome hack bananas */
		host = gethostbyname(argv[i]);
		if (!host) {
			fprintf(stderr, "error in gethostbyname: %s\n", myerr(h_errno));
		} else {
			printf("gethostbyname()\n  hostname: %s\n", host->h_name);
			for (alias = host->h_aliases; alias && *alias; alias++) {
				printf("  aka: %s\n", *alias);
			}
		}

		/* resolve the domain name into a list of addresses */
		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = AF_UNSPEC; /* AF_INET or AF_INET6 */
#if 0
		hints.ai_socktype = 0; /* SOCK_STREAM or SOCK_DGRAM */
#endif
		hints.ai_flags = AI_CANONNAME|AI_V4MAPPED;
#if 0
		hints.ai_protocol = 0; /* any protocol */
		hints.ai_canonname = NULL;
		hints.ai_addr = NULL;
		hints.ai_next = NULL;
#endif
		error = getaddrinfo(argv[i], NULL, &hints, &result);
		if (error != 0) {
			fprintf(stderr, "error in getaddrinfo: %s\n", gai_strerror(error));
			return EXIT_FAILURE;
		}

		printf("getaddrinfo()\n");

		/* loop over all returned results and do inverse lookup */
		for (res = result; res != NULL; res = res->ai_next) {
			ad = NULL;

			printf(">\n  ai_flags = 0x%X ", res->ai_flags);
			printflags(res->ai_flags);
			printf("  ai_family = %d [AF_%s]\n  ai_socktype = %d [%s]\n  ai_protocol = %d [%s]\n", res->ai_family, family(res->ai_family), res->ai_socktype, stype(res->ai_socktype), res->ai_protocol, sockop(res->ai_protocol));
			if (res->ai_canonname && *(res->ai_canonname)) {
				printf("  ai_canonname = \"%s\"\n", res->ai_canonname);
			} else {
				printf("  ai_canonname = NULL\n");
			}
			printf("  ai_addr = {\n");
			if (res->ai_addrlen == sizeof(struct sockaddr_in)) {
				in = (struct sockaddr_in*)(res->ai_addr);
				printf("    sin_family = %d [AF_%s]\n    sin_port = %d\n    sin_addr = {\n", in->sin_family, family(in->sin_family), in->sin_port);
				if (sizeof(in->sin_addr) == sizeof(struct in_addr)) {
					ad = (struct in_addr*)&(in->sin_addr);
					at = in->sin_family;
					printf("      s_addr = 0x%08X (%d.%d.%d.%d)\n", ad->s_addr, ad->s_addr & 0xff, (ad->s_addr >> 8) & 0xff, (ad->s_addr >> 16) & 0xff, ad->s_addr >> 24);
				} else {
					printf("      ??? not an in_addr ???\n");
				}
				printf("    }\n");
			} else {
				printf("    ??? not a sockaddr_in ???\n");
			}
			printf("  }\n");

			/* use new getnameinfo */
			memset((void*)hostname, 0, NI_MAXHOST);
			error = getnameinfo(res->ai_addr, res->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, 0);
			if (error != 0) {
				fprintf(stderr, "error in getnameinfo: %s\n", gai_strerror(error));
			}
			if (*hostname)
				printf("  getnameinfo(ai_addr)\n    hostname: %s\n", hostname);

			/* use old gethostbyaddr */
			if (ad) {
				host = gethostbyaddr(ad, sizeof(ad), at);
				if (!host) {
					fprintf(stderr, "error in gethostbyaddr: %s\n", myerr(h_errno));
				} else {
					printf("  gethostbyaddr(ai_addr->sin_addr)\n    hostname: %s\n", host->h_name);
					for (alias = host->h_aliases; alias && *alias; alias++) {
						printf("  aka: %s\n", *alias);
					}
				}
			}

		}

		freeaddrinfo(result);
		printf("\n");
	}
	return EXIT_SUCCESS;
}
