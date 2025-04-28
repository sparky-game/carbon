// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#define CARBON_COROUTINE__STACK_CAPACITY (1024 * getpagesize())

#if defined(__linux__) && (defined(__amd64__) || defined(_M_AMD64))
#define CARBON_COROUTINE__STORE_REGISTERS       \
  "pushq %rdi\n"                                \
  "pushq %rbp\n"                                \
  "pushq %rbx\n"                                \
  "pushq %r12\n"                                \
  "pushq %r13\n"                                \
  "pushq %r14\n"                                \
  "pushq %r15\n"

#define CARBON_COROUTINE__SLEEP_NONE            \
  "movq %rsp, %rdi\n"                           \
  "movq $0, %rsi\n"                             \
  "jmp carbon_coroutine__switch_ctx\n"

#define CARBON_COROUTINE__SLEEP_READ            \
  "movq %rdi, %rdx\n"                           \
  "movq %rsp, %rdi\n"                           \
  "movq $1, %rsi\n"                             \
  "jmp carbon_coroutine__switch_ctx\n"

#define CARBON_COROUTINE__SLEEP_WRITE           \
  "movq %rdi, %rdx\n"                           \
  "movq %rsp, %rdi\n"                           \
  "movq $2, %rsi\n"                             \
  "jmp carbon_coroutine__switch_ctx\n"

#define CARBON_COROUTINE__RESTORE_REGISTERS     \
  "movq %rdi, %rsp\n"                           \
  "popq %r15\n"                                 \
  "popq %r14\n"                                 \
  "popq %r13\n"                                 \
  "popq %r12\n"                                 \
  "popq %rbx\n"                                 \
  "popq %rbp\n"                                 \
  "popq %rdi\n"                                 \
  "ret\n"

#elif defined(__APPLE__) && defined(__aarch64__)
#define CARBON_COROUTINE__STORE_REGISTERS       \
  "sub sp, sp, #240\n"                          \
  "stp q8, q9, [sp, #0]\n"                      \
  "stp q10, q11, [sp, #32]\n"                   \
  "stp q12, q13, [sp, #64]\n"                   \
  "stp q14, q15, [sp, #96]\n"                   \
  "stp x19, x20, [sp, #128]\n"                  \
  "stp x21, x22, [sp, #144]\n"                  \
  "stp x23, x24, [sp, #160]\n"                  \
  "stp x25, x26, [sp, #176]\n"                  \
  "stp x27, x28, [sp, #192]\n"                  \
  "stp x29, x30, [sp, #208]\n"                  \
  "mov x1, x30\n"                               \
  "str x30, [sp, #224]\n"                       \
  "str x0, [sp, #232]\n"

#define CARBON_COROUTINE__SLEEP_NONE            \
  "mov x0, sp\n"                                \
  "mov x1, #0\n"                                \
  "b _carbon_coroutine__switch_ctx\n"

#define CARBON_COROUTINE__SLEEP_READ            \
  "mov x2, x0\n"                                \
  "mov x0, sp\n"                                \
  "mov x1, #1\n"                                \
  "b _carbon_coroutine__switch_ctx\n"

#define CARBON_COROUTINE__SLEEP_WRITE           \
  "mov x2, x0\n"                                \
  "mov x0, sp\n"                                \
  "mov x1, #2\n"                                \
  "b _carbon_coroutine__switch_ctx\n"

#define CARBON_COROUTINE__RESTORE_REGISTERS     \
  "mov sp, x0\n"                                \
  "ldp q8, q9, [sp, #0]\n"                      \
  "ldp q10, q11, [sp, #32]\n"                   \
  "ldp q12, q13, [sp, #64]\n"                   \
  "ldp q14, q15, [sp, #96]\n"                   \
  "ldp x19, x20, [sp, #128]\n"                  \
  "ldp x21, x22, [sp, #144]\n"                  \
  "ldp x23, x24, [sp, #160]\n"                  \
  "ldp x25, x26, [sp, #176]\n"                  \
  "ldp x27, x28, [sp, #192]\n"                  \
  "ldp x29, x30, [sp, #208]\n"                  \
  "mov x1, x30\n"                               \
  "ldr x30, [sp, #224]\n"                       \
  "ldr x0, [sp, #232]\n"                        \
  "add sp, sp, #240\n"                          \
  "ret x1\n"

#else
#error Target platform is not supported
#endif

static usz carbon_coroutine__current;
static CBN_List carbon_coroutine__active;  // CBN_List<usz>
static CBN_List carbon_coroutine__dead;    // CBN_List<usz>
static CBN_List carbon_coroutine__ctxs;    // CBN_List<CBN_Coroutine_CTX>
static CBN_List carbon_coroutine__asleep;  // CBN_List<usz>
static CBN_List carbon_coroutine__polls;  // CBN_List<struct pollfd>

