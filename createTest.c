#include <stdio.h>
#include <stdlib.h>

// Lista de instruções possíveis com os números correspondentes:

//   +——————————————————————————————————————————————————————————————————————————+
//   ¦                                                                          ¦
//   ¦       0  - LOADMEM: Carregar o valor da memória para o ACC               ¦
//   ¦       1  - LOADVAL: Carregar o valor imediato para o ACC                 ¦
//   ¦       2  - STORE: Armazenar o valor do ACC na memória                    ¦
//   ¦       3  - ADD: Somar o valor da memória ao ACC                          ¦
//   ¦       4  - SUB: Subtrair o valor da memória do ACC                       ¦
//   ¦       5  - MUL: Multiplicar o valor da memória pelo ACC                  ¦
//   ¦       6  - DIV: Dividir o ACC pelo valor da memória                      ¦
//   ¦       7  - INC: Incrementar o ACC                                        ¦
//   ¦       8  - DEC: Decrementar o ACC                                        ¦
//   ¦       9  - AND: Realizar operação AND entre ACC e valor da memória       ¦
//   ¦       10 - OR: Realizar operação OR entre ACC e valor da memória         ¦
//   ¦       11 - NOT: Inverter todos os bits do ACC                            ¦
//   ¦       12 - JMP: Pular para uma instrução específica                      ¦
//   ¦       13 - JZ: Pular se o ACC for zero                                   ¦
//   ¦       14 - JNZ: Pular se o ACC não for zero                              ¦
//   ¦       15 - JG: Pular se o ACC for maior que o valor na memória           ¦
//   ¦       16 - JL: Pular se o ACC for menor que o valor na memória           ¦
//   ¦       17 - JGE: Pular se o ACC for maior ou igual ao valor na memória    ¦
//   ¦       18 - JLE: Pular se o ACC for menor ou igual ao valor na memória    ¦
//   ¦       19 - HLT: Finalizar a execução                                     ¦
//   ¦                                                                          ¦
//   +——————————————————————————————————————————————————————————————————————————+




//   Exemplo de execução:

    // 1, 10,
    // 2, 20,
    // 3, 20,
    // 19, 0

    // 1. Carregar o valor 10 no ACC (LOADVAL 10)
    // 2. Armazenar o valor do ACC na posição 20 da memória (STORE 20)
    // 3. Somar o valor na posição 20 da memória ao ACC (ADD 20)
    // 4. Finalizar a execução (HLT)


#include <stdio.h>
#include <stdlib.h>

void criarArquivo(FILE *arquivo) {
    unsigned char instrucoes[] = {
        1, 10,  // LOADVAL 10
        2, 20,  // STORE 20
        3, 20,  // ADD 20
        19, 0   // HLT
    };

    fwrite(instrucoes, sizeof(unsigned char), sizeof(instrucoes), arquivo);
    printf("Arquivo 'teste.exe' atualizado com as instruções do código. Execute './sp1'.\n\n");
}

void criarArquivoManualmente(FILE *arquivo) {
    unsigned char opcode, operando;
    printf("\nDigitar as instruções no formato:\n<opcode> <operando>\nUma instrução por linha. '19 0' para finalizar.\n\n");

    while (1) {
        printf(">  ");
        if (scanf("%hhu %hhu", &opcode, &operando) != 2) {
            printf("Entrada inválida. Tente novamente.\n");
            while (getchar() != '\n');
            continue;
        }

        fwrite(&opcode, sizeof(unsigned char), 1, arquivo);
        fwrite(&operando, sizeof(unsigned char), 1, arquivo);

        if (opcode == 19 && operando == 0) {
            printf("Instruções finalizadas. Arquivo 'teste.exe' atualizado. Execute './sp1'.\n\n");
            break;
        }
    }
}

int main() {
    char escolha;
    FILE *arquivo = fopen("teste.exe", "wb");
    if (!arquivo) {
        printf("Erro: Não foi possível criar o arquivo 'teste.exe'.\n");
        return 1;
    }

    printf("Escolha uma opção:\n\n");
    printf("1 - Usar arquivo com instruções fixas do código\n");
    printf("2 - Digitar instruções manualmente\n\n");
    printf("Opção: ");
    scanf(" %c", &escolha);

    if (escolha == '1') {
        criarArquivo(arquivo);
    } else if (escolha == '2') {
        criarArquivoManualmente(arquivo);
    } else {
        printf("Opção inválida.\n");
    }

    fclose(arquivo);
    return 0;
}
