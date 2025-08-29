#include "malcolm.h"

char *createResponse(struct s_input input) {

  struct ethhdr header;
  ft_memcpy(header.h_dest, input.in_tha, 6);
  ft_memcpy(header.h_source, input.in_sha, 6);
  header.h_proto = htons(ETH_P_ARP);

  struct arp_eth_ipv4 response;
  // fill in response

  return (NULL);
}
/*
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
*/
