// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

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

#elif defined(_WIN32) && (defined(__amd64__) || defined(_M_AMD64))
#define CARBON_COROUTINE__STORE_REGISTERS       \
  "pushq %rcx\n"                                \
  "pushq %rbx\n"                                \
  "pushq %rbp\n"                                \
  "pushq %rdi\n"                                \
  "pushq %rsi\n"                                \
  "pushq %r12\n"                                \
  "pushq %r13\n"                                \
  "pushq %r14\n"                                \
  "pushq %r15\n"

#define CARBON_COROUTINE__SLEEP_NONE            \
  "movq %rsp, %rcx\n"                           \
  "movq $0, %rdx\n"                             \
  "jmp carbon_coroutine__switch_ctx\n"

#define CARBON_COROUTINE__SLEEP_READ            \
  "movq %rcx, %r8\n"                            \
  "movq %rsp, %rcx\n"                           \
  "movq $1, %rdx\n"                             \
  "jmp carbon_coroutine__switch_ctx\n"

#define CARBON_COROUTINE__SLEEP_WRITE           \
  "movq %rcx, %r8\n"                            \
  "movq %rsp, %rcx\n"                           \
  "movq $2, %rdx\n"                             \
  "jmp carbon_coroutine__switch_ctx\n"

#define CARBON_COROUTINE__RESTORE_REGISTERS     \
  "movq %rcx, %rsp\n"                           \
  "popq %r15\n"                                 \
  "popq %r14\n"                                 \
  "popq %r13\n"                                 \
  "popq %r12\n"                                 \
  "popq %rsi\n"                                 \
  "popq %rdi\n"                                 \
  "popq %rbp\n"                                 \
  "popq %rbx\n"                                 \
  "popq %rcx\n"                                 \
  "ret\n"

#else
#error Target platform is not supported
#endif

#if defined(_WIN32)
#define CARBON_COROUTINE__ALLOC_STACK() VirtualAlloc(0, CARBON_COROUTINE__STACK_CAPACITY, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE)
#define CARBON_COROUTINE__ALLOC_STACK_FAILED 0
#elif defined(__APPLE__)
#define CARBON_COROUTINE__ALLOC_STACK() mmap(0, CARBON_COROUTINE__STACK_CAPACITY, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)
#define CARBON_COROUTINE__ALLOC_STACK_FAILED MAP_FAILED
#else
#define CARBON_COROUTINE__ALLOC_STACK() mmap(0, CARBON_COROUTINE__STACK_CAPACITY, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_STACK | MAP_ANONYMOUS | MAP_GROWSDOWN, -1, 0)
#define CARBON_COROUTINE__ALLOC_STACK_FAILED MAP_FAILED
#endif

typedef enum {
  CBN_COROUTINE_SLEEP_MODE_NONE,
  CBN_COROUTINE_SLEEP_MODE_READ,
  CBN_COROUTINE_SLEEP_MODE_WRITE
} CBN_Coroutine_SleepMode;

typedef struct {
  void *rsp;   // Stack pointer
  void *rsbp;  // Stack base pointer
} CBN_Coroutine_CTX;

static usz carbon_coroutine__current;
static CBN_List carbon_coroutine__active;  // CBN_List<usz>
static CBN_List carbon_coroutine__dead;    // CBN_List<usz>
static CBN_List carbon_coroutine__ctxs;    // CBN_List<CBN_Coroutine_CTX>
static CBN_List carbon_coroutine__asleep;  // CBN_List<usz>
static CBN_List carbon_coroutine__polls;   // CBN_List<struct pollfd>

#ifdef _WIN32
int getpagesize(void) {
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  return si.dwPageSize;
}
#endif

__attribute__((constructor)) CBNINL void carbon_coroutine__init(void) {
  carbon_coroutine__active = carbon_list_create(sizeof(usz));
  carbon_coroutine__dead   = carbon_list_create(sizeof(usz));
  carbon_coroutine__ctxs   = carbon_list_create(sizeof(CBN_Coroutine_CTX));
  carbon_coroutine__asleep = carbon_list_create(sizeof(usz));
#ifdef _WIN32
  CBN_WARN("Polling not implemented for Windows");
  carbon_coroutine__polls  = carbon_list_create(sizeof(char));
#else
  carbon_coroutine__polls  = carbon_list_create(sizeof(struct pollfd));
#endif
  CBN_Coroutine_CTX ctx = {0, 0};
  usz i = 0;
  carbon_list_push(&carbon_coroutine__ctxs, &ctx);
  carbon_list_push(&carbon_coroutine__active, &i);
}

