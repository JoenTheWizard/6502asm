LABEL:
    LDA #$06
    STA $0005

    LDA #$05
    STA $0006

    LDY #$01
    ADC $0004,Y
    STA $0007

    LDX #$03
    INC $0200,X

    DEC $0205,X
    
    LDX #$6a
    INX
    INX
    STX $0008

    LDA #$30
    STA $10