.data
bFoo byte 78
bFoo2 byte 80
wFoo3 word 0

; SDL variables
SDL_INIT_VIDEO qword 32

; Constants
WINDOW_WIDTH qword 640
WINDOW_HEIGHT qword 480

; Globals
bAppName byte "Snake", 0

.code

; SDL functions
extern SDL_Init : proc
extern SDL_CreateWindow : proc

main proc
	call InitSDL
	
	ret
main endp

InitSDL proc
	; Epilogue
	sub rsp, 32

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
	add rsp, 32

	; Prologue
	add rsp, 32

	ret
InitSDL endp

end