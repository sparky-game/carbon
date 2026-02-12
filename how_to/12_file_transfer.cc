// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

bool client(const char *ip, const u16 port, const char *file) {
  auto sock = cbn::net::tcp::Socket::make();
  if (!sock) return false;
  defer { sock->Close(); };
  if (!sock->Connect(ip, port)) return false;
  CBN_INFO("--> %d (file = `%s`)", sock->Write(file, cbn::str::len(file)), file);
  sock->Shutdown(CBN_SOCKET_SHUTDOWN_MODE_WRITE);
  usz buf_sz = 0;
  if (auto n = sock->Read(&buf_sz, sizeof(usz)); n > 0) CBN_INFO("<-- %d (size = %zu)", n, buf_sz);
  else {
    CBN_ERROR("file requested (`%s`) doesn't exist in the server", file);
    return false;
  }
  u8 *buf = (u8 *) cbn::mem::Alloc(buf_sz);
  defer { cbn::mem::Free(buf); };
  u32 crc32 = 0;
  if (auto n = sock->Read(&crc32, sizeof(u32)); n > 0) CBN_INFO("<-- %d (crc32 = %#010x)", n, crc32);
  else return false;
  {// Le Data™
    usz read = 0;
    while (read < buf_sz) {
      auto n = sock->Read(buf + read, buf_sz - read);
      if (n <= 0) break;
      read += n;
      CBN_INFO("<-- %d", n);
    }
    if (read != buf_sz) {
      CBN_ERROR("incomplete file transfer (%zu/%zu). Please, try again.", read, buf_sz);
      return false;
    }
  }
  if (auto i = cbn::crypto::crc32::Compute(buf, buf_sz); crc32 != i) {
    CBN_ERROR("integrity check failed (%#010x != %#010x). Please, try again", crc32, i);
    return false;
  }
  {// Write Le Data™ to filesystem
    FILE *fd = fopen(file, "wb");
    fwrite(buf, buf_sz, 1, fd);
    fclose(fd);
    CBN_INFO("Written file as `%s`", file);
  }
  return true;
}

static void handle_client(cbn::net::tcp::Socket s) {
  defer { s.Close(); };
  char file[CARBON_FS_PATH_MAX_LEN] = {};
  if (auto n = s.Read(file, CARBON_ARRAY_LEN(file)); n > 0) CBN_INFO("<-- %d (file = `%s`)", n, file);
  else return;
  auto sb = cbn::str::Builder::FromFile(file);
  if (!sb) return;
  defer { sb->Free(); };
  CBN_INFO("--> %d (size = %zu)", s.Write(&sb->size, sizeof(usz)), sb->size);
  u32 crc32 = cbn::crypto::crc32::Compute((u8 *) sb->items, sb->size);
  CBN_INFO("--> %d (crc32 = %#010x)", s.Write(&crc32, sizeof(u32)), crc32);
  CBN_INFO("--> %d", s.Write(sb->items, sb->size));
}

bool server(const u16 port) {
  auto sock = cbn::net::tcp::Socket::make();
  if (!sock) return false;
  defer { sock->Close(); };
  if (!sock->Bind("0.0.0.0", port)) return false;
  if (!sock->Listen()) return false;
  for (;;) {
    if (auto s = sock->Accept()) handle_client(*s);
  }
  return true;
}

int main(int argc, char **argv) {
  auto self = CARBON_SHIFT_ARGS(argc, argv);
  if (argc < 1) {
    CBN_ERROR("usage: %s [get|serve]", self);
    return 1;
  }
  auto cmd = CARBON_SHIFT_ARGS(argc, argv);
  if (!cbn::str::cmp(cmd, "get")) {
    if (argc != 3) {
      CBN_ERROR("usage: %s %s [IP] [PORT] [FILE]", self, cmd);
      return 1;
    }
    auto ip = CARBON_SHIFT_ARGS(argc, argv);
    auto port = CARBON_SHIFT_ARGS(argc, argv);
    auto file = CARBON_SHIFT_ARGS(argc, argv);
    if (!cbn::str::IsNumber(port)) {
      CBN_ERROR("PORT (`%s`) needs to be a number", port);
      return 1;
    }
    if (!client(ip, cbn::str::ToNumber(port), file)) return 1;
    return 0;
  }
  if (!cbn::str::cmp(cmd, "serve")) {
    if (argc != 1) {
      CBN_ERROR("usage: %s %s [PORT]", self, cmd);
      return 1;
    }
    auto port = CARBON_SHIFT_ARGS(argc, argv);
    if (!cbn::str::IsNumber(port)) {
      CBN_ERROR("PORT (`%s`) needs to be a number", port);
      return 1;
    }
    if (!server(cbn::str::ToNumber(port))) return 1;
    return 0;
  }
  CBN_ERROR("usage: %s [get|serve]", self);
  return 1;
}
