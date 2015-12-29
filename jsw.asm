;----------------------------------------------
;Jet Set Willy
; Atom version by Kees van Oss  2014
;----------------------------------------------
	.DEFINE asm_code $2400
	.DEFINE header   1		; Header Wouter Ras emulator
	.DEFINE filenaam "JSW"

.org asm_code-22*header

.IF header
;********************************************************************
; ATM Header for Atom emulator Wouter Ras

name_start:
	.byte filenaam			; Filename
name_end:
	.repeat 16-name_end+name_start	; Fill with 0 till 16 chars
	  .byte $0
	.endrep

	.word asm_code			; 2 bytes startaddress
	.word exec			; 2 bytes linkaddress
	.word eind_asm-start_asm	; 2 bytes filelength

;********************************************************************
.ENDIF


exec  = L4E00
start_asm:
	.include "jsw.inc"
	.include "font.inc"
	.include "routines.inc"
	.include "sound.inc"
	.include "joystick.inc"
eind_asm: