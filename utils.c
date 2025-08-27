#include "malcolm.h"

size_t  ft_strlen(const char *s)
{
  size_t  i;

  i = 0;
  while (s[i])
    i++;
  return (i);
}

int ft_isalnum(int c)
{
  if ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122))
    return (1);
  return (0);
}
