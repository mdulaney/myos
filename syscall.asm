[BITS 64]

global _syscall_enter
global _syscall_leave

extern _scheduler_syscall

struc _trap_frame
    .r_rip: resq 1
    .r_rdi: resq 1
    .r_rsi: resq 1
    .r_rdx: resq 1
	  .r_rcx: resq 1

	  .r_r8: resq 1
	  .r_r9: resq 1
	  .r_rax: resq 1
	  .r_rbx: resq 1

	  .r_rbp: resq 1
	  .r_rsp: resq 1
	  .r_r10: resq 1
	  .r_r11: resq 1
	  .r_r12: resq 1

	  .r_r13: resq 1
	  .r_r14: resq 1
	  .r_r15: resq 1
endstruc

_global_thunk:
    dq 0x0

_scheduler_trap_frame:
    istruc _trap_frame
    iend

_pending_trap_frame_addr:
    dq 0x0

_pending_task_control_block:
    dq 0x0

_scheduler_enter_rip:
; _pending_trap_frame_addr
; TODO: Need to populate the pending trap frame into the task data structure
; Need to specify the task ID In the system call
    call _scheduler_do_round

; rcx = the memory location for storing the register context
_save_register_ctx:
	  mov	[_trap_frame.r_r15+rcx], r15
	  mov	[_trap_frame.r_r14+rcx], r14
	  mov	[_trap_frame.r_r13+rcx], r13
	  mov	[_trap_frame.r_r12+rcx], r12
	  mov	[_trap_frame.r_r11+rcx], r11
	  mov	[_trap_frame.r_r10+rcx], r10
	  mov	[_trap_frame.r_rbp+rcx], rbp
	  mov	[_trap_frame.r_rbx+rcx], rbx
	  mov	[_trap_frame.r_rax+rcx], rax
	  mov	[_trap_frame.r_r9 +rcx], r9
	  mov	[_trap_frame.r_r8 +rcx], r8
	  mov	[_trap_frame.r_rcx+rcx], rcx
	  mov	[_trap_frame.r_rdx+rcx], rdx
	  mov	[_trap_frame.r_rsi+rcx], rsi
	  mov	[_trap_frame.r_rdi+rcx], rdi
    mov [_trap_frame.r_rsp+rcx], rsp
    retn

; rdx - pointer to the task structure for the current task
; rcx - the sys call number
_syscall_enter:
    sub rsp, _trap_frame_size
    mov [_pending_task_control_block], rdx
    mov rcx,rsp
    mov [_pending_trap_frame_addr], rcx
    call _save_register_ctx
    mov rax, [_scheduler_enter_rip]
    mov [_global_thunk], rax
    mov rcx, _scheduler_trap_frame
    call _restore_register_ctx
    mov rcx, [_pending_task_control_block]
    jmp [_global_thunk]

; rcx - contains the location of the context to restore
_syscall_leave:
    call _restore_register_ctx
    add rsp, _trap_frame_size
    retn

; rcx - the region of memory from which to restore the register state
_restore_register_ctx:
	  mov	rdi, [_trap_frame.r_rdi+rcx]
	  mov	rsi, [_trap_frame.r_rsi+rcx]
	  mov	rdx, [_trap_frame.r_rdx+rcx]
	  mov	r8,  [_trap_frame.r_r8+rcx]
	  mov	r9,  [_trap_frame.r_r9+rcx]
	  mov	rax, [_trap_frame.r_rax+rcx]
	  mov	rbx, [_trap_frame.r_rbx+rcx]
	  mov	rbp, [_trap_frame.r_rbp+rcx]
	  mov	rsp, [_trap_frame.r_rsp+rcx]
	  mov	r10, [_trap_frame.r_r10+rcx]
	  mov	r11, [_trap_frame.r_r11+rcx]
	  mov	r12, [_trap_frame.r_r12+rcx]
	  mov	r13, [_trap_frame.r_r13+rcx]
	  mov	r14, [_trap_frame.r_r14+rcx]
	  mov	r15, [_trap_frame.r_r15+rcx]
	  mov	rcx, [_trap_frame.r_rcx+rcx]
    retn
