; ===== SDL =====

.data
; SDL constants
SDL_INIT_VIDEO equ 32
SDL_RENDERER_ACCELERATED equ 2
SDL_RENDERER_PRESENTVSYNC equ 4

.code
; SDL functions
extern SDL_CreateWindow : proc
extern SDL_CreateRenderer : proc
extern SDL_Delay : proc
extern SDL_DestroyRenderer : proc
extern SDL_DestroyWindow : proc
extern SDL_Init : proc
extern SDL_Quit : proc

; ===== Program =====
.data

; Macros
CALL_PROC macro func:REQ
	; Prologue
	sub rsp, 32

	call func

	; Epilogue
	add rsp, 32
endm



; Constants
WINDOW_WIDTH equ 640
WINDOW_HEIGHT equ 480

; Variables
bAppName byte "Snake", 0
bWindowPtr qword 0
bRendererPtr qword 0

.code
; Functions

main proc
	call InitSDL
	
	sub rsp, 32
	mov rcx, 100
	call SDL_Delay
	add rsp, 32

	call QuitSDL

	ret
main endp

InitSDL proc
	mov rax, [rsp + 8]

	sub rsp, 32
	mov rcx, SDL_INIT_VIDEO
	call SDL_Init
	add rsp, 32

	sub rsp, 32
	lea rcx, [bAppName]
	mov rdx, WINDOW_WIDTH
	mov r8, WINDOW_HEIGHT
	mov r9, 0
	call SDL_CreateWindow
	mov bWindowPtr, rax
	add rsp, 32

	sub rsp, 32
	mov rcx, rax
	mov rdx, 0
	mov r8, SDL_RENDERER_ACCELERATED
	or r8, SDL_RENDERER_PRESENTVSYNC
	call SDL_CreateRenderer
	mov bRendererPtr, rax
	add rsp, 32

	ret
InitSDL endp

QuitSDL proc
	sub rsp, 16
	mov rcx, bRendererPtr
	call SDL_DestroyRenderer
	add rsp, 16

	sub rsp, 8
	mov rcx, bWindowPtr
	call SDL_DestroyWindow
	add rsp, 8

	sub rsp, 16
	call SDL_Quit
	add rsp, 16
	
	ret
QuitSDL endp

end