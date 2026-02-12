// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_SOCKET__TCP_LISTEN_BACKLOG 69

#ifdef _WIN32
#define CARBON_SOCKET__ERR_FMT "(%d)"
#define CARBON_SOCKET__ERR_VAL WSAGetLastError()
#else
#define CARBON_SOCKET__ERR_FMT "(%s)"
#define CARBON_SOCKET__ERR_VAL strerror(errno)
#endif

#ifdef _WIN32
__attribute__((constructor)) CBNINL void carbon_socket__init(void) {
  WSADATA wsaData;
  if (0 == WSAStartup(MAKEWORD(2, 2), &wsaData)) return;
  CBN_ERROR("unable to init Winsock " CARBON_SOCKET__ERR_FMT, CARBON_SOCKET__ERR_VAL);
}

__attribute__((destructor)) CBNINL void carbon_socket__shutdown(void) {
  WSACleanup();
}
#endif

CBNINL bool carbon_socket__set_non_blocking(i32 fd) {
  bool ok = true;
#ifdef _WIN32
  u_long mode = 1;
  if (0 != ioctlsocket((SOCKET) fd, FIONBIO, &mode)) ok = false;
#else
  i32 flags = fcntl(fd, F_GETFL, 0);
  if (flags < 0) ok = false;
  if (0 > fcntl(fd, F_SETFL, flags | O_NONBLOCK)) ok = false;
#endif
  if (!ok) CBN_ERROR("unable to set socket as non-blocking " CARBON_SOCKET__ERR_FMT, CARBON_SOCKET__ERR_VAL);
  return ok;
}

/* bool carbon_socket_udp_create(CBN_Socket *sock) { */
/*   // ... */
/* } */

/* bool carbon_socket_udp_create_async(CBN_Socket *sock) { */
/*   // ... */
/* } */

bool carbon_socket_tcp_create(CBN_Socket *sock) {
  sock->is_async = false;
  sock->fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  bool err;
#ifdef _WIN32
  err = (SOCKET) sock->fd == INVALID_SOCKET;
#else
  err = sock->fd < 0;
#endif
  if (err) {
    CBN_ERROR("unable to create socket " CARBON_SOCKET__ERR_FMT, CARBON_SOCKET__ERR_VAL);
    return false;
  }
  carbon_memory_set(sock->ip, 0, CARBON_ARRAY_LEN(sock->ip));
  sock->port = 0;
  return true;
}

bool carbon_socket_tcp_create_async(CBN_Socket *sock) {
  bool ret = carbon_socket_tcp_create(sock);
  sock->is_async = true;
  if (ret) ret = carbon_socket__set_non_blocking(sock->fd);
  return ret;
}

bool carbon_socket_tcp_listen(const CBN_Socket *sock) {
  i32 ret = listen(sock->fd, CARBON_SOCKET__TCP_LISTEN_BACKLOG);
  bool err;
#ifdef _WIN32
  err = ret == SOCKET_ERROR;
#else
  err = ret < 0;
#endif
  if (err) {
    CBN_ERROR("unable to listen through socket " CARBON_SOCKET__ERR_FMT, CARBON_SOCKET__ERR_VAL);
    return false;
  }
  CBN_INFO("Listening to connections on tcp://%s:%hu", sock->ip, sock->port);
  return true;
}

CBN_Socket carbon_socket_tcp_accept(const CBN_Socket *sock) {
  if (sock->is_async) carbon_coroutine_sleep_read(sock->fd);
  struct sockaddr_in addr = {0};
  socklen_t addr_len = sizeof(addr);
  CBN_Socket client = {
    .is_async = sock->is_async,
    .fd = accept(sock->fd, (struct sockaddr *) &addr, &addr_len)
  };
  if (!sock->is_async && client.fd < 0) CBN_ERROR("unable to accept connection (%s)", strerror(errno));
  else if (sock->is_async) {
    if (!carbon_socket__set_non_blocking(client.fd)) {
      carbon_socket_close(&client);
      client.fd = -1;
    }
  }
  if (client.fd > 0) {
    inet_ntop(addr.sin_family, &addr.sin_addr, client.ip, CARBON_ARRAY_LEN(client.ip));
    client.port = ntohs(addr.sin_port);
    CBN_INFO("Accepted new connection from tcp://%s:%hu", client.ip, client.port);
  }
  return client;
}

bool carbon_socket_bind(CBN_Socket *sock, const char *ip, u16 port) {
  if (!carbon_net_is_valid_ipv4(ip)) return false;
  struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_addr.s_addr = inet_addr(ip),
    .sin_port = htons(port)
  };
  i32 ret = bind(sock->fd, (struct sockaddr *) &addr, sizeof(addr));
  bool err;
#ifdef _WIN32
  err = ret == SOCKET_ERROR;
#else
  err = ret < 0;
#endif
  if (err) {
    CBN_ERROR("unable to bind socket " CARBON_SOCKET__ERR_FMT, CARBON_SOCKET__ERR_VAL);
    return false;
  }
  carbon_memory_copy(sock->ip, ip, CARBON_ARRAY_LEN(sock->ip));
  sock->port = port;
  return true;
}

bool carbon_socket_connect(const CBN_Socket *sock, const char *ip, u16 port) {
  if (!carbon_net_is_valid_ipv4(ip)) return false;
  struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_addr.s_addr = inet_addr(ip),
    .sin_port = htons(port)
  };
  i32 ret = connect(sock->fd, (struct sockaddr *) &addr, sizeof(addr));
  bool err;
#ifdef _WIN32
  err = ret == SOCKET_ERROR;
#else
  err = ret < 0;
#endif
  if (err) {
    CBN_ERROR("could not connect to socket " CARBON_SOCKET__ERR_FMT, CARBON_SOCKET__ERR_VAL);
    return false;
  }
  CBN_INFO("Connected to %s:%hu", ip, port);
  return true;
}

bool carbon_socket_close(const CBN_Socket *sock) {
  if (0 > close(sock->fd)) {
    CBN_ERROR("unable to close socket (%s)", strerror(errno));
    return false;
  }
  return true;
}

bool carbon_socket_shutdown(const CBN_Socket *sock, CBN_Socket_ShutdownMode mode) {
  if (0 > shutdown(sock->fd, mode)) {
    CBN_ERROR("unable to shutdown socket (%s)", strerror(errno));
    return false;
  }
  return true;
}

i32 carbon_socket_peek(const CBN_Socket *sock, void *buf, usz buf_sz) {
  if (sock->is_async) carbon_coroutine_sleep_read(sock->fd);
  i32 n = recv(sock->fd, buf, buf_sz, MSG_PEEK);
  if (n < 0) CBN_ERROR("unable to peek from socket (%s)", strerror(errno));
  return n;
}

i32 carbon_socket_read(const CBN_Socket *sock, void *buf, usz buf_sz) {
  if (sock->is_async) carbon_coroutine_sleep_read(sock->fd);
  i32 n = recv(sock->fd, buf, buf_sz, 0);
  if (n < 0) CBN_ERROR("unable to read from socket (%s)", strerror(errno));
  return n;
}

i32 carbon_socket_write(const CBN_Socket *sock, const void *buf, usz buf_sz) {
  if (sock->is_async) carbon_coroutine_sleep_write(sock->fd);
  i32 n = send(sock->fd, buf, buf_sz, 0);
  if (n < 0) CBN_ERROR("unable to write to socket (%s)", strerror(errno));
  return n;
}
