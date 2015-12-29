sound:
	sta note
	lda #0
snd_loop1:
	ldx note
snd_loop2:
	dex
	nop
	nop
	bne snd_loop2
	eor #4
	sta $b002
	dey
	bne snd_loop1
	rts

note:	.byte 0
