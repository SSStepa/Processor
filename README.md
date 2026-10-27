# SPU and assembler project
This project is third tasks of MIPT DREC 1 course program for advanced group Program is taught by Dedinsky Ilya Rudolfovich(@ded32_ru)
![Bad1](./BadScreen.jpg)
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
    basic assembler, which takes asm file written according to my standard (see Asm code description) and compiles it into bin code(core info in asm work pattern)
    >[!NOTE] 
    >we assume that we are working with doubles.
    Gives ability to use:
        - choose file to compile and file compile to
        - jumps
        - function calls
        - registers
        - labels
        - comments
    (for more info see Asm Code description)
    Optimizations: 
        - reading from file at one(only one request to in file, than it copied to the string and work continues only with it)
        - outing to files at one (also only one request to file, all work of program is connected with string)
        - works on double stack for data
        - has second stack for calls data
        - works with 4 registers
        - works with RAM(See RAM)

    result of work - .bin file optimized that all commands and variables names take 1 byte and other args 8 bytes(double size)
- processor
    - program which takes compiled file and executes it. 
    - gives ability choose file to compile and file compile to.
    - main data storage source is stack (stack processor)
    - Optimizations:
        - reading from file at one (only one request to in file, than it copied to the string and work continues only with it)
        - regards commands as bin fields (see One byte command optimization)

## Stacks
stacks for processor are used
taken from my project. see [Stack](https://github.com/SSStepa/My_Stack) for more


## Asm code description
![Bad2](./BadApple2.jpg)

Registers to use: `AX BX CX DX`
Register is variable which stares its value until changed

Labels may be used as words:
```
(valid labels)
:1
:next
:hiMyBelovedFriend
```


Valid commands:

| command | explanation |                                   
|:--------------:|:----------------------------------------------------------------------------------------------------:|
|HLT             | ends work of processor                                                                               |
|PUSH 11.1       | pushes const into data stack                                                                         |
|PUSH AX         | pushes value of AX register into data stack                                                          |
|PUSH AX + 11.1  | pushes value of AX + const into data stack                                                           |
|PUSH [1]        | takes info from RAM[1] and pushes into data stack                                                    |
|PUSH [AX]       | pushes info from RAM[AX/value] into data stack                                                       |
|PUSH [1+AX]     | pushes info from RAM[AX.value + 1] into data stack                                                   |
|POP AX          | pops value from stack into AX                                                                        |
|POP [1]         | pops value from stack into RAM[1]                                                                    |
|POP [AX]        | pops value from stack into RAM[AX.value]                                                             |
|POP [1+AX]      | pops value from stack into RAM[AX.value + 1]                                                         |
|OUT_D           | pops element from stack and prints it to stdout                                                      |
|OUT_C           | pops element from stack and prints it to stdout as char                                              |
|IN_D            | takes one char form stdin and writes it into data stack as double                                    |
||Takes two elements from stack and:                                                                                    |
|ADD             | finds their sum                                                                                      |
|SUB             | finds their difference                                                                               |
|MULT            | finds their multiplication                                                                           |
|DIV             | divides one by other                                                                                 |
||And writes result to stack                                                                                            |
||Takes element from stack and finds it's:                                                                              |
|SQRT            | square root                                                                                          |
|SIN             | sin                                                                                                  |
|COS             | cos                                                                                                  |
|TAN             | tan                                                                                                  |
||and writes result into stack                                                                                          |
|JMP 5           | jumps into place in code with index 5(remember - one command one element, one argument - 8 elements) |
|JMP :1          | jumps to place of label declaration. WARNING do not redeclare labels                                 |
||Commands doing JMP if:                                                                                                |
||pop a and b from stack                                                                                                |
|JA              | a < b                                                                                                |
|JB              | a > b                                                                                                |
|JE              | a == b                                                                                               |
|JNE             | a != b                                                                                               |
|JAE             | a <= b                                                                                               |
|JBE             | a >= b                                                                                               |
|JT              | jumps if today is tuesday                                                                            |
|CALL ...        | calls function with label or index(as jmp). remembers place of call into calls stack                 |
|RET             | pops element from calls stack and puts IP to this value                                              |
|DRAW            | prints all RAM to stdout                                                                             |

## Asm work pattern

1) takes asm code in one request to operative (reads file as one line to memory). That helps to make work of assembler faster
2) parses it into lines (to better check of codestyle and ability to use commentaries)
3) goes trough each line and compiles it into:
    - asm listing code (commands code with argument in numbers)
    - bin file for proc. WARNING open in hex redactors. Used by processor to execute
