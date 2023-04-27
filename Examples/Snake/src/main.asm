; ===== SDL =====

.data
; SDL constants
SDL_EVENT_KEY_DOWN equ 768
SDL_EVENT_QUIT equ 256
SDL_INIT_VIDEO equ 32
SDL_KEYCODE_A equ 97
SDL_KEYCODE_E equ 101
SDL_KEYCODE_D equ 100
SDL_KEYCODE_S equ 115
SDL_KEYCODE_W equ 119
SDL_PIXELFORMAT_ARGB8888 equ 372645892
SDL_RENDERER_ACCELERATED equ 2
SDL_RENDERER_PRESENTVSYNC equ 4
SDL_TEXTUREACCESS_STATIC equ 0

; SDL types
SDL_Keysym struct
	scancode dd 0 ; SDL physical key code - see ::SDL_Scancode for details
	sym dd 0 ; SDL virtual key code - see ::SDL_Keycode for details
	modifier dw 0 ; current key modifiers
	unused dd 0

SDL_Keysym ends

SDL_KeyboardEvent struct
	eventType dd 0   ; ::SDL_EVENT_KEY_DOWN or ::SDL_EVENT_KEY_UP
	db 4 dup(0)
    timestamp dq 0   ; In nanoseconds, populated using SDL_GetTicksNS()
    windowID dd 0    ; The window with keyboard focus, if any
    state db 0        ; ::SDL_PRESSED or ::SDL_RELEASED
    repeatKey db 0       ; Non-zero if this is a key repeat
    padding2 db 0
    padding3 db 0
	keysym SDL_Keysym {} ; The key that was pressed or released
SDL_KeyboardEvent ends

SDL_Event union
	eventType dd 0
	key SDL_KeyboardEvent {}
	byte 128 dup(0)
SDL_Event ends

SDL_Event_BYTE_SIZE equ 128

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
extern SDL_WaitEvent : proc

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
.code
extern malloc : proc
extern free : proc

extern Foo : proc

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

ALLOCATE_SHADOW_SPACE macro
	sub rsp, 40 ; Allocate 32 bytes shadow space + 8 bytes for 16-byte aligning stack from FRAME_PROLOGUE
endm

DEALLOCATE_SHADOW_SPACE macro
	add rsp, 40 ; Deallocate 32 bytes shadow space + 8 bytes for 16-byte aligning stack from FRAME_PROLOGUE
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
	ALLOCATE_SHADOW_SPACE
	call func
	DEALLOCATE_SHADOW_SPACE
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
qwSDLEventPtr qword 0

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
	FRAME_PROLOGUE

	STACK_ALLOCATE SDL_Event_BYTE_SIZE
	mov r14, rax

	GameLoop:
		; Render stuff
		mov rcx, qwRendererPtr 
		CALL_C_FUNC SDL_RenderClear
		
		mov rdx, qwSnakePieceTextureBufferPtr
		mov r8, 0
		mov r9, 0
		CALL_C_FUNC SDL_RenderTexture
		
		mov rcx, qwRendererPtr
		CALL_C_FUNC SDL_RenderPresent
		
		; Poll SDL event
		mov rcx, r14
		CALL_C_FUNC SDL_WaitEvent
		
		mov eax, [r14].SDL_KeyboardEvent.eventType
		mov ecx, [r14].SDL_KeyboardEvent.keysym.sym
		mov r8d, [r14].SDL_KeyboardEvent.eventType

		cmp eax, SDL_EVENT_QUIT
			je ExitGameLoop
		cmp eax, SDL_EVENT_KEY_DOWN
			jne GameLoop
			cmp ecx, SDL_KEYCODE_W
				je ExitGameLoop
			cmp ecx, SDL_KEYCODE_S
				je ExitGameLoop
			cmp ecx, SDL_KEYCODE_A
				je ExitGameLoop
			cmp ecx, SDL_KEYCODE_D
				je ExitGameLoop

		jmp GameLoop
	ExitGameLoop:

	FRAME_EPILOGUE
	
	ret
RunGameLoop endp

CreateGameResources proc
	FRAME_PROLOGUE

	; Create SDL texture
	mov rcx, qwRendererPtr
	mov rdx, SDL_PIXELFORMAT_ARGB8888
	mov r8, SDL_TEXTUREACCESS_STATIC
	mov r9, SNAKE_PIECE_TEXTURE_WIDTH
	push SNAKE_PIECE_TEXTURE_HEIGHT
	CALL_C_FUNC SDL_CreateTexture
	pop r8
	mov bSnakePieceTexturePtr, rax
	mov r11, rax

	; Allocate pixel data for snake piece texture
	mov rcx, SNAKE_PIECE_TEXTURE_BYTE_SIZE
	CALL_C_FUNC malloc, SNAKE_PIECE_TEXTURE_BYTE_SIZE
	mov qwSnakePieceTextureBufferPtr, rax

	; Update texture with data
	mov rcx, rax
	mov rdx, 0
	mov r8, r11
	mov r9, SNAKE_PIECE_TEXTURE_ROW_BYTE_SIZE
	CALL_C_FUNC SDL_UpdateTexture

	FRAME_EPILOGUE
	ret
CreateGameResources endp

DestroyGameResources proc
	FRAME_PROLOGUE

	; Deallocate pixel data
	mov rcx, qwSnakePieceTextureBufferPtr
	CALL_C_FUNC free, qwSnakePieceTextureBufferPtr

	FRAME_EPILOGUE
	ret
DestroyGameResources endp

InitSDL proc
	FRAME_PROLOGUE

	mov rcx, SDL_INIT_VIDEO
	CALL_C_FUNC SDL_Init

	lea rcx, [bAppName]
	mov rdx, WINDOW_WIDTH
	mov r8, WINDOW_HEIGHT
	mov r9, 0
	CALL_C_FUNC SDL_CreateWindow
	mov bWindowPtr, rax

	mov rcx, rax
	mov rdx, 0
	mov r8, SDL_RENDERER_ACCELERATED
	or r8, SDL_RENDERER_PRESENTVSYNC
	CALL_C_FUNC SDL_CreateRenderer
	mov qwRendererPtr, rax

	mov rcx, rax
	mov edx, 255
	mov r8, 0
	mov r9, 0
	push 255
	CALL_C_FUNC SDL_SetRenderDrawColor
	pop r8

	FRAME_EPILOGUE
	ret
InitSDL endp

QuitSDL proc
	FRAME_PROLOGUE

	mov rcx, qwRendererPtr
	CALL_C_FUNC SDL_DestroyRenderer

	mov rcx, bWindowPtr
	CALL_C_FUNC SDL_DestroyWindow

	CALL_C_FUNC SDL_Quit
	
	FRAME_EPILOGUE

	ret
QuitSDL endp

end