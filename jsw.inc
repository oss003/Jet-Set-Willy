;
; User equates
;

OSWRCH      = PRINTCHAR
OSBYTE      = $FFF4
cheat	= 0

;
; Code equates
;

L0000		= $0000
L0001		= $0001
L0003		= $0003
L0004		= $0004		; Colour

L0010		= $0010
L0011		= $0011		; Time counter
L0012		= $0012
L0013		= $0013
L0014		= $0014
L0015		= $0015
L0016		= $0016

L004D		= $004D		; YPOS sprites
L004E		= $004E
L004F		= $004F

L0050		= $0050		; Room level address
L0051		= $0051

L0052		= $0052		; Level data pointer
L0053		= $0053
L0054		= $0054

L0055		= $0055		; Current Room nr

L0056		= $0056
L0057		= $0057		; Frame counter
L0058		= $0058
L0059		= $0059
L005A		= $005A
L005B		= $005B
L005C		= $005C
L005D		= $005D
L005E		= $005E
L005F		= $005F

L0060		= $0060
L0061		= $0061
L0062		= $0062
L0063		= $0063		; Death flag
L0064		= $0064
L0065		= $0065
L0066		= $0066
L0067		= $0067
L0068		= $0068
L0069		= $0069		; Jumponly flag
L006A		= $006A
L006B		= $006B
L006C		= $006C
L006D		= $006D
L006E		= $006E
L006F		= $006F

L0070		= $0070
L0071		= $0071
L0072		= $0072
L0073		= $0073
L0074		= $0074
L0075		= $0075
L0076		= $0076
L0077		= $0077
L0078		= $0078
L0079		= $0079
L007A		= $007A
L007B		= $007B
L007C		= $007C
L007D		= $007D
L007E		= $007E
L007F		= $007F

L0080		= $0080
L0081		= $0081
L0082		= $0082
L0083		= $0083
L0087		= $0087		; Nr of sprites on screen
L0088		= $0088
L0089		= $0089
L008A		= $008A
L008B		= $008B		; Jump key flag
L008C		= $008C
L008D		= $008D
L008E		= $008E
L008F		= $008F		; Sound jumpflag

L0090		= $0090
L0091		= $0091
L0092		= $0092		; lb-Spritedata address
L0093		= $0093		; hb-Spritedata address
L0094		= $0094		; Nr of bytes to plot for sprite
L0095		= $0095
L0096		= $0096
L0097		= $0097
L0098		= $0098
L009A		= $009A

L009B		= $009B		; Lives

L009C		= $009C
L009D		= $009D		; Time's up flag
L009E		= $009E		; End of game flag
				; $00 = Game not finished
				; $01 = Game finished
				; $ff = Game end, walk to toilet and jump in
L009F		= $009F

L00C0		= $00C0		; Workspace

L00EC		= $00EC

L00FC		= $00FC

IRQ1V		= $0204

L028A		= $028A		; Unused

L034A		= $034A		; Text cursor address
L034B		= $034B

L0381		= $0381		; Controls
				; $00 = keyboard
				; $40 = Joystick keyboard
				; $80 = Joystick AtoMMC

;L0EBD		= $0EBD

;L79A0		= $971f
;L7F26		= $7F26

LFE00		= $FE00
LFE05		= $FE05
LFE06		= $FE06
LFE07		= $FE07
LFE08		= $FE08
LFE09		= $FE09

;
; Start of code
;
;            *= $0000
;

L0400:
	jmp L0455		; Jump intro screen

;-------------------------------------------
; Scroll bottomline 1 pixel to the left
;-------------------------------------------

L0403:
	ldy #7			; Repeat for 8 lines
L0404:
	ldx scrolltab,y
	asl $961f,x
	rol $961e,x
	rol $961d,x
	rol $961c,x
	rol $961b,x
	rol $961a,x
	rol $9619,x
	rol $9618,x
	rol $9617,x
	rol $9616,x
	rol $9615,x
	rol $9614,x
	rol $9613,x
	rol $9612,x
	rol $9611,x
	rol $9610,x
	rol $960f,x
	rol $960e,x
	rol $960d,x
	rol $960c,x
	rol $960b,x
	rol $960a,x
	rol $9609,x
	rol $9608,x
	rol $9607,x
	rol $9606,x
	rol $9605,x
	rol $9604,x
	rol $9603,x
	rol $9602,x
	rol $9601,x
	rol $9600,x
	dey
	bpl L0404

	jsr $fe66		; Wait 1/60 sec

	rts

scrolltab:
	.byte $00,$20,$40,$60,$80,$a0,$c0,$e0

;--------------------------------------------
; Print Jet Set Miner letters in intro screen
; Every bit set 1 is block
;--------------------------------------------

L0430:
	lda #$0A		; Cusor down 2 lines
	jsr OSWRCH
	jsr OSWRCH

; Print 11 lines of 36 characters

	ldx #$00		; Reset charcounter
L043A:
	lda L065E,X		; Read 8 bits
	sta L007B

	ldy #$07		; Reset bitcounter
L0441:
	lda #$20		; Set space
	lsr L007B		; Shift bit
	bcc L0449		; if 0, print space

	lda #127		; Set block
L0449:
	jsr OSWRCH		; Print character
	dey			; Next bit
	bpl L0441		; Repeat for all 8 bits

	inx			; Point to next 8 bits
	cpx #$35		; Check if all blocks printed
	bne L043A		; If not, repeat

	rts

;--------------------------------------------
; Intro screen with bottom line text scroller
;--------------------------------------------

L0455:

; Set vars
	lda #$88
	sta L004D		; Set YPOS sprites
	sta L028A		; ??
	sta L0057		; Frame counter

	lda #$10
	sta L009B		; Set lives

; Clear screen

	jsr vdu12		; CLS
	jsr L0430		; Print Jet Set Miner letters

; Set text pointer to scroller

L0475:
	lda #>L050C		; Point to scroll text
	sta L0071
	lda #<L050C
	sta L0070

	ldy #$00
	sty L0072		; Reset line counter
L0481:
	ldy L0072		; Set line counter

; Plot character at bottom right position

	lda #$20-1		; Set character entry at 31,22
	sta XPOS
	lda #$16
	sta YPOS

	lda (L0070),Y		; Read scroll character
	jsr OSWRCH		; Print character scroller
	tax
	beq L04BC		; Check if all characters print

	lda #$07
	sta L007B		; Set var

; Wait routine

;            lda L0004              ; A5 04
;L04A0:      cmp L0004              ; C5 04
;            beq L04A0              ; F0 FC

; Scroll bottom line 8 pixels

L04A4:
	jsr L0403		; Scroll bottom line
	dec L007B		; Next pixel line
	bpl L04A4		; Repeat for 8 pixel lines

	jsr L04E9		; Decrement counter??

	jsr L0B15		; Plot Willies for lives
	jsr L04D6		; Check pressing spacebar
L04B4:
	inc L0072		; Next line
	bne L0481		; Repeat for 8 lines

            inc L0071              ; E6 71
            bne L0481              ; D0 C5

L04BC:      lda #$20               ; A9 80
            sta L0070              ; 85 70

L04C0:      lda L0070              ; A5 70
            and #$07               ; 29 07
            bne L04C9              ; D0 03

            jsr L04E9              ; Decrement counter??

L04C9:      jsr L04D6              ; Check pressing spacebar

            dec L0071              ; C6 71
            bne L04C0              ; D0 F0

            dec L0070              ; C6 70
            bne L04C0              ; D0 EC

            beq L0475              ; F0 9F
L04D6:
;            lda #$81               ; Check key
;            ldy #$FF               ; A0 FF
;            ldx #$9D               ; SPACE
;            jsr OSBYTE             ; 20 F4 FF

	bit L0381		; Scan controls
	bmi scan_joymmc
	bvs scan_joykey
	lda #$f0		; Reset matrix counter
	sta $b000

	jsr $fe71		; Scan key
	cpy #0                    ; 8A
            bne L04E8              ; Return
	jmp L04D7

scan_joymmc:
	jsr joyget
	and #$10
	beq L04E8
	jmp L04D7

scan_joykey:
	jsr $fe71
	cpy #9
	bne L04E8

L04D7:
            lda #$8C               ; A9 8C
            sta L004D              ; 85 4D

; If SPACE not pressed, return to L04B4
; If SPACE pressed, return to 

            pla                    ; 68
            pla                    ; 68

L04E8:      rts                    ; 60

;--------------------------------------------
; Decrement counter??
;--------------------------------------------

L04E9:      dec L04FE              ; CE FE 04
            bpl L04F3              ; 10 05

            lda #$07               ; A9 07
            sta L04FE              ; 8D FE 04

L04F3:      ldx L04FE              ; AE FE 04
            lda L04FF,X            ; BD FF 04
            ora #$19               ; 09 19
            sta L0000                ; 85 00
            rts                    ; 60

;--------------------------------------------

L04FE:      .byte $04
L04FF:      .byte $00
L0500:      .byte $22,$24,$20,$24,$06,$02,$04

;--------------------------------------------
; Scroller text
;--------------------------------------------

L050C:
	.byte "...Jet Set Willy was "
	.byte "written for the Elec"
	.byte "tron by Chris Robson"
	.byte "......Jet Set Willy "
	.byte "was prepared by Tyne"
	.byte "soft under licence f"
	.byte "rom Software Project"
	.byte "s and is copyright t"
	.byte "o Software Projects."

	.byte "....This Acorn Atom "
	.byte "conversion is written "
	.byte "by Kees van Oss."
 
	.byte ".....You must guide "
	.byte "Willy to collect all"
	.byte " the items around th"
	.byte "e ho"
L0600:
	.byte "use before midnight "
	.byte "so that Maria will l"
	.byte "et you go to bed...."
	.byte "...Press SPACE to st"
	.byte "art the game."
	.byte $00

;------------------------------------------------
; Jet Set Willy letters intro screen
; Every bit is a character
; 
;------------------------------------------------

L065E:
	.byte $70,$77,$EE,$0E
	.byte $00,$12,$22,$42
	.byte $00,$20,$27,$EE
	.byte $04,$00,$12,$82
	.byte $42,$00,$30,$27
	.byte $EE,$04,$00,$00
	.byte $00,$00,$00,$00
	.byte $00,$00,$00,$00
	.byte $44,$45,$22,$00
	.byte $40,$54,$24,$02
	.byte $00,$54,$45,$3E
	.byte $00,$40,$55,$84
	.byte $00,$00,$7C,$DD

;------------------------------------------------

L692:            
            .byte $09,$00,$08,$C0
            .byte $D7,$9D,$00,$00,$C6,$76,$10,$E2
            .byte $49,$49,$A8,$25,$00,$25,$A8,$40
            .byte $DD,$40,$28,$54,$AA,$55,$AA,$28
            .byte $28,$28,$00,$0A,$BD,$FF,$00,$AA
            .byte $55,$AA,$10,$30,$38,$6C,$C6,$C6
            .byte $6C,$28,$FF,$AA,$55,$80,$5D,$94
            .byte $5D,$80,$22,$55,$00,$55,$88,$55
            .byte $00,$22,$7E,$81,$A5,$99,$99,$A5
            .byte $81,$7E,$03,$01,$0C,$04,$30,$10
            .byte $C0,$40,$FF,$A5,$52,$24,$42,$22
            .byte $40,$04,$00,$2A,$54,$00,$00,$A2
            .byte $45,$00,$42,$99,$66,$A1,$9F,$81
            .byte $66,$5A,$FF,$5A,$FF,$FF,$AA,$55
            .byte $AA,$55

L0700:      .byte $00,$01,$04,$05,$01,$00,$00,$00
            .byte $04,$00,$00,$04,$00,$04,$05,$01
            .byte $04,$05,$06,$06,$04,$01,$01,$03
            .byte $00,$01,$05,$04,$05,$01,$04,$04
            .byte $01,$00,$05,$06,$04,$05,$00,$04
            .byte $00,$05,$05,$04,$05,$01,$01,$04
            .byte $00,$05,$01,$02,$01,$05,$06,$05
            .byte $01,$05,$06,$03,$02,$00,$26,$0C
L0740:      .byte $68,$04,$00,$00,$00,$04,$01,$04
            .byte $00,$00,$04,$03,$03,$00,$04,$00
            .byte $01,$00,$04,$01,$00,$00,$00,$04
            .byte $01,$00,$04,$00,$00,$04,$00,$01
            .byte $00,$04,$00,$00,$00,$00,$01,$01
            .byte $07,$04,$00,$00,$04,$04,$00,$01
            .byte $00,$00,$04,$00,$04,$04,$04,$00
            .byte $00,$04,$07,$00,$00,$00,$40,$40
            .byte $08,$48,$8A,$48,$98,$48,$20,$2C
            .byte $04,$04,$04,$00,$00,$00,$00,$00
L0790:      .byte $00,$0C,$0E

L0793:      .byte $10,$60,$40

L0796:      .byte $20,$FF,$FF,$FF,$02,$07,$02,$00
            .byte $00,$02,$00,$80,$00,$04,$04,$04
            .byte $00,$00,$00,$FE,$FC,$F8,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00

L07E0:      .byte $00
L07E1:      .byte $32
L07E2:      .byte $09

L07E3:      .byte $08,$01,$3C,$05,$08,$80,$0A,$14
            .byte $08,$0A,$1E,$05,$08,$13,$40,$05
            .byte $08,$00,$64,$85,$00,$00,$00,$8F
            .byte $00,$23,$00,$00,$00,$00,$00,$00
            .byte $00,$33,$31,$00,$00,$00,$00,$00
            .byte $00,$00,$FF,$00,$00,$00,$00,$00
            .byte $00,$80,$00,$00,$00,$64,$FF,$00
            .byte $05,$03,$FF,$01,$00,$02,$3A,$00
            .byte $00,$00,$07,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$64,$06,$90
            .byte $64,$06,$90,$64,$06,$90,$64,$06
            .byte $00,$00,$00,$00,$90,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00

L0900:      .byte $02,$01,$01,$02,$02,$02,$02,$02
            .byte $02,$01,$02,$02,$02,$01,$02,$02
            .byte $02,$02,$01,$02,$02,$01,$01,$02
            .byte $01,$01,$03,$02,$02,$03,$02,$03
            .byte $02,$03,$03,$03,$03,$03,$01,$00

L0928:      .byte $03,$03,$03,$03,$02,$03,$03,$02
            .byte $02,$03,$02,$02,$02,$03,$02,$02
            .byte $02,$02,$03,$02,$02,$03,$03,$02
            .byte $03,$03,$01,$03,$03,$01,$02,$01
            .byte $02,$01,$02,$02,$02,$02,$03,$03

;--------------------------------------------
; Wipe parts of screen????
;--------------------------------------------

L0950:
	.byte $00,$00,$00,$00,$00,$00,$00,$00
	.byte $00,$00,$00,$00,$00,$00,$00,$00
	.byte $00,$00,$00,$00,$00,$00,$00,$00
	.byte $00,$00,$00,$00,$00,$00,$00,$00

;            .byte $01,$DD,$DB,$D2,$D0,$97,$8D,$8C
;            .byte $89,$4E,$45,$42,$07,$05,$02,$BF
;            .byte $BD,$BB,$B8,$7E,$74,$72,$70,$35
;            .byte $2B,$29,$EF,$EC,$EA,$E0,$A5,$A2

            .byte $03,$6C

L0972:
	.byte $80,$80,$80,$80,$80,$80,$80,$80
	.byte $80,$80,$80,$80,$80,$80,$80,$80
	.byte $80,$80,$80,$80,$80,$80,$80,$80
	.byte $80,$80,$80,$80,$80,$80,$80,$80

;            .byte $77,$67,$67,$67,$67,$66,$66,$66
;            .byte $66,$65,$65,$65,$64,$64,$64,$62
;            .byte $62,$62,$62,$61,$61,$61,$61,$60
;            .byte $60,$60,$5E,$5E,$5E,$5E,$5D,$5D

            .byte $61,$61

;--------------------------------------------

L0994:      .byte $61,$BC,$B9,$B6,$B4,$B1,$AF,$AC
            .byte $AA,$A8,$A5,$A4,$A3,$A1,$A0,$9F
            .byte $9D,$9B,$9A,$98,$96,$94,$92,$91
            .byte $8F,$8D,$8B,$8A,$88,$86,$84,$82
            .byte $00,$00
L09B6:      .byte $00,$45,$43,$42,$40,$3F,$3D,$3C
            .byte $39,$36,$35,$32,$2F,$2D,$2A,$27
            .byte $25,$23,$20,$1E,$1C,$1A,$18,$15
            .byte $13,$11,$0F,$0C,$0A,$08,$05,$02
            .byte $00,$00,$00,$00,$02,$00,$04,$00
            .byte $06,$00,$08,$00,$0A,$00,$0C,$00
            .byte $0E,$00,$10,$00,$12,$00,$14,$00
            .byte $16,$00,$00

L09F1:      .byte $01,$02,$03,$03,$02,$00

L09F7:      .byte $8D
L09F8:      .byte $8E,$03
L09FA:      .byte $90,$40,$91,$00,$00,$00

L0A00:      .byte $00,$1C,$04,$02,$25,$1C,$0E
L0A07:      .byte $00
L0A08:      .byte $00
L0A09:      .byte $A0
L0A0A:      .byte $6A
L0A0B:      .byte $07
L0A0C:      .byte $01,$8A,$6A,$2E
L0A10:      .byte $30
L0A11:      .byte $0D

;------------------------------------------------
; Moving sprite type
;
; $00 = Static
; $ff = Moving
;------------------------------------------------

L0A12:      .byte $00
            .byte $00,$00,$FF,$00,$00,$00,$00,$00

;------------------------------------------------
; Current XPOS guards
;------------------------------------------------

L0A1B:      .byte $6A					; Current XPOS Willy
L0A1C:      .byte $90,$E0,$78,$58,$1C,$8A,$8A

;------------------------------------------------

L0A23:      .byte $48

;------------------------------------------------
; Current YPOS guards
;------------------------------------------------

L0A24:      .byte $68					; Current YPOS Willy
L0A25:      .byte $18,$68,$70,$68,$48,$60,$AD

;------------------------------------------------

L0A2C:      .byte $48

;------------------------------------------------
; Direction of guards
;------------------------------------------------

L0A2D:      .byte $FE					; Direction Willy
            .byte $02,$00,$00,$02,$FE,$FE,$2C,$CE

;------------------------------------------------
; Min.POS guards
;------------------------------------------------

L0A36:      .byte $86					; Min.POS Willy
            .byte $00,$00,$78,$00,$10,$00,$BD,$26

;------------------------------------------------
; Max.POS guards
;------------------------------------------------

L0A3F:      .byte $0C					; Max.POS Willy
            .byte $E8,$00,$78,$C8,$38,$A8,$8D,$81

;------------------------------------------------

L0A48:      .byte $0A

;------------------------------------------------
; Sprite type
;
; 0 = ??
; 1 = Vertical
; 2 = Willy
; 3 = Horizontal
;------------------------------------------------

L0A49:      .byte $02					; Sprite type Willy
L0A4A:      .byte $03,$01,$01,$01,$03,$20,$F1

;------------------------------------------------
; Checkbyte for wiping hor.guards
;------------------------------------------------

L0A51:      .byte $FF
            .byte $52,$9C,$00,$8B,$03,$71,$C9,$25

;------------------------------------------------
; Direction of guard
;------------------------------------------------

L0A5A:      .byte $D0
            .byte $02,$00,$00,$02,$FE,$FE,$A5,$EC

;------------------------------------------------

L0A63:      .byte $02
L0A64:      .byte $FF
L0A65:      .byte $FF
L0A66:      .byte $00,$FF,$FF,$FF,$7C,$0A

;------------------------------------------------

L0A6C:      .byte $C9
            .byte $01,$00,$00,$01,$00,$00,$7C,$0A

;------------------------------------------------

            .byte $68,$A8,$68,$AA,$68,$28,$60

L0A7C:      .byte $00,$00

L0A7E:      .byte $60,$48

L0A80:      .byte $00,$01,$01,$01,$01,$01,$01,$01
            .byte $01,$01,$01,$01

;------------------------------------------------
; Locations objects
;------------------------------------------------

L0A8C:      .byte $18,$20,$40,$B8,$D8,$E8,$F8,$C9	; lb
            .byte $A0,$D0,$06,$8D

L0A98:      .byte $6E,$65,$5F,$5F,$6C,$6C,$6C,$68	; hb
            .byte $AA,$68,$85,$FC

;------------------------------------------------

L0AA4:      .byte $14,$15,$15,$15

L0AA8:
	.byte <L49B7,<L48D0,<L483C,<L4843,<L4997,<L4999,<L499B
;            .byte $B7,$D0,$3C,$43,$97,$99,$9B
            .byte $68,$84,$08,$08,$08

L0AB4:
	.byte >L49B7,>L48D0,>L483C,>L4843,>L4997,>L4999,>L499B
;            .byte $49,$48,$48,$48,$49,$49,$49
            .byte $08,$08,$08,$08,$08

L0AC0:      .byte $00
L0AC1:      .byte $00,$01,$00,$02,$00,$03,$00,$04
            .byte $00,$05,$00,$06,$00,$07,$00,$08
            .byte $00,$09,$00,$0a,$00,$0b,$00,$0c
            .byte $00,$0d,$00,$0e,$00,$0f,$00,$10
            .byte $00,$11,$00,$12,$00,$13,$00,$14
            .byte $00,$15,$00,$16,$00,$17,$00,$18
            .byte $00,$19,$00,$1a,$00,$1b,$00,$1c
            .byte $00,$1d,$00,$1e,$00,$1f,$00

;--------------------------------------------

L0B00:      jmp L0C58              ; Print 'Items collected 000 Time  7:00pm'
L0B03:      jmp L0B2A              ; 4C 2A 0B
L0B06:      jmp L0C95              ; Print nr of items collected

            jmp L0D50              ; Set vars

L0B0C:      jmp L0E18              ; 4C 18 0E
L0B0F:      jmp L0E40              ; 4C 40 0E
            jmp L1118              ; Calculate screenaddress sprite

L0B15:      jmp L0EE2              ; Plot Willies

L0B18:      jmp L0F21              ; Print roomname

L0B1B:      jsr L0FCB              ; 20 CB 0F
            jmp L1008              ; 4C 08 10

L0B21:	jmp L1038		; Game loop

            jmp L1059              ; 4C 59 10
L0B27:      jmp L10AE              ; Make sound

;--------------------------------------------
L0B2A:      lda L006F              ; A5 6F
            beq L0B6C              ; F0 3E
            jsr L0BD8              ; 20 D8 0B
            lda #$80               ; A9 80
            sta L005D              ; 85 5D
            lda #$00               ; A9 00
            sta L007B              ; 85 7B
            sta L005E              ; 85 5E
            ldx L005F              ; A6 5F
            lda #$1F               ; A9 1F
            sta L007A              ; 85 7A
L0B41:      jsr L0B72              ; 20 72 0B
            dec L007A              ; C6 7A
            bne L0B41              ; D0 F9
            lda L005C              ; A5 5C
            asl A                  ; 0A
            clc                    ; 18
            adc L005F              ; 65 5F
            tay                    ; A8
            cmp #$D2               ; C9 D2
            bne L0B5B              ; D0 08
            lda L005B              ; A5 5B
            eor #$FF               ; 49 FF
            sta L005B              ; 85 5B
            lda #$08               ; A9 08
L0B5B:      cmp #$08               ; C9 08
            bne L0B67              ; D0 08
            lda L005C              ; A5 5C
            eor #$FF               ; 49 FF
            sta L005C              ; 85 5C
            inc L005C              ; E6 5C
L0B67:      sty L005F              ; 84 5F
            jsr L0BEE              ; 20 EE 0B
L0B6C:      jsr L0CDC              ; 20 DC 0C
            jmp L0D91              ; 4C 91 0D

;--------------------------------------------

L0B72:      stx L0096              ; 86 96
            txa                    ; 8A
            bpl L0B7F              ; 10 08

            ldx #$26               ; A2 26
            cmp #$F0               ; C9 F0
            bcs L0B7F              ; B0 02
            ldx #$27               ; A2 27

L0B7F:      lda L0900,X            ; BD 00 09
            cmp L005B              ; C5 5B
            bcc L0B88              ; 90 02

            eor #$FF               ; 49 FF
L0B88:      adc L005D              ; 65 5D
            sta L005D              ; 85 5D
            lda L005E              ; A5 5E
            clc                    ; 18
            adc L0928,X            ; 7D 28 09
            sta L005E              ; 85 5E
            ldy L007A              ; A4 7A
            sta L09B6,Y            ; 99 B6 09
            tax                    ; AA
            lda L005D              ; A5 5D
            sta L0994,Y            ; 99 94 09
            jsr L0BB2              ; 20 B2 0B

            lda L0090              ; A5 90
            ldx L007A              ; A6 7A
            sta L0950,X            ; 9D 50 09
            lda L0091              ; A5 91
            sta L0972,X            ; 9D 72 09

            ldx L0096              ; A6 96
            inx                    ; E8

            rts                    ; 60

;--------------------------------------------

L0BB2:      sta L0097              ; 85 97
            jsr L1118              ; Calculate screenaddress sprite
            lda L0097              ; A5 97
            and #$07               ; 29 07
            tax                    ; AA
            lda L0BD0,X            ; BD D0 0B
            sta L0097              ; 85 97
            ldy #$00               ; A0 00
            and (L0090),Y          ; 31 90
            beq L0BCB              ; F0 04
            lda L007A              ; A5 7A
            sta L007B              ; 85 7B
L0BCB:      lda L0097              ; A5 97
            sta (L0090),Y          ; 91 90
            rts                    ; 60

L0BD0:      .byte $80,$40,$20,$10,$08,$04,$02,$01

;--------------------------------------------

L0BD8:      ldx #$1F               ; A2 1F
            ldy #$00               ; A0 00
L0BDC:      lda L0950,X            ; BD 50 09
            sta L0090              ; 85 90
            lda L0972,X            ; BD 72 09
            sta L0091              ; 85 91
            lda #$00               ; A9 00
            sta (L0090),Y          ; 91 90
            dex                    ; CA
            bpl L0BDC              ; 10 EF
            rts                    ; 60

;--------------------------------------------

L0BEE:      ldx L009F              ; A6 9F
            bne L0C09              ; D0 17
            lda L007B              ; A5 7B
            beq L0C57              ; F0 61
            ldx #$00               ; A2 00
            stx L009A              ; 86 9A
            lda L0A24              ; AD 24 0A
L0BFD:      inx                    ; E8
            cmp L09B6,X            ; DD B6 09
            bcc L0BFD              ; 90 FA
            cpx #$02               ; E0 02
            bcc L0C4A              ; 90 43
            stx L009F              ; 86 9F
L0C09:      lda L0065              ; A5 65
            cmp #$0E               ; C9 0E
            bcs L0C4A              ; B0 3B
            lda #$00               ; A9 00
            sta L0064              ; 85 64
            sta L0065              ; 85 65
            lda L0994,X            ; BD 94 09
            sec                    ; 38
            sbc #$04               ; E9 04
            and #$FE               ; 29 FE
            sta L0A1B              ; 8D 1B 0A
            lda L09B6,X            ; BD B6 09
            sec                    ; 38
            sbc #$06               ; E9 06
            bpl L0C2A              ; 10 02
            lda #$C0               ; A9 C0
L0C2A:      sta L0A24              ; 8D 24 0A
            lda L008E              ; A5 8E
            beq L0C57              ; F0 26
            lda L005B              ; A5 5B
            eor L005C              ; 45 5C
            inx                    ; E8
            eor L008E              ; 45 8E
            bpl L0C3C              ; 10 02
            dex                    ; CA
            dex                    ; CA
L0C3C:      stx L009F              ; 86 9F
            cpx #$03               ; E0 03
            bcs L0C57              ; B0 15
            lda L0A24              ; AD 24 0A
            adc #$04               ; 69 04
            sta L0A24              ; 8D 24 0A
L0C4A:      lda L0A24              ; AD 24 0A
            and #$FC               ; 29 FC
            sta L0A24              ; 8D 24 0A
            lda #$00               ; A9 00
            sta L009F              ; 85 9F
            rts                    ; 60
L0C57:      rts                    ; 60

;--------------------------------------------
; Print 'Items collected 000 Time  7:00pm'
; at bottomline
;--------------------------------------------

L0C58:
	lda #$04-4		; Set cursor 0,23
	sta XPOS
	lda #23
	sta YPOS


	ldx #$FF		; Set counter
L0C5A:
	inx
	lda L0C6B,X 		; Get character
	jsr OSWRCH		; Print
	cpx #31			; All text printed?
	bne L0C5A		; If not, repeat

	jsr L0CB5		; Print nr of itmes collected

	jmp L0D2B		; Goto Print time
L0C6B:
	.byte "Items collected 000 "
	.byte "Time  7:00pm"

;--------------------------------------------
; Print nr of items collected
;--------------------------------------------

L0C95:
	lda #50
	ldy #100
	jsr L5703		; Play item hit sound

	inc L0CC4		; Items collected-1 -> +1

	lda L0CC4		; Items collected-1
	cmp #$0A		; Check overflow digit
	bcc L0CB5		; If no overflow, print nr of itmes collected

	inc L0CC5		; Items collected-10 -> +1
	lda #$00
	sta L0CC4		; Reset Items collected-1

	lda L0CC5		; Items collected-10
	cmp #$08		; Check if less then 80 Items collected
	bcc L0CB5		; If yes, print nr of itmes collected

	inc L009E		; End of game flag

; Print nr of items collected

L0CB5:
	ldx #$11		; Set XPOS
	ldy L0CC5		; Get digit Items collected-10
	jsr L0CC6		; Print number at X,22

	lda L0CC4		; Get digit Items collected-1
	jsr L0CD5		; Print number at X+1,22
	rts

L0CC4:	.byte $00		; Items collected-1
L0CC5:	.byte $00		; Items collected-10

;--------------------------------------------
; Print number in Y as digit at X,22
;--------------------------------------------

L0CC6:
	stx XPOS		; Set XPOS
	lda #23
	sta YPOS		; Set YPOS

; Convert digit to ASCII character and print it

	tya
L0CD5:
	clc
	adc #$30
	jsr OSWRCH		; Print number

	rts

;--------------------------------------------
; Increment and print time
;--------------------------------------------

; Check timer if clock has to be updated

L0CDC:
	dec L0057		; Decrement Frame counter
	bne L0D4A		; Jump if not update clock

	inc L0D4F
	lda L0D4F		; Check clock counter
	cmp #$03		; Check for #2FF clockticks
	bne L0D4A		; Jump if no update clock

; Update clock

	ldy #$00		; Reset clock counter
	sty L0D4F

	inc L0D4B		; Min-1 -> +1
	lda L0D4B		; Min-1
	cmp #$0A		; Check overflow digit
	bcc L0D2B		; If not, print time
	sty L0D4B		; Min-1

	inc L0D4C		; Min-10 -> +1
	lda L0D4C		; Min-10
	cmp #$06		; Check overflow digit
	bcc L0D2B		; If not, print time
	sty L0D4C		; Min-10

	inc L0D4D		; Hour-1 -> +1
	lda L0D4D		; Hour-1
	cmp #$0A		; Check overflow digit
	bcc L0D1B		; If not, Check hour-10 digit
	sty L0D4D		; Hour-1

	lda #$01		; Set Hour-10 digit
	sta L0D4E		; Hour-10
L0D1B:
	lda L0D4D		; Hour-1
	cmp #$02		; C9 02
	bne L0D2B		; D0 09

	lda L0D4E		; Hour-10
	bmi L0D2B		; 30 04

	inc L009D		; Time's up flag

.if cheat=0
	inc L0063		; Time's up, set death flag
.endif

; Print time on bottomline

