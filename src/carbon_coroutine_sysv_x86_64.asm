section .text

global carbon_coroutine_yield
global carbon_coroutine_sleep_read
global carbon_coroutine_sleep_write
global carbon_coroutine_restore_ctx

extern carbon_coroutine_switch_ctx

carbon_coroutine_yield:
  push rdi
  push rbp
  push rbx
  push r12
  push r13
  push r14
  push r15
  mov rdi, rsp
  mov rsi, 0
  jmp carbon_coroutine_switch_ctx

carbon_coroutine_sleep_read:
  push rdi
  push rbp
  push rbx
  push r12
  push r13
  push r14
  push r15
  mov rdx, rdi
  mov rdi, rsp
  mov rsi, 1
  jmp carbon_coroutine_switch_ctx

carbon_coroutine_sleep_write:
  push rdi
  push rbp
  push rbx
  push r12
  push r13
  push r14
  push r15
  mov rdx, rdi
  mov rdi, rsp
  mov rsi, 2
  jmp carbon_coroutine_switch_ctx

carbon_coroutine_restore_ctx:
  mov rsp, rdi
  pop r15
  pop r14
  pop r13
  pop r12
  pop rbx
  pop rbp
  pop rdi
  ret
