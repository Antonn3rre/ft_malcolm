#include "malcolm.h"

int checkInterface(struct s_input *input) {

  (void)input;
  struct ifaddrs *ifap;

  if (getifaddrs(&ifap) == -1)
    return (perror("getifaddr"), 0);
  while (ifap) {

    if (!ifap->ifa_addr || ifap->ifa_addr->sa_family != AF_INET) {
      ifap = ifap->ifa_next;
      continue;
    }
    struct sockaddr_in *addr = (struct sockaddr_in *)ifap->ifa_addr;
    struct sockaddr_in *netmask = (struct sockaddr_in *)ifap->ifa_netmask;

    uint32_t ip_given, ip_iface, mask;

    ft_memcpy(&ip_given, input->in_tip, 4);

    ip_iface = ntohl(addr->sin_addr.s_addr);
    mask = ntohl(netmask->sin_addr.s_addr);

    printf("Test interface %s: ip_given=%08x ip_iface=%08x mask=%08x\n",
           ifap->ifa_name, ip_given, ip_iface, mask);
    if ((ip_given & mask) == (ip_iface & mask)) {
      printf("Found interface: %s\n", ifap->ifa_name);
      ft_memcpy(input->interface, ifap->ifa_name, ft_strlen(ifap->ifa_name));
      input->interface[ft_strlen(ifap->ifa_name)] = 0;
      return 1;
    }

    printf("%s\n", ifap->ifa_name);
    ifap = ifap->ifa_next;
  }
  return (0);
}
