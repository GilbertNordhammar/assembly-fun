.data
bFoo byte 78
bFoo2 byte 80
wFoo3 word 0

.code

extern SDL_Init : proc

extern InitSDLCPlusPlus : proc

extern HelloWorldCStyle : proc
extern GetSum : proc

extern ?Foo@SomeNamespace@@YAXXZ : proc
alias <Foo> = <?Foo@SomeNamespace@@YAXXZ>

extern ?HelloWorldCPlusPlusStyle@@YAXXZ : proc
alias <HelloWorldCPlusPlusStyle> = <?HelloWorldCPlusPlusStyle@@YAXXZ>

mainz proc
	sub rsp, 20h
	mov rcx, 32
	call SDL_Init
	add rsp, 20h

	;call InitSDL

	call HelloWorldCStyle
	call HelloWorldCPlusPlusStyle
	
	sub rsp, 30h
	mov rcx, 1
	mov rdx, 2
	mov r8, 3
	mov r9, 4
	mov dword ptr [rsp + 20h], 5
	call GetSum
	add rsp, 30h
	
	call Foo
	
	ret
mainz endp

InitSDL proc
	sub rsp, 32
	
	mov rcx, 32
	push rcx
	call SDL_Init
	add rsp, 8

	add rsp, 32

	ret
InitSDL endp

end