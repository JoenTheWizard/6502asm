LBL:
    LDA #$01
    STA $03

    LDA #$09
    STA $04

    LDA $03

    LDX $0004
    LDY $0003

    STA $0080
    STA $0f