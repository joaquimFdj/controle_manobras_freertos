#ifndef SUPPORTING_FUNCTIONS_H
#define SUPPORTING_FUNCTIONS_H

#include <stdio.h>

/* 
 * Função para imprimir apenas uma string.
 * Usada em vez de printf direto nos exemplos.
 */
static inline void vPrintString(const char *pcString)
{
    printf("%s", pcString);
    fflush(stdout);
}

/*
 * Função para imprimir uma string seguida de um número.
 */
static inline void vPrintStringAndNumber(const char *pcString, int iValue)
{
    printf("%s %d\n", pcString, iValue);
    fflush(stdout);
}

#endif /* SUPPORTING_FUNCTIONS_H */