L0D2B:
	ldx #$19		; Set XPOS
	ldy L0D4E		; Hour-10
	jsr L0CC6		; Print hour-10 digit at 25,22

	lda L0D4D		; Hour-1
	jsr L0CD5		; Print hour-1 digit at 26,22

	lda #$3A
	jsr OSWRCH		; Print ":" at 27,22

	lda L0D4C		; Min-10
	jsr L0CD5		; Print min-10 digit at 28,22

	lda L0D4B		; Min-1	
	jsr L0CD5		; Print min-1 digit at 29,22
L0D4A:
	rts

L0D4B:	.byte $00		; Min-1
L0D4C:	.byte $00		; Min-10
L0D4D:	.byte $07		; Hour-1
L0D4E:	.byte $F0		; Hour-10 (Space=$F0+$30=$20)

L0D4F:	.byte $00		; Clock counter

;--------------------------------------------
; Set vars
;--------------------------------------------

L0D50:
	lda #$00

	sta L0D4F		; Clock counter
	sta L009A		; ??
	sta L0CC4		; Items collected-1
	sta L0CC5		; Items collected-10
	sta L0D4B		; Min-1
	sta L0D4C		; Min-10
	sta L009E		; End of game flag
	sta L009D		; Time's up flag
	sta L0064		; 85 64
	sta L0065		; 85 65
	sta L004F		; 85 4F

	sta L008F		; Reset sound jumpflag

	lda #$F0		; Set Space
	sta L0D4E		; Hour-10
	lda #$07		; Set 7
	sta L0D4D		; Hour-1

	lda #$98		; Set YPOS sprites
	sta L004D

	lda #$07		; Set 7
	sta L009B		; Lives

	lda #$68
	sta L0A24		; ??

	lda #$A8
	sta L0A1B		; ??

	lda #$15		; Set 21
	sta L0055		; Current Room nr

	jsr L1100		; Fill table

	bit L0381		; Check for Joystick AtoMMC
	bpl no_joy
	jsr joyinit

no_joy:
	rts

;--------------------------------------------

L0D91:      lda L0055              ; Current Room nr
            cmp #$13               ; C9 13
            bne L0DB5              ; D0 1E

            lda L0A24              ; AD 24 0A
            cmp #$67               ; C9 67
            bcs L0DA7              ; B0 09

            ldx #$04               ; A2 04
            cmp #$60               ; C9 60
            bcs L0DB2              ; B0 0E

            inx                    ; E8
            bne L0DB2              ; D0 0B

L0DA7:      lda L0A0A              ; AD 0A 0A
            and #$01               ; 29 01
            eor #$03               ; 49 03
            sta L0A0A              ; 8D 0A 0A
            tax                    ; AA
L0DB2:      stx L0A0A              ; 8E 0A 0A

L0DB5:      cmp #$15               ; C9 15
            bne L0DC7              ; D0 0E

            lda L0057              ; Frame counter
            and #$01               ; 29 01
            bne L0DC7              ; D0 08

            lda L0A0B              ; AD 0B 0A
            eor #$01               ; 49 01
            sta L0A0B              ; 8D 0B 0A

L0DC7:      lda L009E              ; End of game flag
            beq L0E17              ; F0 4C
            bmi L0DE4              ; 30 17

            lda L0055              ; Current Room nr
            cmp #$13               ; C9 13
            bne L0E17              ; D0 44

            lda #$00               ; A9 00
            sta L0A49              ; 8D 49 0A

            lda L0A1B              ; AD 1B 0A
            cmp #$18               ; C9 18
            bne L0E17              ; D0 38

            lda #$FF               ; A9 FF
            sta L009E              ; End of game flag

            rts                    ; 60

L0DE4:      lda #$02               ; A9 02
            sta L008E              ; 85 8E

            lda #$00               ; A9 00
            sta L008B              ; 85 8B

            lda L0055              ; Current Room nr
            cmp #$14               ; C9 14
            bne L0E00              ; D0 0E

            lda L0A1B              ; AD 1B 0A
            cmp #$88               ; C9 88
            beq L0DFD              ; F0 04

            cmp #$B0               ; C9 B0
            bne L0DFF              ; D0 02

L0DFD:      inc L008B              ; E6 8B

L0DFF:      rts                    ; 60

L0E00:      bcc L0E17              ; 90 15

            lda L0A1B              ; AD 1B 0A
            cmp #$D8               ; C9 D8
            bcc L0E17              ; 90 0E

            lda L0A0B              ; AD 0B 0A
            and #$01               ; 29 01
            ora #$08               ; 09 08
            sta L0A0B              ; 8D 0B 0A

            lda #$02               ; A9 02
            sta L009C              ; 85 9C

L0E17:      rts                    ; 60

L0E18:      ldy #$20               ; A0 20
            jsr L0E2B              ; 20 2B 0E
            ldy #$40               ; A0 40
            jsr L0E2B              ; 20 2B 0E
            lda L0A24              ; AD 24 0A
            and #$07               ; 29 07
            beq L0E3F              ; F0 16
            ldy #$60               ; A0 60
L0E2B:      jsr L0E2F              ; 20 2F 0E
            iny                    ; C8
L0E2F:      lda (L0060),Y          ; B1 60
            bpl L0E3F              ; 10 0C
            cmp #$8A               ; C9 8A
            bcs L0E3F              ; B0 08
            and #$0F               ; 29 0F
            tax                    ; AA
            lda #$02               ; A9 02
            sta L0A63,X            ; 9D 63 0A
L0E3F:      rts                    ; 60

L0E40:      ldy #$01               ; A0 01

L0E42:      ldx L0A7C,Y            ; BE 7C 0A
            bmi L0E71              ; 30 2A
            beq L0E6D              ; F0 24
            lda L0057              ; Frame counter
            and #$01               ; 29 01
            bne L0E6D              ; D0 1E
            dex                    ; CA
            bne L0E60              ; D0 0E
            lda #$FF               ; A9 FF
            sta L0A7C,Y            ; 99 7C 0A
            lda L0EDA,Y            ; B9 DA 0E
            sta L0EDE,Y            ; 99 DE 0E
            jmp L0E8A              ; 4C 8A 0E

L0E60:      txa                    ; 8A
            sta L0A7C,Y            ; 99 7C 0A
            cmp #$03               ; C9 03
            bne L0E6D              ; D0 05

;	sty y_tmp
;	ldy #100
;            lda #100               ; A9 20
;            jsr L10AE              ; Make ???? sound
;	ldy y_tmp

L0E6D:      dey                    ; 88
            bpl L0E42              ; 10 D2
            rts                    ; 60

L0E71:      jsr L0E9E              ; 20 9E 0E
            lda L0EDE,Y            ; B9 DE 0E
            clc                    ; 18
            adc L0EDC,Y            ; 79 DC 0E
            sta L0EDE,Y            ; 99 DE 0E
            cmp L0EDA,Y            ; D9 DA 0E
            bne L0E8A              ; D0 07
            lda #$7F               ; A9 7F
            sta L0A7C,Y            ; 99 7C 0A
            bpl L0E6D              ; 10 E3
L0E8A:      jsr L0E9E              ; 20 9E 0E
            ldx #$08               ; A2 08
            jsr L1115              ; 20 15 11
            ldy #$20               ; A0 20
            lda (L0060),Y          ; B1 60
            ldy L0096              ; A4 96
            sta L0EE0,Y            ; 99 E0 0E
            jmp L0E6D              ; 4C 6D 0E

;--------------------------------------------
; Plot/wipe arrow
;--------------------------------------------

L0E9E:      sty L0096              ; 84 96
	stx x_tmp

            lda L0EDE,Y            ; B9 DE 0E
            sta L0A23              ; 8D 23 0A

            ldx L0A7E,Y            ; BE 7E 0A
            stx L0A2C              ; 8E 2C 0A

            jsr L1118              ; Calculate screenaddress sprite

            lda L0ED8,Y            ; B9 D8 0E
            sta L0EBC+1              ; 8D BD 0E
	lda L0ED9,Y
	sta L0EBC+2

            lda L0EE0,Y            ; B9 E0 0E
            bmi L0EC9              ; 30 0F

; Plot arrow

            ldx #$07               ; A0 07
L0EBC:      lda L4760,x            ; B9 60 47
	ldy scrtab,x
            eor (L0090),Y          ; 51 90
            sta (L0090),Y          ; 91 90
            dex                    ; 88
            bne L0EBC              ; D0 F6
	ldx x_tmp
            ldy L0096              ; A4 96
            rts                    ; 60

; Wipe arrow

L0EC9:      lda #$00               ; A9 00
            sta L0EE0,Y            ; 99 E0 0E
            ldx #$07               ; A0 07
L0ED0:
	ldy scrtab,x
            sta (L0090),Y          ; 91 90
            dex                    ; 88
            bne L0ED0              ; D0 FB
	ldx x_tmp
            ldy L0096              ; A4 96
            rts                    ; 60

L0ED8:      .byte <L4770,<L4760
L0ED9:	.byte >L4770,>L4760

;--------------------------------------------

L0EDA:      .byte $00,$F8
L0EDC:      .byte $08,$F8
L0EDE:      .byte $00,$00
L0EE0:      .byte $00,$00

;-------------------------------------------
; Plot sprite 16x16 for every life 
;-------------------------------------------

L0EE2:
	lda #$FF
	sta L0089

	lda L0057		; Once per 4 frames
	and #$03
	bne L0F1B

	lda L0F1C
	clc
	adc #$02
	sta L0F1C

	lsr a
	and #$03
	tax

	lda sprdata_lb,x	; Set sprite data
	sta L0092		; lb-Spritedata address
	lda sprdata_hb,x
	sta L0093		; hb-Spritedata address

	lda #$03
	sta L007B

	lda #$f0
	sta L0097		; Startposition Willy

	lda L009B		; Get lives
	beq L0F1B		; Quit if no lives

	sta L0096		; Set counter
L0F0B:
	lda L0097		; Set XPOS Willy
	clc
	adc #$10		; Next XPOS
	sta L0097

	ldx L004D		; Get YPOS
	jsr L110F		; Plot sprite

	dec L0096		; Decrement counter
	bne L0F0B		; Repeat for all lives
L0F1B:
	rts

sprdata_lb:
	.byte <L4480,<L44A0,<L44C0,<L44E0
sprdata_hb:
	.byte >L4480,>L44A0,>L44C0,>L44E0

;-------------------------------------------

L0F1C:      .byte $E0,$00,$0F,$F0,$FF

;-------------------------------------------
; Print roomname
;-------------------------------------------

L0F21:
	ldy #$04               ; A0 04
L0F23:
	ldx #$02               ; A2 02
	jsr L1112              ; 20 12 11

	sta L09F1,Y            ; 99 F1 09
	dey                    ; 88
	bpl L0F23              ; 10 F5

            ldy #$05               ; A0 05
L0F30:      ldx #$08               ; A2 08
            jsr L1112              ; 20 12 11
            sta L09F7,Y            ; 99 F7 09
            dey                    ; 88
            bpl L0F30              ; 10 F5

            ldx #$06               ; A2 06
            jsr L1112              ; 20 12 11

            ldx #$03               ; A2 03
            jsr L1112              ; 20 12 11

            ldx #$03               ; A2 03
            jsr L1112              ; 20 12 11

            tax                    ; AA
            lda L10D6,X            ; BD D6 10
            sta L0003              ; 85 03

            ldy L0055              ; Current Room nr
            lda L0700,Y            ; B9 00 07

            ldy #$00               ; A0 00
            jsr L10C2              ; 20 C2 10

            ldy L0055              ; Current Room nr
            lda L0740,Y            ; B9 40 07

            ldy #$01               ; A0 01
            jsr L10C2              ; 20 C2 10

            lda #$01               ; Start with capital
            sta L0097              ; 85 97

;            lda #$1F               ; Cursor at ??,17
;            jsr OSWRCH             ; 20 EE FF
            ldx #$04               ; A2 04
            jsr L1112              ; 20 12 11
;	sec
;	sbc #4
	sta XPOS
;            jsr OSWRCH             ; 20 EE FF
;            lda #$11               ; A9 11
;            jsr OSWRCH             ; 20 EE FF
	lda #$11
	sta YPOS
	
L0F7A:      ldx #$05               ; A2 05
            jsr L1112              ; 20 12 11
            cmp #$1B               ; C9 1B
            bcs L0F89              ; B0 06
            jsr L0FBA              ; 20 BA 0F
            jmp L0F7A              ; 4C 7A 0F

L0F89:      bne L0F93              ; D0 08

            lda #$27               ; A9 27
            jsr OSWRCH             ; 20 EE FF

            jmp L0F7A              ; 4C 7A 0F

L0F93:      cmp #$1D               ; C9 1D
            beq L0FA0              ; F0 09
            bcs L0F9A              ; B0 01
            rts                    ; 60

L0F9A:      cmp #$1E               ; C9 1E
            bne L0FA8              ; D0 0A
            inc L0097              ; E6 97

L0FA0:      lda #$20               ; A9 20
            jsr OSWRCH             ; 20 EE FF

            jmp L0F7A              ; 4C 7A 0F

L0FA8:      lda #$13               ; A9 13
            jsr L0FBA              ; 20 BA 0F

            lda #$68               ; A9 68
            jsr OSWRCH             ; 20 EE FF
            lda #$65               ; A9 65
            jsr OSWRCH             ; 20 EE FF
            jmp L0F7A              ; 4C 7A 0F

L0FBA:      clc                    ; 18
            adc #$61               ; 69 61
            ldy L0097              ; A4 97
            beq L0FC7              ; F0 06
            and #$DF               ; 29 DF
            ldx #$00               ; A2 00
            stx L0097              ; 86 97

L0FC7:      jsr OSWRCH             ; 20 EE FF
            rts                    ; 60

;-------------------------------------------

L0FCB:      ldy #$01               ; A0 01
L0FCD:      lda #$00               ; A9 00
            sta L0EE0,Y            ; 99 E0 0E
            sta L0A7C,Y            ; 99 7C 0A
            ldx #$01               ; A2 01
            jsr L1112              ; 20 12 11
            tax                    ; AA
            beq L0FFF              ; F0 22
            ldx #$04               ; A2 04
            jsr L1112              ; 20 12 11
            tax                    ; AA
            beq L0FFF              ; F0 1A
            asl A                  ; 0A
            asl A                  ; 0A
            asl A                  ; 0A
            eor #$78               ; 49 78
            sta L0A7E,Y            ; 99 7E 0A
            ldx #$03               ; A2 03
            jsr L1112              ; 20 12 11
            tax                    ; AA
            lda L1003,X            ; BD 03 10
            sta L0A7C,Y            ; 99 7C 0A
            sta L0EDE,Y            ; 99 DE 0E
            sta L0EE0,Y            ; 99 E0 0E
L0FFF:      dey                    ; 88
            bpl L0FCD              ; 10 CB
            rts                    ; 60

L1003:      .byte $D0,$16,$1E,$26,$7F

L1008:      lda L0055              ; Current Room nr
            cmp #$19               ; C9 19
            bne L1017              ; D0 09

            lda L004F              ; A5 4F
            bne L1017              ; D0 05

            inc L004F              ; E6 4F
            jsr L0C95              ; Print nr of items collected
L1017:      ldx #$A0               ; A2 A0

            lda L0055              ; Current Room nr
L101B:      cmp #$1B               ; C9 1B
            bne L1034              ; D0 15

            lda #$02               ; A9 02
            sta L0A10              ; 8D 10 0A

            lda #$03               ; A9 03
            sta L0A07              ; 8D 07 0A

            ldx #$52               ; A2 52
            ldy #$D4               ; A0 D4
            sty L09F7              ; 8C F7 09
            iny                    ; C8
            sty L09F8              ; 8C F8 09

L1034:      stx L0A09              ; 8E 09 0A
            rts                    ; 60

;--------------------------------------------
; Game loop
;--------------------------------------------

L1038:
	jsr L0400		; Display intro screen
	jsr L0D50		; Set vars
L103E:
	jsr L110C		; Build playfield
L1041:
	lda #$00
	sta L0004		; Reset colour

	jsr L0EE2		; Plot sprite 16x16 for every lifes
 	jsr L1106		; Plot 16x16 sprites
	jsr L1089		; ??
	jsr L10DE		; ??

	lda L0063		; Death flag
	beq L1041

	lda L009B		; Get lives
	bpl L103E		; If lives left, restart

; Game over, no lives left

L1059:
	ldx #$00
	jsr vdu16		; Clear playfield
	jsr L1103		; Animate foot

	lda #$70		; Set nr of colour changes
	sta L10A3

L106E:
;	lda L0089		; Invert colour
;	eor #$ff
;	sta L0089
L1070:
	jsr $fe66		; Wait
	jsr $fe66		; Wait
	jsr $fe66		; Wait
;	cmp L0004		; Check if colour changed
;	bne L1070		; If so, wait

; Print 'Game over'

	lda #11			; Set cursor 11,8
	sta XPOS
	lda #8
	sta YPOS

	ldy #$00		; Reset counter
L1076:
	lda L10A3
	and #1
	beq L1077

	lda L10A4,Y		; Get char
	jmp L1078
L1077:
	lda L10A5,Y		; Get char
L1078:
	jsr OSWRCH		; Print char
	iny			; Next char
	cpy #$0A		; All text printed?
	bne L1076		; If not, repeat

	inc L10A3		; Inc counter
	bpl L106E		; Repeat a few times

	jmp L1038		; Game loop

;--------------------------------------------

L1089:      lda L00EC              ; A5 EC
            cmp #$70               ; C9 70
            bne L1094              ; D0 05
            pla                    ; 68
            pla                    ; 68
            jmp L1038              ; Game loop

L1094:      rts                    ; 60

L10A3:      .byte $00

;                  G   a   m   e   _   _   O
L10A4:      .byte $47,$61,$6D,$65,$09,$09,$4F
;                  v   e   r
            .byte $76,$65,$72

L10A5:	.byte $20,$20,$20,$20,$09,$09,$20,$20,$20,$20

;--------------------------------------------
; Play sound
;--------------------------------------------

L10AE:      stx L10C0              ; 8E C0 10
            sty L10C1              ; 8C C1 10
;            lsr A                  ; 4A
;            tay                    ; A8
            jsr L5703              ; Play sound
            ldx L10C0              ; AE C0 10
            ldy L10C1              ; AC C1 10
            rts                    ; 60

L10C0:      .byte $00
L10C1:      .byte $00

;--------------------------------------------

L10C2:
	eor #$07
	tax
	lda L10CE,x
	ora 3
	sta L0000,y
	rts

L10CE:
            .byte $26,$22,$24,$20
            .byte $06,$02,$04,$00

L10D6:      .byte $19,$10,$09,$08,$11,$10,$01,$00

L10DE:
	jsr $fe6b
	jsr $fe6b
	jsr $fe66
	jsr $fe66
	rts
            lda L0004              ; A5 04
            cmp #$04               ; C9 04
            bcc L10DE              ; 90 FA
            rts                    ; 60

            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $5A,$5A,$5A,$5A,$5A,$5A,$5A,$5A
            .byte $5A,$5A,$5A

L1100:      jmp L1C9E              ; 4C 9E 1C

L1103:      jmp L1DBD              ; Animate foot

L1106:	jmp L1646              ; 4C 46 16

            jmp L1776              ; 4C 76 17

L110C:	jmp L111B		; Build screen

L110F:      jmp L1476              ; Plot sprite 16x16

L1112:      jmp L11B8              ; 4C B8 11
L1115:      jmp L183B              ; 4C 3B 18
L1118:      jmp L1454              ; Calculate screenaddress sprite

;--------------------------------------------
; Build screen
;--------------------------------------------

L111B:	jsr L112C              ; 20 2C 11
	jsr L1776              ; 20 76 17
	jsr L0B1B              ; 20 1B 0B
	jmp L1D08              ; 4C 08 1D

L1127:      .byte $00
L1128:      .byte $00
L1129:      .byte $00
L112A:      .byte $00
L112B:      .byte $00

;--------------------------------------------
; Display screen
;--------------------------------------------

L112C:
	lda #$00		; Reset vars
	sta L0052		; Level data pointer
	sta L0053
	sta L009F
	sta L005B
	sta L0057		; Frame counter

	ldx L0055		; Current Room nr
	jsr L1CC2		; Select leveladdress

	jsr vdu12		; CLS

            jsr L1C1E              ; 20 1E 1C

            jsr L0B18              ; Print roomname

            lda #$FF               ; A9 FF
            sta L0067              ; 85 67

            lda #$D2               ; A9 D2
            sta L005F              ; 85 5F

            lda #$01               ; A9 01
            sta L005C              ; 85 5C
            sta L009C              ; 85 9C

            lda L009D              ; Time's up flag
            sta L0063              ; Death flag

            lda L008E              ; A5 8E
            sta L1127              ; 8D 27 11

            lda L0065              ; A5 65
            sta L1128              ; 8D 28 11

            lda L0A1B              ; AD 1B 0A
            sta L1129              ; 8D 29 11

            lda L0A24              ; AD 24 0A
            sta L112A              ; 8D 2A 11
            sta L0062              ; 85 62

            lda L0064              ; A5 64
            sta L112B              ; 8D 2B 11

            lda #$00               ; A9 00
            sta L0073              ; 85 73
            sta L0070              ; 85 70
            sta L0071              ; 85 71
            sta L0072              ; 85 72

            jsr L0B00              ; Print 'Items collected 000 Time  7:00pm'

            jsr L182D              ; Reset shadowscreen

L1187:      ldy #$FF               ; A0 FF
L1189:      iny                    ; C8
            ldx #$02               ; A2 02
            jsr L11B8              ; 20 B8 11
            cmp #$03               ; C9 03
            beq L1189              ; F0 F6

            sta L007B              ; 85 7B
            sty L0096              ; 84 96
            tya                    ; 98
            asl A                  ; 0A
            asl A                  ; 0A
            adc L007B              ; 65 7B
            sec                    ; 38
            sbc L0096              ; E5 96
            cmp #$06               ; C9 06
            beq L11B7              ; F0 14
            asl A                  ; 0A
            tax                    ; AA
            lda L11DA,X            ; BD DA 11
            sta L11B1+1              ; 8D B2 11
            lda L11DB,X            ; BD DB 11
            sta L11B1+2              ; 8D B3 11
L11B1:      jsr L110F+2              ; 20 11 11

            jmp L1187              ; 4C 87 11

L11B7:      rts                    ; 60

;--------------------------------------------

L11B8:      sty L007B              ; Save Y

            lda #$00               ; A9 00
            sta L0096              ; 85 96

L11BE:      dec L0053              ; Counter $00-$07
            bpl L11CE              ; 10 0C

            lda #$07               ; A9 07
            sta L0053              ; 85 53

            ldy L0052              ; Level data pointer
            lda (L0050),Y          ; Level data pointer
            sta L0054              ; 85 54

            inc L0052              ; Level data pointer

L11CE:      lsr L0054              ; 46 54
            rol L0096              ; 26 96
            dex                    ; CA
            bne L11BE              ; D0 E9

            lda L0096              ; A5 96
            ldy L007B              ; Restore Y

            rts                    ; 60

;--------------------------------------------

L11DA:      .byte <L11EC
L11DB:      .byte >L11EC,<L11F4,>L11F4,<L1203,>L1203,<L124D,>L124D
            .byte <L122A,>L122A,<L120D,>L120D,<L11B7,>L11B7,<L11B7,>L11B7
            .byte <L12A3,>L12A3

L11EC:
            ldx #$03               ; A2 03
            jsr L11B8              ; 20 B8 11
            sta L0072              ; 85 72
            rts                    ; 60
L11F4:
            ldx #$05               ; A2 05
            jsr L11B8              ; 20 B8 11
            sta L0070              ; 85 70
            ldx #$04               ; A2 04
            jsr L11B8              ; 20 B8 11
            sta L0071              ; 85 71
            rts                    ; 60
L1203:
            lda #$01               ; A9 01
            ldx #$00               ; A2 00
            ldy #$05               ; A0 05
            jsr L134A              ; 20 4A 13
            rts                    ; 60
L120D:
            lda L0072              ; A5 72
            sta L0081              ; 85 81
            lda #$02               ; A9 02
            sta L0072              ; 85 72
            lda #$01               ; A9 01
            ldx #$08               ; A2 08
            ldy #$04               ; A0 04
            jsr L134A              ; 20 4A 13
            lda L0081              ; A5 81
            sta L0072              ; 85 72
            lda L0070              ; A5 70
            clc                    ; 18
            adc L0077              ; 65 77
            sta L0070              ; 85 70
            rts                    ; 60
L122A:
            ldx #$04               ; A2 04
            jsr L11B8              ; 20 B8 11
            sta L006E              ; 85 6E
L1231:      ldx #$05               ; A2 05
            jsr L11B8              ; 20 B8 11
            sta L0070              ; 85 70
            ldx #$04               ; A2 04
            jsr L11B8              ; 20 B8 11
            sta L0071              ; 85 71
            jsr L13B1              ; 20 B1 13
            dec L006E              ; C6 6E
            bpl L1231              ; 10 EB
            lda #$00               ; A9 00
            sta L0070              ; 85 70
            sta L0071              ; 85 71
            rts                    ; 60
L124D:
            ldx #$05               ; A2 05
            jsr L11B8              ; 20 B8 11
            sta L0079              ; 85 79
            lda L0070              ; A5 70
            sta L0075              ; 85 75
            lda L0071              ; A5 71
            sta L0076              ; 85 76
            ldx #$04               ; A2 04
            jsr L11B8              ; 20 B8 11
            sta L0071              ; 85 71
            lda L0076              ; A5 76
            sec                    ; 38
            sbc L0071              ; E5 71
            ldx #$01               ; A2 01
            bcs L1272              ; B0 06
            eor #$FF               ; 49 FF
            adc #$01               ; 69 01
            ldx #$FF               ; A2 FF
L1272:      sta L0076              ; 85 76
            stx L007A              ; 86 7A
L1276:      lda #$00               ; A9 00
            sta L007C              ; 85 7C
            lda L0079              ; A5 79
            sta L0074              ; 85 74
            lda L0075              ; A5 75
            sta L0070              ; 85 70
            lda #$01               ; A9 01
            ldx #$80               ; A2 80
            jsr L134A              ; 20 4A 13
            lda L0071              ; A5 71
            clc                    ; 18
            adc L007A              ; 65 7A
            sta L0071              ; 85 71
            dec L0076              ; C6 76
            bpl L1276              ; 10 E2
            lda L0071              ; A5 71
            sec                    ; 38
            sbc L007A              ; E5 7A
            sta L0071              ; 85 71
            lda L0070              ; A5 70
            clc                    ; 18
            adc L0077              ; 65 77
            sta L0070              ; 85 70
            rts                    ; 60
L12A3:
            lda L0070              ; A5 70
            sta L0075              ; 85 75
            lda L0071              ; A5 71
            sta L0076              ; 85 76
            ldx #$01               ; A2 01
            jsr L11B8              ; 20 B8 11
            ldx #$01               ; A2 01
            stx L006D              ; 86 6D
            sta L007D              ; 85 7D
            tay                    ; A8
            beq L12BB              ; F0 02
            ldx #$FF               ; A2 FF
L12BB:      stx L007E              ; 86 7E
            ldx #$04               ; A2 04
            jsr L11B8              ; 20 B8 11
            sta L007A              ; 85 7A
            sta L007F              ; 85 7F
            sec                    ; 38
            sbc L0071              ; E5 71
            sta L0080              ; 85 80
            bcs L12E0              ; B0 13
            eor #$FF               ; 49 FF
            adc #$01               ; 69 01
            sta L0080              ; 85 80
            dec L006D              ; C6 6D
            lda L007A              ; A5 7A
            ldx L0071              ; A6 71
            sta L0071              ; 85 71
            stx L007A              ; 86 7A
            jmp L12F7              ; 4C F7 12
L12E0:      ldx L0080              ; A6 80
            lda L007E              ; A5 7E
            eor #$FF               ; 49 FF
            adc #$00               ; 69 00
            sta L007E              ; 85 7E
            bmi L12F1              ; 30 05
            txa                    ; 8A
            eor #$FF               ; 49 FF
            tax                    ; AA
            inx                    ; E8
L12F1:      txa                    ; 8A
            clc                    ; 18
            adc L0070              ; 65 70
            sta L0070              ; 85 70
L12F7:      lda #$01               ; A9 01
            sta L007C              ; 85 7C
            lda L007A              ; A5 7A
            sta L0074              ; 85 74
            lda L0071              ; A5 71
            sta L0081              ; 85 81
            lda #$01               ; A9 01
            ldx #$80               ; A2 80
            jsr L134A              ; 20 4A 13
            lda L0081              ; A5 81
            sta L0071              ; 85 71
            lda L0070              ; A5 70
            clc                    ; 18
            adc L007E              ; 65 7E
            sta L0070              ; 85 70
            dec L007A              ; C6 7A
            dec L0080              ; C6 80
            bpl L12F7              ; 10 DC
            lda L0075              ; A5 75
            sta L0070              ; 85 70
            lda L0076              ; A5 76
            sta L0071              ; 85 71
            lda L007D              ; A5 7D
            sta L007C              ; 85 7C
            lda L007F              ; A5 7F
            sta L0074              ; 85 74
            lda L0072              ; A5 72
            sta L0081              ; 85 81
            lda #$02               ; A9 02
            sta L0072              ; 85 72
            lda L007D              ; A5 7D
            eor L006C              ; 45 6C
            eor L006D              ; 45 6D
            ldx #$88               ; A2 88
            jsr L134A              ; 20 4A 13
            lda L0081              ; A5 81
            sta L0072              ; 85 72
            lda L0070              ; A5 70
            clc                    ; 18
            adc L0077              ; 65 77
            sta L0070              ; 85 70
            rts                    ; 60

;--------------------------------------------
; Plot row or column sprites
;--------------------------------------------

L134A:      sta L0082              ; 85 82
            stx L0083              ; 86 83
            txa                    ; 8A
            bmi L135F              ; 30 0E

            ldx #$01               ; A2 01
            jsr L11B8              ; 20 B8 11
            sta L007C              ; 85 7C
            tya                    ; 98
            tax                    ; AA
            jsr L11B8              ; 20 B8 11
            sta L0074              ; 85 74

L135F:      lda L0083              ; A5 83
            ldx L0071              ; A6 71
            and #$7F               ; 29 7F
            clc                    ; 18
            adc L007C              ; 65 7C
            sta L007C              ; 85 7C
            bne L136E              ; D0 02
            ldx L0070              ; A6 70

L136E:      lda L0074              ; A5 74
            stx L007B              ; 86 7B
            sec                    ; 38
            sbc L007B              ; E5 7B
            bcs L1380              ; B0 09
            eor #$FF               ; 49 FF
            clc                    ; 18
            adc #$01               ; 69 01
            inc L007C              ; E6 7C
            inc L007C              ; E6 7C
L1380:      sta L0074              ; 85 74
            beq L13A9              ; F0 25
            ldx L007C              ; A6 7C
            lda L13C0,X            ; BD C0 13
            sta L0077              ; 85 77
            lda L13C4,X            ; BD C4 13
            sta L0078              ; 85 78
L1390:      lda L0082              ; A5 82
            beq L1397              ; F0 03
            jsr L13B1              ; 20 B1 13
L1397:      lda L0070              ; A5 70
            clc                    ; 18
            adc L0077              ; 65 77
            sta L0070              ; 85 70
            lda L0071              ; A5 71
            clc                    ; 18
            adc L0078              ; 65 78
            sta L0071              ; 85 71
            dec L0074              ; C6 74
            bne L1390              ; D0 E7
L13A9:      lda L0082              ; A5 82
            beq L13B0              ; F0 03
            jsr L13B1              ; 20 B1 13
L13B0:      rts                    ; 60

