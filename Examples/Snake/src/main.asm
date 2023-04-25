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
	CALL_PROC InitSDL
	
	;mov rcx, 3000
	;CALL_PROC SDL_Delay

	sub rsp, 8
	call QuitSDL
	add rsp, 8

	ret
main endp

InitSDL proc
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
	mov bRendererPtr, rax

	ret
InitSDL endp

QuitSDL proc
	sub rsp, 8
	mov rcx, bRendererPtr
	call SDL_DestroyRenderer
	add rsp, 8

	sub rsp, 8
	mov rcx, bWindowPtr
	call SDL_DestroyWindow
	add rsp, 8

	sub rsp, 8
	call SDL_Quit
	add rsp, 8
	
	ret
QuitSDL endp

end