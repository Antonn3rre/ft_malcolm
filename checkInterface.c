#include "malcolm.h"
#include <sys/socket.h>

int checkInterface(struct s_input *input) {

  if (input->verbose)
    printf("\e[34m[ Searching for an interface ]\n\e[0m");
  struct ifaddrs *ifap;

  // Get interfaces list
  if (getifaddrs(&ifap) == -1)
    return (perror("getifaddr"), 0);

  // Format uint32_t to bit shift with mask
  uint32_t inp_tip, inp_sip;
  ft_memcpy(&inp_tip, input->in_tip, 4);
  ft_memcpy(&inp_sip, input->in_sip, 4);

  struct ifaddrs *tmp = ifap;
  while (ifap) {

    // If not an IPV4 address, skip
    if (!ifap->ifa_addr || ifap->ifa_addr->sa_family != AF_INET) {
      ifap = ifap->ifa_next;
      continue;
    }

    // Store the interface IP address and mask
    struct sockaddr_in *addr = (struct sockaddr_in *)ifap->ifa_addr;
    struct sockaddr_in *netmask = (struct sockaddr_in *)ifap->ifa_netmask;

    uint32_t ip_iface, mask;

    ip_iface = addr->sin_addr.s_addr;
    mask = netmask->sin_addr.s_addr;

    //   printf("inp_tip = %u, inp_sip = %u, ip_iface = %u, mask = %u\n",
    //   inp_tip,
    //        inp_sip, ip_iface, mask);

    // Bit shift to check if the target ip and the source ip are in the same
    // network as the interface
    if ((inp_tip & mask) == (ip_iface & mask) &&
        (inp_sip & mask) == (ip_iface & mask)) {

      input->interIndex = if_nametoindex(ifap->ifa_name);
      printf("\e[32mFound available interface: %s\n\e[0m", ifap->ifa_name);
      freeifaddrs(tmp);
      return (1);
    }
    if (input->verbose)
      printf("\e[33mNot on the same network: %s\e[0m\n", ifap->ifa_name);
    ifap = ifap->ifa_next;
  }
      freeifaddrs(tmp);
  return (0);
}