L13B1:      ldy L0072              ; A4 72
            ldx L09F1,Y            ; BE F1 09
            lda L09F7,Y            ; B9 F7 09
            jsr L14CA              ; 20 CA 14
            jsr L17ED              ; 20 ED 17
            rts                    ; 60

L13C0:      .byte $01,$00,$FF,$00
L13C4:      .byte $00,$01,$00,$FF,$01,$FF,$01,$FF
            .byte $01,$01,$FF,$FF

L13D0:      ldy L0087              ; Nr of sprites on screen
            beq L13E0              ; F0 0C
L13D4:      ldx L0A00,Y            ; BE 00 0A
            lda L140D,X            ; BD 0D 14
            sta L0A09,Y            ; 99 09 0A
            dey                    ; 88
            bne L13D4              ; D0 F4
L13E0:      rts                    ; 60

L13E1:      .byte $08,$01,$01,$04,$04,$04,$04,$04
            .byte $04,$04,$04,$04,$04,$04,$04,$02
            .byte $02,$02,$02,$04,$04,$08,$08,$08
            .byte $02,$02,$04,$08,$04,$04,$04,$01
            .byte $01,$02,$04,$04,$08,$08,$04,$04
            .byte $04,$02,$02,$02

L140D:      .byte $A0,$00,$01,$02,$06,$0A,$0E,$12
            .byte $16,$1A,$1E,$22,$26,$2A,$2E,$32
            .byte $34,$36,$38,$3A,$3E,$42,$4A,$52
            .byte $5A,$5C,$5E,$62,$6A,$6E,$72,$76
            .byte $77,$78,$7A,$7E,$82,$8A,$92,$96
            .byte $9A,$9E

            ldy #$A2               ; A0 A2
            sta L0096              ; 85 96
            and #$F0               ; 29 F0
            sta L007B              ; 85 7B
            lsr A                  ; 4A
            lsr A                  ; 4A
            lsr A                  ; 4A
            lsr A                  ; 4A
            ora L007B              ; 05 7B
            sta L0088              ; 85 88
            lda L0096              ; A5 96
            and #$0F               ; 29 0F
            sta L007B              ; 85 7B
            asl A                  ; 0A
            asl A                  ; 0A
            asl A                  ; 0A
            asl A                  ; 0A
            ora L007B              ; 05 7B
            rts                    ; 60

;----------------------------------------------
; Calculate screen address sprite
;
;   A = XPOS pixels
;   X = YPOS pixels
;----------------------------------------------

L1454:
	sty y_tmp
	stx x_tmp

	lsr a			; lb=X/8
	lsr a
	lsr a
	sta L0090

	txa
	and #7
	tay
	lda scrtab,y
	clc
	adc L0090
	sta L0090

	txa			; hb=Y/8+$80
	lsr a
	lsr a
	lsr a
	clc
	adc #>$8000
	sta L0091

	cpx #191
	bcc L1456
	lda L0091
	sec
	sbc #$20
	sta L0091
L1456:
	ldx x_tmp
	ldy y_tmp
	rts

;----------------------------------------------
; Plot sprite 16x16 pixels
;
;   01 00
;   03 02
;   05 04
;   .. ..
;   31 30
;
; Screen address     = L0090/91
; Spritedata address = L0092/93
;----------------------------------------------
 
L1476:
	jsr L1454		; Calculate screen address sprite of A,X

	lda L0093		; Set sprite data address in 92/93
	sta L1488+2
	sta L14B4+2
	lda L0092		; lb-Spritedata address
	sta L1488+1
	sta L14B4+1

	ldx L0094		; Nr of bytes to plot for sprite
L1485:
	lda L0095
	beq L14B4

; Big sprite 16x16

L1488:
	lda L44E0,X		; Read sprite data
	and L0089		; Mask
	ldy spr_willy,x		; Get screenpointer
	sta (L0090),Y		; Plot byte on screen
	dex			; Next byte

	bmi L14B2		; Check if 32 bytes plot

	cpx #$0f		; Check if 16 bytes plot
	bne L1488
	inc L0091		; Jump 8 pixels down
L1499:
	jmp L1488
L14B2:
;	inc L0091
	rts

; Small sprite 8x16

L14B4:
	lda L44E0,X		; Read sprite data
	and L0089		; Mask
	ldy spr_willy_s,x	; Get screenpointer
	sta (L0090),Y		; Plot byte on screen
	dex			; Next byte

	bmi L14B5		; Check if 16 bytes plot

	cpx #$07		; Check if 8 bytes plot
	bne L14B4

	inc L0091		; Jump 8 pixels down
	jmp L14B4
L14B5:
	jmp L14B2

spr_willy:
	.byte $e0,$e1,$c0,$c1,$a0,$a1,$80,$81
	.byte $60,$61,$40,$41,$20,$21,$00,$01
	.byte $e0,$e1,$c0,$c1,$a0,$a1,$80,$81
	.byte $60,$61,$40,$41,$20,$21,$00,$01

spr_willy_s:
	.byte $e0,$c0,$a0,$80,$60,$40,$20,$00
	.byte $e0,$c0,$a0,$80,$60,$40,$20,$00

;----------------------------------------------
; Calculate char data address
; A=chr
;----------------------------------------------

L14B3:      asl A                  ; 0A
            rol L0091              ; 26 91
            asl A                  ; 0A
            rol L0091              ; 26 91
            asl A                  ; 0A
            rol L0091              ; 26 91
	clc
	adc #<L4F00
            sta L0090              ; 85 90

            lda L0091              ; A5 91
            and #$07               ; 29 07
            adc #>L4F00               ; 69 4F
            sta L0091              ; 85 91
            ldy #$07               ; A0 07
            rts                    ; 60

;----------------------------------------------
; Plot sprites to build screen
;----------------------------------------------

L14CA:      jsr L14B3              ; Calculate char data address

; Copy sprite data to buffer

            lda L14F8,X            ; BD F8 14
            sta L0089              ; 85 89
L14D2:      lda (L0090),Y          ; B1 90
            sta L47A0,Y            ; 99 A0 47
            dey                    ; 88
            bpl L14D2              ; 10 F8

            lda #$0F               ; A9 0F
            sec                    ; 38
            sbc L0071              ; E5 71
            asl A                  ; 0A
            asl A                  ; 0A
            asl A                  ; 0A
            tax                    ; AA
            lda L0070              ; A5 70
            asl A                  ; 0A
            asl A                  ; 0A
            asl A                  ; 0A
            jsr L1454              ; Calculate screenaddress sprite

; Plot sprite from buffer

            ldx #$07               ; A0 07
L14ED:      lda L47A0,x            ; B9 A0 47
            and L0089  
	ldy scrtab,x            ; 25 89
            sta (L0090),Y          ; 91 90
            dex                    ; 88
            bpl L14ED              ; 10 F6
            rts                    ; 60

scrtab:
	.byte $00,$20,$40,$60,$80,$a0,$c0,$e0

;----------------------------------------------

L14F8:      .byte $00,$FF,$FF,$FF

L14FC:      ldy L0A00,X            ; BC 00 0A
            cpy #$12               ; C0 12
            bne L150B              ; D0 08

            lda L0A09,X            ; BD 09 0A
            eor #$01               ; 49 01
            sta L0A09,X            ; 9D 09 0A

L150B:      lda L13E1,Y            ; B9 E1 13
            sta L007B              ; 85 7B

            lda L0A12,X            ; BD 12 0A
            bne L1525              ; D0 10

            lda L0A1B,X            ; BD 1B 0A
            and #$07               ; 29 07
            lsr A                  ; 4A

            ldy L0A2D,X            ; Direction of guard
            bmi L1535              ; 30 15

            clc                    ; 18
            adc #$04               ; 69 04
            bpl L1535              ; 10 10

L1525:      lda L0057              ; Frame counter
            ldy L0A2D,X            ; Direction of guard
            bmi L152E              ; 30 02

            eor #$FF               ; 49 FF
L152E:      clc                    ; 18
            adc L0A24,X            ; 7D 24 0A
            and #$07               ; 29 07
            lsr A                  ; 4A
L1535:      dec L007B              ; C6 7B
            and L007B              ; 25 7B
            clc                    ; 18
            adc L0A09,X            ; 7D 09 0A
            lsr A                  ; 4A
            ror L007B              ; 66 7B
            lsr A                  ; 4A
            ror L007B              ; 66 7B
            lsr A                  ; 4A
            ror L007B              ; 66 7B
            clc                    ; 18

            sta L0093              ; hb-Spritedata address

            lda L007B              ; A5 7B
            and #$E0               ; 29 E0
            sta L0092              ; lb-Spritedata address

	clc
	lda L0092		; lb-Spritedata address
	adc #<L3000
	sta L0092		; lb-Spritedata address
	lda L0093		; hb-Spritedata address
	adc #>L3000
	sta L0093		; hb-Spritedata address

            ldy L0A48,X            ; BC 48 0A
            lda L14F8,Y            ; B9 F8 14
            sta L0089              ; 85 89
            txa                    ; 8A
            tay                    ; A8
            ldx L0A24,Y            ; BE 24 0A
            rts                    ; 60

L155F:      lda L008A              ; A5 8A
            ldy L0A12,X            ; BC 12 0A
            beq L1569              ; F0 03
            clc                    ; 18
            adc #$09               ; 69 09
L1569:      tay                    ; A8
            lda L0A2D,X            ; Direction of guard
            sta L0A5A,X            ; 9D 5A 0A
            clc                    ; 18
            adc L0A1B,Y            ; 79 1B 0A
            cmp L0A12,X            ; DD 12 0A
            bcs L157C              ; B0 03
            sta L0A1B,Y            ; 99 1B 0A
L157C:      cmp L0A36,X            ; DD 36 0A
            bcc L1588              ; 90 07
            beq L1588              ; F0 05
            cmp L0A3F,X            ; DD 3F 0A
            bcc L1596              ; 90 0E
L1588:      lda L0A2D,X            ; Direction of guard
            eor #$FF               ; 49 FF
            sec                    ; 38
            adc #$00               ; 69 00
            sta L0A2D,X            ; Direction of guard
            lda L0A1B,Y            ; B9 1B 0A
L1596:      sta L0A1B,Y            ; 99 1B 0A
            rts                    ; 60

;--------------------------------------------
; Plot all sprites in table
;--------------------------------------------

L159A:      ldx L0087              ; Check nr of sprites to plot
            beq L15B4              ; F0 16
            stx L008A              ; 86 8A

L15A0:      dec L0A63,X            ; DE 63 0A
            bne L15AE              ; D0 09

            jsr L14FC              ; 20 FC 14
            jsr L197C              ; Wipe + plot sprite
            jsr L1935              ; Wipe sprite next to sprite

L15AE:      dec L008A              ; Next sprite
            ldx L008A              ; A6 8A
            bne L15A0              ; If not all plot, repeat

L15B4:      rts                    ; 60

;--------------------------------------------
; Plot player Willy
;
;     A    = XPOS
;     X    = YPOS
; Carry    = 0 - Copy backgroundmask to buffer
;            1 - Copy buffer to backgroundmask
; L0092/93 = Spritedata address
;--------------------------------------------

L15B5:
	ldy L0092		; Get lb-Spritedata address
	sty L15E9+1
	sty L15D7+1
	sty L15DF+1

	ldy L0093		; Get hb-Spritedata address
	sty L15E9+2
	sty L15D7+2
	sty L15DF+2
L15CB:
	jsr L1454		; Screenaddress in L0090/91

	sec
L15D3:
	bcc L15E5

; Copy backgroundmask to buffer

	ldx #$1f		; Set bytecounter
L15D0:
	ldy spr_willy,x		; Load screenpointer
	lda (L0090),y		; Get screendata
L15D7:
	and L110F+2,x		; AND with spritedata
	sta L4720,x		; Save background in buffer

	lda (L0090),y		; Get screendata
L15DF:
	ora L110F+2,x		; ORA with spritedata
	sta (L0090),y		; Put sprite on screen

	dex			; Next byte
	bmi L1615		; If all bytes plotted, jump

	cpx #$0f		; If 16 bytes plotted
	bne L15D0
	inc L0091		; Adjust screenaddress
	jmp L15D0

; Copy buffer to backgroundmask

L15E5:
	jsr $fe6b

	ldx #$1f
L15E6:
	ldy spr_willy,x		; Load screenpointer
L15E9:
	lda L110F+2,x		; Get spritedata
	eor #$FF		; Invert
	and (L0090),Y		; AND with screendata
	ora L4720,X		; ORA with buffer
	sta (L0090),Y		; Restore screen

	dex			; Next byte
	bmi L1615		; If all bytes plotted, jump

	cpx #$0f		; If 16 bytes plotted
	bne L15E6
	inc L0091		; Adjust screenaddress
	jmp L15E6
L1615:
	rts

;---------------------------------------------
; Read key in matrix
;
; Select row and read column:
;
; X = key to be checked
;
; columnbit = 0 - key pressed,
;             1 - key not pressed
;---------------------------------------------

L1616:
	bit L0381		; Check controls
	bvs L1617		; Control = Joykey
	bmi L1618		; Control = JoyMMC

; Control = Keyboard

L1619:
	lda key_row,x
	ora #$f0
	sta $b000		; Select row

	lda $b001		; Read column
	and key_column,x
	bne not_pressed
	lda key_return,x
	rts

not_pressed:
	lda #0
	rts
;                      L   R   U  JL  JR  JU
key_row:	.byte $01,$03,$06,$00,$00,$00
key_column:	.byte $20,$20,$02,$02,$08,$01
key_return:	.byte $fe,$02,$ff,$fe,$02,$ff

; Control = Joykey

L1617:
	inx
	inx
	inx
	jmp L1619

; Control = JoyMMC

L1618:
	jsr joyget
	sta joy_scan

	cpx #0
	bne scan_r
scan_l:
	and #$02
	beq scan_no
	lda #$fe
	rts
scan_r:
	cpx #1
	bne scan_u
	lda joy_scan
	and #$01
	beq scan_no
	lda #$02
	rts
scan_u:
	cpx #2
	bne scan_no
	lda joy_scan
	and #$10
	beq scan_no
	lda #$ff
	rts
scan_no:
	lda #0
	rts

joy_scan:	.byte 0
	
;---------------------------------------------
; Scan keys
;
;  Z  - Left
;  X  - Right
; RET - Jump
;---------------------------------------------

L161F:
	lda L0069		; Check jumponly flag
	beq L1629

	lda #$00		; Reset jumponly flag
	sta L0069
	beq L163E		; Jump scan up
L1629:
	ldx #0			; Scan Z-key (left)
	jsr L1616		; Read matrix
	and #$FE		; Filter left
	sta L008E		; Set action

	ldx #1			; Scan X-key (right)
	jsr L1616		; Read matrix
	and #$02		; Filter right
	clc
	adc L008E
	sta L008E		; Set action
L163E:
	ldx #2			; Scan RET-key (jump)
	jsr L1616		; Read matrix
	sta L008B		; Set jump

	bit L008F		; Check if jumping
	bmi not_jumping
	lda #$80
	sta L008F
not_jumping:
	rts

;--------------------------------------------
;
;--------------------------------------------

L1646:      lda L009F              ; A5 9F
            bne L1650              ; D0 06
            lda L0064              ; A5 64
            ora L0065              ; 05 65
            bne L1653              ; D0 03
L1650:      jsr L161F              ; 20 1F 16
L1653:      jsr L0B03              ; 20 03 0B
            jsr L1E17              ; 20 17 1E
            lda L008E              ; A5 8E
            beq L166E              ; F0 11
            cmp L0A2D              ; Direction of guard
            beq L1667              ; F0 05
            sta L0A2D              ; Direction of guard
            bne L166E              ; D0 07
L1667:      clc                    ; 18
            adc L0A1B              ; 6D 1B 0A
            sta L0A1B              ; 8D 1B 0A
L166E:      jsr L19FE              ; 20 FE 19
            beq L167C              ; F0 09
            lda L0A1B              ; AD 1B 0A
            sec                    ; 38
            sbc L008E              ; E5 8E
            sta L0A1B              ; 8D 1B 0A
L167C:      ldx #$00               ; A2 00
            jsr L183B              ; 20 3B 18
            jsr L1A2F              ; 20 2F 1A
            jsr L1A51              ; 20 51 1A
            lda L0065              ; A5 65
            cmp #$07               ; C9 07
            bcs L16D1              ; B0 44
            lda L006B              ; A5 6B
            beq L16A2              ; F0 11
            lda L008E              ; A5 8E
            beq L16A2              ; F0 0D
            ldx L006C              ; A6 6C
            cmp L1BC1,X            ; DD C1 1B
            beq L16A2              ; F0 06
            inc L0A24              ; EE 24 0A
            inc L0A24              ; EE 24 0A
L16A2:      ldx #$00               ; A2 00
            stx L006B              ; 86 6B
            jsr L183B              ; 20 3B 18
            jsr L185D              ; 20 5D 18
            ldx L006C              ; A6 6C
            lda L0A1B              ; AD 1B 0A
            and #$07               ; 29 07
            cmp L1BBD,X            ; DD BD 1B
            bne L16D1              ; D0 19
            ldy L1BBF,X            ; BC BF 1B
            lda (L0060),Y          ; B1 60
            cmp #$03               ; C9 03
            bne L16D1              ; D0 10
            lda L008E              ; A5 8E
            cmp L1BC1,X            ; DD C1 1B
            bne L16D1              ; D0 09
            lda L0A24              ; AD 24 0A
            sec                    ; 38
            sbc #$02               ; E9 02
            sta L0A24              ; 8D 24 0A
L16D1:      jsr L1B0F              ; 20 0F 1B
            jsr L1B0F              ; 20 0F 1B
            jsr L1B0F              ; 20 0F 1B
            jsr L1CD6              ; 20 D6 1C
            jsr L18D7              ; 20 D7 18
            ldx #$00               ; A2 00
            jsr L183B              ; 20 3B 18
            jsr L0B0C              ; 20 0C 0B
            jsr L159A              ; 20 9A 15
            lda #$B0               ; A9 B0
            sta L15D3              ; 8D D3 15
            dec L009C              ; C6 9C
            beq L1703              ; F0 0F
            lda #$00               ; A9 00

;            lda L0004              ; A5 04
;L16F8:      cmp L0004              ; C5 04
;            beq L16F8              ; F0 FC

            lda L008C              ; A5 8C
            ldx L008D              ; A6 8D
            jsr L15CB              ; 20 CB 15
L1703:      jsr L159A              ; 20 9A 15
            jsr L0B0F              ; 20 0F 0B
            lda #$90               ; A9 90
            sta L15D3              ; 8D D3 15
            dec L009C              ; C6 9C
            bpl L1727              ; 10 15
            ldx #$00               ; A2 00
            jsr L14FC              ; 20 FC 14
            lda L0A1B              ; AD 1B 0A
            and #$F8               ; 29 F8
            sta L008C              ; 85 8C
            stx L008D              ; 86 8D
            ldx L008D              ; A6 8D
            bmi L1727              ; 30 03
            jsr L15B5              ; 20 B5 15
L1727:      ldx #$00               ; A2 00
            stx L009C              ; 86 9C
            jsr L183B              ; 20 3B 18
            jsr L1A6F              ; 20 6F 1A
            jsr L1BD5              ; 20 D5 1B
            lda L0063              ; Death flag
            beq L1762              ; F0 2A

; Death sound

	lda #10
	sta L1764
L1728:
	lda L1764
	ldy #60
	jsr L0B27		; Make death high note sound
	lda L1764
	ora #32
	ldy #40
	jsr L0B27		; Make death low note sound
	inc L1764
	lda L1764
	cmp #20
	bne L1728

            dec L009B              ; Lives
            lda L1127              ; AD 27 11
            sta L008E              ; 85 8E
            lda L1128              ; AD 28 11
            sta L0065              ; 85 65
            lda L1129              ; AD 29 11
            sta L0A1B              ; 8D 1B 0A
            lda L112A              ; AD 2A 11
            sta L0A24              ; 8D 24 0A
            lda L112B              ; AD 2B 11
            sta L0064              ; 85 64
            jsr L1C5E              ; 20 5E 1C

L1762:      rts                    ; 60

L1763:      .byte $00,$04,$04,$04,$04,$02,$02,$02
            .byte $00,$00,$00,$00,$FE,$FE,$FE,$FE
            .byte $FC,$FC,$FC

L1764:	.byte 0

;--------------------------------------------
; Decode level data
;--------------------------------------------

L1776:      ldx #$03               ; A2 03
            jsr L11B8              ; 20 B8 11
            sta L0087              ; Nr of sprites on screen
            tay                    ; A8
            beq L17E3              ; F0 63

L1780:      ldx #$06               ; A2 06
            jsr L11B8              ; 20 B8 11
            sta L0A00,Y            ; 99 00 0A

            ldx #$05               ; A2 05
            jsr L17E6              ; 20 E6 17
            sta L0A1B,Y            ; 99 1B 0A

            ldx #$04               ; A2 04
            jsr L17E6              ; 20 E6 17
            sta L0A24,Y            ; 99 24 0A

            ldx #$05               ; A2 05
            jsr L17E6              ; 20 E6 17
            sta L0A36,Y            ; 99 36 0A

            ldx #$05               ; A2 05
            jsr L17E6              ; 20 E6 17
            sta L0A3F,Y            ; 99 3F 0A

            ldx #$01               ; A2 01
            jsr L11B8              ; 20 B8 11
            asl A                  ; 0A
            beq L17B2              ; F0 02

            lda #$FF               ; A9 FF
L17B2:      sta L0A12,Y            ; 99 12 0A

            ldx #$01               ; A2 01
            jsr L11B8              ; 20 B8 11
            sta L0097              ; 85 97

            ldx #$03               ; A2 03
            jsr L11B8              ; 20 B8 11
            sta L0A2D,Y            ; Direction of guard

            ldx L0097              ; A6 97
            bne L17D0              ; D0 08
            eor #$FF               ; 49 FF
            clc                    ; 18
            adc #$01               ; 69 01
            sta L0A2D,Y            ; Direction of guard

L17D0:      ldx #$02               ; A2 02
            jsr L11B8              ; 20 B8 11
            sta L0A48,Y            ; 99 48 0A
            lda #$00               ; A9 00
            sta L0A5A,Y            ; 99 5A 0A
            sta L0A51,Y            ; 99 51 0A
            dey                    ; 88
            bne L1780              ; D0 9D

L17E3:      jmp L13D0              ; 4C D0 13

;--------------------------------------------

L17E6:      jsr L11B8              ; 20 B8 11
            asl A                  ; 0A
            asl A                  ; 0A
            asl A                  ; 0A
            rts                    ; 60

;--------------------------------------------
; Set tile in shadowscreen (L4800-L49FF)
; X x Y = 32 x 16
;
; Y/X 00: 01: 02: 03:           31:
; 15: 000 001 002 003 ......... 01f
; 14: 020 021 022 023 ......... 02f
;          .....                ..
; 00: 1e0 1e1 1e2 1e3 ......... 1ff
;
; L0070 = X
; L0071 = Y
; L0072 = Tilenr
;
; L0090/91 = Screen address
;
; L0060/61 = Shadowscreen address
;--------------------------------------------

; Calculate relative shadowscreen address

L17ED:
	ldy #0			
	sty L0061
	lda #$0F
	sec
	sbc L0071
	asl A
	asl A
	asl A
	asl A
	asl A
	rol L0061
	adc L0070
	sta L0060

; Add shadowscreen baseaddress to relative address

	clc			
	lda L0060
	adc #<L4800
	sta L0060
	lda L0061
	adc #>L4800
	sta L0061

; Check tilenr

	lda L0072		
	adc #$01	
	cmp #$05
	bcc L1828		; If tilenr<5, jump
	beq L1826		; If tilenr=5, jump

	inc L0067		; Tilenr>5
	ldx L0067

; Save screenaddress

	lda L0090
	sta L0A8C,X
	lda L0091
	sta L0A98,X

; Save Shadowscreen address

	lda L0060
	sta L0AA8,X
	lda L0061
	sta L0AB4,X

; Adjust tilenr

	txa
	ora #$10
L1826:
	ora #$C0

; Set tilenr in shadowscreen

L1828:
	ldy #$00
	sta (L0060),Y
	rts

;--------------------------------------------
; Reset Reset shadowscreen
;--------------------------------------------

L182D:      ldy #$00               ; A0 00
            lda #$80               ; A9 80
L1831:      sta L4800,Y            ; 99 00 48
            sta L4900,Y            ; 99 00 49
            iny                    ; C8
            bne L1831              ; D0 F7
            rts                    ; 60

;--------------------------------------------
; 
;--------------------------------------------

L183B:
            lda #$00               ; A9 24
            sta L0061              ; 85 61
            lda L0A24,X            ; BD 24 0A
            sec                    ; 38
            sbc #$08               ; ???????? #$08
            bcs L1849              ; B0 02
            dec L0061              ; C6 61
L1849:      asl A                  ; 0A
            asl A                  ; 0A
            rol L0061              ; 26 61
            and #$E0               ; 29 E0
            sta L007B              ; 85 7B
            lda L0A1B,X            ; BD 1B 0A
            lsr A                  ; 4A
            lsr A                  ; 4A
            lsr A                  ; 4A
            clc                    ; 18
            adc L007B              ; 65 7B
            sta L0060              ; 85 60

	clc
	lda L0060
	adc #<L4800
	sta L0060
	lda L0061
	adc #>L4800
	sta L0061
	lda L0060
	sta L007B
            rts                    ; 60

;--------------------------------------------

L185D:      ldx L006C              ; A6 6C
            ldy L1BC3,X            ; BC C3 1B
            lda (L0060),Y          ; B1 60
            beq L1868              ; F0 02
            bpl L1872              ; 10 0A
L1868:      inx                    ; E8
            ldy L1BC3,X            ; BC C3 1B
            lda (L0060),Y          ; B1 60
            beq L18BA              ; F0 4A
            bmi L18BA              ; 30 48
L1872:      cmp #$03               ; C9 03
            bne L187D              ; D0 07
            jsr L1B61              ; 20 61 1B
            beq L18BA              ; F0 3F
            bpl L188A              ; 10 0D
L187D:      sta L007B              ; 85 7B
            lda L0A24              ; AD 24 0A
            and #$07               ; 29 07
            cmp #$03               ; C9 03
            bcs L18BA              ; B0 32
            lda L007B              ; A5 7B
L188A:      cmp #$04               ; C9 04
            bne L1891              ; D0 03
            jsr L1BC6              ; 20 C6 1B
L1891:      lda L0064              ; A5 64
            beq L18AD              ; F0 18
            lda L009A              ; A5 9A
            bne L18A5              ; D0 0C
            lda L0A24              ; AD 24 0A
            sec                    ; 38
            sbc L0062              ; E5 62
            bmi L18A7              ; 30 06
            cmp #$21               ; C9 21
            bcc L18A7              ; 90 02
L18A5:

.if cheat=0
	inc L0063		; Set death flag
.endif

L18A7:      lda #$00               ; A9 00
            sta L0065              ; 85 65
            sta L0064              ; 85 64
L18AD:      lda L006B              ; A5 6B
            bne L18B9              ; D0 08
            lda L0A24              ; AD 24 0A
            and #$F8               ; 29 F8
            sta L0A24              ; 8D 24 0A
L18B9:      rts                    ; 60

L18BA:      lda L0065              ; A5 65
            bne L18D6              ; D0 18
            lda L0064              ; A5 64
            bne L18C9              ; D0 07
            lda L0A24              ; AD 24 0A
            sta L0062              ; 85 62
            sta L0064              ; 85 64
L18C9:      lda #$00               ; A9 00
            sta L008E              ; 85 8E
            lda L0A24              ; AD 24 0A
            clc                    ; 18
            adc #$04               ; 69 04
            sta L0A24              ; 8D 24 0A
L18D6:      rts                    ; 60

;--------------------------------------------
;
; L0060/61 = Shadowscreen address
;--------------------------------------------

L18D7:      lda L0087              ; Nr of sprites on screen
            beq L1934              ; Jump if no sprites to plot

            sta L008A              ; Set nr of sprites
L18DD:      ldx L008A              ; A6 8A
            jsr L155F              ; 20 5F 15

            lda #$01               ; A9 01
            sta L0A63,X            ; 9D 63 0A

            lda #$00               ; A9 00
            sta L0A6C,X            ; 9D 6C 0A
            jsr L183B              ; 20 3B 18

            ldy L0A51,X            ; BC 51 0A
            sty L007B              ; 84 7B
            cmp L0A51,X            ; DD 51 0A
            beq L1930              ; F0 37

            inc L0A6C,X            ; FE 6C 0A
            sta L0A51,X            ; 9D 51 0A
            txa                    ; 8A
            ora #$80               ; 09 80
            ldy #$20               ; A0 20
            sta (L0060),Y          ; 91 60

            iny                    ; C8
            sta (L0060),Y          ; 91 60

            ldy #$40               ; A0 40
            sta (L0060),Y          ; 91 60

            iny                    ; C8
            sta (L0060),Y          ; 91 60

            lda L007B              ; A5 7B
            beq L1930              ; F0 1C

            lda L0A5A,X            ; BD 5A 0A
            beq L1930              ; F0 17
            asl A                  ; 0A
            lda L0A12,X            ; BD 12 0A
            and #$02               ; 29 02
            adc #$00               ; 69 00
            tax                    ; AA
            ldy L1974,X            ; BC 74 19

            lda L004E              ; A5 4E
            sta (L0060),Y          ; 91 60

            ldy L1978,X            ; BC 78 19
            sta (L0060),Y          ; 91 60
            ldx L008A              ; A6 8A

L1930:      dec L008A              ; C6 8A
            bne L18DD              ; D0 A9

L1934:      rts                    ; 60

;--------------------------------------------
; Wipe sprites
;
; L008A = 0/1/2
;--------------------------------------------

L1935:
            ldx L008A              ; A6 8A
            lda L0A6C,X            ; BD 6C 0A
            beq L1973              ; F0 37

            lda L0A12,X            ; BD 12 0A
            bne L1973              ; D0 32

	lda #$00		; Sprite 16x8
	sta L0095

	lda #$0F
	sta L0094		; Nr of bytes to plot for sprite

            lda #$F8               ; A9 F8
            ldy L0A5A,X            ; BC 5A 0A
            bpl L1952              ; 10 02

            lda #$0C               ; A9 0C
L1952:      clc                    ; 18
            adc L0A1B,X            ; 7D 1B 0A
            sta L007B              ; 85 7B

; Wipe sprite 16x8

            lda #<L4700               ; A9 00
            sta L0092              ; lb-Spritedata address
            lda #>L4700               ; A9 47
            sta L0093              ; hb-Spritedata address

            lda L0A24,X            ; BD 24 0A
            tax                    ; AA
            lda L007B              ; A5 7B
            jsr L1476              ; Plot sprite 16x16

	lda #$08		; Sprite 16x16
	sta L0095

	lda #$1F
	sta L0094		; Nr of bytes to plot for sprite

            ldx L008A              ; A6 8A

L1973:      rts                    ; 60

;--------------------------------------------

L1974:      .byte $1F,$22,$00,$60
L1978:      .byte $3F,$42,$01,$61

;--------------------------------------------
; Check if vertical sprite is moving up or down
;
; Wipe top for sprite moving down and
; Wipe bottom for sprite moving up
;
; Y = sprite nr
;--------------------------------------------

L197C:
	lda L0A12,Y		; Get sprite moving type
	beq L19C5		; Jump if static

	lda L0A5A,Y		; Get sprite direction
	bmi L19CE		; Jump if moving up

; Vert.sprite is moving down

	ldx L0A00,Y		; ???
	cpx #$21
	beq L19C2		; Plot sprite

	sta L007B		; Save sprite direction
	asl A
	sbc #$00
	sta L0094		; Nr of bytes to plot for sprite

	lda L0092		; Save current sprite data address
	sta L0097
	lda L0093
	sta L0096