__attribute__((destructor)) CBNINL void carbon_coroutine__shutdown(void) {
  carbon_list_destroy(&carbon_coroutine__active);
  carbon_list_destroy(&carbon_coroutine__dead);
  carbon_list_destroy(&carbon_coroutine__ctxs);
  carbon_list_destroy(&carbon_coroutine__asleep);
  carbon_list_destroy(&carbon_coroutine__polls);
}

__attribute__((naked)) void carbon_coroutine__restore_ctx(__attribute__((unused)) void *rsp) {
  __asm__ volatile (CARBON_COROUTINE__RESTORE_REGISTERS);
}

void carbon_coroutine__switch_ctx(void *rsp, CBN_Coroutine_SleepMode sm, i32 fd) {
  usz current_active_item = carbon_list_at(usz, carbon_coroutine__active, carbon_coroutine__current);
  carbon_list_at_raw(CBN_Coroutine_CTX, carbon_coroutine__ctxs, current_active_item).rsp = rsp;
  switch (sm) {
  case CBN_COROUTINE_SLEEP_MODE_NONE:
    ++carbon_coroutine__current;
    break;
  case CBN_COROUTINE_SLEEP_MODE_READ: {
#ifdef _WIN32
    CBN_WARN("Polling not implemented for Windows");
    CARBON_UNUSED(fd);
#else
    carbon_list_push(&carbon_coroutine__asleep, &current_active_item);
    struct pollfd pfd = {
      .fd = fd,
      .events = POLLRDNORM,
      .revents = 0
    };
    carbon_list_push(&carbon_coroutine__polls, &pfd);
    carbon_list_remove(&carbon_coroutine__active, carbon_coroutine__current);
#endif
  } break;
  case CBN_COROUTINE_SLEEP_MODE_WRITE: {
#ifdef _WIN32
    CBN_WARN("Polling not implemented for Windows");
    CARBON_UNUSED(fd);
#else
    carbon_list_push(&carbon_coroutine__asleep, &current_active_item);
    struct pollfd pfd = {
      .fd = fd,
      .events = POLLWRNORM,
      .revents = 0
    };
    carbon_list_push(&carbon_coroutine__polls, &pfd);
    carbon_list_remove(&carbon_coroutine__active, carbon_coroutine__current);
#endif
  } break;
  default: CARBON_UNREACHABLE;
  }
  if (carbon_coroutine__polls.size) {
#ifdef _WIN32
    CBN_WARN("Polling not implemented for Windows");
#else
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
#endif
  }
  CBN_ASSERT(carbon_coroutine__active.size);
  carbon_coroutine__current %= carbon_coroutine__active.size;
  current_active_item = carbon_list_at(usz, carbon_coroutine__active, carbon_coroutine__current);
  CBN_Coroutine_CTX current_ctx_item = carbon_list_at(CBN_Coroutine_CTX, carbon_coroutine__ctxs, current_active_item);
  carbon_coroutine__restore_ctx(current_ctx_item.rsp);
}

CBNINL void carbon_coroutine__finish_current(void) {
  usz current_active_item = carbon_list_at(usz, carbon_coroutine__active, carbon_coroutine__current);
  if (!current_active_item) CARBON_UNREACHABLE;
  carbon_list_push(&carbon_coroutine__dead, &current_active_item);
  carbon_list_remove(&carbon_coroutine__active, carbon_coroutine__current);
  if (carbon_coroutine__polls.size) {
#ifdef _WIN32
    CBN_WARN("Polling not implemented for Windows");
#else
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
#endif
  }
  CBN_ASSERT(carbon_coroutine__active.size);
  carbon_coroutine__current %= carbon_coroutine__active.size;
  current_active_item = carbon_list_at(usz, carbon_coroutine__active, carbon_coroutine__current);
  CBN_Coroutine_CTX current_ctx_item = carbon_list_at(CBN_Coroutine_CTX, carbon_coroutine__ctxs, current_active_item);
  carbon_coroutine__restore_ctx(current_ctx_item.rsp);
}

usz carbon_coroutine_id(void) {
  return carbon_list_at(usz, carbon_coroutine__active, carbon_coroutine__current);
}

usz carbon_coroutine_alive(void) {
  return carbon_coroutine__active.size;
}

