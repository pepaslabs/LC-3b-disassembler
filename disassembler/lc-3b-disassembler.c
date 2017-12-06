// A disassembler for the LC-3b.
// Copyright 2017 Jason Pepas
// Released under the terms of the MIT License.
// See https://opensource.org/licenses/MIT

// This code is best understood in the context of the ISA documentation (e.g.
// many of the variable names are taken from the documentation).
// See https://github.com/pepaslabs/LC-3b/blob/master/doc/LC-3b%20ISA.pdf

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef union {
    uint16_t i;
    uint8_t bytes[2];
} instruction_t;

void do_add(instruction_t i) {
    uint8_t dr = i.bytes[1]>>1 & 0x7;
    uint8_t sr1 = i.i>>6 & 0x7;
    bool bit5 = i.bytes[0] & 1<<5;
    if (bit5) {
        int8_t imm5 = (i.bytes[0] & 0x1F) - 16;
        printf("ADD R%u, R%u, #%i ;\n", dr, sr1, imm5);
    } else {
        uint8_t sr2 = i.bytes[0] & 0x7;
        printf("ADD R%u, R%u, R%u ;\n", dr, sr1, sr2);
    }
    return;
}

void do_and(instruction_t i) {
    uint8_t dr = i.bytes[1]>>1 & 0x7;
    uint8_t sr1 = i.i>>6 & 0x7;
    bool bit5 = i.bytes[0] & 1<<5;
    if (bit5) {
        int8_t imm5 = (i.bytes[0] & 0x1F) - 16;
        printf("AND R%u, R%u, #%i ;\n", dr, sr1, imm5);
    } else {
        uint8_t sr2 = i.bytes[0] & 0x7;
        printf("AND R%u, R%u, R%u ;\n", dr, sr1, sr2);
    }
    return;
}

void do_br(instruction_t i) {
    bool n = i.bytes[1] & 1<<3;
    bool z = i.bytes[1] & 1<<2;
    bool p = i.bytes[1] & 1<<1;
    int16_t offset9 = (i.i & 0x1FF) - 256;
    if (n && z && p) {
        printf("BRnzp %i ;\n", offset9);
    } else if (n && z) {
        printf("BRnz %i ;\n", offset9);
    } else if (z && p) {
        printf("BRzp %i ;\n", offset9);
    } else if (n && p) {
        printf("BRnp %i ;\n", offset9);
    } else if (n) {
        printf("BRn %i ;\n", offset9);
    } else if (z) {
        printf("BRz %i ;\n", offset9);
    } else if (p) {
        printf("BRp %i ;\n", offset9);
    } else {
        printf("BR %i ;\n", offset9);
    }
    return;
}

void do_jmp(instruction_t i) {
    uint8_t baseR = i.i>>6 & 0x7;
    if (baseR == 7) {
        printf("RET ;\n");
    } else {
        printf("JMP R%u ;\n", baseR);
    }
    return;
}

void do_jsr(instruction_t i) {
    bool bit11 = i.bytes[1] & 1<<3;
    if (bit11) {
        int16_t offset11 = (i.i & 0x7FF) - 1024;
        printf("JSR %i ;\n", offset11);
    } else {
        uint8_t baseR = i.i>>6 & 0x7;
        printf("JSRR R%u ;\n", baseR);
    }
    return;
}

void do_ldb(instruction_t i) {
    uint8_t dr = i.bytes[1]>>1 & 0x7;
    uint8_t baseR = i.i>>6 & 0x7;
    int8_t offset6 = (i.bytes[0] & 0x3F) - 32;
    printf("LDB R%u, R%u, #%i ;\n", dr, baseR, offset6);
    return;
}

void do_ldi(instruction_t i) {
    uint8_t dr = i.bytes[1]>>1 & 0x7;
    uint8_t baseR = i.i>>6 & 0x7;
    int8_t offset6 = (i.bytes[0] & 0x3F) - 32;
    printf("LDI R%u, R%u, #%i ;\n", dr, baseR, offset6);
    return;
}

void do_ldr(instruction_t i) {
    uint8_t dr = i.bytes[1]>>1 & 0x7;
    uint8_t baseR = i.i>>6 & 0x7;
    int8_t offset6 = (i.bytes[0] & 0x3F) - 32;
    printf("LDR R%u, R%u, #%i ;\n", dr, baseR, offset6);
    return;
}

void do_lea(instruction_t i) {
    uint8_t dr = i.bytes[1]>>1 & 0x7;
    int16_t offset9 = (i.i & 0x1FF) - 256;
    printf("LEA R%u, %i ;\n", dr, offset9);
    return;
}

void do_not(instruction_t i) {
    uint8_t dr = i.bytes[1]>>1 & 0x7;
    uint8_t sr = i.i>>6 & 0x7;
    printf("NOT R%u, R%u ;\n", dr, sr);
    return;
}

