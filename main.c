#include "malcolm.h"

int main (int argc, char **argv) {

  if (argc != 5) {
    printf("Wrong number of arguments\n");
    return (0);
  }

  if (!check_args(argv))
    return (0);

  return (0);
}