CARBON_INLINE __attribute__((naked)) void carbon_coroutine_restore_ctx(void *rsp) {
  __asm__ volatile (CARBON_COROUTINE__RESTORE_REGISTERS ::: "memory");
}

void carbon_coroutine__switch_ctx(void *rsp, CBN_Coroutine_SleepMode sm, i32 fd) {
  usz current_active_item = carbon_list_at(usz, carbon_coroutine__active, carbon_coroutine__current);
  carbon_list_at_raw(CBN_Coroutine_CTX, carbon_coroutine__ctxs, current_active_item).rsp = rsp;
  switch (sm) {
  case CBN_COROUTINE_SLEEP_MODE_NONE:
    ++carbon_coroutine__current;
    break;
  case CBN_COROUTINE_SLEEP_MODE_READ: {
    carbon_list_push(&carbon_coroutine__asleep, &current_active_item);
    struct pollfd pfd = {
      .fd = fd,
      .events = POLLRDNORM
    };
    carbon_list_push(&carbon_coroutine__polls, &pfd);
    carbon_list_remove(&carbon_coroutine__active, carbon_coroutine__current);
    break;
  }
  case CBN_COROUTINE_SLEEP_MODE_WRITE: {
    carbon_list_push(&carbon_coroutine__asleep, &current_active_item);
    struct pollfd pfd = {
      .fd = fd,
      .events = POLLWRNORM
    };
    carbon_list_push(&carbon_coroutine__polls, &pfd);
    carbon_list_remove(&carbon_coroutine__active, carbon_coroutine__current);
    break;
  }
  default:
    CARBON_ASSERT(0 && "unreachable");
  }
  if (carbon_coroutine__polls.size) {
    i32 timeout = carbon_coroutine__active.size ? 0 : -1;
    i32 result = poll((struct pollfd *) carbon_coroutine__polls.items, carbon_coroutine__polls.size, timeout);
    // TODO: handle error if `poll` call returned -1
    CARBON_NOTUSED(result);
    for (usz i = 0; i < carbon_coroutine__polls.size;) {
      if (carbon_list_at(struct pollfd, carbon_coroutine__polls, i).revents) {
        usz id = carbon_list_at(usz, carbon_coroutine__asleep, i);
        carbon_list_remove(&carbon_coroutine__polls, i);
        carbon_list_remove(&carbon_coroutine__asleep, i);
        carbon_list_push(&carbon_coroutine__active, &id);
      }
      else ++i;
    }
  }
  CARBON_ASSERT(carbon_coroutine__active.size);
  carbon_coroutine__current %= carbon_coroutine__active.size;
  current_active_item = carbon_list_at(usz, carbon_coroutine__active, carbon_coroutine__current);
  CBN_Coroutine_CTX current_ctx_item = carbon_list_at(CBN_Coroutine_CTX, carbon_coroutine__ctxs, current_active_item);
  carbon_coroutine_restore_ctx(current_ctx_item.rsp);
}

CARBON_INLINE void carbon_coroutine__finish_current(void) {
  usz current_active_item = carbon_list_at(usz, carbon_coroutine__active, carbon_coroutine__current);
  if (!current_active_item) CARBON_ASSERT(0 && "unreachable");
  carbon_list_push(&carbon_coroutine__dead, &current_active_item);
  carbon_list_remove(&carbon_coroutine__active, carbon_coroutine__current);
  if (carbon_coroutine__polls.size) {
    i32 timeout = carbon_coroutine__active.size ? 0 : -1;
    i32 result = poll((struct pollfd *) carbon_coroutine__polls.items, carbon_coroutine__polls.size, timeout);
    // TODO: handle error if `poll` call returned -1
    CARBON_NOTUSED(result);
    for (usz i = 0; i < carbon_coroutine__polls.size;) {
      if (carbon_list_at(struct pollfd, carbon_coroutine__polls, i).revents) {
        usz id = carbon_list_at(usz, carbon_coroutine__asleep, i);
        carbon_list_remove(&carbon_coroutine__polls, i);
        carbon_list_remove(&carbon_coroutine__asleep, i);
        carbon_list_push(&carbon_coroutine__active, &id);
      }
      else ++i;
    }
  }
  CARBON_ASSERT(carbon_coroutine__active.size);
  carbon_coroutine__current %= carbon_coroutine__active.size;
  current_active_item = carbon_list_at(usz, carbon_coroutine__active, carbon_coroutine__current);
  CBN_Coroutine_CTX current_ctx_item = carbon_list_at(CBN_Coroutine_CTX, carbon_coroutine__ctxs, current_active_item);
  carbon_coroutine_restore_ctx(current_ctx_item.rsp);
}

