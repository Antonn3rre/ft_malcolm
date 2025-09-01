#include "malcolm.h"
#include <stdio.h>

int parse_packet(char *buffer, struct s_input input) {

  struct arp_eth_ipv4 *arp_resp =
      (struct arp_eth_ipv4 *)(buffer + sizeof(struct ethhdr));

  if (input.verbose)
    printf("\e[34m[ Parsing the packet received ]\n\e[0m");

  printf("\e[34m\tmac address of request = %02x:%02x:%02x:%02x:%02x:%02x\n",
         arp_resp->ar_sha[0], arp_resp->ar_sha[1], arp_resp->ar_sha[2],
         arp_resp->ar_sha[3], arp_resp->ar_sha[4], arp_resp->ar_sha[5]);
  printf("\tip address of request = %d.%d.%d.%d\e[0m\n", arp_resp->ar_sip[0],
         arp_resp->ar_sip[1], arp_resp->ar_sip[2], arp_resp->ar_sip[3]);
  if (input.verbose)
    printf("\e[34m\tarp sent to ip address (our sender ip) = %d.%d.%d.%d\e[0m\n",
           arp_resp->ar_tip[0], arp_resp->ar_tip[1], arp_resp->ar_tip[2],
           arp_resp->ar_tip[3]);

  for (int i = 0; i < 4; i++) {
    if (arp_resp->ar_sip[i] !=
        input.in_tip[i]) { // Comp the sender with my target
      if (input.verbose)
        printf("\e[33mThe ARP request sender IP is not my target's\e[0m\n");
      return (0);
    }
  }
  if (input.verbose)
    printf("\e[32mThe ARP request sender IP matches my target IP\e[0m\n");
  for (int i = 0; i < 6; i++) {
    if (arp_resp->ar_sha[i] != input.in_tha[i]) {
      if (input.verbose)
        printf("\e[33mThe ARP request sender MAC does not match my target's\e[0m\n");
      return (0);
    }
  }
  if (input.verbose)
    printf("\e[32mThe ARP request sender MAC matches my target MAC\e[0m\n");
  for (int i = 0; i < 4; i++) {
    if (arp_resp->ar_tip[i] != input.in_sip[i]) {
      if (input.verbose)
        printf("\e[33mThe ARP request target IP does not match my sender's\e[0m\n");
      return (0);
    }
  }
  if (input.verbose)
    printf("\e[32mThe ARP request target matches my sender IP\e[0m\n");

  return (1);
}
