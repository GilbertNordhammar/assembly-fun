; ===== SDL =====

.data
; SDL constants
SDL_INIT_VIDEO equ 32
SDL_PIXELFORMAT_ARGB8888 equ 372645892
SDL_RENDERER_ACCELERATED equ 2
SDL_RENDERER_PRESENTVSYNC equ 4
SDL_TEXTUREACCESS_STATIC equ 0

.code
; SDL functions
extern SDL_CreateWindow : proc
extern SDL_CreateRenderer : proc
extern SDL_CreateTexture : proc
extern SDL_Delay : proc
extern SDL_DestroyRenderer : proc
extern SDL_DestroyWindow : proc
extern SDL_Init : proc
extern SDL_RenderClear : proc
extern SDL_RenderTexture : proc
extern SDL_RenderPresent : proc
extern SDL_SetRenderDrawColor : proc
extern SDL_UpdateTexture : proc
extern SDL_Quit : proc

; ===== Windows OS =====
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

; ===== C/C++ =====
extern malloc : proc
extern free : proc

extern Foo : proc

.code
; Utility macros

ALLOCATE_SHADOW_SPACE macro
	sub rsp, 40 ; Allocate 32 bytes shadow space + 8 bytes for 16-byte aligning stack from PROC_PROLOGUE
endm

DEALLOCATE_SHADOW_SPACE macro
	add rsp, 40 ; Deallocate 32 bytes shadow space + 8 bytes for 16-byte aligning stack from PROC_PROLOGUE
endm

PROC_PROLOGUE macro
	push rbp
	mov rbp, rsp
endm

PROC_EPILOGUE macro
	mov rsp, rbp ; Pops all local stack variables
	pop rbp
endm

CALL_PROC macro func:REQ
	ALLOCATE_SHADOW_SPACE
	call func
	DEALLOCATE_SHADOW_SPACE
endm

CALL_PROC_ARG1 macro func:REQ, arg0:REQ
	mov rcx, arg0
	CALL_PROC func
endm

CALL_PROC_ARG2 macro func:REQ, arg0:REQ, arg1:REQ
	mov rcx, arg0
	mov rdx, arg1
	CALL_PROC func
endm

CALL_PROC_ARG3 macro func:REQ, arg0:REQ, arg1:REQ, arg2:REQ
	mov rcx, arg0
	mov rdx, arg1
	mov r8d, arg2
	CALL_PROC func
endm

CALL_PROC_ARG4 macro func:REQ, arg0:REQ, arg1:REQ, arg2:REQ, arg3:REQ
	mov rcx, arg0
	mov rdx, arg1
	mov r8d, arg2
	mov r9d, arg3
	CALL_PROC func
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

format0 db "Number of processors: %d retlen: 0x%x retval: 0x%x", 13, 10, 0

; ===== Game =====
.data

; Constants
WINDOW_WIDTH equ 640
WINDOW_HEIGHT equ 480

SNAKE_PIECE_TEXTURE_WIDTH equ 200
SNAKE_PIECE_TEXTURE_HEIGHT equ 200
SNAKE_PIECE_TEXTURE_PIXEL_DEPTH equ 4
SNAKE_PIECE_TEXTURE_ROW_BYTE_SIZE equ SNAKE_PIECE_TEXTURE_WIDTH * SNAKE_PIECE_TEXTURE_PIXEL_DEPTH
SNAKE_PIECE_TEXTURE_BYTE_SIZE equ SNAKE_PIECE_TEXTURE_WIDTH * SNAKE_PIECE_TEXTURE_HEIGHT * SNAKE_PIECE_TEXTURE_PIXEL_DEPTH


; Variables
bAppName byte "Snake", 0
bWindowPtr qword 0
qwRendererPtr qword 0
bSnakePieceTexturePtr qword 0
qwSnakePieceTextureBufferPtr qword 0

.code
; Functions

main proc
	call InitSDL

	call CreateGameResources

	call RunGameLoop

	call DestroyGameResources
	call QuitSDL

	ret
main endp

RunGameLoop proc
	PROC_PROLOGUE

	; Render stuff
	mov rcx, qwRendererPtr 
	CALL_PROC SDL_RenderClear

	;mov rdx, qwSnakePieceTextureBufferPtr
	;mov r8, 0
	;mov r9, 0
	;CALL_PROC SDL_RenderTexture

	mov rcx, qwRendererPtr
	CALL_PROC SDL_RenderPresent

	; Pause execution
	sub rsp, 32
	mov rcx, 3000
	call SDL_Delay
	add rsp, 32

	PROC_EPILOGUE
	ret
RunGameLoop endp

CreateGameResources proc
	PROC_PROLOGUE

	; Create SDL texture
	push SNAKE_PIECE_TEXTURE_HEIGHT
	CALL_PROC_ARG4 SDL_CreateTexture, qwRendererPtr, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SNAKE_PIECE_TEXTURE_WIDTH
	pop r8
	mov bSnakePieceTexturePtr, rax
	mov r11, rax

	; Allocate pixel data for snake piece texture
	CALL_PROC_ARG1 malloc, SNAKE_PIECE_TEXTURE_BYTE_SIZE
	mov qwSnakePieceTextureBufferPtr, rax

	; Update texture with data
	mov rcx, rax
	mov rdx, 0
	mov r8, r11
	mov r9, SNAKE_PIECE_TEXTURE_ROW_BYTE_SIZE
	CALL_PROC SDL_UpdateTexture

	PROC_EPILOGUE
	ret
CreateGameResources endp

DestroyGameResources proc
	PROC_PROLOGUE

	; Deallocate pixel data
	CALL_PROC_ARG1 free, qwSnakePieceTextureBufferPtr

	PROC_EPILOGUE
	ret
DestroyGameResources endp

InitSDL proc
	PROC_PROLOGUE

	mov rcx, SDL_INIT_VIDEO
	CALL_PROC SDL_Init

	lea rcx, [bAppName]
	mov rdx, WINDOW_WIDTH
	mov r8, WINDOW_HEIGHT
	mov r9, 0
	CALL_PROC SDL_CreateWindow
	mov bWindowPtr, rax

	mov rcx, rax
	mov rdx, 0
	mov r8, SDL_RENDERER_ACCELERATED
	or r8, SDL_RENDERER_PRESENTVSYNC
	CALL_PROC SDL_CreateRenderer
	mov qwRendererPtr, rax

	mov rcx, rax
	mov edx, 255
	mov r8, 0
	mov r9, 0
	push 255
	CALL_PROC SDL_SetRenderDrawColor
	pop r8

	PROC_EPILOGUE
	ret
InitSDL endp

QuitSDL proc
	PROC_PROLOGUE

	mov rcx, qwRendererPtr
	CALL_PROC SDL_DestroyRenderer

	mov rcx, bWindowPtr
	CALL_PROC SDL_DestroyWindow

	CALL_PROC SDL_Quit
	
	PROC_EPILOGUE

	ret
QuitSDL endp

end