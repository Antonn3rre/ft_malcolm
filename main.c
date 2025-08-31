#include "malcolm.h"

int g_signal = 0;

void ctrlC(int signum) {
  (void)signum;
  exit(1); // quitte bien mais fd leak
           // TODO: changer le exit
  g_signal = 1;
}

int main(int argc, char **argv) {

  if (argc != 5) {
    printf("Wrong number of arguments\n");
    return (0);
  }

  struct s_input input;
  if (!check_args(argv, &input))
    return (0);
  /*
  // Print args
    printf("arp sender ip address = %d.%d.%d.%d\n", input.in_sip[0],
           input.in_sip[1], input.in_sip[2], input.in_sip[3]);
    printf("arp target ip address = %d.%d.%d.%d\n", input.in_tip[0],
           input.in_tip[1], input.in_tip[2], input.in_tip[3]);
    printf("arp sender mac = %02x.%02x.%02x.%02x.%02x.%02x\n", input.in_sha[0],
           input.in_sha[1], input.in_sha[2], input.in_sha[3], input.in_sha[4],
           input.in_sha[5]);
    printf("arp target mac = %02x.%02x.%02x.%02x.%02x.%02x\n", input.in_tha[0],
           input.in_tha[1], input.in_tha[2], input.in_tha[3], input.in_tha[4],
           input.in_tha[5]);
  */
  signal(SIGINT, ctrlC);

  int sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
  if (sockfd == -1) {
    printf("Error opening socket\n");
    return (0);
  }

  // TODO: associer socket a interface --> necessaire pour envoyer reponse (ou
  // pas ?)

  int numbytes = 0;
  char buffer[1000];
  while (1) {
    numbytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
    if (numbytes < 0) {
      perror("recvfrom");
      break;
    }
    printf("Reçu un paquet ARP de %d octets\n", numbytes);
    if (parse_packet(buffer, input))
      // Break if the request come from the rigth sender
      break;
    if (g_signal == 1)
      break;
  }

  // Create ARP response
  unsigned char responseBuf[42];
  createResponse(input, responseBuf);

  // Send ARP response
  struct sockaddr_ll addr; // Structure attendue au niveau trames Ethernet
  addr.sll_family = AF_PACKET;
  addr.sll_protocol = htons(ETH_P_ARP);
  addr.sll_ifindex = if_nametoindex("enp0s3");
  addr.sll_halen = 6;
  ft_memcpy(addr.sll_addr, input.in_tha, 6);

  if (sendto(sockfd, responseBuf,
             sizeof(struct ethhdr) + sizeof(struct arp_eth_ipv4), 0,
             (struct sockaddr *)&addr, sizeof(struct sockaddr_ll)) == -1)
    printf("Error sendto\n");
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
