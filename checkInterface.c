#include "malcolm.h"
#include <sys/socket.h>

int checkInterface(struct s_input *input) {

  (void)input;
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

      struct ifaddrs *tmp2 = tmp;
      // Re-check ifap to find the interface but with the AF_PACKET format
      // Obj: get its MAC address
      while (tmp2) {
        if (tmp2->ifa_addr &&
            !ft_memcmp(tmp2->ifa_name, ifap->ifa_name,
                       ft_strlen(ifap->ifa_name) &&
                           tmp->ifa_addr->sa_family == AF_PACKET)) {

          // Store IP and MAC addresses then quit
          printf("Found interface: %s\n", ifap->ifa_name);
          ft_memcpy(input->interface_ip,
                    &((struct sockaddr_in *)ifap->ifa_addr)->sin_addr, 4);
          ft_memcpy(input->interface_mac,
                    ((struct sockaddr_ll *)(tmp2->ifa_addr))->sll_addr, 6);
          return (1);
        }
        tmp2 = tmp2->ifa_next;
      }
    }
    printf("Not good: %s\n", ifap->ifa_name);
    ifap = ifap->ifa_next;
  }
  return (0);
}
