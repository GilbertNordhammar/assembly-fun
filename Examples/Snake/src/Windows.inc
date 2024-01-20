.code
; Windows funtions
extern __chkstk : proc

.data
; Windows system calls
NtAllocateVirtualMemory equ 0x18
NtQuerySystemInformation equ 0x36

; Windows constants
NT_SUCCESS equ 0
STATUS_INFO_LENGTH_MISMATCH equ 0xC0000004
MEM_COMMIT equ 0x00001000
MEM_RESERVE equ 0x00002000 
PAGE_READWRITE equ 0x04