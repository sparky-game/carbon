.align 2

.global carbon_coroutine_yield
.global carbon_coroutine_sleep_read
.global carbon_coroutine_sleep_write
.global carbon_coroutine_restore_ctx

.extern carbon_coroutine_switch_ctx

carbon_coroutine_yield:
  stp x30, x0, [sp, #-16]!
  stp x1, x2, [sp, #-16]!
  stp x3, x4, [sp, #-16]!
  stp x5, x6, [sp, #-16]!
  stp x7, x8, [sp, #-16]!
  mov x0, sp
  mov x1, #0
  b carbon_coroutine_switch_ctx

carbon_coroutine_sleep_read:
  stp x30, x0, [sp, #-16]!
  stp x1, x2, [sp, #-16]!
  stp x3, x4, [sp, #-16]!
  stp x5, x6, [sp, #-16]!
  stp x7, x8, [sp, #-16]!
  mov x2, x0
  mov x0, sp
  mov x1, #1
  b carbon_coroutine_switch_ctx

carbon_coroutine_sleep_write:
  stp x30, x0, [sp, #-16]!
  stp x1, x2, [sp, #-16]!
  stp x3, x4, [sp, #-16]!
  stp x5, x6, [sp, #-16]!
  stp x7, x8, [sp, #-16]!
  mov x2, x0
  mov x0, sp
  mov x1, #2
  b carbon_coroutine_switch_ctx

carbon_coroutine_restore_ctx:
  ldp x7, x8, [sp], #16
  ldp x5, x6, [sp], #16
  ldp x3, x4, [sp], #16
  ldp x1, x2, [sp], #16
  ldp x30, x0, [sp], #16
  mov sp, x0
  ret
