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

void *ft_memcpy(void *dest, const void *src, size_t n) {
  char *d;
  const char *s;
  size_t i;

  i = 0;
  d = (char *)dest;
  s = (const char *)src;
  while (i < n) {
    d[i] = s[i];
    i++;
  }
  return (dest);
}

int ft_memcmp(const void *s1, const void *s2, size_t n) {
  size_t i;
  const unsigned char *str1;
  const unsigned char *str2;

  if (n == 0)
    return (0);
  str1 = (const unsigned char *)s1;
  str2 = (const unsigned char *)s2;
  i = 0;
  while (i < n) {
    if (str1[i] != str2[i])
      return (str1[i] - str2[i]);
    i++;
  }
  if (i == n)
    return (str1[i - 1] - str2[i - 1]);
  return (str1[i] - str2[i]);
}
