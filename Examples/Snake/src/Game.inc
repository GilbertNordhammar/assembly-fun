include SDL.inc
include Windows.inc
include CPlusPlus.inc
include ASM_Utils.inc

.data

; Constants
WINDOW_WIDTH equ 640
WINDOW_HEIGHT equ 480

COLOR_BUFFER_TEXTURE_WIDTH equ WINDOW_WIDTH
COLOR_BUFFER_TEXTURE_HEIGHT equ WINDOW_HEIGHT
COLOR_BUFFER_PIXEL_DEPTH equ 4
COLOR_BUFFER_ROW_BYTE_SIZE equ COLOR_BUFFER_TEXTURE_WIDTH * COLOR_BUFFER_PIXEL_DEPTH
COLOR_BUFFER_BYTE_SIZE equ COLOR_BUFFER_TEXTURE_WIDTH * COLOR_BUFFER_TEXTURE_HEIGHT * COLOR_BUFFER_PIXEL_DEPTH

SQUARE_STATE_INACTIVE equ 0
SQUARE_STATE_ACTIVE equ 1
SNAKE_DIRECTION_UP equ 00b
SNAKE_DIRECTION_RIGHT equ 01b
SNAKE_DIRECTION_DOWN equ 10b
SNAKE_DIRECTION_LEFT equ 11b

SQUARE_BYTE_SIZE equ sizeof byte
GRID_WIDTH equ 20
GRID_HEIGHT equ 20
GRID_BYTE_SIZE equ GRID_WIDTH * GRID_HEIGHT * SQUARE_BYTE_SIZE

; Types
Snake struct
	qwSquareIndicesListPtr dw 0
	bLength db 0
	bDirection db SNAKE_DIRECTION_UP
Snake ends

SNAKE_BYTE_SIZE equ sizeof Snake

; Variables
bAppNameStr byte "Snake", 0
qwWindowPtr qword 0
qwRendererPtr qword 0
qwColorBufferSDLTexturePtr qword 0
qwColorBufferPtr qword 0
qwSquareListPtr qword 0

;evnt SDL_Event <>
playerSnake Snake { 0, 1, SNAKE_DIRECTION_UP }

.code
; Functions

RunGameLoop proc
	FRAME_PROLOGUE

	STACK_ALLOCATE SDL_Event_BYTE_SIZE
	
	mov r13, rax ; SDLevent
	lea r14, playerSnake

	GameLoop:
		; Render stuff
		mov rcx, qwRendererPtr 
		CALL_C_FUNC SDL_RenderClear
		
		mov rcx, qwRendererPtr 
		mov rdx, qwColorBufferSDLTexturePtr
		mov r8, 0
		mov r9, 0
		CALL_C_FUNC SDL_RenderTexture
		
		mov rcx, qwRendererPtr
		CALL_C_FUNC SDL_RenderPresent
		
		; Poll SDL event
		mov rcx, r13
		CALL_C_FUNC SDL_WaitEvent
		
		mov eax, [r13].SDL_KeyboardEvent.eventType
		mov ecx, [r13].SDL_KeyboardEvent.keysym.sym
		mov r8d, [r13].SDL_KeyboardEvent.eventType

		cmp eax, SDL_EVENT_QUIT
			je ExitGameLoop
		cmp eax, SDL_EVENT_KEY_DOWN
			jne GameLoop
			
			cmp ecx, SDL_KEYCODE_W
				mov r15b, SNAKE_DIRECTION_UP 
				je EndOfInputPolling
			cmp ecx, SDL_KEYCODE_S
				mov r15b, SNAKE_DIRECTION_DOWN
				je EndOfInputPolling
			cmp ecx, SDL_KEYCODE_A
				mov r15b, SNAKE_DIRECTION_RIGHT
				je EndOfInputPolling
			cmp ecx, SDL_KEYCODE_D
				mov r15b, SNAKE_DIRECTION_LEFT
				je EndOfInputPolling
			
			EndOfInputPolling:
				mov [r14].Snake.bDirection, r15b

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
	mov r9, COLOR_BUFFER_TEXTURE_WIDTH
	
	push COLOR_BUFFER_TEXTURE_HEIGHT
	CALL_C_FUNC SDL_CreateTexture
	sub rsp, 8

	mov qwColorBufferSDLTexturePtr, rax
	mov r13, rax ; SDL texture ptr

	; Allocate pixel data for snake piece texture
	mov rcx, COLOR_BUFFER_BYTE_SIZE
	CALL_C_FUNC malloc
	mov qwColorBufferPtr, rax
	mov r12, rax ; Color buffer ptr

	; Initialize color buffer
	;xor rsi, rsi
	mov rdi, r12 ; destination
	mov eax, 0FF00FFFFh ; source
	mov ecx, COLOR_BUFFER_BYTE_SIZE / 4 ; Number of dwords to write
	cld
	rep stosd

	; Update SDL texture with data
	mov rcx, r13 ; SDL texture ptr
	mov rdx, 0
	mov r8, r12 ; Color buffer ptr
	mov r9, COLOR_BUFFER_ROW_BYTE_SIZE
	CALL_C_FUNC SDL_UpdateTexture

	; Allocate grid
	mov rcx, GRID_BYTE_SIZE
	CALL_C_FUNC malloc
	mov qwSquareListPtr, rax

	FRAME_EPILOGUE
	ret
CreateGameResources endp

DestroyGameResources proc
	FRAME_PROLOGUE

	; Deallocate pixel data
	mov rcx, qwColorBufferPtr
	CALL_C_FUNC free

	mov rcx, qwSquareListPtr
	CALL_C_FUNC free

	; TODO: Destroy SDL resources

	FRAME_EPILOGUE
	ret
DestroyGameResources endp

InitSDL proc
	FRAME_PROLOGUE

	mov rcx, SDL_INIT_VIDEO
	CALL_C_FUNC SDL_Init

	lea rcx, [bAppNameStr]
	mov rdx, WINDOW_WIDTH
	mov r8, WINDOW_HEIGHT
	mov r9, 0
	CALL_C_FUNC SDL_CreateWindow
	mov qwWindowPtr, rax

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

	mov rcx, qwWindowPtr
	CALL_C_FUNC SDL_DestroyWindow

	CALL_C_FUNC SDL_Quit
	
	FRAME_EPILOGUE

	ret
QuitSDL endp