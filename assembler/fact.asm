IN

CALL :factorial

PUSH BX

OUT
HLT



:factorial
    POP AX

    PUSH 1
    PUSH AX
    JBE :base


    PUSH AX
    PUSH -1
    ADD

    CALL :factorial
    PUSH BX

    PUSH AX
    PUSH 1
    ADD
    POP AX
    
    PUSH AX
    MULT

    POP BX
    RET

    :base
        PUSH 1
        POP BX
        RET