; Wipe sprite 16x8

	lda #<L4700		; Set clear sprite address
	sta L0092
	lda #>L4700
	sta L0093

	lda L0A24,Y		; Get POS vert.guard
	sec
	sbc #8			; Correction screen address

	tax			; Set YPOS
	lda L0A1B,Y		; Set XPOS
	jsr L1476		; Wipe top part

	inc L0091		; Correction sprite address

	lda L0096		; Restore hb sprite address
	sta L0093

	lda #$1F		; Set Nr of bytes to plot for sprite
	sta L0094

; plot sprite 16x16

	lda L0097		; Set lb sprite data
	sta L1488+1
	sta L14B4+1

	jsr L19F4		; Wipe screenbytes below sprite

	rts

L19C2:
	ldx L0A24,Y            ; BE 24 0A
L19C5:
	lda L0A1B,Y            ; B9 1B 0A
	and #$F8               ; 29 F8
	jsr L1476              ; Plot sprite 16x16
	rts                    ; 60

; Vert.sprite is moving up

L19CE:
	lda L0A1B,Y		; Get POS hor.guard
	jsr L1476		; Plot sprite 16x16

	lda #<L4700		; Set lb sprite data
	sta L1488+1
	sta L14B4+1

	lda #>L4700		; Set hb sprite data
	sta L0093

	ldy L008A		; Get guard nr
	lda L0A5A,Y		; Get direction guard

	eor #$FF		; Calculate nr of bytes to plot
	clc
	adc #$01
	asl A
	pha			; Save nr byte to plot +1
	sbc #$00
	sta L0094		; Nr of bytes to plot for sprite
	
	pla			; Set wipe screen address
	lsr a
	tay
	clc
	lda L0090
	adc scrtab,y
	sta L0090
	lda L0091
	adc #0
	sta L0091	

	jsr L19F4		; Wipe screenbytes below sprite

	lda #$1F
	sta L0094		; Nr of bytes to plot for sprite

	rts

L19F4:
	ldx L0094		; Nr of bytes to plot for sprite

	lda L0093		; Set hb sprite data
	sta L1488+2
	sta L14B4+2
	jmp L1499		; Wipe bytes

;--------------------------------------------

L19FE:      ldx #$00               ; A2 00
            stx L0096              ; 86 96
            jsr L183B              ; 20 3B 18
            ldy #$20               ; A0 20
            lda L008E              ; A5 8E
            beq L1A1E              ; F0 13
            bmi L1A0E              ; 30 01
            iny                    ; C8
L1A0E:      jsr L1A21              ; 20 21 1A
            jsr L1A21              ; 20 21 1A
            lda L0A24              ; AD 24 0A
            and #$07               ; 29 07
            beq L1A1E              ; F0 03
            jsr L1A21              ; 20 21 1A
L1A1E:      lda L0096              ; A5 96
            rts                    ; 60

L1A21:      lda (L0060),Y          ; B1 60
            cmp #$02               ; C9 02
            bne L1A29              ; D0 02
            inc L0096              ; E6 96
L1A29:      tya                    ; 98
            clc                    ; 18

            adc #$20               ; 69 20

            tay                    ; A8
            rts                    ; 60

L1A2F:      lda L008B              ; A5 8B
            beq L1A50              ; F0 1D
            inc L0064              ; E6 64
            lda #$12               ; A9 12
            sta L0065              ; 85 65
            inc L008B              ; E6 8B
            lda L0A24              ; AD 24 0A
            sec                    ; 38
            sbc #$08               ; E9 08
            sta L0062              ; 85 62
            jsr L1A51              ; 20 51 1A
            beq L1A50              ; F0 08

;	ldy #100
;            lda #100               ; A9 08
;            jsr L0B27              ; Make ???? sound

            jsr L1E17              ; 20 17 1E
L1A50:      rts                    ; 60

L1A51:      lda L0A24              ; AD 24 0A
            and #$07               ; 29 07
            bne L1A6E              ; D0 16
            ldy #$00               ; A0 00
            lda (L0060),Y          ; B1 60
            eor #$02               ; 49 02
            beq L1A67              ; F0 07
            iny                    ; C8
            lda (L0060),Y          ; B1 60
            eor #$02               ; 49 02
            bne L1A6E              ; D0 07
L1A67:      jsr L1E17              ; 20 17 1E
            lda #$00               ; A9 00
            sta L0065              ; 85 65
L1A6E:      rts                    ; 60

L1A6F:      lda L0A24              ; AD 24 0A
            ora #$07               ; 09 07
            sec                    ; 38
            sbc L0A24              ; ED 24 0A
            sta L0096              ; 85 96
            ldy #$20               ; A0 20
            lda #$1E               ; A9 1E
            jsr L1A89              ; 20 89 1A
            ldy #$21               ; A0 21
            lda #$1F               ; A9 1F
            jsr L1A89              ; 20 89 1A
            rts                    ; 60

L1A89:      sta L0097              ; 85 97
            ldx L0096              ; A6 96
            jsr L1AA2              ; 20 A2 1A
            ldx #$07               ; A2 07
            jsr L1AA2              ; 20 A2 1A
            lda #$07               ; A9 07
            sec                    ; 38
            sbc L0096              ; E5 96
            beq L1AA1              ; F0 05
            tax                    ; AA
            dex                    ; CA
            jsr L1AA2              ; 20 A2 1A
L1AA1:      rts                    ; 60

L1AA2:      tya                    ; 98
            clc                    ; 18

            adc #$20               ; 69 20

            sta L007B              ; 85 7B
            lda (L0060),Y          ; B1 60
            sta L0068              ; 85 68
            bpl L1AC2              ; 10 14
            ldy L0097              ; A4 97
L1AB0:      lda L4720,Y            ; B9 20 47
            beq L1AB8              ; F0 03
            jsr L1ACF              ; 20 CF 1A
L1AB8:      dey                    ; 88
            dey                    ; 88
            dex                    ; CA
            bpl L1AB0              ; 10 F3
            sty L0097              ; 84 97
            ldy L007B              ; A4 7B
            rts                    ; 60

L1AC2:      inx                    ; E8
            txa                    ; 8A
            asl A                  ; 0A
            eor #$FF               ; 49 FF
            sec                    ; 38
            adc L0097              ; 65 97
            sta L0097              ; 85 97
            ldy L007B              ; A4 7B
            rts                    ; 60

L1ACF:      stx L0070              ; 86 70
            sty L0071              ; 84 71
            lda #$00               ; A9 00
            sta L4720,Y            ; 99 20 47
            lda L0068              ; A5 68
            and #$7F               ; 29 7F
            beq L1B07              ; F0 29
            cmp #$4E               ; C9 4E
            bcs L1AF3              ; B0 11
            cmp #$02               ; C9 02
            bne L1AEC              ; D0 06
            lda L0055              ; Current Room nr
            cmp #$15               ; C9 15
            beq L1AEE              ; F0 02
L1AEC:

.if cheat=0
	inc L0063		; Set death flag
.endif

L1AEE:      ldx L0070              ; A6 70
            ldy L0071              ; A4 71
            rts                    ; 60

L1AF3:      and #$0F               ; 29 0F
            tax                    ; AA
            lda L0A80,X            ; BD 80 0A
            beq L1AEE              ; F0 F3
            lda #$00               ; A9 00
            sta L0A80,X            ; 9D 80 0A
            stx L0067              ; 86 67
            jsr L0B06              ; Print nr of items collected
            bpl L1AEE              ; 10 E7
L1B07:      lda L006F              ; A5 6F
            bne L1AEE              ; D0 E3

.if cheat=0
	inc L0063		; Set death flag
.endif

            bne L1AEE              ; D0 DF
L1B0F:      ldx L0066              ; A6 66
            bmi L1B60              ; 30 4D
            cpx L0067              ; E4 67
            bcc L1B58              ; 90 41
            ldx L0067              ; A6 67

            lda L0A8C,X            ; BD 8C 0A
            sta L0090              ; 85 90
            lda L0A98,X            ; BD 98 0A
            sta L0091              ; 85 91

            ldy L0A80,X            ; BC 80 0A
            beq L1B36              ; F0 0E
            dec L0A80,X            ; DE 80 0A
            bne L1B32              ; D0 05
            lda #$04               ; A9 04
            sta L0A80,X            ; 9D 80 0A
L1B32:      ldy L0A80,X            ; BC 80 0A
            dey                    ; 88
L1B36:      lda L14F8,Y            ; B9 F8 14
            sta L0089              ; 85 89

; Plot char

	stx x_tmp
	ldx #$07               ; A0 07
L1B3D:
	ldy scrtab,x
	lda L47A0,x            ; B9 A0 47
            and L0089              ; 25 89
            sta (L0090),Y          ; 91 90
	dex                    ; 88
            bpl L1B3D              ; 10 F6


	ldx x_tmp

            lda L0AA8,X            ; BD A8 0A
            sta L0090              ; 85 90
            lda L0AB4,X            ; BD B4 0A
            sta L0091              ; 85 91

            txa                    ; 8A
            ora #$E0               ; 09 E0
            ldy #$00               ; A0 00
            sta (L0090),Y          ; 91 90
L1B58:      dec L0067              ; C6 67
            bpl L1B60              ; 10 04
            lda #$0B               ; A9 0B
            sta L0067              ; 85 67
L1B60:      rts                    ; 60

x_tmp:	.byte 0
y_tmp:	.byte 0

L1B61:      ldx L006C              ; A6 6C
            tya                    ; 98
            cmp L1BC3,X            ; DD C3 1B
            bne L1BB6              ; D0 4D
            inc L006B              ; E6 6B
            lda L0A24              ; AD 24 0A
            and #$07               ; 29 07
            sta L007B              ; 85 7B
            lda L0A1B              ; AD 1B 0A
            and #$07               ; 29 07
            eor L1BB9,X            ; 5D B9 1B
            sec                    ; 38
            adc L1BBB,X            ; 7D BB 1B
            bmi L1B8C              ; 30 0C
            cmp L007B              ; C5 7B
            bcc L1B8C              ; 90 08
            dec L0A24              ; CE 24 0A
            dec L0A24              ; CE 24 0A
            bcs L1BB3              ; B0 27
L1B8C:      clc                    ; 18
            adc #$04               ; 69 04
            cmp L007B              ; C5 7B
            bcc L1BB3              ; 90 20
            sbc #$02               ; E9 02
            sta L007B              ; 85 7B
            lda L0A24              ; AD 24 0A
            and #$F8               ; 29 F8
            ora L007B              ; 05 7B
            sbc #$01               ; E9 01
            sta L0A24              ; 8D 24 0A
            lda #$01               ; A9 01
            ldx L0055              ; Current Room nr
            cpx #$16               ; E0 16
            beq L1BAF              ; F0 04
            cpx #$1D               ; E0 1D
            bne L1BB1              ; D0 02
L1BAF:      lda #$04               ; A9 04
L1BB1:      tax                    ; AA
            rts                    ; 60

L1BB3:      lda #$00               ; A9 00
            rts                    ; 60
L1BB6:      lda #$FF               ; A9 FF
            rts                    ; 60

L1BB9:      .byte $FF,$00
L1BBB:      .byte $05,$FE
L1BBD:      .byte $00,$06
L1BBF:      .byte $41,$40
L1BC1:      .byte $02,$FE
L1BC3:      .byte $61,$60,$61

L1BC6:      lda L008E              ; A5 8E
            beq L1BCE              ; F0 04
            cmp L006A              ; C5 6A
            bne L1BD4              ; D0 06
L1BCE:      lda L006A              ; A5 6A
            sta L008E              ; 85 8E
            sta L0069              ; 85 69
L1BD4:      rts                    ; 60

L1BD5:      ldy #$00               ; A0 00
            lda L0A1B              ; AD 1B 0A
            beq L1C03              ; F0 27

            iny                    ; C8
            cmp #$F6               ; C9 F6
            beq L1C03              ; F0 22

            iny                    ; C8

            ldx #$68               ; A2 68
            lda L0A24              ; AD 24 0A
            bmi L1C0B              ; 30 22

            iny                    ; C8

            ldx #$01               ; A2 01
            cmp #$71               ; C9 71
            bcc L1C1D              ; 90 2D

            lda L0064              ; A5 64
            beq L1C0B              ; F0 17

            lda L0A24              ; AD 24 0A
            sec                    ; 38
            sbc L0062              ; E5 62
            cmp #$22               ; C9 22
            bcc L1C0B              ; 90 0D

            sta L009A              ; 85 9A

            jmp L1C0B              ; 4C 0B 1C

L1C03:      eor #$F4               ; 49 F4
            sta L0A1B              ; 8D 1B 0A
            jmp L1C12              ; 4C 12 1C

L1C0B:      stx L0A24              ; 8E 24 0A

            lda #$00               ; A9 00
            sta L008E              ; 85 8E

L1C12:      lda L0AA4,Y            ; B9 A4 0A
            sta L0055              ; Current Room nr

            jsr L1C5E              ; 20 5E 1C

            jsr L111B              ; 20 1B 11

L1C1D:      rts                    ; 60

;----------------------------------------------
;
;----------------------------------------------

L1C1E:      ldx #$04               ; A2 04
            jsr L11B8              ; 20 B8 11
            tay                    ; A8
            dey                    ; 88
            sty L0066              ; 84 66
            bmi L1C34              ; 30 0B

L1C29:      ldx #$01               ; A2 01
            jsr L11B8              ; 20 B8 11
            sta L0A80,Y            ; 99 80 0A
            dey                    ; 88
            bpl L1C29              ; 10 F5

L1C34:      ldy #$00               ; A0 00
L1C36:      ldx #$06               ; A2 06
            jsr L11B8              ; 20 B8 11
            sta L0AA4,Y            ; 99 A4 0A
            iny                    ; C8
            cpy #$04               ; C0 04
            bne L1C36              ; D0 F3

            ldx #$01               ; A2 01

            jsr L11B8              ; 20 B8 11

            asl A                  ; 0A
            asl A                  ; 0A
            sec                    ; 38
            sbc #$02               ; E9 02

            sta L006A              ; 85 6A

            ldx #$01               ; A2 01
            jsr L11B8              ; 20 B8 11
            sta L006C              ; 85 6C

            ldx #$01               ; A2 01
            jsr L11B8              ; 20 B8 11

            sta L006F              ; 85 6F

            rts                    ; 60

;--------------------------------------------
;
;--------------------------------------------

L1C5E:      ldy #$00               ; A0 00
            ldx L0066              ; A6 66
            bmi L1C95              ; 30 31

            lda #$04               ; A9 04
            sta L0097              ; 85 97

L1C68:      dec L0097              ; C6 97
            bpl L1C71              ; 10 05

            lda #$07               ; A9 07
            sta L0097              ; 85 97
            iny                    ; C8

L1C71:      lda L0A80,X            ; BD 80 0A
            beq L1C78              ; F0 02

            lda #$FF               ; A9 FF
L1C78:      sta L007B              ; 85 7B
            stx L0096              ; 86 96

            ldx L0097              ; A6 97
            lda L1C96,X            ; BD 96 1C
            eor #$FF               ; 49 FF
            and L007B              ; 25 7B
            sta L007B              ; 85 7B

            lda (L0050),Y          ; B1 50
            and L1C96,X            ; 3D 96 1C
            ora L007B              ; 05 7B
            sta (L0050),Y          ; 91 50

            ldx L0096              ; A6 96
            dex                    ; CA
            bpl L1C68              ; 10 D3

L1C95:      rts                    ; 60

L1C96:      .byte $7F,$BF,$DF,$EF,$F7,$FB,$FD,$FE

;--------------------------------------------
;
;--------------------------------------------

; Fill table 0A80-0A8B with 1

L1C9E:      ldy #$0B               ; A0 0B
            lda #$01               ; A9 01
L1CA2:      sta L0A80,Y            ; 99 80 0A
            dey                    ; 88
            bpl L1CA2              ; 10 FA

            lda #$3C               ; Select level
            sta L0081              ; 85 81

L1CAC:      ldx L0081              ; A6 81
            jsr L1CC2              ; Select leveladdress

            ldx #$04               ; A2 04
            jsr L11B8              ; 20 B8 11
            sta L0066              ; 85 66

            dec L0066              ; C6 66
            jsr L1C5E              ; 20 5E 1C

            dec L0081              ; C6 81
            bne L1CAC              ; D0 EB

            rts                    ; 60

;--------------------------------------------
; Set address leveldata with X as pointer in 50/51
;--------------------------------------------

L1CC2:
	lda #$00		; Reset vars
	sta L0052
	sta L0053

	lda L1F00,X		; Set leveldata pointer
	sta L0050
	lda L1F3D,X
	sta L0051

	rts

;--------------------------------------------
; Scroll 2 lines in tile 4
;--------------------------------------------

L1CD6:      lda L0056              ; A5 56
            beq L1D07              ; F0 2D

            lda L0057              ; Frame counter
            and #$07               ; 29 07
            tax                    ; AA

            lda L0058              ; A5 58
            sta L007B              ; 85 7B

            lda L0059              ; A5 59
            sta L0090              ; 85 90
            lda L005A              ; A5 5A
            sta L0091              ; 85 91

            ldy #$00               ; A0 00
            clc                    ; 18
L1CEE:      lda L4780,X            ; BD 80 47
            sta (L0090),Y          ; 91 90
	tya
	clc
	adc #$40
	tay
;            iny                    ; C8
;            iny                    ; C8
            lda L4788,X            ; BD 88 47
            sta (L0090),Y          ; 91 90
            tya                    ; 98
;            adc #$06               ; 69 06
	adc #$c1
            tay                    ; A8
;            bne L1D03              ; D0 03
;            inc L0091              ; E6 91
;            clc                    ; 18
L1D03:      dec L007B              ; C6 7B
            bne L1CEE              ; D0 E7
L1D07:      rts                    ; 60

;--------------------------------------------
; Locate tile 4 in shadowscreen
; Address return on L0059/5A
;--------------------------------------------

L1D08:
	ldy #<L4800		; Set address shadowscr
	sty L0060
	lda #>L4800
	sta L0061

	lda #$00		; Set shadow top/bottom
	sta L0056

	ldy #0
L1D14:
	jsr L1D52		; Read tilenr
	bne L1D14		; Check if tile=4

	sty L0059		; Yes, store position

	lda L0059
	asl a
	rol L0056
	asl a
	rol L0056
	asl a
	rol L0056
	clc
	lda L0056
	adc #$80
	sta L005A

	tya
	and #31
	sta L0059

            ldx #$00               ; A2 00
            dec L0056              ; E6 56
L1D47:      inx                    ; E8
            jsr L1D52              ; 20 52 1D
            beq L1D47              ; F0 FA
            stx L0058              ; 86 58
            jmp L1D63              ; 4C 63 1D

L1D52:      iny                    ; C8
            bne L1D5E              ; D0 09
            inc L0061              ; E6 61
            inc L0056              ; C6 56
            bne L1D5E              ; D0 03
            pla                    ; 68
            pla                    ; 68
            rts                    ; 60

L1D5E:      lda (L0060),Y          ; B1 60
            cmp #$04               ; C9 04
            rts                    ; 60

;--------------------------------------------

L1D63:      lda L09FA              ; AD FA 09
            jsr L14B3              ; Calculate char data address
            ldy #$00               ; A0 00
            lda L006A              ; A5 6A
            bmi L1D76              ; 30 07
            ldx #$00               ; A2 00
            jsr L1D8E              ; 20 8E 1D
            bmi L1D7B              ; 30 05

L1D76:      ldx #$07               ; A2 07
            jsr L1D9F              ; 20 9F 1D

L1D7B:      ldy #$02               ; A0 02
            lda L006A              ; A5 6A
            bmi L1D88              ; 30 07
            ldx #$0F               ; A2 0F
            jsr L1D9F              ; 20 9F 1D
            bmi L1D8D              ; 30 05

L1D88:      ldx #$08               ; A2 08
            jsr L1D8E              ; 20 8E 1D

L1D8D:      rts                    ; 60

L1D8E:      lda (L0090),Y          ; B1 90
            sta L0097              ; 85 97
            lda #$07               ; A9 07
            sta L007A              ; 85 7A
L1D96:      jsr L1DB0              ; 20 B0 1D
            inx                    ; E8
            dec L007A              ; C6 7A
            bpl L1D96              ; 10 F8
            rts                    ; 60

L1D9F:      lda (L0090),Y          ; B1 90
            sta L0097              ; 85 97
            lda #$07               ; A9 07
            sta L007A              ; 85 7A
L1DA7:      jsr L1DB0              ; 20 B0 1D
            dex                    ; CA
            dec L007A              ; C6 7A
            bpl L1DA7              ; 10 F8
            rts                    ; 60

L1DB0:      clc                    ; 18
            lda L0097              ; A5 97
            bpl L1DB6              ; 10 01
            sec                    ; 38
L1DB6:      rol A                  ; 2A
            sta L0097              ; 85 97
            sta L4780,X            ; 9D 80 47
            rts                    ; 60

;--------------------------------------------
; Animate foot
;--------------------------------------------

L1DBD:      lda #$C8               ; A9 C8
            sta L07E1              ; 8D E1 07

            lda #$00               ; A9 00
            sta L028A              ; 8D 8A 02

            ldx #$00               ; A2 00
            jsr L1CC2              ; Select leveladdress

            jsr L1776              ; 20 76 17

            lda #$A1               ; A9 A1
            sta L0A0B              ; 8D 0B 0A

            lda #$01               ; A9 01
            sta L0A64              ; 8D 64 0A
            sta L0A65              ; 8D 65 0A
            sta L0A66              ; 8D 66 0A

            jsr L159A              ; Plot foot, Willy + pilar

            lda #>L3000            ; Select foot sprite
            sta L0093              ; hb-Spritedata address
            lda #<L3000               ; A9 00
            sta L0092              ; lb-Spritedata address

L1DEA:      lda L0A1C              ; AD 1C 0A
            ldx L0A25              ; AE 25 0A
            jsr L1476              ; Plot sprite 16x16
            dec L07E1              ; CE E1 07
            dec L07E1              ; CE E1 07

;            lda #$00
	lda L0A25
	eor #$ff
	lsr a
	ldy #7
            jsr L0B27              ; Make foot sink sound

            ldx #$0F               ; A2 0F
            jsr L1E0E              ; 20 0E 1E

            inc L0A25              ; EE 25 0A
            lda L0A25              ; AD 25 0A
            cmp #$61               ; C9 61
            bne L1DEA              ; D0 DD

            rts                    ; 60

;--------------------------------------------

L1E0E:      ldy #$00               ; A0 00
L1E10:      dey                    ; 88
            bne L1E10              ; D0 FD
            dex                    ; CA
            bne L1E10              ; D0 FA
            rts                    ; 60

L1E17:      lda L0065              ; A5 65
            beq L1E2F              ; F0 14
            tax                    ; AA
            lda L0A24              ; AD 24 0A
            clc                    ; 18
            adc L1763,X            ; 7D 63 17
            sta L0A24              ; 8D 24 0A

	pha
	lda L0065
	ldy #20
	jsr sound
	pla

            dec L0065              ; C6 65
            bne L1E2F              ; D0 05
            and #$FC               ; 29 FC
            sta L0A24              ; 8D 24 0A

L1E2F:      rts                    ; 60

            .byte $22,$23,$20,$68,$69,$74,$6D,$6F
            .byte $6E,$3D,$6F,$74,$68,$65,$72,$2B
            .byte $31,$32,$3A,$61,$72,$72,$6F,$77
            .byte $3D,$6F,$74,$68,$65,$72,$2B,$31
            .byte $35,$0D,$01,$26,$12,$20,$73,$6F
            .byte $75,$6E,$3D,$6F,$74,$68,$65,$72
            .byte $2B,$33,$39,$0D,$01,$27,$1F,$20
            .byte $63,$72,$69,$70,$3D,$6F,$74,$68
            .byte $65,$72,$2B,$32,$34,$3A,$62,$6F
            .byte $77,$3D,$6F,$74,$68,$65,$72,$2B
            .byte $32,$37,$0D,$01,$28,$18,$20,$70
            .byte $6F,$6C,$6B,$79,$3D,$26,$34,$46
            .byte $3A,$6D,$6E,$62,$61,$63,$3D,$26
            .byte $34,$45,$0D,$01,$F3,$05,$20,$0D
            .byte $01,$F4,$11,$20,$E3,$20,$43,$3D
            .byte $30,$20,$B8,$20,$32,$20,$88,$32
            .byte $0D,$01,$FE,$0D,$20,$50,$25,$3D
            .byte $26,$31,$31,$30,$30,$0D,$02,$08
            .byte $05,$20,$0D,$03,$E8,$0A,$5B,$4F
            .byte $50,$54,$20,$43,$0D,$03,$F2,$0E
            .byte $20,$4A,$4D,$50,$20,$72,$73,$74
            .byte $6B,$79,$0D,$03,$F7,$0D,$20,$4A
            .byte $4D,$50,$20,$66,$6F,$6F,$74,$0D
            .byte $03,$FC,$0E,$20,$4A,$4D,$50,$20
            .byte $64,$6F,$77,$69,$6C,$0D,$04,$01
            .byte $0E,$20,$4A,$4D,$50,$20,$6D,$69


;------------------------------------------------
; Level pointers, 61 screens
;------------------------------------------------

L1F00:
;            .byte $71,$7A,$BB,$07,$40,$8F,$C2,$1A
;            .byte $72,$A7,$E2,$2D,$83,$F3,$36,$7B
;            .byte $BB,$FD,$43,$85,$B6,$FA,$33,$70
;            .byte $A8,$DA,$00,$50,$A6,$DB,$1A,$4E
;            .byte $71,$A9,$EF,$3B,$80,$E5,$28,$69
;            .byte $99,$B9,$02,$57,$AA,$0B,$3B,$6B
;            .byte $A2,$CA,$0E,$74,$CD,$20,$5C,$B3
;            .byte $04,$36,$70,$E0,$25
	.byte <L2F71,<L1F7A,<L1FBB,<L2007,<L2040,<L208F,<L20C2,<L211A
	.byte <L2172,<L21A7,<L21E2,<L222D,<L2283,<L22F3,<L2336,<L237B
	.byte <L23BB,<L23FD,<L2443,<L2485,<L24B6,<L24FA,<L2533,<L2570
	.byte <L25A8,<L25DA,<L2600,<L2650,<L26A6,<L26DB,<L271A,<L274E
	.byte <L2771,<L27A9,<L27EF,<L283B,<L2880,<L28E5,<L2928,<L2969
	.byte <L2999,<L29B9,<L2A02,<L2A57,<L2AAA,<L2B0B,<L2B3B,<L2B6B
	.byte <L2BA2,<L2BCA,<L2C0E,<L2C74,<L2CCD,<L2D20,<L2D5C,<L2DB3
	.byte <L2E04,<L2E36,<L2E70,<L2EE0,<L2F25

L1F3D:
;            .byte $10,$00,$00,$01,$01,$01,$01,$02
;            .byte $02,$02,$02,$03,$03,$03,$04,$04
;            .byte $04,$04,$05,$05,$05,$05,$06,$06
;            .byte $06,$06,$07,$07,$07,$07,$08,$08
;            .byte $08,$08,$08,$09,$09,$09,$0A,$0A
;            .byte $0A,$0A,$0B,$0B,$0B,$0C,$0C,$0C
;            .byte $0C,$0C,$0D,$0D,$0D,$0E,$0E,$0E
;            .byte $0F,$0F,$0F,$0F,$10
	.byte >L2F71,>L1F7A,>L1FBB,>L2007,>L2040,>L208F,>L20C2,>L211A
	.byte >L2172,>L21A7,>L21E2,>L222D,>L2283,>L22F3,>L2336,>L237B
	.byte >L23BB,>L23FD,>L2443,>L2485,>L24B6,>L24FA,>L2533,>L2570
	.byte >L25A8,>L25DA,>L2600,>L2650,>L26A6,>L26DB,>L271A,>L274E
	.byte >L2771,>L27A9,>L27EF,>L283B,>L2880,>L28E5,>L2928,>L2969
	.byte >L2999,>L29B9,>L2A02,>L2A57,>L2AAA,>L2B0B,>L2B3B,>L2B6B
	.byte >L2BA2,>L2BCA,>L2C0E,>L2C74,>L2CCD,>L2D20,>L2D5C,>L2DB3
	.byte >L2E04,>L2E36,>L2E70,>L2EE0,>L2F25

;------------------------------------------------
; Level data, 61 screens
;------------------------------------------------

L1F7A:      .byte $F2,$20,$98
            .byte $A1,$C9,$EF,$60,$77,$3F,$62,$67
            .byte $FB,$6E,$AA,$81,$8C,$F8,$CB,$AE
            .byte $91,$78,$A0,$CC,$4A,$62,$C6,$82
            .byte $49,$26,$43,$66,$99,$B9,$71,$34
            .byte $BA,$4E,$96,$94,$16,$1D,$0A,$56
            .byte $1C,$79,$0A,$10,$21,$0A,$5D,$56
            .byte $56,$6C,$D8,$F4,$47,$B1,$4F,$9D
            .byte $91,$89,$35,$FD,$55,$00

L1FBB:      .byte $00,$C1
            .byte $10,$95,$DC,$04,$59,$18,$53,$96
            .byte $97,$1E,$A3,$AD,$33,$C4,$3C,$5D
            .byte $AC,$38,$DB,$DA,$8D,$5B,$F7,$19
            .byte $24,$59,$E6,$E1,$9B,$0F,$9F,$BC
            .byte $09,$35,$30,$17,$FD,$50,$32,$57
            .byte $B3,$EC,$35,$2D,$A7,$D0,$A4,$5C
            .byte $9E,$FC,$E8,$D9,$37,$25,$A0,$23
            .byte $4F,$94,$20,$40,$AC,$13,$C2,$C0
            .byte $20,$B3,$C2,$A9,$AB,$8A,$D6,$DC
            .byte $94,$01

L2007:      .byte $18,$42,$60,$F8,$7B,$2B
            .byte $CA,$2B,$C8,$CA,$89,$59,$44,$39
            .byte $7B,$FF,$8B,$CE,$D1,$03,$07,$DB
            .byte $10,$8E,$1A,$8D,$71,$38,$0D,$66
            .byte $CD,$8E,$F9,$46,$56,$88,$12,$A1
            .byte $44,$F1,$38,$5A,$21,$46,$D9,$E0
            .byte $89,$EB,$74,$61,$F2,$43,$BB,$AE
            .byte $9D,$48,$00

L2040:      .byte $18,$46,$11,$44,$DB
            .byte $89,$48,$CB,$48,$4A,$49,$5B,$84
            .byte $15,$7F,$9D,$BD,$FF,$41,$90,$B3
            .byte $26,$86,$D8,$9C,$0E,$1C,$6C,$E1
            .byte $23,$2D,$76,$A2,$A6,$4E,$B7,$EC
            .byte $F9,$06,$1C,$58,$6A,$D2,$42,$15
            .byte $96,$4A,$B4,$10,$2D,$13,$53,$96
            .byte $6C,$18,$D6,$5A,$22,$5D,$F8,$FD
            .byte $A4,$A4,$41,$ED,$AC,$D4,$45,$ED
            .byte $B2,$CC,$46,$ED,$B9,$9C,$4A,$CC
            .byte $B7,$03

L208F:      .byte $18,$C1,$40,$F4,$7B,$8B
            .byte $88,$4B,$4A,$88,$89,$2E,$04,$35
            .byte $72,$4D,$49,$F9,$7A,$12,$A9,$8B
            .byte $E6,$82,$17,$8A,$88,$0A,$50,$4D
            .byte $08,$3B,$70,$B0,$91,$6C,$38,$DF
            .byte $24,$00,$CA,$50,$19,$5D,$F8,$FD
            .byte $30,$61,$0A,$04,$12

