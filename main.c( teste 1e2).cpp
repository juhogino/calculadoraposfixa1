#include <stdio.h>
#include <math.h>
#include "calculadora.c"
#include "calculadora.h"

int main() {
    char expressao[100]; 

    printf("expressao em notacao posfixa: ");
    scanf("%[^\n]", expressao);

    double resultado = avaliarExpressaoPosfixada(expressao);
    printf("resultado: %.3f\n", resultado);
 
	printf("Notacao infixa: ");
    traduzirPosfixaParaInfixa(expressao);
    return 0;
}

