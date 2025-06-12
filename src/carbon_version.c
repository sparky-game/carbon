// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

char *carbon_version(usz *major, usz *minor, usz *patch) {
  if (major) *major = CARBON_VERSION_MAJOR;
  if (minor) *minor = CARBON_VERSION_MINOR;
  if (patch) *patch = CARBON_VERSION_PATCH;
  return (char *) CARBON_VERSION;
}