L20C2:      .byte $18,$C5,$31
            .byte $6C,$B9,$8B,$48,$CB,$C8,$12,$CA
            .byte $5F,$0C,$8A,$6D,$6E,$2A,$91,$3C
            .byte $B1,$2B,$B9,$89,$10,$7B,$33,$17
            .byte $E9,$42,$42,$5D,$24,$07,$0E,$B6
            .byte $F0,$91,$16,$3B,$D1,$52,$A7,$5B
            .byte $F6,$7C,$03,$0E,$2C,$35,$69,$A1
            .byte $0A,$4B,$25,$5A,$88,$96,$89,$29
            .byte $4B,$36,$8C,$2E,$FC,$7E,$5A,$D2
            .byte $60,$F6,$54,$EA,$A2,$76,$59,$66
            .byte $23,$F6,$5C,$4E,$A5,$76,$96,$10
            .byte $01,$0A,$35,$B1,$04

L211A:      .byte $34,$46,$64
            .byte $6C,$6A,$17,$90,$E2,$EF,$E6,$E5
            .byte $BB,$C3,$88,$24,$A1,$42,$1E,$49
            .byte $86,$44,$A2,$31,$38,$70,$B0,$85
            .byte $8F,$B6,$F8,$C9,$66,$96,$0F,$F8
            .byte $20,$DF,$CF,$55,$FE,$AD,$77,$7D
            .byte $95,$DB,$D2,$5C,$70,$18,$79,$B9
            .byte $CC,$ED,$A7,$16,$99,$39,$93,$21
            .byte $28,$D5,$69,$61,$21,$76,$77,$06
            .byte $5D,$5C,$BB,$E6,$49,$49,$82,$D9
            .byte $55,$69,$92,$DA,$65,$42,$14,$58
            .byte $A4,$38,$10,$A0,$35

L2172:      .byte $18,$27,$4E
            .byte $2E,$79,$8F,$4A,$49,$76,$0F,$0F
            .byte $BD,$4C,$3A,$7B,$B3,$F3,$D7,$D1
            .byte $FB,$2F,$77,$25,$91,$03,$A3,$D5
            .byte $3C,$58,$C4,$FE,$61,$01,$57,$2D
            .byte $E1,$12,$AA,$15,$08,$AB,$48,$A3
            .byte $A7,$4C,$4D,$AB,$1F,$CA,$59,$80
            .byte $5D,$02

L21A7:      .byte $F2,$24,$05,$89,$E8,$76
            .byte $70,$A0,$37,$A6,$BF,$EF,$15,$02
            .byte $B9,$44,$62,$05,$D9,$C5,$F1,$A2
            .byte $73,$F4,$80,$85,$06,$7E,$CC,$2A
            .byte $CA,$C8,$C2,$F9,$FE,$80,$29,$17
            .byte $C8,$D2,$F6,$E1,$D5,$E9,$72,$F3
            .byte $E9,$C7,$BF,$3F,$A2,$53,$C7,$8F
            .byte $22,$87,$86,$AF,$06

L21E2:      .byte $40,$D2,$90
            .byte $84,$DC,$A4,$BB,$7A,$E2,$3B,$3B
            .byte $2E,$26,$8B,$C5,$24,$DE,$FF,$A2
            .byte $73,$F4,$08,$68,$10,$86,$4A,$22
            .byte $7C,$5D,$B0,$64,$8D,$81,$32,$17
            .byte $E8,$32,$44,$F5,$37,$60,$70,$80
            .byte $06,$C6,$8A,$26,$71,$3E,$D1,$7C
            .byte $F2,$B4,$CD,$75,$6D,$86,$71,$E3
            .byte $97,$C3,$CD,$2B,$53,$E7,$51,$44
            .byte $12,$30,$34,$1B,$D2,$64,$95,$4C

L222D:      .byte $40,$31,$B4,$8C,$DC,$A5,$5B,$DB
            .byte $24,$5A,$9A,$2D,$A2,$FE,$03,$39
            .byte $0B,$74,$F2,$05,$19,$BB,$85,$10
            .byte $31,$52,$E4,$C9,$B3,$17,$AD,$DA
            .byte $F8,$FB,$73,$B8,$4E,$52,$27,$05
            .byte $01,$06,$3C,$14,$78,$20,$C0,$81
            .byte $41,$56,$38,$1A,$0C,$48,$4A,$17
            .byte $76,$7B,$C6,$33,$1E,$07,$D7,$2B
            .byte $9E,$26,$D7,$4B,$9E,$17,$D7,$6B
            .byte $5E,$36,$D7,$29,$5E,$17,$D7,$49
            .byte $C1,$26,$D7,$E9,$92,$26

L2283:      .byte $98,$66
            .byte $51,$46,$F7,$36,$D5,$37,$36,$D4
            .byte $D5,$5E,$04,$E0,$F8,$A4,$B3,$24
            .byte $DD,$26,$B4,$BE,$96,$54,$12,$2F
            .byte $24,$5A,$90,$4A,$6E,$E6,$22,$1D
            .byte $9C,$0F,$64,$31,$DC,$47,$B2,$B6
            .byte $C7,$57,$E2,$A4,$D2,$63,$F1,$3E
            .byte $70,$E8,$48,$09,$53,$91,$A8,$D0
            .byte $8A,$35,$1B,$5E,$BD,$FB,$8C,$98
            .byte $AB,$E8,$52,$E5,$41,$97,$A1,$02
            .byte $C3,$CB,$86,$A9,$2C,$C3,$47,$55
            .byte $4C,$49,$CD,$F8,$4D,$34,$60,$E5
            .byte $44,$9B,$1C,$47,$C9,$12,$2B,$53
            .byte $66,$E9,$42,$8E,$47,$4A,$2A,$61
            .byte $93,$85,$1A,$6E,$4A,$00

L22F3:      .byte $C0,$70
            .byte $28,$8B,$35,$09,$AA,$6A,$80,$2B
            .byte $2B,$4E,$C7,$89,$21,$91,$11,$8B
            .byte $C6,$63,$C4,$92,$08,$B2,$8B,$07
            .byte $86,$CC,$E6,$7C,$7F,$E5,$F9,$7F
            .byte $DE,$4A,$65,$84,$E2,$74,$38,$4B
            .byte $FD,$48,$2A,$65,$05,$86,$9B,$B3
            .byte $66,$D8,$55,$BA,$CA,$45,$90,$A5
            .byte $44,$9D,$86,$3C,$D4,$39,$D3,$8F
            .byte $02

L2336:      .byte $98,$E5,$38,$2D,$79,$97,$4B
            .byte $49,$4A,$94,$95,$7A,$42,$FA,$28
            .byte $48,$CA,$69,$3F,$EE,$34,$39,$70
            .byte $60,$A9,$60,$34,$AB,$7E,$B4,$09
            .byte $7E,$E0,$82,$6D,$0E,$04,$B1,$E2
            .byte $10,$8E,$4B,$B2,$60,$59,$A2,$50
            .byte $E1,$A2,$0B,$72,$9F,$F8,$C4,$81
            .byte $89,$06,$CC,$81,$89,$86,$34,$91
            .byte $8B,$26,$B5,$C4,$40,$02

L237B:      .byte $98,$17
            .byte $78,$3D,$FB,$7D,$BC,$4B,$86,$6C
            .byte $BE,$5D,$04,$03,$E8,$2A,$B9,$FF
            .byte $35,$92,$F2,$83,$04,$47,$E7,$CC
            .byte $71,$43,$8A,$3F,$33,$4B,$F2,$53
            .byte $16,$25,$28,$72,$E4,$45,$68,$C0
            .byte $89,$40,$56,$13,$65,$AE,$DA,$AC
            .byte $FB,$08,$BA,$D0,$ED,$91,$E9,$01
            .byte $98,$69,$A9,$0D,$DA,$3E

L23BB:      .byte $F2,$BC
            .byte $28,$18,$C9,$F3,$F5,$59,$12,$F8
            .byte $E9,$D1,$2A,$B2,$91,$94,$5F,$13
            .byte $96,$BD,$E8,$1C,$3D,$82,$0C,$29
            .byte $CF,$97,$41,$CE,$E1,$55,$20,$50
            .byte $10,$60,$FA,$F0,$00,$4A,$60,$68
            .byte $92,$15,$19,$36,$52,$B4,$D8,$41
            .byte $97,$99,$4A,$CC,$B4,$E5,$91,$96
            .byte $97,$77,$54,$52,$5A,$3F,$B4,$45

L23FD:      .byte $7C,$41,$22,$4D,$D4,$56,$22,$50
            .byte $1C,$51,$90,$7F,$11,$E3,$22,$B0
            .byte $8C,$C4,$71,$60,$78,$68,$A9,$C0
            .byte $03,$81,$4B,$F3,$4B,$72,$F6,$99
            .byte $70,$D1,$64,$D4,$79,$90,$50,$D9
            .byte $0A,$B6,$9C,$3C,$EB,$A4,$87,$00
            .byte $63,$FD,$8A,$73,$04,$74,$9E,$2E
            .byte $31,$0A,$79,$EE,$91,$22,$8B,$BB
            .byte $24,$51,$14,$98,$B3,$4E

L2443:      .byte $20,$CA
            .byte $94,$15,$CD,$04,$73,$F9,$F3,$72
            .byte $B2,$2D,$42,$C0,$85,$22,$5F,$47
            .byte $6F,$8E,$84,$1C,$A8,$85,$31,$4F
            .byte $FC,$50,$0F,$04,$1A,$6A,$D0,$B8
            .byte $7D,$07,$1A,$94,$49,$93,$21,$9B
            .byte $25,$CD,$44,$99,$4A,$F7,$1C,$39
            .byte $78,$D8,$16,$8B,$1D,$7F,$94,$20
            .byte $02,$1A,$8D,$72,$A7,$7A,$D9,$00

L2485:      .byte $20,$29,$B2,$AC,$CB,$32,$32,$B2
            .byte $88,$D3,$D3,$BE,$22,$0D,$48,$99
            .byte $C4,$07,$09,$8E,$CE,$99,$03,$27
            .byte $A1,$FF,$E3,$C7,$69,$C0,$40,$00
            .byte $7E,$48,$36,$E3,$A5,$40,$2C,$E5
            .byte $20,$B2,$80,$E4,$21,$EC,$1A,$00
            .byte $30

L24B6:      .byte $58,$56,$19,$27,$B0,$A5,$0A
            .byte $F0,$A3,$24,$27,$DD,$45,$65,$E7
            .byte $9F,$06,$16,$0B,$CB,$0E,$14,$FC
            .byte $40,$9E,$7B,$F3,$D7,$5F,$F3,$38
            .byte $FD,$0A,$10,$40,$B2,$0D,$F2,$45
            .byte $9C,$20,$F2,$24,$51,$A7,$13,$19
            .byte $0E,$DF,$98,$B1,$4B,$17,$90,$3D
            .byte $A6,$D8,$20,$BB,$86,$0C,$21,$31
            .byte $65,$FD,$AA,$AF,$02

L24FA:      .byte $58,$51,$55
            .byte $15,$7D,$26,$0A,$24,$00,$C7,$C5
            .byte $7E,$44,$FD,$07,$41,$E6,$D1,$39
            .byte $73,$F2,$AB,$12,$B5,$15,$20,$A3
            .byte $08,$E8,$87,$79,$2F,$F8,$81,$2B
            .byte $C8,$62,$23,$57,$1A,$EA,$32,$B6
            .byte $60,$3A,$5D,$E8,$F4,$08,$39,$0B
            .byte $00,$D8,$F1,$0C,$2E,$0A

L2533:      .byte $A0,$6A
            .byte $9C,$37,$5C,$A5,$A5,$24,$89,$A3
            .byte $A3,$EE,$01,$38,$D0,$B4,$81,$71
            .byte $8B,$BF,$FF,$11,$90,$F2,$6B,$40
            .byte $EB,$03,$07,$BE,$CC,$47,$DA,$68
            .byte $50,$FD,$E8,$95,$ED,$A7,$94,$F5
            .byte $09,$B0,$2A,$81,$11,$12,$AA,$C4
            .byte $69,$91,$45,$D8,$B0,$1E,$D2,$D4
            .byte $90,$40,$01

L2570:      .byte $A0,$1B,$BC,$8F,$BC
            .byte $05,$05,$1E,$F8,$EE,$EF,$3C,$42
            .byte $1A,$49,$F9,$41,$82,$A3,$73,$E6
            .byte $40,$23,$4D,$80,$A8,$12,$C3,$4F
            .byte $0C,$B0,$2A,$55,$B9,$6A,$B9,$EA
            .byte $6D,$FA,$39,$3A,$A5,$DE,$3F,$39
            .byte $38,$EB,$11,$A5,$05,$A8,$94,$E9
            .byte $18,$A8,$29

L25A8:      .byte $A0,$9B,$42,$90,$DC
            .byte $04,$05,$F8,$F1,$6E,$6F,$1C,$A1
            .byte $1B,$49,$F9,$35,$61,$D9,$81,$92
            .byte $1F,$5A,$06,$F8,$F0,$7C,$00,$29
            .byte $5A,$0E,$10,$4C,$B0,$FF,$AF,$3C
            .byte $96,$88,$01,$00,$C6,$9D,$02,$77
            .byte $4A,$44,$01,$83,$06

L25DA:      .byte $60,$58,$A2
            .byte $D9,$CC,$A4,$A5,$24,$AB,$C3,$C3
            .byte $3C,$A2,$52,$13,$C6,$08,$CB,$9E
            .byte $77,$4E,$9F,$F9,$0C,$E7,$B0,$87
            .byte $BF,$90,$13,$30,$98,$1F,$AA,$7A
            .byte $A9,$43,$03

L2600:      .byte $D8,$B4,$A5,$28,$B9
            .byte $85,$4A,$49,$02,$07,$05,$3E,$84
            .byte $FF,$07,$81,$1D,$C0,$BE,$1C,$09
            .byte $39,$70,$18,$17,$67,$CD,$87,$A4
            .byte $52,$BD,$15,$6B,$BC,$58,$A9,$C5
            .byte $32,$2D,$D6,$5E,$7D,$4C,$95,$BA
            .byte $AF,$00,$05,$64,$CC,$64,$ED,$7E
            .byte $3A,$31,$BE,$D2,$54,$15,$74,$41
            .byte $E0,$13,$AD,$55,$FE,$68,$42,$39
            .byte $53,$33,$CC,$3A,$55,$49,$34,$14
            .byte $55,$71,$00

L2650:      .byte $D8,$72,$EC,$58,$F5
            .byte $DD,$4A,$F9,$13,$FB,$FA,$58,$84
            .byte $FF,$67,$05,$E3,$D9,$80,$48,$5E
            .byte $74,$CE,$1C,$98,$1F,$59,$F8,$E5
            .byte $01,$E0,$FF,$79,$0C,$76,$79,$28
            .byte $51,$AE,$CA,$B2,$35,$9B,$A1,$B2
            .byte $E6,$68,$D2,$BC,$D5,$52,$BC,$B0
            .byte $83,$2E,$2C,$7B,$AE,$0B,$06,$3A
            .byte $09,$6B,$04,$76,$06,$BA,$02,$F6
            .byte $0C,$47,$83,$A6,$0B,$17,$02,$36
            .byte $05,$9F,$04,$F6,$08,$77,$04,$76
            .byte $0E

L26A6:      .byte $60,$BB,$4A,$92,$3D,$BD,$BC
            .byte $24,$F1,$3C,$3D,$1E,$21,$A8,$88
            .byte $29,$3F,$0D,$2C,$16,$96,$1D,$28
            .byte $E0,$81,$EF,$F8,$99,$E2,$87,$19
            .byte $08,$96,$1F,$1E,$5C,$38,$30,$D5
            .byte $FE,$17,$89,$77,$2A,$16,$5F,$9F
            .byte $7E,$A8,$CE,$19,$5F,$09

L26DB:      .byte $D8,$71
            .byte $DD,$14,$B9,$B9,$4B,$49,$02,$BB
            .byte $38,$5F,$44,$FF,$87,$38,$F0,$44
            .byte $1F,$38,$13,$3D,$30,$17,$E7,$BF
            .byte $91,$8E,$24,$7F,$A2,$04,$78,$A9
            .byte $82,$2E,$DC,$7C,$86,$B3,$60,$3A
            .byte $A5,$9A,$01,$01,$89,$C5,$A0,$2A
            .byte $EE,$4F,$03,$66,$16,$6C,$03,$66
            .byte $16,$AE,$40,$6E,$06

L271A:      .byte $E0,$79,$5A
            .byte $96,$BC,$A5,$A5,$24,$25,$9C,$1C
            .byte $27,$22,$09,$48,$F9,$35,$A0,$F5
            .byte $83,$20,$91,$03,$07,$BE,$0A,$46
            .byte $AB,$EA,$47,$07,$D8,$20,$20,$91
            .byte $11,$15,$A9,$CC,$51,$B0,$92,$FD
            .byte $C8,$E3,$03,$ED,$1F,$C7,$09,$6C
            .byte $14

L274E:      .byte $E0,$07,$FA,$98,$DC,$A4,$A5
            .byte $AE,$03,$AE,$2E,$5F,$61,$20,$40
            .byte $EA,$61,$E7,$E2,$81,$86,$9F,$18
            .byte $46,$18,$27,$91,$1F,$01,$F9,$D8
            .byte $0F,$49,$0F,$00

L2771:      .byte $E0,$87,$C6,$14
            .byte $38,$A5,$A5,$24,$BD,$CF,$CF,$9E
            .byte $22,$21,$40,$EA,$A5,$0C,$44,$6C
            .byte $60,$1C,$90,$2F,$90,$7C,$BF,$47
            .byte $F9,$2B,$D6,$BF,$09,$D0,$7F,$93
            .byte $BE,$FE,$02,$59,$84,$C0,$E5,$B1
            .byte $42,$18,$C8,$95,$95,$CC,$4C,$34
            .byte $57,$57,$7F,$15

L27A9:      .byte $F2,$41,$64,$86
            .byte $DD,$DD,$C1,$5E,$52,$C2,$CE,$F6
            .byte $55,$1A,$39,$8D,$5F,$CA,$2E,$92
            .byte $03,$F3,$67,$39,$08,$10,$48,$A8
            .byte $C0,$43,$05,$15,$2E,$D8,$60,$2B
            .byte $A7,$91,$55,$FE,$92,$ED,$95,$19
            .byte $B7,$87,$2E,$33,$61,$2E,$0A,$EF
            .byte $93,$D4,$2F,$98,$C8,$08,$27,$60
            .byte $EA,$14,$19,$10,$C8,$88,$33,$D0
            .byte $2A,$00

L27EF:      .byte $10,$C6,$56,$A4,$D4,$A4
            .byte $A5,$6C,$F4,$AC,$2C,$3D,$41,$1A
            .byte $49,$F9,$3A,$FA,$94,$C8,$88,$13
            .byte $7B,$E0,$40,$2F,$FF,$40,$40,$3E
            .byte $30,$72,$B2,$9F,$73,$8D,$96,$EC
            .byte $F7,$5C,$7F,$E5,$BA,$2A,$4D,$5D
            .byte $F5,$AB,$46,$AD,$C6,$6A,$3E,$6C
            .byte $1A,$76,$71,$DA,$13,$43,$0E,$B0
            .byte $6B,$C3,$05,$B0,$45,$A3,$0C,$B0
            .byte $6B,$63,$0B,$B0,$27,$00

L283B:      .byte $10,$25
            .byte $76,$1C,$3C,$A5,$A5,$24,$81,$CD
            .byte $CD,$1E,$A1,$FE,$53,$22,$23,$4E
            .byte $EC,$81,$E9,$F3,$D1,$AF,$73,$3C
            .byte $EB,$E3,$39,$17,$81,$80,$63,$05
            .byte $1E,$21,$A8,$58,$23,$25,$6B,$2D
            .byte $7A,$21,$05,$11,$E3,$90,$51,$E2
            .byte $A8,$79,$62,$60,$01,$F6,$64,$C8
            .byte $01,$76,$6D,$78,$07,$B6,$68,$14
            .byte $01,$F6,$04

L2880:      .byte $74,$9C,$3A,$C9,$7E
            .byte $37,$35,$36,$21,$37,$32,$BC,$19
            .byte $C8,$EE,$FE,$A7,$44,$46,$9C,$D8
            .byte $F4,$DE,$7B,$06,$08,$FB,$52,$06
            .byte $22,$36,$30,$0E,$9C,$8F,$02,$1D
            .byte $E8,$8A,$AF,$55,$68,$55,$24,$48
            .byte $F5,$57,$B6,$C7,$32,$44,$1E,$BE
            .byte $C2,$4B,$B9,$F6,$AC,$45,$F1,$A2
            .byte $77,$E7,$7E,$B9,$BF,$88,$4C,$26
            .byte $59,$D8,$43,$17,$91,$C9,$7C,$C6
            .byte $D5,$01,$15,$ED,$E0,$44,$5C,$99
            .byte $05,$B6,$0D,$45,$63,$06,$46,$69
            .byte $05,$D3,$13,$6D,$A9,$94,$09,$05

L28E5:      .byte $FE,$4F,$32,$37,$5D,$9F,$06,$52
            .byte $FA,$54,$3A,$DA,$16,$91,$10,$68
            .byte $1D,$9D,$33,$AF,$91,$94,$0F,$CC
            .byte $F7,$FF,$BC,$55,$E4,$48,$01,$02
            .byte $CC,$F7,$7B,$86,$BF,$02,$04,$7A
            .byte $08,$1B,$28,$9B,$AE,$45,$4E,$5C
            .byte $B8,$71,$E7,$C9,$9B,$1F,$7B,$AC
            .byte $98,$4E,$79,$2E,$98,$59,$DB,$AD
            .byte $C3,$46,$02

L2928:      .byte $90,$E6,$5E,$9E,$DC
            .byte $A4,$A5,$24,$25,$34,$35,$CE,$21
            .byte $21,$D0,$3A,$3A,$67,$1E,$01,$29
            .byte $1F,$38,$1F,$FD,$15,$A7,$AE,$48
            .byte $F5,$8F,$5A,$C1,$0F,$5C,$20,$A8
            .byte $C8,$22,$BF,$47,$4A,$BE,$EA,$A9
            .byte $47,$8B,$9B,$92,$C7,$83,$9B,$75
            .byte $32,$CA,$3D,$B4,$A7,$4B,$9A,$29
            .byte $64,$08,$B1,$19

L2969:      .byte $90,$15,$DE,$BC
            .byte $5C,$A5,$99,$25,$79,$18,$18,$2C
            .byte $62,$FE,$E3,$40,$EB,$03,$E7,$A3
            .byte $A0,$52,$A7,$49,$BE,$C0,$0F,$91
            .byte $25,$41,$86,$22,$3D,$4E,$72,$C9
            .byte $82,$D6,$33,$D3,$8F,$92,$94,$01
            .byte $00,$DE,$78,$04

L2999:      .byte $38,$2F,$8A,$65
            .byte $B9,$33,$4A,$49,$52,$31,$88,$05
            .byte $5C,$FE,$47,$A3,$B3,$F9,$61,$E7
            .byte $E2,$81,$03,$F1,$A2,$A0,$A2,$E9
            .byte $82,$EE,$07,$00

L29B9:      .byte $74,$51,$95,$75
            .byte $34,$E5,$96,$92,$E4,$E0,$E2,$BC
            .byte $88,$70,$25,$5F,$67,$2F,$B8,$5A
            .byte $18,$78,$A0,$95,$66,$61,$96,$9F
            .byte $41,$12,$72,$42,$DA,$25,$A3,$5C
            .byte $D3,$E9,$E1,$5D,$A4,$98,$68,$32
            .byte $11,$A8,$42,$B3,$01,$57,$01,$53
            .byte $16,$EB,$39,$9D,$2E,$A2,$A4,$F7
            .byte $38,$49,$EE,$06,$52,$2B,$95,$BF
            .byte $CA,$33,$0D,$6C,$19

L2A02:      .byte $FC,$B2,$AA
            .byte $9A,$E9,$C6,$CE,$26,$D9,$CE,$CA
            .byte $72,$11,$93,$23,$21,$2F,$3B,$7F
            .byte $A0,$09,$8A,$55,$50,$0C,$02,$C4
            .byte $16,$AA,$C8,$28,$2D,$C5,$1D,$48
            .byte $D8,$B6,$EE,$3C,$64,$B4,$64,$05
            .byte $2B,$56,$69,$D2,$7C,$D0,$24,$22
            .byte $85,$A2,$C2,$F0,$75,$80,$10,$10
            .byte $21,$C4,$48,$88,$93,$20,$BA,$14
            .byte $B7,$4D,$A6,$C7,$09,$17,$20,$D0
            .byte $7A,$27,$FA,$91,$31,$2F,$50,$D0
            .byte $1D,$00

L2A57:      .byte $B8,$68,$CA,$35,$B9,$D2
            .byte $D1,$4A,$C6,$D3,$50,$5F,$0C,$3B
            .byte $7B,$C1,$83,$11,$58,$C4,$9F,$AB
            .byte $24,$DE,$FF,$70,$14,$95,$1C,$E0
            .byte $8F,$C4,$2B,$B1,$DA,$8C,$96,$70
            .byte $A8,$9A,$A2,$88,$32,$46,$55,$8C
            .byte $D5,$27,$5B,$BA,$DB,$67,$69,$CE
            .byte $A2,$C8,$69,$70,$A9,$F8,$F2,$65
            .byte $74,$81,$CA,$13,$32,$6B,$F8,$6A
            .byte $85,$97,$B0,$4B,$C7,$39,$E0,$F1
            .byte $E2,$95,$70,$2B,$5B

L2AAA:      .byte $50,$B7,$D5
            .byte $B6,$DD,$A4,$91,$25,$31,$91,$28
            .byte $2D,$C2,$84,$4D,$02,$93,$FB,$9F
            .byte $21,$0C,$E4,$58,$AC,$3A,$50,$10
            .byte $51,$05,$26,$28,$CA,$37,$69,$55
            .byte $E6,$09,$D3,$D7,$AA,$BE,$9A,$08
            .byte $A1,$5C,$02,$E5,$72,$28,$07,$BB
            .byte $9A,$B4,$EB,$76,$E9,$D3,$89,$17
            .byte $3F,$61,$62,$45,$0B,$16,$20,$C2
            .byte $73,$CF,$28,$70,$CB,$84,$2A,$B0
            .byte $A8,$64,$75,$78,$A8,$38,$73,$FC
            .byte $E8,$02,$8D,$70,$CB,$82,$8E,$70
            .byte $AB,$42,$8C,$70,$6B,$00

L2B0B:      .byte $B8,$69
            .byte $DB,$3D,$BB,$51,$C0,$49,$02,$C3
            .byte $C0,$58,$44,$A1,$82,$94,$73,$3B
            .byte $3D,$4B,$52,$3E,$40,$C7,$13,$B1
            .byte $64,$3C,$5D,$82,$71,$01,$C4,$A0
            .byte $89,$74,$C1,$C7,$23,$8D,$36,$30
            .byte $68,$2E,$84,$C9,$48,$4D

L2B3B:      .byte $B8,$EB
            .byte $BB,$6E,$FD,$BE,$24,$5C,$04,$3B
            .byte $27,$3D,$C2,$FE,$07,$5D,$3B,$D0
            .byte $61,$55,$20,$3D,$DF,$AF,$B1,$4B
            .byte $29,$96,$4F,$27,$89,$38,$DF,$80
            .byte $D7,$E8,$C2,$F6,$47,$59,$E7,$F6
            .byte $D1,$74,$DE,$F8,$51,$00

L2B6B:      .byte $B8,$1B
            .byte $FA,$5E,$76,$8F,$4C,$CE,$07,$63
            .byte $FF,$79,$44,$FF,$37,$00,$E2,$F9
            .byte $C0,$E4,$29,$8A,$F3,$FD,$0A,$D2
            .byte $8B,$04,$15,$76,$34,$20,$B8,$10
            .byte $9A,$34,$B4,$8A,$2C,$79,$D3,$56
            .byte $8C,$2E,$58,$7C,$A4,$89,$25,$1E
            .byte $29,$D7,$E2,$36,$05

L2BA2:      .byte $B8,$1F,$87
            .byte $81,$9B,$FD,$C2,$FF,$7C,$7F,$7D
            .byte $DE,$CB,$FC,$07,$09,$40,$FC,$E0
            .byte $50,$30,$1D,$E1,$7C,$17,$68,$28
            .byte $5A,$A0,$48,$96,$05,$59,$8A,$54
            .byte $89,$2E,$34,$7A,$00

L2BCA:      .byte $78,$98,$FC
            .byte $31,$B5,$9F,$1E,$9F,$E0,$1D,$1D
            .byte $4E,$44,$65,$E7,$F4,$4B,$9C,$E0
            .byte $03,$87,$39,$CE,$84,$B3,$7D,$7E
            .byte $F9,$37,$71,$D4,$AB,$FE,$BB,$48
            .byte $3E,$37,$C0,$86,$14,$2D,$C6,$BD
            .byte $47,$29,$5C,$7C,$D0,$50,$B2,$10
            .byte $86,$2E,$70,$7C,$4C,$06,$E3,$01
            .byte $A5,$85,$E5,$1D,$C9,$CA,$E4,$45
            .byte $0D

L2C0E:      .byte $F6,$8F,$73,$30,$E5,$CA,$AD
            .byte $8D,$CD,$80,$F5,$35,$8F,$88,$FF
            .byte $9A,$B0,$E4,$21,$A2,$35,$88,$07
            .byte $0E,$AC,$C1,$32,$0C,$33,$E7,$D0
            .byte $FC,$77,$FF,$9C,$AD,$CA,$76,$7B
            .byte $AE,$C8,$07,$57,$98,$AC,$02,$4A
            .byte $C2,$B0,$03,$0F,$E7,$97,$80,$64
            .byte $8B,$67,$B9,$2A,$55,$B0,$21,$AB
            .byte $BE,$4C,$79,$37,$EE,$C5,$08,$59
            .byte $F6,$6C,$38,$73,$7B,$BA,$8A,$94
            .byte $10,$89,$13,$47,$4F,$3F,$51,$2B
            .byte $83,$83,$54,$95,$91,$83,$52,$53
            .byte $A2,$83,$D6,$C2,$F2,$8E,$04

L2C74:      .byte $78
            .byte $5A,$F2,$59,$5E,$AA,$AB,$A8,$A9
            .byte $2A,$2B,$BD,$82,$FD,$47,$5D,$64
            .byte $2E,$67,$62,$1F,$80,$90,$8C,$03
            .byte $07,$FE,$60,$CC,$F7,$FF,$3C,$1E
            .byte $78,$C5,$60,$A0,$88,$E8,$A0,$4C
            .byte $72,$97,$AF,$21,$C5,$8A,$0A,$4B
            .byte $22,$97,$2C,$CB,$0D,$3B,$36,$3A
            .byte $5D,$40,$F3,$5C,$E5,$4B,$2C,$52
            .byte $35,$AB,$32,$4A,$4D,$EB,$07,$5A
            .byte $1D,$DB,$BE,$52,$55,$29,$36,$4A
            .byte $15,$4A,$2A,$5A,$F5,$2A,$2F,$0A

L2CCD:      .byte $30,$AF,$C5,$B5,$3C,$A5,$A5,$F0
            .byte $AD,$71,$71,$1E,$C1,$FF,$4B,$04
            .byte $95,$28,$F2,$78,$AC,$80,$88,$0F
            .byte $0C,$94,$41,$DE,$7C,$F4,$DB,$1C
            .byte $9D,$C5,$76,$9B,$C2,$46,$2B,$D6
            .byte $59,$B1,$DA,$46,$72,$A8,$E0,$AB
            .byte $06,$59,$D8,$F7,$85,$7F,$02,$48
            .byte $3D,$5A,$E7,$E9,$7E,$CF,$C2,$7E
            .byte $CA,$7B,$52,$9A,$66,$76,$5A,$FA
            .byte $A0,$76,$55,$26,$67,$F6,$5C,$8E
            .byte $66,$76,$09

