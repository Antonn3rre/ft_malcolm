#ifndef MALCOLM_H
#define MALCOLM_H

#include "stdio.h"
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

struct s_input {
  unsigned char in_sha[6];
  unsigned char in_sip[4];
  unsigned char in_tha[6];
  unsigned char in_tip[4];
};

struct arp_eth_ipv4 {

  // Former elements of arphdr
  unsigned short int ar_hrd; // Format of hardware address
  unsigned short int ar_pro; // Format of protocol address
  unsigned char ar_hln;      // Length of hardware address
  unsigned char ar_pln;      // Length of protocol address
  unsigned short int ar_op;  // ARP opcode

  // Aditionnal element to parse
  unsigned char ar_sha[ETH_ALEN]; // Sender MAC address
  unsigned char ar_sip[4];        // Sender IP address
  unsigned char ar_tha[ETH_ALEN]; // Target MAC address
  unsigned char ar_tip[4];        // Target IP address
};

// checkInput.c
int check_mac(char *str, unsigned char tab[6]);
int check_ip(char *str, unsigned char tab[4]);
int check_args(char **argv, struct s_input *input);

// utils.c
size_t ft_strlen(const char *s);
int ft_isalnum(int c);
int strict_atoi(const char *nptr, int *i);
void *ft_memcpy(void *dest, const void *src, size_t n);

// parsePacket
int parse_packet(char *buffer, struct s_input input);

// createResponse
char *createResponse(struct s_input input);

#endif // !MALCOLM_H
