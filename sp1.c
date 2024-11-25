#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// OpCodes
#define LOADMEM 0
#define LOADVAL 1
#define STORE 2
#define ADD 3
#define SUB 4
#define MUL 5
#define DIV 6
#define INC 7
#define DEC 8
#define AND 9
#define OR 10
#define NOT 11
#define JMP 12
#define JZ 13
#define JNZ 14
#define JG 15
#define JL 16
#define JGE 17
#define JLE 18
#define HLT 19

// Flags
#define ZERO 1
#define CARRY 2
#define OVFLOW 4

typedef struct inst {
    unsigned char opcode;
    unsigned char operando;
} inst;

void exibirBinario(inst *emulator, int tamanho) {
    printf("\nInstruções em binário:\n\n");
    printf("+—————————————————————+\n");
    printf("|  Opcode  |   Addr   |\n");
    printf("├----------+----------┤\n");
    for (int i = 0; i < tamanho; i++) {
        printf("| ");
        for (int j = 7; j >= 0; j--) {
            printf("%d", (emulator[i].opcode >> j) & 1);
        }
        printf(" | ");
        for (int j = 7; j >= 0; j--) {
            printf("%d", (emulator[i].operando >> j) & 1);
        }
        printf(" |");
        printf("\n├----------+----------┤\n");
    }
    printf("+—————————————————————+");
}

void exibirInstrucoes(inst *emulator, int tamanho) {
    const char *opcodes[] = {
        "LOADMEM", "LOADVAL", "STORE", "ADD", "SUB", "MUL", "DIV", "INC",
        "DEC", "AND", "OR", "NOT", "JMP", "JZ", "JNZ", "JG",
        "JL", "JGE", "JLE", "HLT"
    };

    printf("\nInstruções: \n\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%s %d\n", opcodes[emulator[i].opcode], emulator[i].operando);
    }
}

int main() {
    FILE *arquivo = fopen("teste.exe", "rb");
    if (!arquivo) {
        printf("Arquivo não encontrado. Execute 'createTest.c'.\n");
        return 1;
    }

    unsigned char acc = 0;
    unsigned char pc = 0;
    unsigned char stat = 0;
    unsigned char aux[2];
    unsigned char mem[256] = {0};
    inst emulator[256];
    inst historico[256];
    int historico_tamanho = 0;

    while (fread(aux, sizeof(unsigned char), 2, arquivo)) {
        emulator[pc].opcode = aux[0];
        emulator[pc].operando = aux[1];
        pc++;
    }
    fclose(arquivo);

    pc = 0;

    while (pc < 256 && emulator[pc].opcode != HLT) {
        historico[historico_tamanho++] = emulator[pc];

        switch (emulator[pc].opcode) {
            case LOADMEM:
                acc = mem[emulator[pc].operando];
                pc++;
                break;

            case LOADVAL:
                acc = emulator[pc].operando;
                pc++;
                break;

            case STORE:
                mem[emulator[pc].operando] = acc;
                pc++;
                break;

            case ADD:
                if (acc + mem[emulator[pc].operando] > 255)
                    stat |= CARRY;
                acc += mem[emulator[pc].operando];
                pc++;
                break;

            case SUB:
                acc -= mem[emulator[pc].operando];
                pc++;
                break;

            case MUL:
                if (acc * mem[emulator[pc].operando] > 255)
                    stat |= OVFLOW;
                acc *= mem[emulator[pc].operando];
                pc++;
                break;

            case DIV:
                if (mem[emulator[pc].operando] == 0) {
                    printf("Erro: divisão por zero.\n");
                    return 1;
                }
                acc /= mem[emulator[pc].operando];
                pc++;
                break;

            case INC:
                acc++;
                pc++;
                break;

            case DEC:
                acc--;
                pc++;
                break;

            case AND:
                acc &= mem[emulator[pc].operando];
                pc++;
                break;

            case OR:
                acc |= mem[emulator[pc].operando];
                pc++;
                break;

            case NOT:
                acc = ~acc;
                pc++;
                break;

            case JMP:
                pc = emulator[pc].operando / 2;
                break;

            case JZ:
                pc = (acc == 0) ? emulator[pc].operando / 2 : pc + 1;
                break;

            case JNZ:
                pc = (acc != 0) ? emulator[pc].operando / 2 : pc + 1;
                break;

            case JG:
                pc = (acc > mem[emulator[pc].operando]) ? emulator[pc].operando / 2 : pc + 1;
                break;

            case JL:
                pc = (acc < mem[emulator[pc].operando]) ? emulator[pc].operando / 2 : pc + 1;
                break;

            case JGE:
                pc = (acc >= mem[emulator[pc].operando]) ? emulator[pc].operando / 2 : pc + 1;
                break;

            case JLE:
                pc = (acc <= mem[emulator[pc].operando]) ? emulator[pc].operando / 2 : pc + 1;
                break;

            case HLT:
                printf("ACC: %d - STAT: %d\n", acc, stat);
                break;

            default:
                printf("Erro: instrução inválida no endereço %d.\n", pc);
                return 1;
        }

        if (acc == 0)
            stat |= ZERO;
        else
            stat &= ~ZERO;
    }

    printf("\n\nACC: %d\nSTAT: %d\n", acc, stat);
    char resposta;
    printf("\n'S' pra exibir as instruções executadas.\n\n");
    scanf(" %c", &resposta);

    if (resposta == 's' || resposta == 'S') {
        exibirInstrucoes(historico, historico_tamanho);
        printf("\n");
        exibirBinario(historico, historico_tamanho);
        printf("\n");
    }

    return 0;
}
