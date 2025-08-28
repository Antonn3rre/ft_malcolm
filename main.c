#include "malcolm.h"

int g_signal = 0;

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
    // ecouter les signaux
    if (g_signal == 1)
      break;
  }

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
