LDX #$01

LDA #$80
STA $03

LDA #$06
STA $80

LDA #$01
STA $81

LDY #$0c
STY $82

INC $03
INC $03

ADC ($02,X)