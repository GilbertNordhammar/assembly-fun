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