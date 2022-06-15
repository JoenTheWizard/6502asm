LABEL:
    LDA #$02
    STA $0000

    LDA #$05
    STA $0001

    ADC $0000
    STA $0003

    LDA $0001
    ADC $0003
    STA $0007