void carbon_coroutine_go(void (*f)(void *), void *arg) {
  usz id;
  if (carbon_coroutine__dead.size) {
    --carbon_coroutine__dead.size;
    id = carbon_list_at(usz, carbon_coroutine__dead, carbon_coroutine__dead.size);
  }
  else {
    CBN_Coroutine_CTX ctx = {0, 0};
    carbon_list_push(&carbon_coroutine__ctxs, &ctx);
    id = carbon_coroutine__ctxs.size - 1;
    carbon_list_at_raw(CBN_Coroutine_CTX, carbon_coroutine__ctxs, id).rsbp = CARBON_COROUTINE__ALLOC_STACK();
    CBN_ASSERT(carbon_list_at(CBN_Coroutine_CTX, carbon_coroutine__ctxs, id).rsbp != CARBON_COROUTINE__ALLOC_STACK_FAILED);
  }
  void **rsp = (void **) ((u8 *) carbon_list_at_raw(CBN_Coroutine_CTX, carbon_coroutine__ctxs, id).rsbp + CARBON_COROUTINE__STACK_CAPACITY);
#if defined(__linux__) && (defined(__amd64__) || defined(_M_AMD64))
  *(--rsp) = (void *) carbon_coroutine__finish_current;
  *(--rsp) = (void *) f;
  *(--rsp) = arg;  // pushq %rdi
  *(--rsp) = 0;    // pushq %rbp
  *(--rsp) = 0;    // pushq %rbx
  *(--rsp) = 0;    // pushq %r12
  *(--rsp) = 0;    // pushq %r13
  *(--rsp) = 0;    // pushq %r14
  *(--rsp) = 0;    // pushq %r15
#elif defined(__APPLE__) && defined(__aarch64__)
  *(--rsp) = arg;  // x0
  *(--rsp) = (void *) carbon_coroutine__finish_current;
  *(--rsp) = (void *) f;
  *(--rsp) = 0;    // x19
  *(--rsp) = 0;    // x20
  *(--rsp) = 0;    // x21
  *(--rsp) = 0;    // x22
  *(--rsp) = 0;    // x23
  *(--rsp) = 0;    // x24
  *(--rsp) = 0;    // x25
  *(--rsp) = 0;    // x26
  *(--rsp) = 0;    // x27
  *(--rsp) = 0;    // x28
  *(--rsp) = 0;    // x29
  *(--rsp) = 0;    // x30
  *(--rsp) = 0;    // q8
  *(--rsp) = 0;    // q9
  *(--rsp) = 0;    // q10
  *(--rsp) = 0;    // q11
  *(--rsp) = 0;    // q12
  *(--rsp) = 0;    // q13
  *(--rsp) = 0;    // q14
  *(--rsp) = 0;    // q15
  *(--rsp) = 0;    // ???
  *(--rsp) = 0;    // ???
  *(--rsp) = 0;    // ???
  *(--rsp) = 0;    // ???
  *(--rsp) = 0;    // ???
  *(--rsp) = 0;    // ???
  *(--rsp) = 0;    // ???
#elif defined(_WIN32) && (defined(__amd64__) || defined(_M_AMD64))
  *(--rsp) = (void *) carbon_coroutine__finish_current;
  *(--rsp) = (void *) f;
  *(--rsp) = arg;  // pushq %rcx
  *(--rsp) = 0;    // pushq %rbx
  *(--rsp) = 0;    // pushq %rbp
  *(--rsp) = 0;    // pushq %rdi
  *(--rsp) = 0;    // pushq %rsi
  *(--rsp) = 0;    // pushq %r12
  *(--rsp) = 0;    // pushq %r13
  *(--rsp) = 0;    // pushq %r14
  *(--rsp) = 0;    // pushq %r15
#else
#error Target platform is not supported
#endif
  carbon_list_at_raw(CBN_Coroutine_CTX, carbon_coroutine__ctxs, id).rsp = rsp;
  carbon_list_push(&carbon_coroutine__active, &id);
}

__attribute__((naked)) void carbon_coroutine_yield(void) {
  __asm__ volatile (CARBON_COROUTINE__STORE_REGISTERS CARBON_COROUTINE__SLEEP_NONE);
}

__attribute__((naked)) void carbon_coroutine_sleep_read(__attribute__((unused)) i32 fd) {
  __asm__ volatile (CARBON_COROUTINE__STORE_REGISTERS CARBON_COROUTINE__SLEEP_READ);
}

__attribute__((naked)) void carbon_coroutine_sleep_write(__attribute__((unused)) i32 fd) {
  __asm__ volatile (CARBON_COROUTINE__STORE_REGISTERS CARBON_COROUTINE__SLEEP_WRITE);
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
