.data
bFoo byte 78
bFoo2 byte 80
wFoo3 word 0

.code
extern HelloWorldCStyle : proc
extern GetSum : proc

extern ?Foo@SomeNamespace@@YAXXZ : proc
alias <Foo> = <?Foo@SomeNamespace@@YAXXZ>

extern ?HelloWorldCPlusPlusStyle@@YAXXZ : proc
alias <HelloWorldCPlusPlusStyle> = <?HelloWorldCPlusPlusStyle@@YAXXZ>

main proc
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
main endp

end