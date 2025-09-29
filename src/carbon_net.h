/*
**  $$=====================$$
**  ||       Network       ||
**  $$=====================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

/**
 * @brief Translate via DNS lookup a domain name to IP address format (IPv4 or IPv6).
 * @param domain The domain name to resolve.
 * @return A string representing the resolved IP address.
 */
CBNDEF char *carbon_net_resolve_dns_to_ip(const char *domain);