void carbon_coroutine_init(void) {
  carbon_coroutine__active = carbon_list_create(sizeof(usz));
  carbon_coroutine__dead   = carbon_list_create(sizeof(usz));
  carbon_coroutine__ctxs   = carbon_list_create(sizeof(CBN_Coroutine_CTX));
  carbon_coroutine__asleep = carbon_list_create(sizeof(usz));
  carbon_coroutine__polls  = carbon_list_create(sizeof(struct pollfd));
  CBN_Coroutine_CTX ctx = {0, 0};
  usz i = 0;
  carbon_list_push(&carbon_coroutine__ctxs, &ctx);
  carbon_list_push(&carbon_coroutine__active, &i);
}

void carbon_coroutine_shutdown(void) {
  carbon_list_destroy(&carbon_coroutine__active);
  carbon_list_destroy(&carbon_coroutine__dead);
  carbon_list_destroy(&carbon_coroutine__ctxs);
  carbon_list_destroy(&carbon_coroutine__asleep);
  carbon_list_destroy(&carbon_coroutine__polls);
}

usz carbon_coroutine_id(void) {
  return carbon_list_at(usz, carbon_coroutine__active, carbon_coroutine__current);
}

usz carbon_coroutine_alive(void) {
  return carbon_coroutine__active.size;
}

void carbon_coroutine_go(void (*f)(void *), void *arg) {
  usz id;
  if (carbon_coroutine__dead.size) id = carbon_list_at(usz, carbon_coroutine__dead, --carbon_coroutine__dead.size);
  else {
    CBN_Coroutine_CTX ctx = {0, 0};
    carbon_list_push(&carbon_coroutine__ctxs, &ctx);
    id = carbon_coroutine__ctxs.size - 1;
    carbon_list_at_raw(CBN_Coroutine_CTX, carbon_coroutine__ctxs, id).rsbp = mmap(0, CARBON_COROUTINE__STACK_CAPACITY, PROT_WRITE | PROT_READ,
#ifdef __APPLE__
                                                                                  MAP_PRIVATE | MAP_ANONYMOUS,
#else
                                                                                  MAP_PRIVATE | MAP_STACK | MAP_ANONYMOUS | MAP_GROWSDOWN,
#endif
                                                                                  -1, 0);
    CARBON_ASSERT(carbon_list_at(CBN_Coroutine_CTX, carbon_coroutine__ctxs, id).rsbp != MAP_FAILED);
  }
  void **rsp = (void **) ((u8 *) carbon_list_at_raw(CBN_Coroutine_CTX, carbon_coroutine__ctxs, id).rsbp + CARBON_COROUTINE__STACK_CAPACITY);
#if defined(__amd64__) || defined(_M_AMD64)
  *(--rsp) = (void *) carbon_coroutine__finish_current;
  *(--rsp) = (void *) f;
  *(--rsp) = arg;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
#elif defined(__aarch64__)
  *(--rsp) = arg;
  *(--rsp) = (void *) carbon_coroutine__finish_current;
  *(--rsp) = (void *) f;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
  *(--rsp) = 0;
#else
#error CPU architecture is not supported
#endif
  carbon_list_at_raw(CBN_Coroutine_CTX, carbon_coroutine__ctxs, id).rsp = rsp;
  carbon_list_push(&carbon_coroutine__active, &id);
}

__attribute__((naked)) void carbon_coroutine_yield(void) {
  __asm__ volatile (CARBON_COROUTINE__STORE_REGISTERS CARBON_COROUTINE__SLEEP_NONE ::: "memory");
}

__attribute__((naked)) void carbon_coroutine_sleep_read(i32 fd) {
  __asm__ volatile (CARBON_COROUTINE__STORE_REGISTERS CARBON_COROUTINE__SLEEP_READ ::: "memory");
}

__attribute__((naked)) void carbon_coroutine_sleep_write(i32 fd) {
  __asm__ volatile (CARBON_COROUTINE__STORE_REGISTERS CARBON_COROUTINE__SLEEP_WRITE ::: "memory");
}

void carbon_coroutine_wakeup(usz id) {
  carbon_list_foreach(usz, carbon_coroutine__asleep) {
    if (it.var == id) {
      carbon_list_remove(&carbon_coroutine__asleep, carbon_list_find(&carbon_coroutine__asleep, &id));
      carbon_list_remove(&carbon_coroutine__polls, carbon_list_find(&carbon_coroutine__polls, &id));
      carbon_list_push(&carbon_coroutine__active, &id);
      return;
    }
  }
}
