// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#define CARBON_NET_RESOLVE_DNS_MAX_BUFFERS 4

CARBON_INLINE struct addrinfo *carbon_net__resolve_dns_to_addrs(const char *domain) {
  struct addrinfo *res = 0, hints;
  carbon_memory_set(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  i32 status = getaddrinfo(domain, 0, &hints, &res);
  if (status != 0) {
    carbon_log_error("unable to getaddrinfo (%s)", gai_strerror(status));
    return 0;
  }
  return res;
}

char *carbon_net_resolve_dns_to_ip(const char *domain) {
  static usz i = 0;
  static char xs[CARBON_NET_RESOLVE_DNS_MAX_BUFFERS][INET6_ADDRSTRLEN];
  struct addrinfo *addrs = carbon_net__resolve_dns_to_addrs(domain);
  if (!addrs) return 0;
  char *x = xs[i];
  carbon_memory_set(x, 0, INET6_ADDRSTRLEN);
  void *addr = 0;
  switch (addrs->ai_addr->sa_family) {
  case AF_INET:
    addr = &((struct sockaddr_in *) addrs->ai_addr)->sin_addr;
    break;
  case AF_INET6:
    addr = &((struct sockaddr_in6 *) addrs->ai_addr)->sin6_addr;
    break;
  default: CARBON_UNREACHABLE;
  }
  inet_ntop(addrs->ai_addr->sa_family, addr, x, INET6_ADDRSTRLEN);
  freeaddrinfo(addrs);
  ++i;
  if (i >= CARBON_NET_RESOLVE_DNS_MAX_BUFFERS) i = 0;
  return x;
}