4) fills labels array;
5) deletes all first run results except labels array
6) fills asm listing and bin lines with correct labels (one more time doing step 3 but now all labels are correct)
7) outs both lines into files at once (each file we write in one request to memory, which makes it faster);

Assembler uses DSL:

### Asm DSL:

Making requests to out text and bin files easier to get:
``` c

#define BIN_STR (data -> binOpcode).str // - attach to bin text line (line with all already compiled code) 
#define BIN_LEN (data -> binOpcode).len // -  attack to index of first not fulled element in binary code line

#define TEXT_STR (data -> textOpcode).str // - attach to assembler listing line (line with all already compiled code)
#define TEXT_LEN (data -> textOpcode).len // -  attach to index if first not fulled element in assembler listing line

```

### Labels
You are able to use labels instead of direct indexes of code lines
label syntax is `:[name]` where name may be any characters line with no whitespaces 

>[!WARNING]
>Do not redeclare labels. It will stop work of problem with error

labels also may be numbers line `:1` it is regarded as work "1"

## Proc work pattern
1) reads bin info in one request to file. that makes program faster. all bin code is stored as line in dynamic memory
2) starts reading from line of chars (each command is one byte, that is same with sizeof(char))
3) command is regarded as one byte where mem:1-reg:1-konst:1-code:5 division is used (see One byte command optimization)
4) argument may take one (register name) or 8 (konst double) bytes
5) doesn't account alignment (windows architecture)
6) accepts that program finishes with HLT. or Undefined behaver may occur

## One byte command optimization
My processor has only 24 commands (to see them look ams code description). So all them may be packed in 5 bytes(2^5 different variants). so three first bytes are used to understand the type of argument:
|1|2|3|4 5 6 7 8|
|-|-|-|-----|
|wark with memory|work with register|work with const| command name|

arguments of different types may be used (for example you can combine pushing to memory with index from register. See Examples or Ams work pattern for more)

## In and Out files 

Both proc and asm programs give you ability to insert input and output files on your own. use flags wile running:
```
-i [name] - input file name 
-o [name] - output file name
```

## RAM
is double array with size which is stored in ```./proc/proc.h/:RAM_INIT_SIZE```
to write and get info from [] syntax is used:
```
PUSH[0]
POP[0]
```
Pushes info to memory with index 0 and takes this value back. Info from memory doesn't disappear

## Examples of usage:

Print number:
``` asm
PUSH 67
OUT_D
HLT
```
Printf latter A
``` asm
PUSH 65
OUT_C
HLT
```

Sum elements and out:
``` asm
IN_D
IN_D
ADD
OUT
HLT

```

on the git hub are my realizations of programs:
- circle.asm - draw circle using RAM
- fact.asm - recursively calculating fact of number
- sqr - solves square equation

- bad apple - gives you perfect badApple video to watch
for creating and parsing bad apple script BadConvert.cpp used (see BadConvert)


## BadConvert

this program:
 - uses analog of RAM to use delta codding (we store info about how our RAM looks now and change only elements which are changed in this frame)
 - RAM size is same with frame size (one frame - one print of RAM)
 - opens each file in which one frame info is stared 
 - takes it as one line
 - analyses it according to delta coding (change RAM elements only if they are changed)
 - writes out to file this info and command to draw frame
 - closes file and opens next


