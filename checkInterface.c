#include "malcolm.h"
// #include <cstdint>

int checkInterface(struct s_input *input) {

  (void)input;
  struct ifaddrs *ifap;

  // Get interfaces list
  if (getifaddrs(&ifap) == -1)
    return (perror("getifaddr"), 0);

  uint32_t inp_tip, inp_sip;
  ft_memcpy(&inp_tip, input->in_tip, 4);
  ft_memcpy(&inp_sip, input->in_sip, 4);

  while (ifap) {

    if (!ifap->ifa_addr || ifap->ifa_addr->sa_family != AF_INET) {
      ifap = ifap->ifa_next;
      continue;
    }
    struct sockaddr_in *addr = (struct sockaddr_in *)ifap->ifa_addr;
    struct sockaddr_in *netmask = (struct sockaddr_in *)ifap->ifa_netmask;

    uint32_t ip_iface, mask;

    ip_iface = addr->sin_addr.s_addr;
    mask = netmask->sin_addr.s_addr;

    printf("inp_tip = %u, inp_sip = %u, ip_iface = %u, mask = %u\n", inp_tip,
           inp_sip, ip_iface, mask);
    if ((inp_tip & mask) == (ip_iface & mask) &&
        (inp_sip & mask) == (ip_iface & mask)) {
      // TODO: reboucler pour trouver mac
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
