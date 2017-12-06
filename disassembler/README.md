This is an [LC-3b](https://en.wikipedia.org/wiki/LC-3) disassembler.

This code is best understood in the context of the [ISA documentation](../doc/LC-3b%20ISA.pdf) (e.g. many variable names correspond to the ISA doc).

## Compilation

```
$ gcc lc-3b-disassembler.c
```

## Demo

```
$ cat /dev/urandom | ./a.out
```

Example output:

```
RTI ;
BRz -21 ;
STI R1, R1, #-8 ;
STB R0, R0, #-20 ;
RTI ;
STB R2, R7, #-23 ;
LDR R3, R2, #18 ;
STB R4, R7, #-2 ;
LDB R5, R7, #17 ;
STI R3, R3, #17 ;
JSR -746 ;
LDR R7, R6, #17 ;
RTI ;
AND R3, R7, R3 ;
STR R0, R2, #-28 ;
LDI R6, R1, #10 ;
TRAP 0xdd ;
```