L2D20:      .byte $B0,$6C,$F5,$3D,$FC
            .byte $A4,$A5,$F0,$33,$71,$71,$66,$62
            .byte $FE,$C3,$51,$54,$72,$E0,$20,$2B
            .byte $FF,$40,$3E,$C8,$B0,$42,$94,$C0
            .byte $83,$88,$40,$06,$F2,$60,$C0,$02
            .byte $0A,$7C,$CA,$F9,$D5,$5F,$9D,$D4
            .byte $0E,$0F,$99,$A4,$72,$4F,$95,$F0
            .byte $64,$0F,$95,$64,$05,$1F,$09

L2D5C:      .byte $FC
            .byte $75,$6F,$9E,$ED,$86,$8A,$2C,$D9
            .byte $8E,$84,$68,$11,$04,$F2,$FD,$8F
            .byte $3A,$97,$AF,$A3,$F7,$3F,$43,$18
            .byte $C8,$91,$90,$03,$F2,$D5,$1A,$7B
            .byte $28,$41,$3F,$89,$FA,$8A,$5D,$8E
            .byte $11,$2D,$98,$6E,$E8,$8F,$A4,$62
            .byte $7F,$27,$D5,$D7,$E5,$AB,$BE,$98
            .byte $08,$BF,$54,$F2,$65,$F2,$2F,$A5
            .byte $4B,$50,$88,$72,$79,$A2,$89,$25
            .byte $1E,$55,$20,$A7,$36,$59,$50,$C7
            .byte $36,$59,$88,$C2,$26,$05

L2DB3:      .byte $78,$3B
            .byte $DA,$3D,$83,$21,$C0,$C1,$C0,$C0
            .byte $C2,$5C,$84,$17,$8B,$49,$BC,$FF
            .byte $19,$C2,$40,$8E,$84,$1C,$22,$FF
            .byte $C8,$31,$AB,$FB,$F1,$97,$38,$69
            .byte $97,$EC,$E5,$4B,$0B,$F2,$77,$79
            .byte $89,$92,$AE,$5C,$A7,$75,$57,$D1
            .byte $53,$B6,$EA,$61,$37,$6C,$5C,$28
            .byte $88,$30,$41,$87,$08,$2E,$92,$2E
            .byte $B4,$7F,$EC,$B9,$62,$E6,$A8,$6F
            .byte $A6,$1F,$69,$FB,$05,$1F,$0D

L2E04:      .byte $B0
            .byte $9F,$C7,$91,$54,$A5,$21,$24,$03
            .byte $C0,$C1,$3E,$21,$FF,$83,$51,$60
            .byte $46,$0E,$0C,$FB,$F0,$68,$30,$F2
            .byte $24,$F8,$00,$48,$CB,$17,$40,$38
            .byte $31,$C0,$18,$7C,$9C,$88,$02,$0A
            .byte $95,$E4,$72,$0F,$ED,$1A,$81,$FA
            .byte $0B

L2E36:      .byte $F3,$FF,$C7,$79,$9E,$B8,$0D
            .byte $14,$08,$04,$18,$10,$E0,$22,$F2
            .byte $3F,$47,$E9,$D3,$02,$11,$8B,$C8
            .byte $81,$7A,$89,$A9,$C9,$2F,$28,$C9
            .byte $BA,$00,$F2,$03,$B0,$CF,$92,$C9
            .byte $82,$4B,$EA,$EB,$E4,$C7,$1E,$57
            .byte $60,$97,$B9,$72,$68,$4F,$D9,$F1
            .byte $FC,$D4,$00

L2E70:      .byte $70,$5D,$D7,$15,$B1
            .byte $A5,$A5,$24,$89,$08,$75,$10,$3A
            .byte $89,$CD,$11,$58,$44,$6E,$76,$8F
            .byte $03,$4C,$D2,$81,$03,$1F,$F8,$29
            .byte $B7,$CC,$4D,$A5,$4E,$2A,$B5,$51
            .byte $99,$91,$CA,$F4,$5C,$AA,$E5,$52
            .byte $35,$95,$C9,$A9,$6C,$5F,$A8,$14
            .byte $A5,$CE,$1C,$25,$CB,$9A,$24,$73
            .byte $A6,$6A,$DD,$16,$2F,$AB,$DD,$5A
            .byte $9F,$B1,$B5,$F1,$A6,$AC,$B5,$67
            .byte $4A,$6D,$BA,$A1,$6A,$D0,$0E,$47
            .byte $C9,$1A,$1D,$46,$CC,$D8,$70,$E2
            .byte $C6,$87,$2E,$52,$D4,$78,$EC,$A3
            .byte $06,$3A,$1E,$90,$06,$3A,$1E,$E2
            .byte $45,$36,$0E

L2EE0:      .byte $70,$3D,$EB,$AD,$2D
            .byte $17,$27,$16,$8A,$C6,$C7,$1E,$22
            .byte $8B,$C5,$24,$DE,$FF,$70,$14,$95
            .byte $1C,$38,$F0,$6B,$3E,$10,$95,$EF
            .byte $F8,$59,$3D,$BF,$E6,$C5,$9C,$04
            .byte $06,$8F,$5F,$89,$55,$B2,$66,$9B
            .byte $3E,$D8,$AF,$49,$64,$39,$62,$C3
            .byte $85,$C5,$1E,$27,$AE,$CD,$43,$32
            .byte $2D,$EA,$97,$2B,$49,$80,$58,$01

L2F25:      .byte $F4,$FD,$3C,$0F,$72,$5B,$9C,$5B
            .byte $C0,$9B,$9D,$F9,$8A,$C9,$91,$90
            .byte $17,$9D,$CB,$07,$E6,$67,$14,$7E
            .byte $BC,$7E,$1E,$4C,$9B,$29,$3E,$61
            .byte $C6,$D4,$A8,$1D,$16,$82,$1F,$1C
            .byte $40,$2F,$19,$6A,$8F,$C2,$02,$8D
            .byte $21,$CB,$0C,$35,$7A,$AC,$48,$17
            .byte $D5,$DC,$7E,$42,$11,$01,$22,$ED
            .byte $D9,$64,$76,$65,$A7,$F4,$03,$6D
            .byte $AE,$24,$32,$05

L2F71:      .byte $86,$FC,$79,$3F
            .byte $84,$FC,$00,$00,$06,$3D,$00,$A0
            .byte $02,$00,$00,$81,$42,$24,$18,$3C
            .byte $18,$3C,$42,$F0,$F9,$BF,$1F,$0C
            .byte $10,$28,$44,$AB,$AB,$AD,$AA,$AA
            .byte $AE,$55,$55,$6E,$74,$38,$1E,$6E
            .byte $74,$38,$1E,$03,$02,$3C,$08,$70
            .byte $20,$C0,$80,$40,$A0,$5C,$22,$21
            .byte $22,$14,$08,$AA,$55,$AA,$FF,$FF
            .byte $55,$AA,$55,$70,$07,$77,$77,$00
            .byte $3B,$0B,$7D,$14,$2A,$14,$09,$6A
            .byte $9C,$08,$08,$C0,$00,$30,$00,$0C
            .byte $00,$03,$00,$7B,$00,$ED,$00,$AA
            .byte $55,$AA,$55,$BB,$77,$EE,$DD,$BB
            .byte $77,$EE,$DD,$FF,$FF,$AA,$55,$AA
            .byte $55,$AA,$55,$0E,$C6,$95,$B0,$36
            .byte $86,$C0,$ED,$03,$03,$0C,$0C,$30
            .byte $30,$C0,$C0,$6F,$00,$B7,$00,$AA
            .byte $55,$AA,$55

; Sprites upside down

L3000:
            .byte $3C,$76,$43,$8D,$80,$02,$80,$01
            .byte $84,$09,$88,$35,$48,$42,$20,$80
            .byte $20,$80,$10,$80,$10,$80,$10,$80
            .byte $10,$80,$10,$80,$10,$80,$10,$80

            .byte $37,$EC,$77,$EE,$00,$00,$6D,$76
            .byte $EE,$F7,$ED,$77,$DF,$FB,$D5,$5B
            .byte $DB,$BB,$D5,$5B,$EF,$F7,$EF,$F7
            .byte $6F,$F6,$00,$00,$77,$EE,$37,$EC

            .byte $06,$60,$02,$40,$02,$40,$0F,$F0
            .byte $0F,$F0,$0F,$F0,$14,$98,$37,$6C
            .byte $3F,$FC,$1F,$F8,$07,$80,$01,$E0
            .byte $01,$40,$01,$E0,$03,$C0,$03,$00

            .byte $02,$60,$06,$40,$02,$40,$0F,$F0
            .byte $0F,$F0,$0F,$F0,$14,$98,$37,$6C
            .byte $3F,$FC,$1F,$F8,$07,$80,$01,$E0
            .byte $01,$40,$01,$E0,$03,$C0,$03,$00

            .byte $06,$60,$02,$40,$02,$40,$0F,$F0
            .byte $0F,$F0,$0F,$F0,$14,$92,$37,$66
            .byte $3F,$FE,$1F,$FC,$07,$80,$01,$E0
            .byte $01,$40,$01,$E0,$03,$C0,$03,$00

            .byte $06,$60,$02,$40,$02,$40,$0F,$F0
            .byte $0F,$F0,$0F,$F0,$14,$90,$37,$60
            .byte $3F,$FE,$1F,$FF,$07,$80,$01,$E0
            .byte $01,$40,$01,$E0,$03,$C0,$03,$00

            .byte $0F,$FB,$0F,$FB,$1F,$DB,$1F,$DF
            .byte $3F,$EE,$3F,$F0,$3F,$F8,$00,$08
            .byte $00,$2F,$00,$CF,$03,$0F,$0C,$0F
            .byte $30,$0F,$00,$0F,$00,$3F,$00,$0F

            .byte $0F,$FB,$0F,$FB,$1F,$DB,$1F,$DF
            .byte $3F,$EE,$3F,$F0,$3F,$F8,$00,$08
            .byte $3F,$EF,$00,$0F,$00,$0F,$00,$0F
            .byte $00,$0F,$00,$0F,$00,$3F,$00,$0F

            .byte $0F,$FB,$0F,$FB,$1F,$DB,$1F,$DF
            .byte $3F,$EE,$3F,$F0,$3F,$F8,$00,$08
            .byte $0F,$CF,$47,$CF,$7F,$8F,$7F,$8F
            .byte $01,$8F,$01,$8F,$01,$BF,$03,$AF

            .byte $0F,$FB,$0F,$FB,$1F,$DB,$1F,$DF
            .byte $3F,$EE,$3F,$F0,$3F,$F8,$00,$08
            .byte $0F,$CF,$07,$CF,$0E,$8F,$5D,$8F
            .byte $3B,$2F,$1E,$2F,$04,$3F,$00,$0F

            .byte $00,$00,$00,$00,$2A,$AA,$00,$00
            .byte $2A,$AA,$00,$00,$7F,$FF,$7F,$FF
            .byte $7F,$FF,$7C,$3F,$7C,$3F,$7F,$FF
            .byte $7F,$FF,$2A,$AA,$00,$00,$00,$00

            .byte $04,$00,$05,$00,$09,$40,$32,$50
            .byte $3C,$94,$7F,$25,$7F,$C9,$FF,$F2
            .byte $FE,$7C,$BC,$3F,$2E,$7F,$4B,$FE
            .byte $12,$FE,$04,$BC,$01,$2C,$00,$48

            .byte $00,$00,$10,$08,$17,$E8,$08,$10
            .byte $17,$E8,$0F,$F0,$17,$E8,$0F,$F0
            .byte $16,$68,$0E,$70,$16,$68,$0F,$F0
            .byte $17,$E8,$0F,$F0,$07,$E0,$00,$00

            .byte $00,$20,$00,$A0,$02,$90,$0A,$4C
            .byte $29,$3C,$A4,$FE,$93,$FE,$4F,$FF
            .byte $3E,$7F,$FC,$3D,$FE,$74,$7F,$D2
            .byte $7F,$48,$3D,$20,$34,$80,$12,$00

            .byte $02,$8A,$01,$54,$20,$D8,$20,$DB
            .byte $24,$F9,$2E,$AB,$3B,$AE,$22,$DA
            .byte $71,$74,$A8,$D8,$A9,$54,$22,$02
            .byte $02,$02,$00,$00,$00,$00,$00,$00

            .byte $00,$00,$04,$00,$06,$8A,$09,$54
            .byte $08,$D9,$18,$D9,$1E,$F9,$E7,$AB
            .byte $23,$DE,$52,$FA,$51,$54,$00,$D8
            .byte $01,$74,$02,$02,$02,$02,$00,$00

            .byte $00,$40,$00,$80,$01,$00,$02,$8A
            .byte $05,$54,$6C,$D8,$96,$F8,$2B,$FB
            .byte $4B,$A9,$11,$8B,$03,$FE,$02,$DA
            .byte $01,$54,$00,$F8,$03,$8E,$02,$02

            .byte $00,$00,$04,$00,$06,$8A,$09,$54
            .byte $08,$D9,$18,$D9,$1E,$F9,$E7,$AB
            .byte $23,$DE,$52,$FA,$51,$54,$00,$D8
            .byte $01,$74,$02,$02,$02,$02,$00,$00

            .byte $20,$04,$3B,$DC,$A0,$04,$FD,$B7
            .byte $A2,$44,$3B,$7C,$20,$04,$1D,$B0
            .byte $00,$00,$3B,$7C,$A0,$04,$BD,$B5
            .byte $E0,$07,$BB,$7D,$AE,$EC,$20,$04

            .byte $20,$04,$B7,$EC,$BD,$B5,$E0,$07
            .byte $BB,$7D,$A2,$44,$3D,$B4,$00,$00
            .byte $1B,$78,$20,$04,$3D,$B4,$A0,$04
            .byte $FB,$7F,$A0,$04,$37,$B4,$20,$04

            .byte $20,$04,$3D,$ED,$A0,$05,$FB,$77
            .byte $A2,$45,$3D,$BD,$20,$04,$1B,$70
            .byte $00,$00,$3D,$BC,$20,$04,$3B,$75
            .byte $E0,$07,$3D,$BD,$3A,$DC,$20,$04

            .byte $20,$04,$2E,$EC,$3B,$75,$E0,$07
            .byte $3D,$BD,$24,$84,$3B,$74,$00,$00
            .byte $1D,$B8,$20,$04,$3B,$75,$A0,$05
            .byte $FD,$BF,$A0,$05,$3B,$75,$20,$04

            .byte $0F,$F0,$17,$E8,$18,$18,$33,$CC
            .byte $24,$24,$69,$92,$6A,$4A,$AA,$2B
            .byte $A9,$2B,$64,$4A,$53,$92,$28,$24
            .byte $37,$CC,$18,$18,$17,$E8,$0F,$F0

            .byte $07,$80,$08,$78,$13,$F4,$24,$0C
            .byte $49,$E4,$D2,$12,$D4,$CA,$D5,$2A
            .byte $D4,$2B,$52,$4B,$49,$93,$64,$26
            .byte $73,$CC,$38,$18,$07,$F0,$00,$E0

            .byte $01,$80,$07,$E0,$18,$18,$73,$E6
            .byte $E4,$13,$C9,$C9,$D2,$25,$D4,$95
            .byte $D4,$55,$D2,$55,$C9,$95,$A4,$25
            .byte $73,$CE,$18,$18,$07,$E0,$01,$80

            .byte $01,$E0,$1F,$F0,$28,$18,$33,$CC
            .byte $24,$26,$49,$93,$52,$4B,$54,$2B
            .byte $D4,$AB,$D3,$2A,$C8,$4A,$67,$92
            .byte $30,$26,$1F,$DC,$08,$E0,$07,$00

            .byte $80,$40,$80,$40,$C0,$C0,$C0,$C0
            .byte $61,$80,$61,$80,$73,$80,$3F,$00
            .byte $33,$00,$12,$00,$1E,$00,$0C,$00
            .byte $0C,$00,$0C,$00,$0C,$00,$0C,$00

            .byte $80,$04,$80,$04,$C0,$0C,$40,$08
            .byte $60,$18,$60,$18,$30,$30,$3B,$70
            .byte $1C,$E0,$0C,$C0,$07,$80,$07,$80
            .byte $04,$80,$0C,$C0,$08,$40,$08,$40

            .byte $20,$01,$20,$01,$30,$03,$10,$02
            .byte $18,$06,$18,$06,$0C,$0C,$0E,$DC
            .byte $07,$38,$03,$30,$01,$E0,$01,$E0
            .byte $01,$20,$03,$30,$02,$10,$02,$10

            .byte $02,$01,$02,$01,$03,$03,$03,$03
            .byte $01,$86,$01,$86,$01,$CE,$00,$FC
            .byte $00,$CC,$00,$48,$00,$78,$00,$30
            .byte $00,$30,$00,$30,$00,$30,$00,$30

            .byte $00,$00,$00,$00,$00,$00,$7F,$FE
            .byte $40,$02,$FF,$FF,$DE,$7B,$C0,$03
            .byte $C0,$03,$DE,$7B,$FF,$FF,$40,$02
            .byte $7F,$FE,$00,$00,$00,$00,$00,$00

            .byte $00,$50,$00,$B8,$01,$6C,$02,$E6
            .byte $05,$C3,$0B,$8E,$16,$1D,$2C,$3A
            .byte $5C,$34,$B8,$68,$71,$D0,$C3,$A0
            .byte $67,$40,$36,$80,$1D,$00,$0A,$00

            .byte $07,$E0,$1F,$F8,$14,$28,$16,$68
            .byte $16,$68,$16,$68,$16,$68,$14,$28
            .byte $14,$28,$16,$68,$16,$68,$16,$68
            .byte $16,$68,$14,$28,$1F,$F8,$07,$E0

            .byte $0A,$00,$1D,$00,$36,$80,$67,$40
            .byte $C3,$A0,$71,$D0,$B8,$68,$5C,$34
            .byte $2C,$3A,$16,$1D,$0B,$8E,$05,$C3
            .byte $02,$E6,$01,$6C,$00,$B8,$00,$50

            .byte $E7,$00,$A5,$00,$C3,$00,$A5,$00
            .byte $E7,$00,$81,$00,$42,$00,$42,$00
            .byte $24,$00,$24,$00,$18,$00,$7E,$00
            .byte $81,$00,$FF,$00,$99,$00,$7E,$00

            .byte $E0,$70,$A0,$50,$C0,$30,$A0,$50
            .byte $E0,$70,$80,$10,$40,$20,$20,$40
            .byte $10,$80,$09,$00,$1F,$80,$30,$C0
            .byte $39,$C0,$26,$40,$1F,$80,$00,$00

            .byte $E0,$07,$A0,$05,$C0,$03,$A0,$05
            .byte $E0,$07,$80,$01,$60,$06,$1B,$D8
            .byte $06,$60,$0E,$70,$0E,$70,$09,$90
            .byte $07,$E0,$00,$00,$00,$00,$00,$00

            .byte $0E,$07,$0A,$05,$0C,$03,$0A,$05
            .byte $0E,$07,$08,$01,$04,$02,$02,$04
            .byte $01,$08,$00,$90,$01,$F8,$03,$0C
            .byte $03,$9C,$02,$64,$01,$F8,$00,$00

            .byte $FF,$C0,$5D,$80,$5D,$80,$FF,$C0
            .byte $00,$00,$5F,$80,$5F,$80,$5F,$80
            .byte $2F,$00,$2F,$00,$52,$80,$8C,$40
            .byte $12,$20,$21,$10,$12,$00,$0C,$00

            .byte $3F,$F0,$1B,$B0,$1B,$B0,$3F,$F0
            .byte $00,$00,$17,$E0,$17,$E0,$17,$E0
            .byte $0B,$C0,$0B,$C0,$14,$A0,$23,$10
            .byte $4C,$C8,$83,$04,$00,$02,$00,$01

            .byte $0F,$FC,$0B,$74,$0B,$74,$0F,$FC
            .byte $00,$00,$05,$F8,$05,$F8,$05,$F8
            .byte $02,$F0,$02,$F0,$05,$28,$08,$C4
            .byte $11,$E2,$20,$01,$40,$00,$80,$00

            .byte $03,$FF,$02,$ED,$02,$ED,$03,$FF
            .byte $00,$00,$01,$7E,$01,$7E,$01,$7E
            .byte $00,$BC,$00,$BC,$01,$4A,$02,$31
            .byte $04,$CC,$08,$30,$00,$00,$00,$00

            .byte $38,$00,$6C,$00,$7F,$00,$FC,$C0
            .byte $FF,$30,$FC,$CC,$7C,$32,$7C,$0C
            .byte $7C,$00,$7C,$20,$6C,$40,$44,$40
            .byte $6D,$80,$7E,$00,$7C,$00,$38,$00

            .byte $0E,$00,$1B,$00,$1F,$00,$7F,$00
            .byte $9F,$80,$DF,$80,$DF,$40,$7F,$40
            .byte $1F,$A0,$1F,$A0,$1B,$50,$11,$50
            .byte $1B,$20,$1F,$08,$1F,$94,$0E,$60

            .byte $03,$80,$06,$C0,$07,$C0,$07,$C0
            .byte $07,$C0,$77,$C0,$8F,$C0,$EF,$C0
            .byte $EF,$C0,$7F,$C2,$0E,$C4,$04,$44
            .byte $06,$D8,$07,$E0,$07,$C0,$03,$80

            .byte $00,$E0,$01,$B0,$01,$F0,$07,$F0
            .byte $09,$F8,$0D,$F8,$0D,$F4,$07,$F4
            .byte $01,$FA,$01,$FA,$01,$B5,$01,$15
            .byte $01,$B2,$01,$F0,$01,$F0,$00,$E0

            .byte $0C,$00,$3F,$00,$5D,$80,$5A,$80
            .byte $BD,$40,$BA,$C0,$BD,$C0,$BF,$C0
            .byte $9F,$C0,$5F,$80,$5F,$80,$4F,$80
            .byte $2F,$00,$2F,$00,$16,$00,$0C,$00

            .byte $0E,$00,$3F,$80,$5D,$40,$5A,$C0
            .byte $BD,$60,$BE,$A0,$BF,$F0,$5F,$F0
            .byte $47,$F0,$33,$F8,$0C,$78,$03,$98
            .byte $00,$70,$00,$00,$00,$00,$00,$00

            .byte $03,$C0,$1F,$F8,$23,$54,$5F,$AE
            .byte $5F,$56,$BF,$EF,$9F,$FF,$5F,$FE
            .byte $4F,$FE,$21,$FC,$1C,$78,$03,$C0
            .byte $00,$00,$00,$00,$00,$00,$00,$00

            .byte $00,$70,$01,$8C,$02,$7E,$02,$EA
            .byte $05,$D7,$05,$EB,$0B,$D7,$0B,$FE
            .byte $0B,$FE,$13,$FC,$17,$F0,$17,$C0
            .byte $0E,$00,$00,$00,$00,$00,$00,$00

            .byte $03,$80,$03,$00,$03,$00,$03,$00
            .byte $07,$C0,$18,$30,$23,$88,$44,$44
            .byte $88,$22,$90,$13,$90,$13,$88,$22
            .byte $44,$44,$23,$88,$18,$30,$07,$C0

            .byte $04,$40,$08,$E0,$1D,$D0,$0F,$80
            .byte $07,$C0,$18,$30,$20,$08,$43,$84
            .byte $84,$42,$88,$23,$88,$23,$84,$42
            .byte $43,$84,$20,$08,$18,$30,$07,$C0

            .byte $00,$00,$00,$00,$07,$C0,$1F,$F0
            .byte $3F,$F8,$7F,$FC,$7F,$F0,$FF,$80
            .byte $F0,$00,$FF,$FE,$FF,$FE,$7E,$7C
            .byte $7C,$3C,$3E,$78,$1F,$F0,$07,$C0

            .byte $00,$00,$00,$00,$07,$C0,$1F,$F0
            .byte $3F,$E0,$7F,$80,$7E,$00,$F8,$00
            .byte $F0,$00,$FC,$00,$FF,$80,$7C,$FC
            .byte $78,$7C,$3C,$F8,$1F,$F0,$07,$C0

            .byte $08,$C0,$14,$80,$0F,$E0,$05,$40
            .byte $02,$80,$07,$C0,$46,$C4,$3D,$78
            .byte $0A,$A0,$25,$48,$10,$10,$0B,$A0
            .byte $08,$20,$0A,$A0,$08,$20,$07,$C0

            .byte $06,$20,$02,$50,$0F,$E0,$05,$40
            .byte $0A,$A0,$17,$D0,$16,$D0,$1D,$70
            .byte $0A,$A0,$05,$40,$00,$00,$39,$38
            .byte $08,$20,$0A,$A0,$08,$20,$07,$C0

            .byte $00,$00,$0D,$B0,$12,$48,$23,$C4
            .byte $CD,$B3,$CD,$B3,$CD,$B3,$CD,$B3
            .byte $01,$80,$45,$91,$89,$A2,$01,$80
            .byte $45,$91,$01,$80,$00,$00,$01,$80

            .byte $00,$00,$0D,$B0,$12,$48,$23,$C4
            .byte $CD,$B3,$CD,$B3,$CD,$B3,$CD,$B3
            .byte $01,$80,$89,$A2,$45,$91,$01,$80
            .byte $89,$A2,$01,$80,$00,$00,$01,$80

            .byte $0E,$00,$3F,$80,$7F,$C0,$FF,$E0
            .byte $FF,$F0,$DF,$FC,$D3,$FF,$CF,$FF
            .byte $9F,$FF,$5F,$FF,$D1,$FF,$C0,$7F
            .byte $80,$3F,$00,$1F,$00,$0F,$00,$03

            .byte $00,$E0,$03,$F8,$07,$FC,$8F,$FE
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $DB,$FF,$D7,$FF,$4F,$1F,$9C,$07
            .byte $D8,$03,$50,$01,$00,$00,$00,$00

            .byte $00,$0E,$00,$1F,$70,$3F,$F8,$3F
            .byte $FC,$7F,$FF,$FF,$DF,$FF,$DF,$FF
            .byte $8F,$FF,$57,$FF,$DB,$F1,$DF,$E0
            .byte $8F,$C0,$07,$C0,$03,$80,$00,$00

            .byte $00,$00,$38,$00,$7C,$00,$FE,$00
            .byte $FF,$01,$DF,$C7,$DF,$FF,$C7,$FF
            .byte $9B,$FF,$5F,$FF,$DF,$FF,$C7,$FF
            .byte $83,$FF,$01,$FF,$00,$FE,$00,$38

            .byte $02,$C0,$0B,$F0,$16,$78,$38,$1C
            .byte $53,$CA,$6E,$76,$55,$EF,$BF,$F7
            .byte $71,$8F,$A4,$27,$65,$A6,$53,$CE
            .byte $3E,$FC,$1D,$F8,$0E,$F0,$03,$C0

            .byte $02,$C0,$0B,$F0,$1C,$38,$38,$1C
            .byte $72,$4E,$5F,$FA,$AA,$77,$5D,$BB
            .byte $B3,$CF,$C5,$A7,$64,$26,$51,$8C
            .byte $3F,$FC,$1A,$F8,$0D,$F0,$03,$C0

            .byte $03,$C0,$0B,$F0,$16,$78,$2C,$3C
            .byte $5A,$5E,$2C,$3A,$DD,$F7,$BE,$7F
            .byte $F1,$8F,$C7,$E7,$65,$A6,$50,$0C
            .byte $3F,$FC,$15,$78,$0B,$F0,$03,$C0

            .byte $02,$C0,$0B,$F0,$1C,$38,$38,$1C
            .byte $72,$4E,$5F,$FA,$AA,$77,$5D,$BB
            .byte $B3,$CF,$C5,$A7,$64,$26,$51,$8C
            .byte $3F,$FC,$1A,$F8,$0D,$F0,$03,$C0

            .byte $38,$00,$18,$00,$7F,$00,$6F,$00
            .byte $7F,$00,$2A,$00,$7F,$00,$3F,$80
            .byte $1F,$80,$0C,$00,$16,$00,$FF,$00
            .byte $17,$00,$2A,$80,$15,$00,$0E,$00

            .byte $38,$00,$18,$80,$1F,$C0,$1B,$C0
            .byte $1F,$C0,$0A,$80,$1F,$C0,$0F,$E0
            .byte $07,$E0,$03,$00,$05,$80,$7F,$C0
            .byte $05,$C0,$0A,$A0,$05,$40,$03,$80

            .byte $00,$60,$06,$30,$07,$F0,$06,$F0
            .byte $07,$F0,$02,$A0,$07,$F0,$03,$F8
            .byte $01,$F8,$00,$C0,$01,$60,$0F,$F0
            .byte $01,$70,$02,$A8,$01,$50,$00,$E0

            .byte $00,$38,$01,$18,$01,$FC,$01,$BC
            .byte $01,$FC,$00,$A8,$01,$FC,$00,$FE
            .byte $00,$7E,$00,$30,$00,$58,$01,$FC
            .byte $00,$5C,$00,$AA,$00,$54,$00,$38

            .byte $1C,$00,$18,$80,$3F,$80,$3D,$80
            .byte $3F,$80,$15,$00,$3F,$80,$7F,$00
            .byte $7E,$00,$0C,$00,$1A,$00,$3F,$80
            .byte $3A,$00,$55,$00,$2A,$00,$1C,$00

            .byte $06,$00,$0C,$60,$0F,$E0,$0F,$60
            .byte $0F,$E0,$05,$40,$0F,$E0,$1F,$C0
            .byte $1F,$80,$03,$00,$06,$80,$0F,$F0
            .byte $0E,$80,$15,$40,$0A,$80,$07,$00

            .byte $00,$1C,$01,$18,$03,$F8,$03,$D8
            .byte $03,$F8,$01,$50,$03,$F8,$07,$F0
            .byte $07,$E0,$00,$C0,$01,$A0,$03,$FE
            .byte $03,$A0,$05,$50,$02,$A0,$01,$C0

            .byte $00,$1C,$00,$18,$00,$FE,$00,$F6
            .byte $00,$FE,$00,$54,$00,$FE,$01,$FC
            .byte $01,$F8,$00,$30,$00,$68,$00,$FF
            .byte $00,$E8,$01,$54,$00,$A8,$00,$70

            .byte $0D,$00,$1F,$00,$3F,$00,$06,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

            .byte $87,$20,$0F,$20,$03,$20,$07,$40
            .byte $4F,$40,$03,$40,$07,$40,$0F,$C0
            .byte $01,$80,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

            .byte $20,$C4,$01,$C4,$0B,$C4,$00,$C8
            .byte $21,$C8,$0B,$C8,$00,$C8,$01,$C8
            .byte $03,$C8,$00,$C8,$01,$D0,$03,$D0
            .byte $00,$D0,$01,$D0,$03,$F0,$00,$60

            .byte $04,$FA,$00,$3A,$02,$7A,$00,$FA
            .byte $01,$36,$00,$74,$00,$F4,$00,$34
            .byte $00,$74,$00,$FC,$00,$18,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

            .byte $58,$00,$7C,$00,$7E,$00,$30,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

            .byte $27,$08,$27,$80,$26,$10,$17,$00
            .byte $17,$80,$16,$00,$17,$00,$1F,$80
            .byte $0C,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

            .byte $11,$82,$11,$C0,$11,$E8,$09,$80
            .byte $09,$C2,$09,$E8,$09,$80,$09,$D0
            .byte $09,$E0,$09,$80,$05,$C0,$05,$E0
            .byte $05,$80,$05,$C0,$07,$E0,$03,$00

            .byte $02,$78,$02,$61,$02,$70,$02,$78
            .byte $02,$60,$01,$70,$01,$78,$01,$60
            .byte $01,$70,$01,$F8,$00,$C0,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

            .byte $00,$00,$00,$00,$05,$00,$08,$80
            .byte $1F,$A0,$FD,$40,$FA,$A0,$55,$40
            .byte $3F,$C0,$17,$80,$13,$40,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

            .byte $00,$14,$00,$28,$02,$54,$02,$A8
            .byte $07,$50,$3E,$A8,$3D,$50,$16,$B0
            .byte $0F,$F0,$05,$E0,$08,$D0,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

            .byte $00,$00,$00,$00,$01,$04,$00,$88
            .byte $01,$FA,$0F,$D4,$0F,$AA,$05,$54
            .byte $03,$FC,$01,$78,$02,$34,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

            .byte $00,$00,$00,$00,$00,$22,$00,$22
            .byte $00,$7E,$03,$FF,$03,$EB,$01,$55
            .byte $00,$EB,$00,$56,$00,$AB,$00,$15
            .byte $00,$0A,$00,$15,$00,$0A,$00,$03

            .byte $00,$00,$00,$00,$44,$00,$44,$00
            .byte $7E,$00,$FF,$C0,$D7,$C0,$AA,$80
            .byte $D7,$00,$6A,$00,$D5,$00,$A8,$00
            .byte $50,$00,$A8,$00,$50,$00,$80,$00

            .byte $00,$00,$00,$00,$20,$80,$11,$00
            .byte $5F,$80,$2B,$F0,$55,$F0,$2A,$A0
            .byte $3F,$C0,$1E,$80,$2C,$40,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

            .byte $28,$00,$14,$00,$2A,$40,$15,$40
            .byte $0A,$E0,$15,$7C,$0A,$BC,$0D,$68
            .byte $0F,$F0,$07,$A0,$0B,$10,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

            .byte $00,$00,$00,$00,$00,$A0,$01,$10
            .byte $05,$F8,$02,$BF,$05,$5F,$02,$AA
            .byte $03,$FC,$01,$E8,$02,$C8,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

            .byte $C0,$E0,$CE,$C0,$47,$C2,$AE,$E2
            .byte $EF,$E3,$7B,$BF,$3C,$7F,$0F,$E3
            .byte $0D,$62,$07,$C2,$05,$45,$05,$45
            .byte $05,$42,$0A,$A0,$15,$50,$0A,$A0

            .byte $0E,$02,$06,$E2,$07,$C2,$0E,$E3
            .byte $0F,$EF,$3A,$BF,$7C,$7A,$6F,$E2
            .byte $CD,$65,$A7,$C5,$45,$42,$45,$40
            .byte $CA,$A0,$D5,$50,$CA,$A0,$C0,$00

            .byte $7F,$FE,$AF,$79,$2F,$78,$2F,$78
            .byte $2F,$78,$2F,$78,$2D,$68,$13,$90
            .byte $1F,$E0,$17,$60,$17,$60,$15,$40
            .byte $0B,$A0,$05,$C0,$04,$80,$03,$00

            .byte $7F,$FE,$97,$BD,$17,$BC,$17,$BC
            .byte $17,$BC,$17,$BC,$16,$B4,$09,$C8
            .byte $07,$F8,$05,$D8,$05,$D8,$05,$50
            .byte $02,$E8,$01,$70,$01,$20,$00,$C0

            .byte $6E,$00,$00,$00,$76,$00,$6E,$00
            .byte $00,$00,$00,$00,$76,$00,$00,$00
            .byte $6E,$00,$76,$00,$00,$00,$6E,$00
            .byte $76,$00,$6E,$00,$00,$00,$76,$00

            .byte $1D,$80,$24,$00,$2B,$60,$4B,$B0
            .byte $55,$00,$91,$00,$AA,$D8,$91,$80
            .byte $94,$B8,$AA,$E8,$90,$80,$4B,$D0
            .byte $51,$B0,$27,$60,$2A,$00,$1D,$80

            .byte $03,$C0,$0D,$70,$12,$88,$28,$54
            .byte $55,$0A,$48,$42,$9A,$05,$A4,$91
            .byte $C8,$45,$B5,$11,$40,$2A,$68,$82
            .byte $32,$14,$10,$98,$0C,$30,$03,$C0

            .byte $01,$B8,$00,$24,$06,$D4,$0D,$D2
            .byte $00,$AA,$00,$89,$1B,$55,$01,$89
            .byte $1D,$29,$17,$55,$01,$09,$0B,$D2
            .byte $0D,$8A,$06,$E4,$00,$54,$01,$B8

            .byte $60,$00,$18,$00,$0E,$C0,$1F,$C0
            .byte $1F,$00,$9F,$00,$7E,$00,$3E,$00
            .byte $3C,$00,$F8,$00,$F0,$00,$50,$00
            .byte $38,$00,$08,$00,$06,$00,$00,$00

            .byte $04,$00,$02,$00,$01,$00,$03,$B0
            .byte $07,$F0,$0F,$C0,$2F,$C0,$1F,$80
            .byte $0F,$80,$3F,$00,$0E,$00,$3C,$00
            .byte $14,$80,$0F,$00,$00,$00,$00,$00

            .byte $06,$00,$01,$80,$00,$EC,$01,$FC
            .byte $01,$F0,$09,$F0,$07,$E0,$0B,$E0
            .byte $07,$C0,$03,$80,$07,$00,$0D,$00
            .byte $03,$80,$00,$80,$00,$60,$00,$00

            .byte $00,$F0,$00,$3B,$00,$7F,$00,$7C
            .byte $00,$7C,$01,$FC,$00,$78,$01,$F8
            .byte $00,$70,$01,$E0,$00,$A0,$00,$70
            .byte $00,$08,$00,$04,$00,$00,$00,$00

            .byte $06,$00,$1C,$00,$56,$00,$7F,$00
            .byte $3F,$00,$3F,$00,$3F,$00,$9E,$00
            .byte $6C,$00,$3E,$00,$1E,$00,$16,$00
            .byte $0C,$00,$0C,$00,$04,$00,$04,$00

            .byte $01,$00,$03,$80,$1D,$80,$0F,$C0
            .byte $0F,$C0,$0F,$C0,$0F,$C0,$27,$90
            .byte $1B,$60,$1F,$E0,$13,$20,$17,$A0
            .byte $13,$20,$13,$20,$08,$40,$04,$80

            .byte $01,$80,$00,$E0,$01,$A8,$03,$F8
            .byte $03,$F0,$03,$F0,$03,$F0,$01,$E4
            .byte $00,$D8,$01,$F0,$01,$E0,$01,$A0
            .byte $00,$C0,$00,$C0,$00,$80,$00,$80

            .byte $00,$20,$00,$70,$00,$6E,$00,$FC
            .byte $00,$FC,$00,$FC,$00,$FC,$02,$79
            .byte $01,$B6,$01,$FE,$01,$7A,$01,$7A
            .byte $01,$02,$01,$32,$00,$84,$00,$48

            .byte $3C,$00,$76,$00,$E5,$00,$A5,$00
            .byte $EE,$00,$C5,$00,$46,$00,$34,$00
            .byte $2C,$00,$62,$00,$A3,$00,$77,$00
            .byte $A5,$00,$A7,$00,$6E,$00,$3C,$00

            .byte $3C,$00,$76,$00,$E5,$00,$A5,$00
            .byte $EE,$00,$C5,$00,$46,$F0,$35,$D8
            .byte $02,$7C,$02,$0C,$00,$44,$03,$FC
            .byte $01,$48,$00,$B0,$00,$00,$00,$00

            .byte $3C,$3C,$76,$76,$E5,$E5,$A5,$A5
            .byte $EE,$EE,$C5,$C5,$46,$46,$34,$34
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

            .byte $00,$3C,$00,$76,$00,$E5,$00,$A5
            .byte $00,$EE,$00,$C5,$0D,$46,$12,$B4
            .byte $3F,$C0,$22,$00,$30,$40,$3E,$40
            .byte $1B,$80,$0F,$00,$00,$00,$00,$00

            .byte $00,$00,$11,$88,$02,$40,$25,$A4
            .byte $15,$78,$7F,$FE,$D7,$75,$B2,$27
            .byte $D7,$75,$7F,$FE,$20,$04,$35,$5C
            .byte $1A,$D8,$0D,$F0,$06,$60,$03,$C0

            .byte $01,$00,$04,$20,$01,$80,$0A,$50
            .byte $15,$78,$7F,$FE,$AE,$EB,$E4,$4D
            .byte $AE,$EB,$7F,$FE,$20,$04,$35,$5C
            .byte $1A,$B8,$0D,$F0,$06,$60,$03,$C0

            .byte $04,$20,$11,$88,$02,$40,$25,$A4
            .byte $15,$78,$7F,$FE,$ED,$DB,$A8,$93
            .byte $ED,$DB,$7F,$FE,$20,$04,$35,$5C
            .byte $1A,$D8,$0D,$F0,$06,$60,$03,$C0

            .byte $11,$44,$04,$00,$41,$90,$0A,$42
            .byte $15,$78,$7F,$FE,$DB,$B7,$C9,$15
            .byte $DB,$B7,$7F,$FE,$20,$04,$35,$5C
            .byte $1A,$E8,$1D,$F0,$06,$60,$03,$C0

            .byte $07,$E0,$8C,$31,$59,$9A,$36,$6C
            .byte $1D,$B8,$0D,$F0,$9E,$F9,$B1,$8D
            .byte $6A,$56,$31,$8C,$1F,$F8,$1F,$F8
            .byte $3F,$FC,$DF,$FB,$0F,$F0,$07,$E0

            .byte $07,$E0,$4C,$32,$58,$1A,$32,$4C
            .byte $1D,$B8,$8D,$F1,$9E,$F9,$71,$8E
            .byte $2A,$54,$31,$8C,$1F,$F8,$1F,$F8
            .byte $FF,$FF,$1F,$F8,$0F,$F0,$07,$E0

            .byte $C7,$E3,$2C,$34,$18,$18,$12,$48
            .byte $9A,$59,$8D,$B1,$5D,$FA,$30,$8C
            .byte $2A,$54,$31,$8C,$1F,$F8,$3F,$FC
            .byte $DF,$FB,$1F,$F8,$0F,$F0,$07,$E0

            .byte $07,$E0,$8C,$31,$78,$1E,$12,$48
            .byte $1D,$B8,$8D,$F1,$5E,$FA,$71,$8E
            .byte $2A,$54,$2E,$74,$11,$88,$1F,$F8
            .byte $3F,$FC,$5F,$FA,$8F,$F1,$07,$E0

            .byte $0C,$FD,$12,$FC,$2B,$C6,$04,$82
            .byte $59,$81,$2E,$49,$DD,$63,$6E,$BF
            .byte $99,$F9,$6D,$5C,$8A,$AE,$35,$5C
            .byte $4A,$A8,$15,$5E,$2A,$BE,$15,$03

            .byte $BF,$30,$3F,$48,$63,$D4,$41,$20
            .byte $81,$9A,$92,$74,$C6,$BB,$FD,$76
            .byte $9F,$99,$3A,$B6,$75,$51,$3A,$AC
            .byte $15,$52,$7A,$A8,$FD,$54,$40,$A8

            .byte $05,$20,$14,$28,$20,$84,$14,$A8
            .byte $2A,$22,$40,$0C,$12,$0A,$50,$00
            .byte $40,$01,$40,$04,$48,$84,$68,$D4
            .byte $AA,$45,$EA,$D7,$BF,$EF,$FD,$BF

            .byte $00,$00,$00,$00,$01,$40,$0A,$90
            .byte $14,$04,$08,$A8,$22,$82,$4A,$10
            .byte $08,$04,$42,$01,$82,$44,$2A,$54
            .byte $AA,$D1,$AC,$D7,$FF,$FB,$FD,$BF

            .byte $08,$00,$14,$00,$08,$00,$14,$00
            .byte $2A,$00,$14,$00,$2A,$00,$55,$00
            .byte $FF,$00,$FF,$80,$FE,$80,$FD,$00
            .byte $7E,$80,$7F,$40,$36,$80,$0C,$00

            .byte $02,$00,$05,$00,$02,$00,$05,$00
            .byte $0A,$80,$05,$00,$2A,$80,$35,$40
            .byte $3F,$C0,$3F,$E0,$3F,$A0,$3F,$40
            .byte $1F,$A0,$1F,$D0,$0D,$A0,$03,$00

            .byte $00,$80,$01,$40,$00,$80,$09,$40
            .byte $02,$A0,$01,$40,$0A,$A0,$05,$50
            .byte $0F,$F0,$0F,$F8,$0F,$E8,$0F,$D0
            .byte $07,$E8,$07,$F4,$03,$68,$00,$C0

            .byte $00,$20,$00,$50,$00,$20,$00,$50
            .byte $00,$A8,$00,$50,$02,$A8,$03,$54
            .byte $03,$FC,$03,$FE,$03,$FA,$03,$F4
            .byte $01,$FA,$01,$FD,$00,$DA,$00,$30

            .byte $81,$00,$41,$00,$3E,$00,$08,$00
            .byte $04,$00,$04,$00,$2A,$80,$55,$40
            .byte $20,$80,$5F,$40,$AF,$A0,$44,$40
            .byte $2B,$80,$55,$40,$2A,$80,$04,$00

            .byte $00,$00,$20,$40,$11,$80,$0E,$00
            .byte $02,$00,$02,$00,$15,$40,$2A,$A0
            .byte $10,$40,$2A,$A0,$5F,$50,$22,$20
            .byte $15,$C0,$2A,$A0,$15,$40,$02,$00

            .byte $00,$00,$02,$04,$01,$88,$00,$70
            .byte $00,$40,$00,$40,$02,$A8,$05,$54
            .byte $02,$08,$05,$F4,$0B,$EA,$04,$44
            .byte $03,$A8,$05,$54,$02,$A8,$00,$40

            .byte $00,$81,$00,$82,$00,$7C,$00,$10
            .byte $00,$20,$00,$20,$01,$54,$02,$AA
            .byte $01,$54,$02,$FA,$05,$7D,$02,$22
            .byte $01,$D4,$02,$AA,$01,$54,$00,$20

            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$80,$05,$00,$1A,$80
            .byte $75,$40,$BB,$80,$6F,$40,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$03,$C0,$37,$E0
            .byte $5D,$70,$36,$A0,$01,$50,$02,$80
            .byte $01,$40,$00,$A0,$00,$00,$00,$00

            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$08,$00,$50,$19,$A8
            .byte $2F,$54,$19,$B8,$00,$F4,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

            .byte $00,$00,$00,$00,$00,$00,$00,$0A
            .byte $00,$14,$00,$28,$00,$14,$03,$6A
            .byte $05,$D7,$03,$7E,$00,$3D,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

            .byte $00,$00,$00,$00,$00,$00,$50,$00
            .byte $28,$00,$14,$00,$28,$00,$56,$C0
            .byte $EB,$A0,$7E,$C0,$BC,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$10,$00,$0A,$00,$15,$98
            .byte $2A,$F4,$1D,$98,$2F,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$03,$C0,$07,$EC
            .byte $0E,$BA,$05,$6C,$0A,$80,$01,$40
            .byte $02,$80,$05,$00,$00,$00,$00,$00

            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$01,$00,$00,$A0,$01,$58
            .byte $02,$AE,$01,$DD,$02,$F6,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

            .byte $3C,$00,$13,$00,$14,$00,$1E,$00
            .byte $22,$00,$21,$80,$43,$00,$4F,$00
            .byte $5D,$00,$5A,$00,$3A,$00,$3A,$00
            .byte $2E,$00,$D4,$00,$3C,$00,$18,$00

            .byte $07,$00,$02,$00,$02,$00,$07,$80
            .byte $08,$80,$08,$40,$10,$60,$11,$C0
            .byte $17,$80,$1F,$00,$0E,$80,$0E,$80
            .byte $0B,$80,$3F,$00,$0F,$00,$06,$00

            .byte $01,$40,$06,$30,$01,$40,$01,$E0
            .byte $02,$20,$02,$18,$04,$30,$04,$F0
            .byte $05,$D0,$05,$A0,$03,$A0,$03,$A0
            .byte $0A,$E0,$05,$40,$0B,$C0,$01,$80

            .byte $00,$74,$00,$22,$00,$25,$00,$78
            .byte $00,$88,$00,$86,$01,$0C,$01,$3C
            .byte $01,$74,$01,$68,$00,$E8,$00,$E8
            .byte $00,$B8,$03,$50,$00,$B0,$00,$60

            .byte $2E,$00,$44,$00,$A4,$00,$1E,$00
            .byte $11,$00,$61,$00,$30,$80,$3C,$80
            .byte $2E,$80,$16,$80,$17,$00,$17,$00
            .byte $1D,$00,$0A,$C0,$0D,$00,$06,$00

            .byte $02,$80,$0C,$60,$02,$80,$07,$80
            .byte $04,$40,$18,$40,$0C,$20,$0F,$20
            .byte $0B,$A0,$05,$A0,$05,$C0,$05,$C0
            .byte $07,$50,$02,$A0,$03,$D0,$01,$80

            .byte $00,$E0,$00,$40,$00,$40,$01,$E0
            .byte $01,$10,$02,$10,$06,$08,$03,$88
            .byte $01,$E8,$00,$F8,$01,$70,$01,$70
            .byte $01,$D0,$00,$FC,$00,$F0,$00,$60

            .byte $00,$78,$01,$90,$00,$50,$00,$78
            .byte $00,$44,$01,$84,$00,$C2,$00,$F2
            .byte $00,$BA,$00,$5A,$00,$5C,$00,$5C
            .byte $00,$74,$00,$2B,$00,$3C,$00,$18

            .byte $2E,$00,$40,$80,$AC,$40,$0D,$40
            .byte $40,$00,$37,$00,$00,$00,$1E,$00
            .byte $0C,$00,$0C,$00,$12,$00,$21,$00
            .byte $2D,$00,$21,$00,$12,$00,$0C,$00

            .byte $07,$00,$10,$60,$23,$50,$2B,$00
            .byte $00,$20,$0E,$C0,$00,$00,$07,$80
            .byte $03,$00,$0E,$C0,$1D,$A0,$1F,$00
            .byte $1F,$E0,$1F,$00,$1D,$A0,$0E,$C0

            .byte $03,$B0,$00,$08,$0A,$C0,$08,$D4
            .byte $04,$08,$01,$D0,$00,$00,$01,$E0
            .byte $00,$C0,$04,$C8,$06,$D8,$07,$38
            .byte $0F,$FC,$07,$38,$06,$18,$04,$08

            .byte $00,$DC,$01,$00,$00,$35,$02,$B1
            .byte $01,$02,$00,$B8,$00,$00,$00,$78
            .byte $00,$30,$00,$DC,$01,$6E,$00,$3E
            .byte $01,$FE,$00,$3E,$01,$6E,$00,$DC

            .byte $00,$00,$00,$00,$07,$E0,$1F,$F8
            .byte $3F,$FC,$7F,$DE,$7F,$EE,$FB,$F7
            .byte $FB,$F7,$7D,$F6,$7F,$FE,$3D,$EC
            .byte $1F,$B8,$07,$E0,$00,$00,$00,$00

            .byte $00,$00,$01,$80,$07,$E0,$0F,$F0
            .byte $1F,$38,$3F,$DC,$3B,$EC,$7B,$F6
            .byte $7D,$FE,$3F,$F4,$3F,$FC,$1F,$F8
            .byte $0D,$70,$07,$E0,$01,$80,$00,$00

            .byte $01,$80,$07,$E0,$0F,$F0,$1F,$38
            .byte $1F,$D8,$3F,$EC,$3F,$EC,$37,$FC
            .byte $37,$F4,$3B,$FC,$3F,$7C,$1F,$78
            .byte $1E,$F8,$0F,$F0,$07,$E0,$01,$80

            .byte $00,$00,$01,$80,$07,$E0,$0F,$F0
            .byte $1F,$38,$3F,$DC,$37,$EC,$77,$F6
            .byte $7B,$FE,$3F,$F4,$3F,$FC,$1E,$B8
            .byte $0F,$F0,$07,$E0,$01,$80,$00,$00

            .byte $07,$70,$03,$62,$03,$61,$03,$E2
            .byte $03,$E5,$02,$2A,$05,$D4,$02,$A8
            .byte $07,$70,$0B,$E0,$16,$A0,$29,$C0
            .byte $53,$60,$A3,$60,$40,$00,$80,$00

            .byte $04,$70,$0E,$60,$17,$64,$03,$EA
            .byte $03,$E4,$02,$2A,$05,$D4,$03,$E8
            .byte $06,$30,$0B,$E0,$56,$A0,$A9,$C0
            .byte $56,$30,$06,$30,$00,$00,$00,$00

            .byte $07,$70,$03,$60,$03,$60,$03,$E2
            .byte $03,$E1,$82,$22,$45,$D5,$A3,$6A
            .byte $56,$B4,$2B,$E0,$1E,$A0,$01,$C0
            .byte $03,$60,$03,$60,$00,$00,$00,$00

            .byte $07,$10,$03,$38,$03,$74,$03,$E0
            .byte $03,$E1,$02,$22,$05,$D5,$02,$2A
            .byte $07,$F4,$0B,$E0,$56,$A0,$A9,$C0
            .byte $56,$30,$06,$30,$00,$00,$00,$00

            .byte $1D,$C0,$45,$00,$AA,$84,$55,$44
            .byte $2A,$A4,$15,$50,$07,$2C,$08,$82
            .byte $12,$44,$2F,$E0,$2B,$E4,$17,$44
            .byte $15,$C4,$0A,$84,$05,$0E,$02,$04

            .byte $1D,$C4,$45,$04,$AA,$84,$55,$40
            .byte $2A,$AC,$15,$52,$07,$04,$08,$80
            .byte $12,$44,$2F,$E4,$2B,$E4,$17,$44
            .byte $15,$CE,$0A,$84,$05,$00,$02,$00

            .byte $31,$C0,$70,$C0,$5B,$80,$1F,$00
            .byte $6F,$60,$7F,$E0,$3F,$C0,$1F,$80
            .byte $0F,$00,$06,$00,$0F,$00,$1F,$00
            .byte $0B,$00,$1F,$80,$0F,$00,$0F,$00

            .byte $0E,$E0,$07,$60,$06,$E0,$03,$C0
            .byte $0D,$F0,$0E,$F0,$07,$E0,$07,$E0
            .byte $03,$C0,$01,$80,$03,$C0,$07,$C0
            .byte $02,$C0,$07,$E0,$03,$C0,$03,$C0

            .byte $00,$E0,$00,$60,$00,$60,$00,$F0
            .byte $01,$B8,$01,$D8,$01,$D8,$01,$D8
            .byte $00,$F0,$00,$60,$00,$F0,$01,$F0
            .byte $00,$B0,$01,$F8,$00,$F0,$00,$F0

            .byte $00,$EE,$00,$76,$00,$6E,$00,$3C
            .byte $00,$DF,$00,$EF,$00,$7E,$00,$7E
            .byte $00,$3C,$00,$18,$00,$3C,$00,$7C
            .byte $00,$2C,$00,$7E,$00,$3C,$00,$3C

