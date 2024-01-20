
; Defines
.data
TRUE equ 1
FALSE equ 0

; Utility macros

STACK_ALLOCATE macro size:REQ
	sub rsp, size
	mov rax, rsp
endm

STACK_DEALLOCATE macro size:REQ
	add rsp, size
endm

FRAME_PROLOGUE macro
	push rbp
	mov rbp, rsp
endm

FRAME_EPILOGUE macro
	mov rsp, rbp ; Pops all local stack variables
	pop rbp
endm

CALL_C_FUNC macro func:REQ
	mov r12, rsp ; Save old stack pointer
	and rsp, 0fffffff0h ; 16-bit align stack pointer
	sub rsp, 32 ; Allocate shadow space
	call func
	mov rsp, r12 ; Restore stack pointer
endm

SYSCALL_GET_SYSTEM_BASIC_INFO macro
	_SYSTEM_BASIC_INFORMATION struct 8
		Reserved1 byte 24 dup (?)
		Reserved2 qword 4 dup (?)
		NumberOfProcessors sbyte ?
	_SYSTEM_BASIC_INFORMATION ends

	local basicinfo : _SYSTEM_BASIC_INFORMATION
	local retlen : qword
	mov retlen, 0

	;sub rsp, 28h
	
	mov eax, 36h ; NtQuerySystemInformation
	mov r10, 0 ; System information class (SystemBasicInformation)
	lea rdx, basicinfo
	mov r8, sizeof basicinfo
	lea r9, retlen

	;add rsp, 28h

	syscall
endm

SYSCALL_ALLOCATE_VIRTUAL_MEMORY macro
	;mov r10,rcx
	;mov rax, NtAllocateVirtualMemory ; System call number
	;mov rcx, -1 ; Process handle (-1 = current process)
	;lea rdx, bMemory ; Pointer to base address (allocated by system)
	;mov r8, 0 ; Zero-initialize region size
	;mov r9, MEM_COMMIT ; Allocation type
	;or r9, MEM_RESERVE
	;syscall
endm