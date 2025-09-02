#include "malcolm.h"
#include <sys/socket.h>
#include <unistd.h>

int checkHostname(char *str, unsigned char tab[4]) {

	struct addrinfo hints, *res = NULL;
	ft_memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;

	if (getaddrinfo(str, NULL, &hints, &res) != 0)
		return (0);  // If hostname does not exists
	struct sockaddr_in *addr = (struct sockaddr_in *)res->ai_addr;
	ft_memcpy(tab, &addr->sin_addr, 4);
	freeaddrinfo(res);
  return (1);
}

int check_ip(char *str, unsigned char tab[4]) {
  int i = 0;
  int numOfNum = 0;
  int value = 0;

  while (str[i] && numOfNum < 4) {
    value = strict_atoi(str, &i);
    if (value == -1)
      return (checkHostname(str, tab));
    else
      tab[numOfNum] = value;
    numOfNum++;
    if (!str[i])
      break;
    if (str[i] != '.')
      return (checkHostname(str, tab));
    i++;
  }
  if (numOfNum != 4)
    return (checkHostname(str, tab));
  return (1);
}

int hex_char_to_val(char c) {
  if (c >= '0' && c <= '9')
    return c - '0';
  if (c >= 'a' && c <= 'f')
    return 10 + (c - 'a');
  if (c >= 'A' && c <= 'F')
    return 10 + (c - 'A');
  return -1;
}

int check_mac(char *str, unsigned char tab[6]) {
  if (ft_strlen(str) != 17)
    return (0);

  int i = 0;
  int hi = 0;
  int j = 0;
  while (str[i]) {
    if (i == 2 || i == 5 || i == 8 || i == 11 || i == 14) {
      if (str[i] != ':')
        return (0);
    } else {
      if (!(str[i] >= '0' && str[i] <= '9') &&
          !(str[i] >= 'a' && str[i] <= 'f') &&
          !(str[i] >= 'A' && str[i] <= 'F'))
        return (0);
      if (i + 1 == 2 || i + 1 == 5 || i + 1 == 8 || i + 1 == 11 ||
          i + 1 == 14 || i + 1 == 17) {
        int lo = hex_char_to_val(str[i]);
        tab[j++] = (hi << 4) | lo;
      } else {
        hi = hex_char_to_val(str[i]);
      }
    }
    i++;
  }
  return (1);
}

int check_args(char **argv, struct s_input *input, int option) {

  int i = 1;

  input->verbose = 0;
  if (option) {
    if (ft_strncmp("-v", argv[i], 3)) {
      return (printExpectedInput(), 0);
    } else {
      input->verbose = 1;
      i++;
    }
  }
  if (input->verbose)
    printf("\e[34m[ Checking input ]\e[0m\n");

  // IP that (should) receive the request
  if (!check_ip(argv[i], input->in_sip)) {
    printf("\e[31munknown host or invalid IP address: %s\e[0m\n", argv[1]);
    return (printExpectedInput(), 0);
  }
  // IP that sends the request (my target)
  if (!check_ip(argv[i + 2], input->in_tip)) {
    printf("\e[31munknown host or invalid IP address: %s\e[0m\n", argv[i + 2]);
    return (printExpectedInput(), 0);
  }
  // MAC to send back (spoofed one)
  if (!check_mac(argv[i + 1], input->in_sha)) {
    printf("\e[31minvalid MAC address: %s\e[0m\n", argv[i + 1]);
    return (printExpectedInput(), 0);
  }
  // MAC that sends the request (my target)
  if (!check_mac(argv[i + 3], input->in_tha)) {
    printf("\e[31minvalid MAC address: %s\e[0m\n", argv[i + 3]);
    return (printExpectedInput(), 0);
  }

  // Print args
  if (input->verbose) {
    printf("\e[34mSource IP = %d.%d.%d.%d\n", input->in_sip[0], input->in_sip[1],
           input->in_sip[2], input->in_sip[3]);
    printf("Source MAC address = %02x:%02x:%02x:%02x:%02x:%02x\n",
           input->in_sha[0], input->in_sha[1], input->in_sha[2],
           input->in_sha[3], input->in_sha[4], input->in_sha[5]);
    printf("Target IP = %d.%d.%d.%d\n", input->in_tip[0], input->in_tip[1],
           input->in_tip[2], input->in_tip[3]);
    printf("Target MAC address = %02x:%02x:%02x:%02x:%02x:%02x\e[0m\n",
           input->in_tha[0], input->in_tha[1], input->in_tha[2],
           input->in_tha[3], input->in_tha[4], input->in_tha[5]);
  }

  return (1);
}
