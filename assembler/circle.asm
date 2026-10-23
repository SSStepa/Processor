PUSH 0
POP AX ;x
PUSH 0
POP BX ;y

:main
    ; count place in RAM
    PUSH BX
    PUSH 100
    MULT
    PUSH AX
    ADD
    POP CX
    PUSH 32
    POP [CX]

    ; put pixel if needed
    PUSH AX
    PUSH 50
    SUB
    PUSH AX
    PUSH 50
    SUB
    MULT
    PUSH BX
    PUSH 50
    SUB
    PUSH BX
    PUSH 50
    SUB
    MULT
    PUSH 6 ; 
    MULT
    ADD
    PUSH 400
    JA :putPixel
    :back

    PUSH 1
    PUSH AX
    ADD
    POP AX
    PUSH AX
    PUSH 100
    JE :newLine
    :backLine

    PUSH 9999
    PUSH CX
    JB :main

    DRAW
    HLT


  
:putPixel
    PUSH 42
    POP [CX]
    JMP :back

:newLine
    PUSH 10
    POP [CX]
    PUSH 1
    PUSH BX
    ADD
    POP BX
    PUSH 0
    POP AX
    JMP :backLine