; Data Willy sprites

L4480:      .byte $77,$00,$6E,$00,$76,$00,$3C,$00	; Willy right 1
            .byte $FB,$00,$F7,$00,$7E,$00,$7E,$00
            .byte $3C,$00,$18,$00,$3C,$00,$3E,$00
            .byte $34,$00,$7E,$00,$3C,$00,$3C,$00

L44A0:      .byte $07,$00,$06,$00,$06,$00,$0F,$00	; Willy right 2
            .byte $1D,$80,$1B,$80,$1B,$80,$1B,$80
            .byte $0F,$00,$06,$00,$0F,$00,$0F,$80
            .byte $0D,$00,$1F,$80,$0F,$00,$0F,$00

L44C0:      .byte $07,$70,$06,$E0,$07,$60,$03,$C0	; Willy right 3
            .byte $0F,$B0,$0F,$70,$07,$E0,$07,$E0
            .byte $03,$C0,$01,$80,$03,$C0,$03,$E0
            .byte $03,$40,$07,$E0,$03,$C0,$03,$C0

L44E0:      .byte $03,$8C,$03,$0E,$01,$DA,$00,$F8	; Willy right 4
            .byte $06,$F6,$07,$FE,$03,$FC,$01,$F8
            .byte $00,$F0,$00,$60,$00,$F0,$00,$F8
            .byte $00,$D0,$01,$F8,$00,$F0,$00,$F0
L4500:
            .byte $77,$00,$6E,$00,$76,$00,$3C,$00
            .byte $FB,$00,$F7,$00,$7E,$00,$7E,$00
            .byte $3C,$00,$18,$00,$3C,$00,$3E,$00
            .byte $34,$00,$7E,$00,$3C,$00,$3C,$00

            .byte $07,$00,$06,$00,$06,$00,$0F,$00
            .byte $1D,$80,$1B,$80,$1B,$80,$1B,$80
            .byte $0F,$00,$06,$00,$0F,$00,$0F,$80
            .byte $0D,$00,$1F,$80,$0F,$00,$0F,$00

            .byte $07,$70,$06,$E0,$07,$60,$03,$C0
            .byte $0F,$B0,$0F,$70,$07,$E0,$07,$E0
            .byte $03,$C0,$01,$80,$03,$C0,$03,$E0
            .byte $03,$40,$07,$E0,$03,$C0,$03,$C0

            .byte $03,$8C,$03,$0E,$01,$DA,$00,$F8
            .byte $06,$F6,$07,$FE,$03,$FC,$01,$F8
            .byte $00,$F0,$00,$60,$00,$F0,$00,$F8
            .byte $00,$D0,$01,$F8,$00,$F0,$00,$F0

            .byte $31,$C0,$70,$C0,$5B,$80,$1F,$00
            .byte $6F,$60,$7F,$E0,$3F,$C0,$1F,$80
            .byte $0F,$00,$06,$00,$0F,$00,$1F,$00
            .byte $0B,$00,$1F,$80,$0F,$00,$0F,$00

            .byte $0E,$E0,$07,$60,$06,$E0,$03,$C0
            .byte $0D,$F0,$0E,$F0,$07,$E0,$07,$E0
            .byte $03,$C0,$01,$80,$03,$C0,$07,$C0
            .byte $02,$C0,$07,$E0,$03,$C0,$03,$C0

            .byte $00,$E0,$00,$60,$00,$60,$00,$F0
            .byte $01,$B8,$01,$D8,$01,$D8,$01,$D8
            .byte $00,$F0,$00,$60,$00,$F0,$01,$F0
            .byte $00,$B0,$01,$F8,$00,$F0,$00,$F0

            .byte $00,$EE,$00,$76,$00,$6E,$00,$3C
            .byte $00,$DF,$00,$EF,$00,$7E,$00,$7E
            .byte $00,$3C,$00,$18,$00,$3C,$00,$7C
            .byte $00,$2C,$00,$7E,$00,$3C,$00,$3C

;------------------------------------------------

L4600:
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
L4700:
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
;            .byte $aa,$aa,$55,$55,$aa,$aa,$55,$55
;            .byte $aa,$aa,$55,$55,$aa,$aa,$55,$55
;            .byte $aa,$aa,$55,$55,$aa,$aa,$55,$55
;            .byte $aa,$aa,$55,$55,$aa,$aa,$55,$55

L4720:      .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

;------------------------------------------------
; Arrow sprites
;------------------------------------------------

L4760:
            .byte $00,$00,$00,$41,$FE,$41,$00,$00	; Arrow1
            .byte $00,$00,$00,$00,$00,$00,$00,$00
L4770:
            .byte $00,$00,$00,$82,$7F,$82,$00,$00	; Arrow 2
            .byte $00,$00,$00,$00,$00,$00,$00,$00
;------------------------------------------------

L4780:      .byte $00,$00,$00,$00,$00,$00,$00,$00
L4788:      .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
L47A0:      .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

;------------------------------------------------
; L4800-L49FF = Shadowscreen
;------------------------------------------------

L483C	= L4800+$3c
L4843	= L4800+$43
L48D0	= L4800+$d0

L4800:      .byte $02,$01,$01,$02,$02,$02,$02,$02
            .byte $02,$01,$02,$02,$02,$01,$02,$02
            .byte $02,$02,$01,$02,$02,$01,$01,$02
            .byte $01,$01,$03,$02,$02,$03,$02,$03
            .byte $02,$03,$03,$03,$03,$03,$01,$00
            .byte $03,$03,$03,$03,$02,$03,$03,$02
            .byte $02,$03,$02,$02,$02,$03,$02,$02
            .byte $02,$02,$03,$02,$02,$03,$03,$02
            .byte $03,$03,$01,$03,$03,$01,$02,$01
            .byte $02,$01,$02,$02,$02,$02,$03,$03
            .byte $01,$DD,$DB,$D2,$D0,$97,$8D,$8C
            .byte $89,$4E,$45,$42,$07,$05,$02,$BF
            .byte $BD,$BB,$B8,$7E,$74,$72,$70,$35
            .byte $2B,$29,$EF,$EC,$EA,$E0,$A5,$A2
            .byte $03,$6C,$77,$67,$67,$67,$67,$66
            .byte $66,$66,$66,$65,$65,$65,$64,$64
            .byte $64,$62,$62,$62,$62,$61,$61,$61
            .byte $61,$60,$60,$60,$5E,$5E,$5E,$5E
            .byte $5D,$5D,$61,$61,$61,$BC,$B9,$B6
            .byte $B4,$B1,$AF,$AC,$AA,$A8,$A5,$A4
            .byte $A3,$A1,$A0,$9F,$9D,$9B,$9A,$98
            .byte $96,$94,$92,$91,$8F,$8D,$8B,$8A
            .byte $88,$86,$84,$82,$00,$00,$00,$45
            .byte $43,$42,$40,$3F,$3D,$3C,$39,$36
            .byte $35,$32,$2F,$2D,$2A,$27,$25,$23
            .byte $20,$1E,$1C,$1A,$18,$15,$13,$11
            .byte $0F,$0C,$0A,$08,$05,$02,$00,$00
            .byte $00,$00,$02,$00,$04,$00,$06,$00
            .byte $08,$00,$0A,$00,$0C,$00,$0E,$00
            .byte $10,$00,$12,$00,$14,$00,$16,$00
            .byte $00,$01,$02,$03,$03,$02,$00,$8D
            .byte $8E,$03,$90,$40,$91,$00,$00,$00

L4997	= L4900+$b7
L4999	= L4900+$99
L499B	= L4900+$9b
L49B7	= L4900+$b7

