#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "semphr.h"
#include "supporting_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

SemaphoreHandle_t xBinarySemaphore;

 // ainda uso struct para representar motor
typedef struct motor {
    int velocidade_atual;
} motor;

 // ainda usando struct para representar o quadricoptero
typedef struct quadricoptero {
    motor frontal_esquerdo;
    motor frontal_direito;
    motor traseiro_esquerdo;
    motor traseiro_direito;
} quadricoptero;

 // mesma funcao simples pra criar o quadricoptero
quadricoptero criar_quadricoptero() {
    motor frontal_esquerdo = {0};
    motor frontal_direito = {0};
    motor traseiro_esquerdo = {0};
    motor traseiro_direito = {0};
    quadricoptero q = {
        frontal_esquerdo,
        frontal_direito,
        traseiro_esquerdo,
        traseiro_direito
    };
    return q;
}

 // enum para nomear tipos de movimento
typedef enum {
    HORARIO, 
    ANTIHORARIO,
    FRENTE,
    TRAS,
    ESQUERDA,
    DIREITA
} TipoMovimento;

// MUDANCA: criando variavel volatile e GLOBAL para representar o quadricoptero
volatile quadricoptero q;
 // MUDANCA: crio variaveis volatile e GLOBAIS representando o tipo atual de movimento
 // de cada manobra - nao uso mais struct taskCall.
volatile TipoMovimento sentido;
volatile TipoMovimento direcao;
volatile TipoMovimento orientacao;
// ainda uso as funcoes simples de alteracao do valor de velocidade
// para simplificar
void aumentar_velocidade(motor **motores, int quant) {
    for (int i = 0; i < quant; i++) {
        motores[i]->velocidade_atual += 10;
    }
}

void diminuir_velocidade(motor **motores, int quant) {
    for (int i = 0; i < quant, i++) {
        if ((motores[i]->velocidade_atual - 10) > 0) {
            motores[i]->velocidade_atual -= 10;
        }
        else if ((motores[i]->velocidade_atual - 10) <= 0) {
            motores[i]->velocidade_atual = 10;
        }
    }
}

 // funcao que printa velocidades modificada para acessar variavel global diretamente
void print_velocidades() {
    vPrintStringAndNumber("\nVELOCIDADE MOTOR FRONTAL ESQUERDO: ", 
        q.frontal_esquerdo.velocidade_atual);
    vPrintStringAndNumber("\nVELOCIDADE MOTOR FRONTAL DIREITO: ", 
        q.frontal_direito.velocidade_atual);
    vPrintStringAndNumber("\nVELOCIDADE MOTOR TRASEIRO ESQUERDO: ", 
        q.traseiro_esquerdo.velocidade_atual);
    vPrintStringAndNumber("\nVELOCIDADE MOTOR TRASEIRO DIREITO: ", 
        q.traseiro_direito.velocidade_atual);
}

 // nao recebera parametros pois so acessa GLOBAIS
