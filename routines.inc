;----------------------------------------------------------------
; vdu16, set clear 4
;----------------------------------------------------------------

vdu22:
	lda #$f0
	sta $b000
	rts

;----------------------------------------------------------------
; vdu12, clear screen
;----------------------------------------------------------------

vdu12:
	lda #24
	jmp cls
vdu16:
	lda #17
cls:
	sta YMAX

	lda #0
	sta YPOS
	sta XPOS
clrline:
	lda #$20
	jsr PRINTCHAR
	lda YPOS
	cmp YMAX
	bne clrline

	lda #0
	sta XPOS	
	sta YPOS
	rts

YMAX:	.byte 0

;----------------------------------------------------------------
; NEWLINE
;
; XPOS=0
; YPOS=YPOS+1
;----------------------------------------------------------------

NEWLINE:
	lda #1
	sta XPOS
	inc YPOS
	rts

;----------------------------------------------------------------
; OSCLI, loadfile
;
; X=lb data
; Y=hb data
;
; data: $8000,"filename"
;----------------------------------------------------------------

OSCLI:

	stx $80
	sty $81

; Set load-/exec address

	ldy #0
	lda ($80),y
	sta $82		; LB Load
	sta $84		; LB Exec
	iny
	lda ($80),y
	sta $83		; HB Load
	sta $85		; HB Exec

; Set filename pointer

	lda $80
	clc
	adc #2
	sta $80
	lda $81
	adc #0
	sta $82

; ------ BUGBUGBUG -------
;
; Copy filename to $140
;

	ldy #0
nameloop:
	lda ($80),y
	sta $140,y
	iny
	cmp #$0d
	bne nameloop

; ------ BUGBUGBUG -------

; Load file

	ldx #$80
	jmp $ffe0

