LDA #$02
STA $03

ldy #$01

STA ($02),Y

ADC ($02),Y
STA $202

lda #$02
cmp ($02),Y
