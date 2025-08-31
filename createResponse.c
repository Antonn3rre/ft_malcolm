#include "malcolm.h"

unsigned char *createResponse(struct s_input input) {

  unsigned char buffer[42]; // 14 (Ethernet) + 28 (ARP IPv4)
  struct ethhdr *header = (struct ethhdr *)buffer;
  ft_memcpy(header->h_dest, input.in_tha, 6);
  ft_memcpy(header->h_source, input.in_sha, 6);
  header->h_proto = htons(ETH_P_ARP);

  struct arp_eth_ipv4 *response =
      (struct arp_eth_ipv4 *)(buffer + sizeof(struct ethhdr));
  // fill in response
  response->ar_hrd = htons(ARPHRD_ETHER); // MAC Ethernet
  response->ar_pro = htons(ETH_P_IP);
  response->ar_hln = 6;
  response->ar_pln = 4;
  response->ar_op = 2; // Means that it's a response

  ft_memcpy(response->ar_sha, input.in_sha, 6);
  ft_memcpy(response->ar_sip, input.in_sip, 4);
  ft_memcpy(response->ar_tha, input.in_tha, 6);
  ft_memcpy(response->ar_tip, input.in_tip, 4);

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