void vTaskGuinada(void *pvParameters) {
    for (;;) {
        if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdTRUE) { 
            if (sentido == HORARIO) {
            // ainda usei funcoes aumentar/diminuir trabalhando com PONTEIROS
                motor *motores_aumentar[2] = {&q.frontal_esquerdo, &q.traseiro_direito};
                aumentar_velocidade(motores_aumentar, 2);
                motor *motores_diminuir[2] = {&q.frontal_direito, &q.traseiro_esquerdo};
                diminuir_velocidade(motores_diminuir, 2);
                vPrintString("\nGUINADA HORARIA FOI EXECUTADA");
            }
            else if (sentido == ANTIHORARIO) {
                motor *motores_aumentar[2] = {&q.frontal_direito, &q.traseiro_esquerdo};
                aumentar_velocidade(motores_aumentar, 2);
                motor *motores_diminuir[2] = {&q.frontal_esquerdo, &q.traseiro_direito};
                diminuir_velocidade(motores_diminuir, 2);
                vPrintString("\nGUINADA ANTIHORARIA FOI EXECUTADA");
            }
            print_velocidades();
            xSemaphoreGive(xBinarySemaphore);
        }
    // delay dessa forma melhorou visualizacao no terminal
    vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

 // nao recebera parametros pois so acessa GLOBAIS
void vTaskArfagem(void *pvParameters) {
    for (;;) {
        if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdTRUE) {
            if (direcao == FRENTE) {
                // ainda usei funcoes aumentar/diminuir trabalhando com PONTEIROS
                motor *motores_aumentar[2] = {&q.traseiro_esquerdo, &q.traseiro_direito};
                aumentar_velocidade(motores_aumentar, 2);
                motor *motores_diminuir[2] = {&q.frontal_esquerdo, &q.frontal_direito};
                diminuir_velocidade(motores_diminuir, 2);
                vPrintString("\nARFAGEM PARA FRENTE FOI EXECUTADA");
            }
            else if (direcao == TRAS) {
                motor *motores_aumentar[2] = {&q.frontal_esquerdo, &q.frontal_direito};
                aumentar_velocidade(motores_aumentar, 2);
                motor *motores_diminuir[2] = {&q.traseiro_esquerdo, &q.traseiro_direito};
                diminuir_velocidade(motores_diminuir, 2);
                vPrintString("\nARFAGEM PARA TRAS FOI EXECUTADA");
            }
            print_velocidades();
            xSemaphoreGive(xBinarySemaphore);
        }
    // delay dessa forma melhorou visualizacao no terminal
    vTaskDelay(pdMS_TO_TICKS(4000));
    }
}

// nao recebera parametros pois so acessa GLOBAIS
void vTaskRolagem(void *pvParameters) {
    for (;;) {
        if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdTRUE) {
            if (orientacao == ESQUERDA) {
                // ainda usei funcoes aumentar/diminuir trabalhando com PONTEIROS
                motor *motores_aumentar[2] = {&q.frontal_direito, &q.traseiro_direito};
                aumentar_velocidade(motores_aumentar, 2);
                motor *motores_diminuir[2] = {&q.frontal_esquerdo, &q.traseiro_esquerdo};
                diminuir_velocidade(motores_diminuir, 2);
                vPrintString("\nROLAGEM PARA A ESQUERDA FOI EXECUTADA");
            }
            else if (orientacao == DIREITA) {
                motor *motores_aumentar[2] = {&q.frontal_esquerdo, &q.traseiro_esquerdo};
                aumentar_velocidade(motores_aumentar, 2);
                motor *motores_diminuir[2] = {&q.frontal_direito, &q.traseiro_direito};
                diminuir_velocidade(motores_diminuir, 2);
                vPrintString("\nROLAGEM PARA A DIREITA FOI EXECUTADA");
            }
            print_velocidades();
            xSemaphoreGive(xBinarySemaphore);
        }
        // delay dessa forma melhorou visualizacao no terminal
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

 // nao recebera parametros pois so acessa GLOBAIS
void vTaskRadioFrequencia(void *pvParameters) {
    for (;;) {
        if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdTRUE) {
            //gera valor aleatorio entre 1 e 100
            int valor_aleatorio = (rand() % 100) + 1;
            // define tipo de movimento com base em Par ou Impar
            if ((valor_aleatorio % 2) == 0) {
                sentido = HORARIO;
                direcao = FRENTE;
                orientacao = ESQUERDA;
            } else {
                sentido = ANTIHORARIO;
                direcao = TRAS;
                orientacao = DIREITA;
            }
        xSemaphoreGive(xBinarySemaphore);
        }
    vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main_( void ) {
    // gera seed
    srand(time(NULL));
    q = criar_quadricoptero();

    xBinarySemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(xBinarySemaphore);

    sentido = HORARIO;
    direcao = FRENTE;
    orientacao = ESQUERDA;

    xTaskCreate(vTaskRadioFrequencia, "tarefa-radiofreq", 256, NULL, 1, NULL);
    xTaskCreate(vTaskGuinada, "tarefa-guinada", 256, NULL, 1, NULL);
    xTaskCreate(vTaskArfagem, "tarefa-arfagem", 256, NULL, 1, NULL);
    xTaskCreate(vTaskRolagem, "tarefa-rolagem", 256, NULL, 1, NULL);
    vTaskStartScheduler();

    return 0;
}