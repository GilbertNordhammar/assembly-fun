; ===== SDL =====

include Game.inc

.code
main proc
	call InitSDL

	call CreateGameResources

	call RunGameLoop

	call DestroyGameResources
	call QuitSDL

	ret
main endp

end