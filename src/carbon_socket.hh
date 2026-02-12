/*
  $$====================$$
  ||       Socket       ||
  $$====================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

typedef enum {
  CBN_SOCKET_SHUTDOWN_MODE_READ,
  CBN_SOCKET_SHUTDOWN_MODE_WRITE,
  CBN_SOCKET_SHUTDOWN_MODE_BOTH
} CBN_Socket_ShutdownMode;

/**
 * @brief ...
 */
CBNDEF_T(CBN_Socket) {
  i32 fd;
  bool is_async;
  char ip[CARBON_NET_IPV4_MAX_LEN];
  u16 port;
};
CBNDEF_AKA(cbn::net, Socket, CBN_Socket);


/**
 */
CBNDEF bool carbon_socket_udp_create(CBN_Socket *sock);

/**
 */
CBNDEF bool carbon_socket_udp_create_async(CBN_Socket *sock);

/**
 */
CBNDEF bool carbon_socket_tcp_create(CBN_Socket *sock);

/**
 */
CBNDEF bool carbon_socket_tcp_create_async(CBN_Socket *sock);

/**
 */
CBNDEF bool carbon_socket_tcp_listen(const CBN_Socket *sock);

/**
 */
CBNDEF CBN_Socket carbon_socket_tcp_accept(const CBN_Socket *sock);

/**
 */
CBNDEF bool carbon_socket_bind(CBN_Socket *sock, const char *ip, u16 port);

/**
 */
CBNDEF bool carbon_socket_connect(const CBN_Socket *sock, const char *ip, u16 port);

/**
 */
CBNDEF bool carbon_socket_close(const CBN_Socket *sock);

/**
 */
CBNDEF bool carbon_socket_shutdown(const CBN_Socket *sock, CBN_Socket_ShutdownMode mode);

/**
 */
CBNDEF i32 carbon_socket_peek(const CBN_Socket *sock, void *buf, usz buf_sz);

/**
 */
CBNDEF i32 carbon_socket_read(const CBN_Socket *sock, void *buf, usz buf_sz);

/**
 */
CBNDEF i32 carbon_socket_write(const CBN_Socket *sock, const void *buf, usz buf_sz);

#ifdef __cplusplus
struct CBN_Socket : CBN_Socket_t {
  using ShutdownMode = CBN_Socket_ShutdownMode;

  bool Bind(const char *ip_, u16 port_) {
    return carbon_socket_bind(this, ip_, port_);
  }

  bool Connect(const char *ip_, u16 port_) const {
    return carbon_socket_connect(this, ip_, port_);
  }

  bool Close(void) const {
    return carbon_socket_close(this);
  }

  bool Shutdown(ShutdownMode mode) const {
    return carbon_socket_shutdown(this, mode);
  }

  i32 Peek(void *buf, usz buf_sz) const {
    return carbon_socket_peek(this, buf, buf_sz);
  }

  i32 Read(void *buf, usz buf_sz) const {
    return carbon_socket_read(this, buf, buf_sz);
  }

  i32 Write(const void *buf, usz buf_sz) const {
    return carbon_socket_write(this, buf, buf_sz);
  }
};

struct CBN_Socket_UDP : CBN_Socket {
  static cbn::Opt<CBN_Socket_UDP> make(void);

  static cbn::Opt<CBN_Socket_UDP> make_async(void);
};
CBNDEF_AKA(cbn::net::udp, Socket, CBN_Socket_UDP);

struct CBN_Socket_TCP : CBN_Socket {
  static cbn::Opt<CBN_Socket_TCP> make(void) {
    CBN_Socket_TCP sock {};
    if (!carbon_socket_tcp_create(&sock)) return {};
    return sock;
  }

  static cbn::Opt<CBN_Socket_TCP> make_async(void) {
    CBN_Socket_TCP sock {};
    if (!carbon_socket_tcp_create_async(&sock)) return {};
    return sock;
  }

  bool Listen(void) const {
    return carbon_socket_tcp_listen(this);
  }

  cbn::Opt<CBN_Socket_TCP> Accept(void) const {
    CBN_Socket_TCP sock { carbon_socket_tcp_accept(this) };
    if (sock.fd == -1) return {};
    return sock;
  }
};
CBNDEF_AKA(cbn::net::tcp, Socket, CBN_Socket_TCP);
#endif
