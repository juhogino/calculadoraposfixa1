#include "calculadora.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

void inicializar(Pilha *pilha) {
    pilha->topo = NULL;
}

void empilhar(Pilha *pilha, double valor) {
    No *novoNo = (No *)malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("falha na alocacao de memoria\n");
        exit(EXIT_FAILURE);
    }
    novoNo->dado = valor;
    novoNo->proximo = pilha->topo;
    pilha->topo = novoNo;
}

double desempilhar(Pilha *pilha) {
    if (pilha->topo != NULL) {
        No *temp = pilha->topo;
        double valor = temp->dado;
        pilha->topo = temp->proximo;
        free(temp);
        return valor;
    } else {
        printf("pilha vazia\n");
        exit(EXIT_FAILURE);
    }
}

double avaliarExpressaoPosfixada(const char *expressao) {
    Pilha pilha;
    inicializar(&pilha);

    char *endptr;
    double valor;

    for (int i = 0; expressao[i] != '\0'; ++i) {
        if (isdigit(expressao[i]) || (expressao[i] == '-' && isdigit(expressao[i + 1]))) {
            valor = strtod(&expressao[i], &endptr);
            empilhar(&pilha, valor);
            i += endptr - &expressao[i] - 1;
        } else if (expressao[i] == ' ' || expressao[i] == '\t') {
            continue;
        } else {
            double operando;
            switch (expressao[i]) {
                case '+':
                    empilhar(&pilha, desempilhar(&pilha) + desempilhar(&pilha));
                    break;
                case '-':
                    operando = desempilhar(&pilha);
                    empilhar(&pilha, desempilhar(&pilha) - operando);
                    break;
                case '*':
                    empilhar(&pilha, desempilhar(&pilha) * desempilhar(&pilha));
                    break;
                case '/':
                    operando = desempilhar(&pilha);
                    if (operando != 0.0) {
                        empilhar(&pilha, desempilhar(&pilha) / operando);
                    } else {
                        printf("erro na divisao por zero\n");
                        exit(EXIT_FAILURE);
                    }
                    break;
                case '^':
                    operando = desempilhar(&pilha);
                    empilhar(&pilha, pow(desempilhar(&pilha), operando));
                    break;
                case 'l':
                    // Logaritmo 
                    empilhar(&pilha, log10(desempilhar(&pilha)));
                    i += 2; 
                    break;
                case 's':
                    // Seno
                    empilhar(&pilha, sin(desempilhar(&pilha)));
                    i += 2; 
                    break;
                case 'c':
                    // Cosseno
                    empilhar(&pilha, cos(desempilhar(&pilha)));
                    i += 2; 
                    break;
                case 't':
                    // Tangente
                    empilhar(&pilha, tan(desempilhar(&pilha)));
                    i += 2; 
                    break;
                default:
                    printf("operador invalido\n");
                    exit(EXIT_FAILURE);
            }
        }
    }

    if (pilha.topo == NULL) {
        printf("pilha vazia\n");
        exit(EXIT_FAILURE);
    }

    double resultado = desempilhar(&pilha);

    if (pilha.topo != NULL) {
        printf("pilha nao esta vazia apos a avaliacao\n");
        exit(EXIT_FAILURE);
    }

    return resultado;
}

void traduzirPosfixaParaInfixa(const char *expressao) {
    Pilha pilha;
    inicializar(&pilha);

    for (int i = 0; expressao[i] != '\0'; ++i) {
        if (isdigit(expressao[i]) || (expressao[i] == '-' && isdigit(expressao[i + 1]))) {
            double valor = strtod(&expressao[i], NULL);
            empilhar(&pilha, valor);
            while (isdigit(expressao[i]) || expressao[i] == '-' || expressao[i] == '.') {
                i++;
            }
            i--;  
        } else if (expressao[i] == ' ' || expressao[i] == '\t') {
            continue;
        } else {
            double operando2 = desempilhar(&pilha);
            double operando1;

            int j = i;
            while (isalpha(expressao[j])) {
                j--;
            }

            if (j >= 0 && (expressao[j] == 'l' || expressao[j] == 's' || expressao[j] == 'c' || expressao[j] == 't')) {
                
                operando1 = desempilhar(&pilha);
                printf("%c(%.3f)", expressao[j], operando1);
            } else {
                
                operando1 = desempilhar(&pilha);
                if (expressao[i] == '*' || expressao[i] == '/') {
                    printf(" %c %0.3f", expressao[i], operando2);
                } else {
                    printf("(%0.3f %c %0.3f)", operando1, expressao[i], operando2);
                    
                }
            }

            empilhar(&pilha, 0.0);  
        }
    }

    double ultimoOperando = desempilhar(&pilha);

    if (pilha.topo != NULL) {
        printf(" Pilha não está vazia após a tradução\n");
        return;
    }

    printf("\n");
}

