#include "malcolm.h"
#include <linux/if_ether.h>

int g_signal = 0;

struct arp_eth_ipv4 {
  unsigned short int ar_hrd;
  unsigned short int ar_pro;
  unsigned char ar_hln;
  unsigned char ar_pln;
  unsigned short int ar_op;
  unsigned char ar_sha[ETH_ALEN]; // Sender hardware address
  unsigned char ar_sip[4];        // Sender IP address
  unsigned char ar_tha[ETH_ALEN]; // Target hardware address
  unsigned char ar_tip[4];        // Target IP address
};

void ctrlC(int signum) {
  (void)signum;
  exit(1); // quitte bien mais fd leak
  g_signal = 1;
}

int main(int argc, char **argv) {

  if (argc != 5) {
    printf("Wrong number of arguments\n");
    return (0);
  }

  if (!check_args(argv))
    return (0);

  signal(SIGINT, ctrlC);

  int sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
  if (sockfd == -1) {
    printf("Error opening socket\n");
    return (0);
  }

  // associer socket a interface --> necessaire pour envoyer reponse

  int numbytes = 0;
  char buffer[1000];
  while (1) {
    numbytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
    if (numbytes < 0) {
      perror("recvfrom");
      break;
    }
    printf("Reçu un paquet ARP de %d octets\n", numbytes);
    printf("\n\nDetail:\n\n%s\n", buffer);
    break; // pour tester deja 1 request
    // ecouter les signaux
    if (g_signal == 1)
      break;
  }

  struct ethhdr *rcv_resp = (struct ethhdr *)buffer;
  struct arp_eth_ipv4 *arp_resp =
      (struct arp_eth_ipv4 *)(buffer + sizeof(struct ethhdr));

  if (ntohs(rcv_resp->h_proto) != ETH_P_ARP) { // pas certain d'en avoir besoin
    printf("Soucis h h_proto\n");
    return (0);
  }

  printf("arp sender ip address = %d.%d.%d.%d\n", arp_resp->ar_sip[0],
         arp_resp->ar_sip[1], arp_resp->ar_sip[2], arp_resp->ar_sip[3]);
  printf("arp target ip address = %d.%d.%d.%d\n", arp_resp->ar_tip[0],
         arp_resp->ar_tip[1], arp_resp->ar_tip[2], arp_resp->ar_tip[3]);

  /*
struct in_addr sender_a;
memset(&sender_a, 0, sizeof(struct in_addr));
memcpy(&sender_a.s_addr, arp_resp->sender_ip, sizeof(uint32_t));
*/
  close(sockfd);
  return (0);
}

// socket
// AF_INET if you want to communicate with TCP or UDP
// PF_PACKET if you want to send and receive at the most basic level
//
// SOCK_RAW : trames brutes (sans traitement)
//
// ETH_P_ARP : macro, pour ne recevoir que celles dont EtherType = ARP
