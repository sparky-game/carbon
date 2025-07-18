// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

#define CARBON_NET_RESOLVE_DNS_MAX_BUFFERS 4
#define CARBON_NET_IPV6_MAX_LEN 46

#ifdef _WIN32
CARBON_INLINE PDNS_RECORD carbon_net__resolve_dns_to_addrs(const char *domain) {
  PDNS_RECORD addrs = 0;
  if (ERROR_SUCCESS != DnsQuery_A(domain, DNS_TYPE_A | DNS_TYPE_AAAA, DNS_QUERY_STANDARD, 0, &addrs, 0)) {
    CBN_ERROR("unable to DnsQuery_A");
    return 0;
  }
  return addrs;
}
#else
CARBON_INLINE struct addrinfo *carbon_net__resolve_dns_to_addrs(const char *domain) {
  struct addrinfo *res = 0, hints;
  carbon_memory_set(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  i32 status = getaddrinfo(domain, 0, &hints, &res);
  if (status != 0) {
    CBN_ERROR("unable to getaddrinfo (%s)", gai_strerror(status));
    return 0;
  }
  return res;
}
#endif

char *carbon_net_resolve_dns_to_ip(const char *domain) {
  static usz i = 0;
  static char xs[CARBON_NET_RESOLVE_DNS_MAX_BUFFERS][CARBON_NET_IPV6_MAX_LEN];
  char *x = xs[i];
  carbon_memory_set(x, 0, CARBON_NET_IPV6_MAX_LEN);
#ifdef _WIN32
  extern PSTR RtlIpv4AddressToStringA(const IP4_ADDRESS *, PSTR);
  extern PSTR RtlIpv6AddressToStringA(const IP6_ADDRESS *, PSTR);
  PDNS_RECORD addrs = carbon_net__resolve_dns_to_addrs(domain);
  if (!addrs) return 0;
  PDNS_RECORD addr = addrs;
  while (addr) {
    switch (addr->wType) {
    case DNS_TYPE_A:
      RtlIpv4AddressToStringA(&addr->Data.A.IpAddress, x);
      break;
    case DNS_TYPE_AAAA:
      RtlIpv6AddressToStringA(&addr->Data.AAAA.Ip6Address, x);
      break;
    default: CARBON_UNREACHABLE;
    }
    addr = addr->pNext;
  }
  DnsRecordListFree(addrs, DNS_FREE_TYPE);
  if (!addr) {
    CBN_ERROR("No valid IP found");
    return 0;
  }
#else
  struct addrinfo *addrs = carbon_net__resolve_dns_to_addrs(domain);
  if (!addrs) return 0;
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
  inet_ntop(addrs->ai_addr->sa_family, addr, x, CARBON_NET_IPV6_MAX_LEN);
  freeaddrinfo(addrs);
#endif
  ++i;
  if (i >= CARBON_NET_RESOLVE_DNS_MAX_BUFFERS) i = 0;
  return x;
}
