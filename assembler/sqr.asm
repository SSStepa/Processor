IN
POP AX
IN 
POP BX
IN
POP CX

PUSH 0
PUSH AX
JE :aIsZero

PUSH 0
PUSH BX
JE :bIsZero

PUSH 0
PUSH CX
JE :cIsZero

PUSH BX
PUSH BX
MULT
PUSH 4
PUSH AX
PUSH CX
MULT
MULT
SUB
POP DX

PUSH 0
PUSH DX
JE :dIsZero

PUSH 0
PUSH DX
JA :TwoRoots

JMP :NoRoots


:aIsZero
    PUSH 0
    PUSH BX
    JE :abAreZero

    PUSH 0
    PUSH CX
    SUB
    PUSH BX
    DIV
    OUT
    HLT

    :abAreZero
        PUSH 0
        PUSH CX
        JE :abcAreZero

        JMP :NoRoots

        :abcAreZero
            PUSH inf
            OUT
            HLT

:bIsZero
   PUSH 0
   PUSH CX
   SUB
   PUSH AX
   DIV
   POP DX
   PUSH 0
   PUSH DX
   JB :NoRoots

   PUSH DX
   SQRT
   POP DX
   PUSH DX
   OUT
   PUSH 0
   PUSH DX
   SUB
   OUT
   HLT

:cIsZero
    PUSH 0
    OUT

    PUSH 0
    PUSH BX
    SUB
    PUSH AX
    DIV
    OUT
    HLT
    

:dIsZero
    PUSH 0
    PUSH BX
    SUB
    PUSH 2
    DIV
    PUSH AX
    DIV
    OUT
    HLT

:TwoRoots
    PUSH 0
    PUSH BX
    SUB
    PUSH DX
    SQRT
    ADD
    PUSH 2
    DIV
    PUSH AX
    DIV
    OUT

    PUSH 0
    PUSH BX
    SUB
    PUSH DX
    SQRT
    SUB
    PUSH 2
    DIV
    PUSH AX
    DIV
    OUT
    HLT
    
:NoRoots
    PUSH nan
    OUT
    HLT