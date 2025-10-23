/*
**  $$=====================$$
**  ||       Network       ||
**  $$=====================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#define CARBON_NET_IPV4_MAX_LEN 16

/**
 * @brief Check if the provided IPv4 address is valid format-wise.
 * @param ip The IPv4 address to validate.
 * @return A boolean value indicating whether is valid or not.
 */
CBNDEF bool carbon_net_is_valid_ipv4(const char *ip);

/**
 * @brief Translate via DNS lookup a domain name to IPv4 address format.
 * @param domain The domain name to resolve.
 * @return A string representing the resolved IPv4 address.
 */
CBNDEF char *carbon_net_resolve_dns_to_ipv4(const char *domain);
