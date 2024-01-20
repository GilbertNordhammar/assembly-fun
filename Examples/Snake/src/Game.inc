include SDL.inc
include Windows.inc
include CPlusPlus.inc
include ASM_Utils.inc

.data

; Constants
WINDOW_PIXEL_WIDTH equ 640
WINDOW_PIXEL_HEIGHT equ 640

COLOR_BUFFER_PIXEL_WIDTH equ WINDOW_PIXEL_WIDTH
COLOR_BUFFER_PIXEL_HEIGHT equ WINDOW_PIXEL_HEIGHT
COLOR_BUFFER_PIXEL_SIZE equ COLOR_BUFFER_PIXEL_WIDTH * COLOR_BUFFER_PIXEL_HEIGHT
COLOR_BUFFER_PIXEL_DEPTH equ 4
COLOR_BUFFER_ROW_BYTE_SIZE equ COLOR_BUFFER_PIXEL_WIDTH * COLOR_BUFFER_PIXEL_DEPTH
COLOR_BUFFER_BYTE_SIZE equ COLOR_BUFFER_PIXEL_WIDTH * COLOR_BUFFER_PIXEL_HEIGHT * COLOR_BUFFER_PIXEL_DEPTH

SQUARE_STATE_INACTIVE equ 0
SQUARE_STATE_ACTIVE equ 1
SNAKE_DIRECTION_UP equ 00b
SNAKE_DIRECTION_RIGHT equ 01b
SNAKE_DIRECTION_DOWN equ 10b
SNAKE_DIRECTION_LEFT equ 11b

CELL_PIXEL_WIDTH equ 20
CELL_PIXEL_HEIGHT equ 20
CELL_PIXEL_WIDTH_BYTE_SIZE equ CELL_PIXEL_WIDTH * COLOR_BUFFER_PIXEL_DEPTH
CELL_PIXEL_BYTE_SIZE equ CELL_PIXEL_WIDTH * CELL_PIXEL_HEIGHT * COLOR_BUFFER_PIXEL_DEPTH

SQUARE_BYTE_SIZE equ sizeof byte
GRID_CELL_WIDTH equ COLOR_BUFFER_PIXEL_WIDTH / CELL_PIXEL_WIDTH
GRID_CELL_HEIGHT equ COLOR_BUFFER_PIXEL_HEIGHT / CELL_PIXEL_HEIGHT
GRID_BYTE_SIZE equ GRID_CELL_WIDTH * GRID_CELL_HEIGHT * SQUARE_BYTE_SIZE
GRID_CELL_ROW_PIXEL_BYTE_SIZE equ GRID_CELL_WIDTH * CELL_PIXEL_BYTE_SIZE


CLEAR_COLOR equ 0FF000000h
SNAKE_COLOR equ 0FFFFFFFFh

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
qwGridPtr qword 0

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
		mov r11, qwColorBufferPtr
		mov r12, qwColorBufferSDLTexturePtr

		; Clear color buffer
		mov rdi, r11 ; destination
		mov eax, CLEAR_COLOR ; source
		mov ecx, COLOR_BUFFER_PIXEL_SIZE ; Number of dwords to write
		cld ; Set direction to forwards
		rep stosd

		mov ecx, 2 ; Row index
		mov edx, 2; Column index
		mov r8d, SNAKE_COLOR
		call ColorCell

		; Present color buffer to screen
		mov rcx, r12 ; SDL texture ptr
		mov rdx, 0
		mov r8, r11 ; Color buffer ptr
		mov r9, COLOR_BUFFER_ROW_BYTE_SIZE
		CALL_C_FUNC SDL_UpdateTexture

		; Render frame
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
				mov r9b, SNAKE_DIRECTION_UP
				cmove r15d, r9d
				je EndOfInputPolling
			cmp ecx, SDL_KEYCODE_S
				mov r9b, SNAKE_DIRECTION_DOWN
				cmove r15d, r9d
				je EndOfInputPolling
			cmp ecx, SDL_KEYCODE_A
				mov r9b, SNAKE_DIRECTION_LEFT
				cmove r15d, r9d
				je EndOfInputPolling
			cmp ecx, SDL_KEYCODE_D
				mov r9b, SNAKE_DIRECTION_RIGHT
				cmove r15d, r9d
				je EndOfInputPolling
			
			EndOfInputPolling:
				mov [r14].Snake.bDirection, r15b
				jmp GameLoop
	ExitGameLoop:

	FRAME_EPILOGUE
	
	ret
RunGameLoop endp

; ecx = cell row index, edx = cell column index, r8d = color
ColorCell proc
	FRAME_PROLOGUE

	push r11
	push r12

	mov r11, qwColorBufferPtr

	; Add pixel offset for column
	mov eax, CELL_PIXEL_WIDTH_BYTE_SIZE
	mul edx
	add r11, rax

	; Add pixel offset for row
	mov eax, GRID_CELL_ROW_PIXEL_BYTE_SIZE
	mul ecx
	add r11, rax

	mov r12, CELL_PIXEL_HEIGHT
	mov eax, r8d ; color value (source)
	cld ; Set direction to forwards
	ColorCellRow:
		mov rdi, r11 ; offsetet color buffer ptr (destination)
		mov ecx, CELL_PIXEL_WIDTH ; Number of pixels (dw) to write
		rep stosd ; Copy color into cell row pixels

		add r11, COLOR_BUFFER_ROW_BYTE_SIZE ; Increment pixel row

		dec r12
		jnz ColorCellRow

	pop r12
	pop r11

	FRAME_EPILOGUE
	ret
ColorCell endp

CreateGameResources proc
	FRAME_PROLOGUE

	; Create SDL texture
	mov rcx, qwRendererPtr
	mov rdx, SDL_PIXELFORMAT_ARGB8888
	mov r8, SDL_TEXTUREACCESS_STATIC
	mov r9, COLOR_BUFFER_PIXEL_WIDTH
	
	push COLOR_BUFFER_PIXEL_HEIGHT
	CALL_C_FUNC SDL_CreateTexture
	sub rsp, 8

	mov qwColorBufferSDLTexturePtr, rax
	mov r13, rax ; SDL texture ptr

	; Allocate pixel data for snake piece texture
	mov rcx, COLOR_BUFFER_BYTE_SIZE
	CALL_C_FUNC malloc
	mov qwColorBufferPtr, rax
	mov r12, rax ; Color buffer ptr

	; Allocate grid
	mov rcx, GRID_BYTE_SIZE
	CALL_C_FUNC malloc
	mov qwGridPtr, rax

	FRAME_EPILOGUE
	ret
CreateGameResources endp

DestroyGameResources proc
	FRAME_PROLOGUE

	; Deallocate pixel data
	mov rcx, qwColorBufferPtr
	CALL_C_FUNC free

	mov rcx, qwGridPtr
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
	mov rdx, WINDOW_PIXEL_WIDTH
	mov r8, WINDOW_PIXEL_HEIGHT
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