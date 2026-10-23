# SPU and assembler project
This project is third tasks of MIPT DREC 1 course program for advanced group Program is taught by Dedinsky Ilya Rudolfovich(@ded32_ru)
## Description 
### Structure of project
```
Processor/
├── proc/
│   ├─ main.cpp
│   ├─ proc.cpp
│   └─ proc.h
│
├── assembler/
│   ├─ assembler.cpp
│   └─ assembler.h
│
├── basic.h
├── basic.cpp
├── stack.h
├── stack.cpp
├── README.TXT
└── .gitignore
```

Project consists of two main parts:
- assembler
    basic assembler, which takes asm file written according to my standard (see Asm code description) and compiles it into bin code
    [!NOTE] 
    we assume that we are working with doubles.
    Gives ability to use:
        - jumps
        - function calls
        - registers
        - labels
        - comments
    (for more info see Asm Code description)
    Optimizations: 
        - reading from file at one
        - outing to files at one
        - works on double stack for data
        - has second stack for calls data
        - works with 4 registers
        - works with RAM

    result of work - .bin file optimized that all commands and variables names take 1 byte and other args 8 bytes(double size)
- processor
    program which takes compiled file and executes it. 
    Optimizations:
        - reading from file at one 
        - regards commands as bin fields (see Code executing)

## Stacks
stacks for process are used
taken from my project. see [Staks](https://github.com/SSStepa/My_Stack) for more
## Asm code description

Registers to use: `AX BX CX DX`

Labels may be used as words:
```
(valid labels)
:1
:next
:hiMyBelovedFriend
```


Valid commands:
``` ams
HLT             - ends work of processor

PUSH 11.1       - pushes const into data stack
PUSH AX         - pushes value of AX register into data stack
PUSH AX + 11.1  - pushes value of AX + const into data stack
PUSH [1]        - takes info from RAM[1] and pushes into data stack
PUSH [AX]       - pushes info from RAM[AX/value] into data stack
PUSH [1+AX]     - pushes info from RAM[AX.value + 1] into data stack

POP AX          - pops value from stack into AX
POP [1]         - pops value from stack into RAM[1]
POP [AX]        - pops value from stack into RAM[AX.value]
POP [1+AX]      - pops value from stack into RAM[AX.value + 1]

OUT_D           - pops element from stack and prints it to stdout
OUT_C           - pops element from stack and prints it to stdout as char
IN_D            - takes one char form stdin and writes it into data stack as double

Takes two elements from stack and:
ADD             - finds their sum
SUB             - finds their difference
MULT            - finds their multiplication 
DIV             - divides one by other
And writes result to stack

Takes element from stack and finds it's:
SQRT            - square root
SIN             - sin
COS             - cos
TAN             - tan
and writes result into stack

JMP 5           - jumps into place in code with index 5(remember - one command one element, one argument - 8 elements)
JMP :1          - jumps to place of label declaration. WARNING do not redeclare labels

Commands doing JMP if:
pop a and b from stack
JA              - a < b
JB              - a > b
JE              - a == b
JNE             - a != b
JAE             - a <= b
JBE             - a >= b
JT              - jumps if today is tuesday

CALL ...        - calls function with label or index(as jmp). remembers place of call into calls stack
RET             - pops element from calls stack and puts IP to this value


DRAW            - prints all RAM to stdout
```

## Asm work pattern

1) takes file full
2) parses it into lines
3) goes trough each line and compiles it into:
    - asm listing code (commands code with argument in normal numbers)
    - bin file for proc. WARNING open in hex redactors. 
4) fills labels array;
5) deletes all first run results except labels array
6) fills asm listing and bin lines with correct labels
7) outs both lines into files at once

