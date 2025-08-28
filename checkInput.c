#include "malcolm.h"

int check_ip(char *str) {
  int i = 0;
  int numOfNum = 0;

  while (str[i]) {
    if (strict_atoi(str, &i) == -1)
      return (0);
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

int check_mac(char *str) {
  if (ft_strlen(str) != 17)
    return (0);

  int i = 0;
  while (str[i]) {
    if (i == 2 || i == 5 || i == 8 || i == 11 || i == 14) {
      if (str[i] != ':')
        return (0);
    } else {
      if (!ft_isalnum(str[i]))
        return (0);
    }
    i++;
  }
  return (1);
}

int check_args(char **argv) {
  if (!check_ip(argv[1])) {
    printf("Bad IP adress: %s\n", argv[1]);
    return (0);
  }
  if (!check_ip(argv[3])) {
    printf("Bad IP adress: %s\n", argv[3]);
    return (0);
  }
  if (!check_mac(argv[2])) {
    printf("Bad MAC adress: %s\n", argv[2]);
    return (0);
  }
  if (!check_mac(argv[4])) {
    printf("Bad MAC adress: %s\n", argv[4]);
    return (0);
  }
  return (1);
}