L4900:      .byte $00,$1C,$04,$02,$25,$1C,$0E,$00
            .byte $00,$A0,$6A,$07,$01,$8A,$6A,$2E
            .byte $30,$0D,$00,$00,$00,$FF,$00,$00
            .byte $00,$00,$00,$6A,$90,$E0,$78,$58
            .byte $1C,$8A,$8A,$48,$68,$18,$68,$70
            .byte $68,$48,$60,$AD,$48,$FE,$02,$00
            .byte $00,$02,$FE,$FE,$2C,$CE,$86,$00
            .byte $00,$78,$00,$10,$00,$BD,$26,$0C
            .byte $E8,$00,$78,$C8,$38,$A8,$8D,$81
            .byte $0A,$02,$03,$01,$01,$01,$03,$20
            .byte $F1,$FF,$52,$9C,$00,$8B,$03,$71
            .byte $C9,$25,$D0,$02,$00,$00,$02,$FE
            .byte $FE,$A5,$EC,$02,$FF,$FF,$00,$FF
            .byte $FF,$FF,$7C,$0A,$C9,$01,$00,$00
            .byte $01,$00,$00,$7C,$0A,$68,$A8,$68
            .byte $AA,$68,$28,$60,$00,$00,$60,$48
            .byte $00,$01,$01,$01,$01,$01,$01,$01
            .byte $01,$01,$01,$01,$18,$20,$40,$B8
            .byte $D8,$E8,$F8,$C9,$A0,$D0,$06,$8D
            .byte $6E,$65,$5F,$5F,$6C,$6C,$6C,$68
            .byte $AA,$68,$85,$FC,$14,$15,$15,$15
            .byte $B7,$D0,$3C,$43,$97,$99,$9B,$68
            .byte $84,$08,$08,$08,$49,$48,$48,$48
            .byte $49,$49,$49,$08,$08,$08,$08,$08
            .byte $00,$20,$01,$60,$02,$A0,$03,$E0
            .byte $05,$20,$06,$60,$07,$A0,$08,$E0
            .byte $0A,$20,$0B,$60,$0C,$A0,$0D,$E0
            .byte $0F,$20,$10,$60,$11,$A0,$12,$E0
            .byte $14,$20,$15,$60,$16,$A0,$17,$E0
            .byte $19,$20,$1A,$60,$1B,$A0,$1C,$E0
            .byte $1E,$20,$1F,$60,$20,$A0,$21,$E0
            .byte $23,$20,$24,$60,$25,$A0,$26,$00

;------------------------------------------------
; Data is moved from L4A00->L0400
;------------------------------------------------

L4A00:      .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

;------------------------------------------------
; Data is moved from L4B00->L0500
;------------------------------------------------

L4B00:      .byte $22,$24,$20,$24,$06,$02,$04,$04
            .byte $27,$1F,$04,$1C
            .byte "...Jetset Willy was "
            .byte "written for the Elec"
            .byte "tron by Chris Robson"
            .byte "......Jet Set Willy "
            .byte "was prepared by Tyne"
            .byte "soft under licence f"
            .byte "rom Software Project"
            .byte "s and is copyright t"
            .byte "o Software Projects."
            .byte ".....You must guide "
            .byte "Willy to collect all"
            .byte " the items around th"
            .byte "e ho"

;------------------------------------------------
; Data is moved from L4C00->L0600
;------------------------------------------------

L4C00:      .byte "use before midnight "
            .byte "so that Maria will l"
            .byte "et you go to bed...."
            .byte "...Press SPACE to st"
            .byte "art the game."
            .byte $00,$70,$77,$EE,$0E,$00,$12,$22
            .byte $42,$00,$20,$27,$EE,$04,$00,$12
            .byte $82,$42,$00,$30,$27,$EE,$04,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$44,$45,$22,$00,$40,$54,$24
            .byte $02,$00,$54,$45,$3E,$00,$40,$55
            .byte $84,$00,$00,$7C,$DD,$09,$00,$08
            .byte $C0,$D7,$9D,$00,$00,$C6,$76,$10
            .byte $E2,$49,$49,$A8,$25,$00,$25,$A8
            .byte $40,$DD,$40,$28,$54,$AA,$55,$AA
            .byte $28,$28,$28,$00,$0A,$BD,$FF,$00
            .byte $AA,$55,$AA,$10,$30,$38,$6C,$C6
            .byte $C6,$6C,$28,$FF,$AA,$55,$80,$5D
            .byte $94,$5D,$80,$22,$55,$00,$55,$88
            .byte $55,$00,$22,$7E,$81,$A5,$99,$99
            .byte $A5,$81,$7E,$03,$01,$0C,$04,$30
            .byte $10,$C0,$40,$FF,$A5,$52,$24,$42
            .byte $22,$40,$04,$00,$2A,$54,$00,$00
            .byte $A2,$45,$00,$42,$99,$66,$A1,$9F
            .byte $81,$66,$5A,$FF,$5A,$FF,$FF,$AA
            .byte $55,$AA,$55

;------------------------------------------------
; Data is moved from L4D00->L0700
;------------------------------------------------

L4D00:      .byte $00,$01,$04,$05,$01,$00,$00,$00
            .byte $04,$00,$00,$04,$00,$04,$05,$01
            .byte $04,$05,$06,$06,$04,$01,$01,$03
            .byte $00,$01,$05,$04,$05,$01,$04,$04
            .byte $01,$00,$05,$06,$04,$05,$00,$04
            .byte $00,$05,$05,$04,$05,$01,$01,$04
            .byte $00,$05,$01,$02,$01,$05,$06,$05
            .byte $01,$05,$06,$03,$02,$00,$26,$0C
            .byte $68,$04,$00,$00,$00,$04,$01,$04
            .byte $00,$00,$04,$03,$03,$00,$04,$00
            .byte $01,$00,$04,$01,$00,$00,$00,$04
            .byte $01,$00,$04,$00,$00,$04,$00,$01
            .byte $00,$04,$00,$00,$00,$00,$01,$01
            .byte $07,$04,$00,$00,$04,$04,$00,$01
            .byte $00,$00,$04,$00,$04,$04,$04,$00
            .byte $00,$04,$07,$00,$00,$00,$40,$40
            .byte $08,$48,$8A,$48,$98,$48,$20,$2C
            .byte $04,$04,$04,$00,$00,$00,$00,$00
            .byte $00,$0C,$0E,$10,$60,$40,$20,$FF
            .byte $FF,$FF,$02,$07,$02,$00,$00,$02
            .byte $00,$80,$00,$04,$04,$04,$00,$00
            .byte $00,$FE,$FC,$F8,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$32,$09,$08,$01,$3C,$05,$08
            .byte $80,$0A,$14,$08,$0A,$1E,$05,$08
            .byte $13,$40,$05,$08,$00,$64,$85,$00
            .byte $00,$00,$8F,$00,$23,$00,$00,$00

;------------------------------------------------
; Program entry
;------------------------------------------------

L4E00:
	ldx #0
	txa
zero_loop:
	sta 0,x
	inx
	bne zero_loop

	jsr vdu22		; CLEAR4
	jsr vdu12		; CLS
	
;	jsr L5700		; Set new IRQ vector

; Clear L4A00-L$AFF

	ldx #0			
	txa
L4E0F:
	sta L4A00,X
	dex
	bne L4E0F

; Reset vars

	lda #$1f
	sta L0094		; Nr of bytes to plot for sprite

	lda #8
	sta L0095

	jmp L0B21		; Set return address L0B21

;--------------------------------------------

L4E5E:      .byte $4B,$EA,$EB
            .byte $E4,$C7,$1E,$57,$60,$97,$B9,$72
            .byte $68,$4F,$D9,$F1,$FC,$D4,$00,$70
            .byte $5D,$D7,$15,$B1,$A5,$A5,$24,$89
            .byte $08,$75,$10,$3A,$89,$CD,$11,$58
            .byte $44,$6E,$76,$8F,$03,$4C,$D2,$81
            .byte $03,$1F,$F8,$29,$B7,$CC,$4D,$A5
            .byte $4E,$2A,$B5,$51,$99,$91,$CA,$F4
            .byte $5C,$AA,$E5,$52,$35,$95,$C9,$A9
            .byte $6C,$5F,$A8,$14,$A5,$CE,$1C,$25
            .byte $CB,$9A,$24,$73,$A6,$6A,$DD,$16
            .byte $2F,$AB,$DD,$5A,$9F,$B1,$B5,$F1
            .byte $A6,$AC,$B5,$67,$4A,$6D,$BA,$A1
            .byte $6A,$D0,$0E,$47,$C9,$1A,$1D,$46
            .byte $CC,$D8,$70,$E2,$C6,$87,$2E,$52
            .byte $D4,$78,$EC,$A3,$06,$3A,$1E,$90
            .byte $06,$3A,$1E,$E2,$45,$36,$0E,$70
            .byte $3D,$EB,$AD,$2D,$17,$27,$16,$8A
            .byte $C6,$C7,$1E,$22,$8B,$C5,$24,$DE
            .byte $FF,$70,$14,$95,$1C,$38,$F0,$6B
            .byte $3E,$10,$95,$EF,$F8,$59,$00

; Character definitions

L4F00:
            .byte $00,$DE,$6C,$88,$12,$40,$04,$00
            .byte $22,$FF,$88,$FF,$22,$FF,$88,$FF
            .byte $F8,$88,$9E,$BD,$BD,$9E,$88,$F8
            .byte $03,$00,$0C,$00,$30,$00,$C0,$00
            .byte $33,$FF,$33,$00,$FF,$00,$AA,$00
            .byte $18,$18,$3C,$7E,$62,$62,$62,$7E
            .byte $FF,$FF,$FD,$BB,$D6,$72,$60,$40
            .byte $22,$77,$22,$88,$22,$77,$22,$88
            .byte $03,$84,$48,$30,$03,$84,$48,$30
            .byte $FF,$5B,$D6,$AF,$D2,$B7,$E2,$F7
            .byte $01,$40,$04,$46,$4F,$79,$F0,$F9
            .byte $A0,$14,$82,$50,$0A,$41,$28,$05
            .byte $D7,$B3,$75,$6D,$5C,$18,$08,$04
            .byte $BD,$AA,$5D,$A2,$F3,$0A,$06,$01
            .byte $49,$6A,$3B,$3A,$FC,$1E,$6B,$E8
            .byte $42,$81,$E7,$81,$5A,$7E,$3C,$42
            .byte $81,$42,$24,$18,$3C,$18,$3C,$42
            .byte $F0,$F9,$BF,$1F,$0C,$10,$28,$44
            .byte $AB,$AB,$AD,$AA,$AA,$AE,$55,$55
            .byte $6E,$74,$38,$1E,$6E,$74,$38,$1E
            .byte $03,$02,$3C,$08,$70,$20,$C0,$80
            .byte $40,$A0,$5C,$22,$21,$22,$14,$08
            .byte $AA,$55,$AA,$FF,$FF,$55,$AA,$55
            .byte $70,$07,$77,$77,$00,$3B,$0B,$7D
            .byte $14,$2A,$14,$09,$6A,$9C,$08,$08
            .byte $C0,$00,$30,$00,$0C,$00,$03,$00
            .byte $7B,$00,$ED,$00,$AA,$55,$AA,$55
            .byte $BB,$77,$EE,$DD,$BB,$77,$EE,$DD
            .byte $FF,$FF,$AA,$55,$AA,$55,$AA,$55
            .byte $0E,$C6,$95,$B0,$36,$86,$C0,$ED
            .byte $03,$03,$0C,$0C,$30,$30,$C0,$C0
            .byte $6F,$00,$B7,$00,$AA,$55,$AA,$55
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $BB,$BB,$38,$BB,$BB,$BB,$83,$BB
            .byte $BF,$5C,$58,$30,$28,$58,$48,$30
            .byte $80,$C0,$20,$F0,$F8,$4C,$9A,$01
            .byte $04,$03,$C9,$D8,$30,$36,$23,$01
            .byte $19,$60,$0E,$F0,$03,$3C,$C7,$38
            .byte $C2,$34,$0B,$44,$88,$0B,$30,$08
            .byte $C0,$80,$30,$20,$0C,$08,$03,$02
            .byte $20,$18,$A9,$DB,$7E,$7E,$3C,$00
            .byte $F5,$FA,$BF,$1F,$26,$14,$22,$10
            .byte $C0,$C0,$30,$B0,$0C,$0C,$03,$0B
            .byte $1F,$F3,$E1,$60,$30,$2C,$40,$80
            .byte $09,$12,$0A,$1F,$E4,$08,$08,$08
            .byte $42,$29,$54,$26,$87,$23,$2A,$04
            .byte $30,$3C,$4F,$43,$C2,$86,$84,$00
            .byte $FF,$AA,$13,$AA,$77,$AA,$55,$AA
            .byte $DF,$EF,$F1,$3E,$CF,$F6,$39,$DF
            .byte $0C,$1C,$1C,$1E,$1B,$3F,$78,$F8
            .byte $FF,$AA,$77,$AA,$77,$AA,$55,$AA
            .byte $88,$A9,$A9,$BB,$BF,$BF,$BF,$5E
            .byte $AA,$55,$AA,$58,$30,$10,$10,$08
            .byte $A5,$28,$52,$55,$55,$49,$A2,$B5
            .byte $89,$4A,$12,$34,$18,$34,$52,$91
            .byte $1E,$2D,$37,$7B,$6D,$5E,$B8,$C0
            .byte $01,$1E,$06,$7A,$1A,$E8,$68,$A0
            .byte $07,$FF,$FC,$38,$18,$10,$20,$00
            .byte $01,$81,$06,$58,$20,$40,$88,$82
            .byte $60,$B8,$BE,$5D,$5A,$35,$2A,$0A
            .byte $C0,$C0,$30,$30,$0C,$0C,$03,$03
            .byte $EE,$00,$BB,$00,$EE,$00,$BB,$00
            .byte $D1,$E2,$C4,$C8,$D1,$E2,$C4,$C8
            .byte $F1,$AA,$F8,$AA,$00,$EE,$00,$BB
            .byte $19,$49,$49,$49,$49,$49,$49,$AA
            .byte $80,$40,$20,$50,$48,$54,$52,$55
            .byte $66,$66,$66,$00,$66,$66,$66,$66
            .byte $51,$55,$54,$55,$15,$55,$45,$55
            .byte $18,$24,$18,$2C,$5E,$5E,$E7,$7E
            .byte $33,$BB,$DD,$CC,$55,$99,$DD,$EE
            .byte $27,$F6,$00,$DC,$D1,$04,$77,$80
            .byte $40,$E4,$4E,$44,$44,$44,$EE,$FF
            .byte $00,$80,$C0,$C0,$50,$98,$DC,$EE
            .byte $80,$C0,$E0,$B0,$90,$94,$D6,$77
            .byte $00,$7F,$7F,$63,$6B,$63,$7F,$7F
            .byte $B5,$D5,$E5,$B7,$93,$95,$D6,$77
            .byte $28,$AA,$0A,$00,$55,$55,$55,$FF
            .byte $80,$C0,$E0,$D0,$58,$9C,$DE,$CD
            .byte $11,$F8,$07,$08,$F0,$0F,$D0,$3F
            .byte $55,$99,$DD,$CC,$55,$99,$DD,$CC
            .byte $F0,$F0,$F0,$FF,$FF,$00,$00,$FF
            .byte $01,$03,$05,$0A,$54,$28,$30,$48
            .byte $FF,$FF,$F2,$24,$40,$02,$00,$88
            .byte $44,$AA,$11,$44,$11,$AA,$44,$11
            .byte $18,$3C,$7E,$99,$99,$7E,$42,$3C
            .byte $80,$40,$E0,$30,$A8,$D4,$22,$89
            .byte $70,$55,$00,$BB,$AA,$AA,$AA,$AA
            .byte $18,$24,$DB,$DB,$24,$18,$18,$18
            .byte $AA,$55,$22,$44,$22,$44,$22,$44
            .byte $44,$44,$FF,$11,$11,$FF,$55,$AA
            .byte $81,$81,$42,$24,$18,$18,$18,$18
            .byte $AA,$A8,$AD,$AF,$20,$AA,$55,$55
            .byte $11,$22,$88,$11,$44,$88,$22,$44
            .byte $28,$54,$AA,$54,$A8,$54,$14,$14
            .byte $C0,$40,$B0,$50,$AC,$54,$AB,$55
            .byte $FF,$FF,$FF,$AA,$55,$18,$18,$18
            .byte $04,$04,$AE,$AE,$A2,$42,$42,$EE
            .byte $FF,$FF,$00,$FF,$00,$00,$00,$00
            .byte $00,$15,$BF,$15,$00,$51,$FB,$51
            .byte $18,$5A,$BD,$19,$7E,$99,$24,$24
            .byte $F0,$AA,$78,$FF,$AA,$55,$AA,$55
            .byte $40,$A0,$F0,$F8,$FC,$7A,$25,$02
            .byte $FE,$01,$7D,$85,$81,$81,$81,$01
            .byte $42,$A5,$5A,$24,$24,$5A,$A5,$42
            .byte $20,$40,$A0,$18,$16,$09,$08,$04
            .byte $FF,$BD,$A5,$A5,$A5,$42,$00,$00
            .byte $09,$42,$90,$24,$09,$42,$90,$24
            .byte $E0,$40,$20,$10,$0E,$04,$02,$01
            .byte $F0,$55,$AA,$55,$55,$AA,$2A,$08
            .byte $FF,$FF,$55,$AA,$55,$AA,$00,$00
            .byte $92,$52,$4A,$49,$29,$25,$A4,$92
            .byte $20,$22,$14,$D8,$1B,$28,$44,$04
            .byte $70,$8C,$72,$FA,$EF,$AD,$25,$21
            .byte $5B,$A4,$A4,$55,$0A,$00,$00,$00
            .byte $59,$25,$26,$D9,$29,$D6,$54,$96
            .byte $49,$49,$49,$49,$49,$49,$49,$49
            .byte $A8,$25,$00,$25,$A8,$40,$DD,$40
            .byte $28,$54,$AA,$55,$AA,$28,$28,$28
            .byte $00,$0A,$BD,$FF,$00,$AA,$55,$AA
            .byte $10,$30,$38,$6C,$C6,$C6,$6C,$28
            .byte $FF,$AA,$55,$80,$5D,$94,$5D,$80
            .byte $22,$55,$00,$55,$88,$55,$00,$22
            .byte $7E,$81,$A5,$99,$99,$A5,$81,$7E
            .byte $03,$01,$0C,$04,$30,$10,$C0,$40
            .byte $FF,$A5,$52,$24,$42,$22,$40,$04
            .byte $00,$2A,$54,$00,$00,$A2,$45,$00
            .byte $42,$99,$66,$A1,$9F,$81,$66,$5A
            .byte $FF,$5A,$FF,$FF,$AA,$55,$AA,$55
            .byte $03,$03,$0D,$0E,$34,$39,$D2,$E4
            .byte $00,$A2,$00,$00,$00,$46,$00,$93
            .byte $4A,$29,$25,$49,$92,$A4,$25,$94
            .byte $80,$40,$20,$10,$08,$04,$02,$01
            .byte $01,$00,$07,$09,$13,$06,$7C,$98
            .byte $05,$38,$45,$82,$82,$82,$44,$38
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $44,$44,$BB,$44,$44,$44,$BB,$44
            .byte $01,$02,$04,$08,$00,$20,$40,$80
            .byte $70,$20,$20,$50,$50,$88,$88,$00
            .byte $FF,$FF,$5A,$99,$BD,$5A,$3C,$5A
            .byte $33,$11,$44,$CC,$33,$11,$44,$CC
            .byte $01,$02,$04,$10,$08,$20,$40,$80
            .byte $00,$40,$09,$64,$92,$2D,$96,$FF
            .byte $1F,$AA,$00,$55,$F8,$55,$00,$AA
            .byte $01,$02,$04,$28,$14,$20,$40,$80
            .byte $A5,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $1B,$04,$0A,$77,$87,$B1,$EA,$0A
            .byte $FF,$AA,$55,$FF,$00,$00,$00,$00
            .byte $69,$90,$90,$69,$96,$69,$69,$96
            .byte $03,$00,$0C,$00,$32,$07,$C2,$00
            .byte $18,$24,$A4,$CA,$95,$0A,$15,$0E
            .byte $FF,$6B,$B5,$00,$AD,$00,$4A,$00
            .byte $00,$51,$FB,$51,$00,$AA,$BF,$AA
            .byte $00,$00,$00,$00,$7C,$FE,$7F,$3E
            .byte $C0,$C0,$70,$70,$0C,$3C,$0B,$7F
            .byte $55,$55,$55,$55,$51,$5F,$40,$7F
            .byte $AD,$52,$29,$00,$01,$02,$00,$00
            .byte $D6,$6B,$66,$DB,$69,$6B,$D9,$6D
            .byte $15,$00,$AA,$55,$AA,$00,$15,$0A
            .byte $03,$08,$24,$10,$40,$20,$80,$80
            .byte $A5,$4A,$24,$02,$04,$00,$02,$00
            .byte $15,$55,$55,$24,$25,$55,$49,$22
            .byte $82,$41,$3E,$6A,$B7,$19,$25,$44
            .byte $80,$C0,$E0,$70,$38,$9C,$4E,$C7
            .byte $C6,$AA,$A8,$2A,$59,$24,$00,$00
            .byte $02,$14,$2E,$55,$2A,$45,$2A,$00
            .byte $55,$55,$5D,$D7,$70,$00,$00,$00
            .byte $22,$11,$44,$88,$22,$11,$44,$88
            .byte $BF,$BF,$BB,$B1,$BB,$BB,$BF,$BF
            .byte $66,$66,$66,$66,$66,$66,$66,$66
            .byte $BF,$15,$00,$51,$FB,$51,$00,$15
            .byte $FF,$3F,$03,$3F,$FF,$3F,$03,$3F
            .byte $FF,$FE,$FC,$F9,$F0,$E0,$C2,$90
            .byte $AA,$54,$AA,$54,$28,$10,$28,$10
            .byte $FE,$55,$2A,$AA,$55,$AA,$AA,$FF
            .byte $11,$44,$22,$88,$11,$44,$22,$88
            .byte $01,$04,$22,$08,$11,$44,$22,$88
            .byte $40,$90,$24,$48,$12,$24,$09,$02
            .byte $A5,$00,$A5,$00,$AA,$FF,$FF,$55
            .byte $F0,$F0,$78,$B8,$4C,$32,$0D,$03
            .byte $CE,$FF,$00,$E7,$00,$7F,$00,$FB
            .byte $EE,$11,$6C,$92,$11,$C9,$22,$9C
            .byte $24,$12,$44,$AA,$55,$AA,$5B,$FF
            .byte $F0,$AA,$3C,$AA,$55,$AA,$55,$AA
            .byte $00,$C0,$00,$30,$00,$0C,$00,$03
            .byte $B5,$AA,$55,$4A,$25,$42,$01,$02
            .byte $66,$B6,$6D,$BB,$66,$B9,$6D,$B6
            .byte $66,$AA,$5F,$BC,$63,$AC,$6B,$A4
            .byte $01,$02,$04,$09,$12,$24,$48,$90
            .byte $A5,$AA,$BD,$66,$66,$66,$66,$66
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$00
            .byte $AC,$53,$2A,$D0,$8A,$24,$22,$04
            .byte $F7,$EF,$00,$00,$00,$66,$66,$00
            .byte $DA,$D5,$7F,$2E,$11,$1F,$0A,$1B
            .byte $18,$18,$F8,$3C,$7E,$60,$60,$7E
            .byte $A0,$40,$80,$00,$00,$00,$00,$00
            .byte $51,$AA,$00,$AA,$15,$AA,$00,$AA
            .byte $8E,$F1,$B9,$D5,$EF,$D7,$BB,$FF
            .byte $00,$00,$18,$14,$1A,$35,$6F,$D8
            .byte $FF,$FF,$A5,$58,$24,$42,$20,$00
            .byte $10,$2A,$55,$A0,$10,$20,$10,$08
            .byte $18,$18,$14,$22,$22,$55,$51,$C9
            .byte $7E,$BF,$BF,$FF,$2C,$2C,$2C,$FF
            .byte $07,$55,$1F,$00,$FF,$FF,$66,$66
            .byte $81,$64,$CD,$0F,$61,$DA,$33,$AA
            .byte $FF,$FF,$00,$00,$00,$00,$00,$00
            .byte $66,$99,$24,$24,$77,$88,$8D,$72
            .byte $1C,$2A,$5F,$7D,$BE,$FE,$D2,$7C
            .byte $07,$55,$07,$00,$01,$C3,$66,$18
            .byte $EE,$AA,$38,$FF,$FF,$10,$10,$38
            .byte $02,$1D,$22,$5E,$BE,$7C,$38,$10
            .byte $FF,$22,$44,$88,$55,$22,$00,$00
            .byte $AA,$70,$AA,$6C,$B1,$5F,$AA,$55
            .byte $FF,$A5,$52,$24,$42,$22,$40,$04
            .byte $00,$2A,$54,$00,$00,$A2,$45,$00
            .byte $FF,$18,$E7,$10,$C7,$38,$00,$00
            .byte $0A,$41,$28,$05,$A0,$14,$82,$50
            .byte $24,$24,$24,$5A,$DB,$BD,$C3,$7E
            .byte $AA,$55,$AA,$55,$AA,$55,$AA,$55
            .byte $00,$19,$F9,$1F,$00,$19,$FF,$1F
            .byte $FF,$FF,$AA,$55,$AA,$55,$00,$00
            .byte $AA,$55,$55,$AA,$55,$AA,$AA,$55
            .byte $46,$89,$89,$36,$48,$83,$84,$68
            .byte $FF,$FE,$F8,$F0,$E0,$C0,$C0,$C0
            .byte $F6,$FF,$E7,$00,$00,$00,$00,$00
            .byte $74,$38,$66,$BF,$BF,$9D,$42,$3C
            .byte $5A,$FF,$22,$44,$22,$40,$00,$00
            .byte $55,$AA,$44,$22,$55,$AA,$44,$22
            .byte $04,$04,$04,$88,$50,$20,$10,$08
            .byte $E0,$AA,$00,$66,$66,$66,$66,$66
            .byte $18,$18,$18,$18,$18,$9B,$7F,$3B
            .byte $FF,$44,$44,$FF,$11,$11,$FF,$00
            .byte $15,$F1,$8F,$A8,$8A,$F8,$1F,$11
            .byte $FF,$00,$FF,$AA,$55,$FF,$00,$00
            .byte $FF,$FF,$FF,$AA,$55,$AA,$55,$00
            .byte $69,$D2,$A5,$4B,$96,$2D,$5A,$B4
            .byte $F0,$F0,$F0,$F0,$66,$66,$00,$00
            .byte $F9,$FF,$3F,$D5,$AA,$15,$4A,$84
            .byte $AC,$AE,$01,$AD,$AD,$02,$AC,$F8
            .byte $AA,$55,$82,$41,$82,$41,$82,$41
            .byte $A8,$55,$A8,$56,$CF,$2E,$26,$C1
            .byte $A5,$FF,$BD,$AA,$55,$BA,$45,$83
            .byte $FF,$AA,$FF,$A4,$A4,$A7,$E4,$3C
            .byte $E1,$84,$1E,$48,$E1,$84,$1E,$48
            .byte $18,$3C,$7E,$FF,$AF,$55,$3A,$10
            .byte $48,$39,$26,$DA,$5B,$64,$9C,$12
            .byte $00,$00,$00,$0A,$0D,$3C,$F2,$C0
            .byte $F7,$AA,$55,$EF,$00,$00,$00,$00
            .byte $30,$60,$C0,$80,$00,$00,$00,$00
            .byte $08,$10,$20,$21,$12,$AC,$40,$20
            .byte $FF,$FF,$BF,$FB,$FF,$EF,$FF,$FE
            .byte $BB,$FF,$6D,$DB,$7F,$D7,$7D,$AF
            .byte $01,$03,$07,$0F,$1E,$37,$7F,$FB
            .byte $4E,$AA,$00,$40,$40,$44,$AA,$FF
            .byte $04,$05,$0B,$0B,$17,$37,$6F,$00
            .byte $BB,$DD,$DD,$EE,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

L5700:      jmp L57B2              ; 4C B2 57

;--------------------------------------------
; Play note
;--------------------------------------------

L5703:
	jsr sound
	rts

            lda L07E3,Y            ; B9 E3 07
            cmp L0016              ; C5 16
            bcc L572B              ; 90 21
            sta L0016              ; 85 16

            lda L07E0,Y            ; B9 E0 07
            sta L0010              ; 85 10

            lda L07E1,Y            ; B9 E1 07
            sta L0012              ; 85 12
;            sta LFE06              ; 8D 06 FE

            lda L07E2,Y            ; B9 E2 07
            sta L0011              ; 85 11

            lda #$03               ; A9 03
            sta L0014              ; 85 14

;            lda #$22               ; Set sound and MODE4
;            sta LFE07              ; 8D 07 FE

            lda #$20               ; A9 20
            sta L0015              ; 85 15

L572B:      rts                    ; 60

;--------------------------------------------
;--------------------------------------------

L572C:      ldy L0010              ; A4 10
            bne L5735              ; D0 05
            dec L0011              ; C6 11
            beq L5758              ; F0 24
            rts                    ; 60

L5735:      bpl L574E              ; 10 17
            lda L0015              ; A5 15
            eor #$02               ; 49 02
            sta L0015              ; 85 15
            sta LFE07              ; Toggle sound
            inc L0012              ; E6 12
            inc L0012              ; E6 12
            lda L0012              ; A5 12
            sta LFE06              ; 8D 06 FE
            dec L0011              ; C6 11
            beq L5758              ; F0 0B
            rts                    ; 60

L574E:      dec L0011              ; C6 11
            bne L5769              ; D0 17
            inc L0010              ; E6 10
            dec L0014              ; C6 14
            bne L5764              ; D0 0C

L5758:      lda #$20               ; MODE 4, no sound
            sta LFE07              ; 8D 07 FE
            lda #$00               ; A9 00
            sta L0016              ; 85 16
            sta L0010              ; 85 10
            rts                    ; 60

L5764:      lda L0790,Y            ; B9 90 07
            sta L0011              ; 85 11

L5769:      lda L0013              ; A5 13
            clc                    ; 18
            adc L0793,Y            ; 79 93 07
            sta L0013              ; 85 13
            lda L0012              ; A5 12
            adc L0796,Y            ; 79 96 07
            sta L0012              ; 85 12
            sta LFE06              ; 8D 06 FE
            rts                    ; 60

;--------------------------------------------
; New IRQ vector
;--------------------------------------------

L577C:      lda L00FC              ; A5 FC
            pha                    ; 48
            txa                    ; 8A
            pha                    ; 48
            tya                    ; 98
            pha                    ; 48

            ldx $00                ; A6 00
            lda LFE00              ; Wait for sync
            and #$08               ; 29 08
            beq L579A              ; F0 0E

            inc L0004              ; E6 04

            ldx #$03               ; Delay
            ldy #$00               ; A0 00
L5792:      dey                    ; 88
            bne L5792              ; D0 FD
            dex                    ; CA
            bne L5792              ; D0 FA

            ldx L0001              ; A6 01

L579A:      stx LFE09              ; Set colours
            txa                    ; 8A
            asl A                  ; 0A
            sta LFE08              ; 8D 08 FE

            jsr L572C              ; 20 2C 57

            lda #$F0               ; Clear IRQ's
            sta LFE05              ; 8D 05 FE

            pla                    ; 68
            tay                    ; A8
            pla                    ; 68
            tax                    ; AA
            pla                    ; 68
            sta L00FC              ; 85 FC
            rti                    ; 40

;--------------------------------------------
; Set new IRQ vector
;--------------------------------------------

L57B2:      sei                    ; 78
            lda IRQ1V              ; AD 04 02
            sta L57CB              ; 8D CB 57
            lda IRQ1V+1              ; AD 05 02
            sta L57CC              ; 8D CC 57
            lda #<L577C               ; A9 7C
            sta IRQ1V              ; 8D 04 02
            lda #>L577C               ; A9 57
            sta IRQ1V+1              ; 8D 05 02
            cli                    ; 58
            rts                    ; 60

L57CB:      .byte $47
L57CC:      .byte $DB

;--------------------------------------------

L57CD:      .byte $EF,$BF,$F7,$71,$8F,$A4,$27
            .byte $65,$A6,$53,$CE,$3E,$FC,$1D,$F8
            .byte $0E,$F0,$03,$C0,$02,$C0,$0B,$F0
            .byte $1C,$38,$38,$1C,$72,$4E,$5F,$FA
            .byte $AA,$77,$5D,$BB,$B3,$CF,$C5,$A7
            .byte $64,$26,$51,$8C,$3F,$FC,$1A,$F8
            .byte $0D,$F0,$03,$00

