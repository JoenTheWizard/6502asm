START:
    LDA #$01
    STA $03

    LDA #$09
    STA $04

    LDA $03

    LDX $0004
    LDY $0003

    STA $0080
    STA $0f

    STX $ab

    LDA #$01
    STA $0200
    LDA #$05
    STA $0201
    LDA #$08
    STA $0202
COOL:
    INY
    STX $10, Y
    DEC $0001, X
    ADC $01, X
    ASL A
    STA $0203

    LDA $0201
    STA $0204

    LDA $0008, Y
    LDY $0610
    NOP
