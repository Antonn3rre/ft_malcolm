#include "malcolm.h"

// atoi with only positive num
// take pointeur of i [counter]
// num cannot be more than 255
// return -1 if error
int strict_atoi(const char *nptr, int *i) {
  int num;

  num = 0;
  while (nptr[*i] >= '0' && nptr[*i] <= '9') {
    num = num * 10 + (nptr[*i] - 48);
    (*i)++;
    if (num > 255)
      return (-1);
  }
  return (num);
}

size_t ft_strlen(const char *s) {
  size_t i;

  i = 0;
  while (s[i])
    i++;
  return (i);
}

int ft_isalnum(int c) {
  if ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122))
    return (1);
  return (0);
}
