#include "malcolm.h"

int check_ip(char *str, unsigned char tab[4]) {
  int i = 0;
  int numOfNum = 0;
  int value = 0;

  while (str[i] && numOfNum < 4) {
    value = strict_atoi(str, &i);
    if (value == -1)
      return (0);
    else
      tab[numOfNum] = value;
    numOfNum++;
    if (!str[i])
      break;
    if (str[i] != '.')
      return (0);
    i++;
  }
  if (numOfNum != 4)
    return (0);
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

int check_args(char **argv, struct s_input *input) {
	// IP that (should) receive the request
  if (!check_ip(argv[1], input->in_sip)) {
    printf("Bad IP adress: %s\n", argv[1]);
    return (0);
  }
  // IP that sends the request (my target)
  if (!check_ip(argv[3], input->in_tip)) {
    printf("Bad IP adress: %s\n", argv[3]);
    return (0);
  }
  // MAC to send back (spoofed one)
  if (!check_mac(argv[2], input->in_sha)) {
    printf("Bad MAC adress: %s\n", argv[2]);
    return (0);
  }
  // MAC that sends the request (my target)
  if (!check_mac(argv[4], input->in_tha)) {
    printf("Bad MAC adress: %s\n", argv[4]);
    return (0);
  }
  return (1);
}