void do_rti(instruction_t i) {
    printf("RTI ;\n");
    return;
}

void do_shf(instruction_t i) {
    uint8_t dr = i.bytes[1]>>1 & 0x7;
    uint8_t sr = i.i>>6 & 0x7;
    bool a = i.bytes[0] & 1<<5;
    bool d = i.bytes[0] & 1<<4;
    int8_t imm4 = (i.bytes[0] & 0xF) - 8;
    if (d) {
        if (a) {
            printf("RSHFA R%u, R%u, #%i\n", dr, sr, imm4);
        } else {
            printf("RSHFL R%u, R%u, #%i\n", dr, sr, imm4);
        }
    } else {
        printf("LSHF R%u, R%u, #%i\n", dr, sr, imm4);
    }
    return;
}

void do_stb(instruction_t i) {
    uint8_t sr = i.bytes[1]>>1 & 0x7;
    uint8_t baseR = i.i>>6 & 0x7;
    int8_t offset6 = (i.bytes[0] & 0x3F) - 32;
    printf("STB R%u, R%u, #%i ;\n", sr, baseR, offset6);
    return;
}

void do_sti(instruction_t i) {
    uint8_t sr = i.bytes[1]>>1 & 0x7;
    uint8_t baseR = i.i>>6 & 0x7;
    int8_t offset6 = (i.bytes[0] & 0x3F) - 32;
    printf("STI R%u, R%u, #%i ;\n", sr, baseR, offset6);
    return;
}

void do_str(instruction_t i) {
    uint8_t sr = i.bytes[1]>>1 & 0x7;
    uint8_t baseR = i.i>>6 & 0x7;
    int8_t offset6 = (i.bytes[0] & 0x3F) - 32;
    printf("STR R%u, R%u, #%i ;\n", sr, baseR, offset6);
    return;
}

void do_trap(instruction_t i) {
    uint8_t offset8 = i.bytes[0];
    switch (offset8) {
    case 0x20:
        printf("TRAP 0x%x ; GETC\n", offset8);
        break;
    case 0x21:
        printf("TRAP 0x%x ; OUT\n", offset8);
        break;
    case 0x22:
        printf("TRAP 0x%x ; PUTS\n", offset8);
        break;
    case 0x23:
        printf("TRAP 0x%x ; IN\n", offset8);
        break;
    case 0x25:
        printf("TRAP 0x%x ; HALT\n", offset8);
        break;
    default:
        printf("TRAP 0x%x ;\n", offset8);
        break;
    }
    return;
}

#define ADD  0x1 // 0b0001
#define AND  0x5 // 0b0101
#define BR   0x0 // 0b0000
#define JMP  0xC // 0b1100
#define JSR  0x4 // 0b0100
#define LDB  0x2 // 0b0010
#define LDI  0xA // 0b1010
#define LDR  0x6 // 0b0110
#define LEA  0xE // 0b1110
#define NOT  0x9 // 0b1001
#define RTI  0x8 // 0b1000
#define STB  0x3 // 0b0011
#define STI  0xB // 0b1011
#define STR  0x7 // 0b0111
#define TRAP 0xF // 0b1111
#define SHF  0xD // 0b1101

void do_instruction(instruction_t i) {
    uint8_t opcode = i.bytes[1] >> 4;
    switch (opcode) {
    case ADD:
        do_add(i);
        break;
    case AND:
        do_and(i);
        break;
    case BR:
        do_br(i);
        break;
    case JMP:
        do_jmp(i);
        break;
    case JSR:
        do_jsr(i);
        break;
    case LDB:
        do_ldb(i);
        break;
    case LDI:
        do_ldi(i);
        break;
    case LDR:
        do_ldr(i);
        break;
    case LEA:
        do_lea(i);
        break;
    case NOT:
        do_not(i);
        break;
    case RTI:
        do_rti(i);
        break;
    case STB:
        do_stb(i);
        break;
    case STI:
        do_sti(i);
        break;
    case STR:
        do_str(i);
        break;
    case TRAP:
        do_trap(i);
        break;
    case SHF:
        do_shf(i);
        break;
    default:
        fprintf(stderr, "Error: undefined behavior.\n");
        exit(4);
    }
}

int main(int argc, char **argv) {
    // Read two bytes of binary LC-3b machine language at a time, decode
    // that into assembly and print out the assembly.
    while(1) {
        instruction_t i;
        ssize_t ret;
        ret = read(STDIN_FILENO, i.bytes, 2);
        switch (ret) {
        case -1: // error
            perror(NULL);
            exit(1);
        case 0: // EOF
            exit(0);
        case 1:
            fprintf(stderr, "Error: input buffer underrun.\n");
            exit(2);
        case 2:
            do_instruction(i);
            break;
        default:
            fprintf(stderr, "Error: undefined behavior.\n");
            exit(3);
        }
    }
}
