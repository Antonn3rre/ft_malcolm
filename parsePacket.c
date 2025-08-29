#include "malcolm.h"

int parse_packet(char *buffer, struct s_input input) {

  struct ethhdr *rcv_resp = (struct ethhdr *)buffer;
  struct arp_eth_ipv4 *arp_resp =
      (struct arp_eth_ipv4 *)(buffer + sizeof(struct ethhdr));

  if (ntohs(rcv_resp->h_proto) != ETH_P_ARP) { // pas certain d'en avoir besoin
    printf("Soucis h h_proto\n");
    return (0);
  }

  printf("arp sender ip address = %d.%d.%d.%d\n", arp_resp->ar_sip[0],
         arp_resp->ar_sip[1], arp_resp->ar_sip[2], arp_resp->ar_sip[3]);
  printf("arp sent to ip address = %d.%d.%d.%d\n", arp_resp->ar_tip[0],
         arp_resp->ar_tip[1], arp_resp->ar_tip[2], arp_resp->ar_tip[3]);
  printf("arp sender mac = %02x.%02x.%02x.%02x.%02x.%02x\n",
         arp_resp->ar_sha[0], arp_resp->ar_sha[1], arp_resp->ar_sha[2],
         arp_resp->ar_sha[3], arp_resp->ar_sha[4], arp_resp->ar_sha[5]);

  for (int i = 0; i < 4; i++) {
    //	  printf("IP comp: %d et %d\n", arp_resp->ar_sip[i], input.in_sip[i]);
    if (arp_resp->ar_sip[i] !=
        input.in_tip[i]) // Comp the sender with my target
      return (0);
  }
  printf("Target IP ok\n");
  for (int i = 0; i < 6; i++) {
    if (arp_resp->ar_sha[i] != input.in_tha[i])
      return (0);
  }
  printf("Target MAC ok\n");
  for (int i = 0; i < 4; i++) {
    //	  printf("IP comp: %d et %d\n", arp_resp->ar_sip[i], input.in_sip[i]);
    if (arp_resp->ar_tip[i] != input.in_sip[i])
      return (0);
  }
  printf("Sender IP ok\n");

  return (1);
}
