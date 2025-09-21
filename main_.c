// codigo de joaquimFdj

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "semphr.h"
#include "supporting_functions.h"
#include <stdio.h>

SemaphoreHandle_t xMutex;

typedef struct motor {
	int velocidade_atual;
} motor;

typedef struct quadricoptero {
	motor frontal_esquerdo;
	motor frontal_direito;
	motor traseiro_esquerdo;
	motor traseiro_direito;
} quadricoptero;

quadricoptero cria_quadricoptero() {
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

typedef enum {
	HORARIO, 
	ANTIHORARIO, 
	FRENTE,
	TRAS,
	ESQUERDA,
	DIREITA
} TipoMovimento;

typedef struct taskCall {	
		quadricoptero* q;
		TipoMovimento movimento;
	} taskCall;

typedef struct controladorMovimentos {
	taskCall *movimentoArfagem;
	taskCall *movimentoRolagem;
	taskCall *movimentoGuinada;
} controladorMovimentos;

void aumentar_velocidade(motor **motores, int quant) {
	for (int i = 0; i < quant; i++) {
		motores[i]->velocidade_atual += 10;
	}
}

void diminuir_velocidade(motor **motores, int quant) {
	for (int i = 0; i< quant; i++) {
		if ((motores[i]->velocidade_atual - 10) > 0) {
		motores[i]->velocidade_atual -= 10;
		} 
		else if ((motores[i]->velocidade_atual - 10) <= 0) {
			motores[i]->velocidade_atual = 0;
		}
	}
}

void print_velocidades(quadricoptero *q) {
	vPrintStringAndNumber("\nVELOCIDADE MOTOR FRONTAL ESQUERDO: ", q->frontal_esquerdo.velocidade_atual);
	vPrintStringAndNumber("\nVELOCIDADE MOTOR FRONTAL DIREITO: ", q->frontal_direito.velocidade_atual);
	vPrintStringAndNumber("\nVELOCIDADE MOTOR TRASEIRO ESQUERDO: ", q->traseiro_esquerdo.velocidade_atual);
	vPrintStringAndNumber("\nVELOCIDADE MOTOR TRASEIRO DIREITO: ", q->traseiro_direito.velocidade_atual);
}

void vTaskArfagem(void *pvParameters) {

	taskCall *parameters = (taskCall*)pvParameters;
	quadricoptero *q = parameters->q;

	for (;;) {

		if (xSemaphoreTake(xMutex, portMAX_DELAY)) {
			if (parameters->movimento == FRENTE) {
				motor *motores_aumentar[2] = {&q->traseiro_esquerdo, &q->traseiro_direito};
				aumentar_velocidade(motores_aumentar, 2);

				motor *motores_diminuir[2] = {&q->frontal_esquerdo, &q->frontal_direito};
				diminuir_velocidade(motores_diminuir, 2);

				vPrintString("\nARFAGEM PARA FRENTE FOI EXECUTADA");
			} 
			else if (parameters->movimento == TRAS) {
				motor *motores_aumentar[2] = {&q->frontal_esquerdo, &q->frontal_direito};
				aumentar_velocidade(motores_aumentar, 2);
				
				motor *motores_diminuir[2] = {&q->traseiro_esquerdo, &q->traseiro_direito};
				diminuir_velocidade(motores_diminuir, 2);

				vPrintString("\nARFAGEM PARA TRAS FOI EXECUTADA");
			}

			print_velocidades(q);
			xSemaphoreGive(xMutex);
		}

		vTaskDelay(pdMS_TO_TICKS(4000));
	}

} 

void vTaskRolagem(void *pvParameters) {

	taskCall *parameters = (taskCall*)pvParameters;
	quadricoptero *q = parameters->q;

	for (;;) {

		if (xSemaphoreTake(xMutex, portMAX_DELAY)) {
			if (parameters->movimento == ESQUERDA) {
				motor *motores_aumentar[2] = {&q->frontal_esquerdo, &q->traseiro_direito};
				aumentar_velocidade(motores_aumentar, 2);

				motor *motores_diminuir[2] = {&q->frontal_direito, &q->traseiro_esquerdo};
				diminuir_velocidade(motores_diminuir, 2);

				vPrintString("\nROLAGEM PARA A ESQUERDA FOI EXECUTADA");
			} 
			else if (parameters->movimento == DIREITA) {
				motor *motores_aumentar[2] = {&q->frontal_direito, &q->traseiro_esquerdo};
				aumentar_velocidade(motores_aumentar, 2);
				
				motor *motores_diminuir[2] = {&q->frontal_esquerdo, &q->traseiro_direito};
				diminuir_velocidade(motores_diminuir, 2);

				vPrintString("\nROLAGEM PARA A DIREITA FOI EXECUTADA");
			}

			print_velocidades(q);
			xSemaphoreGive(xMutex);
		}
	
		vTaskDelay(pdMS_TO_TICKS(2000));
	}
	
} 

void vTaskGuinada(void *pvParameters) {

	taskCall *parameters = (taskCall*)pvParameters;
	quadricoptero *q = parameters->q;

	for (;;) {

		if (xSemaphoreTake(xMutex, portMAX_DELAY)) {
			if (parameters->movimento == HORARIO) {
				motor *motores_aumentar[2] = {&q->frontal_esquerdo, &q->traseiro_esquerdo};
				aumentar_velocidade(motores_aumentar, 2);

				motor *motores_diminuir[2] = {&q->frontal_direito, &q->traseiro_direito};
				diminuir_velocidade(motores_diminuir, 2);

				vPrintString("\nGUINADA HORARIA FOI EXECUTADA");
			} 
			else if (parameters->movimento == ANTIHORARIO) {
				motor *motores_aumentar[2] = {&q->frontal_direito, &q->traseiro_direito};
				aumentar_velocidade(motores_aumentar, 2);
				
				motor *motores_diminuir[2] = {&q->frontal_esquerdo, &q->traseiro_esquerdo};
				diminuir_velocidade(motores_diminuir, 2);

				vPrintString("\nGUINADA ANTIHORARIA FOI EXECUTADA");
			}

			print_velocidades(q);
			xSemaphoreGive(xMutex);
		}
		
		vTaskDelay(pdMS_TO_TICKS(1000));
	}

} 

void vTaskIntercalarMovimentos(void *pvParameters) {

	for (;;) {	
		if (xSemaphoreTake(xMutex, portMAX_DELAY)) {
			controladorMovimentos *calls = (controladorMovimentos*)pvParameters;
			taskCall *callArfagem = calls->movimentoArfagem;
			taskCall *callRolagem = calls->movimentoRolagem;
			taskCall *callGuinada = calls->movimentoGuinada;

			if (callArfagem->movimento == FRENTE) {
				callArfagem->movimento = TRAS;
			} else {
				callArfagem->movimento = FRENTE;
			}

			if (callRolagem->movimento == ESQUERDA) {
				callRolagem->movimento = DIREITA;
			} else {
				callRolagem->movimento = ESQUERDA;
			}

			if (callGuinada->movimento == HORARIO) {
				callGuinada->movimento = ANTIHORARIO;
			} else {
				callGuinada->movimento = HORARIO;
			}

			xSemaphoreGive(xMutex);
		}

		vTaskDelay(pdMS_TO_TICKS(5000));
	}

}

int main_( void ) {

	quadricoptero q = cria_quadricoptero();
	taskCall call_arfagem = {&q, FRENTE};
	taskCall call_rolagem = {&q, ESQUERDA};
	taskCall call_guinada = {&q, HORARIO};
	controladorMovimentos controlador = {
		&call_arfagem,
		&call_rolagem,
		&call_guinada
	};

	xMutex = xSemaphoreCreateMutex();
	if (xMutex == NULL) {
		printf("\nErro ao tentar criar o mutex\n");
		return 2;
	}

	xTaskCreate(vTaskGuinada, "tarefa-guinada", 128, &call_guinada, 1, NULL);
	xTaskCreate(vTaskArfagem, "tarefa-arfagem", 128, &call_arfagem, 1, NULL);
	xTaskCreate(vTaskRolagem, "tarefa-rolagem", 128, &call_rolagem, 1, NULL);

	xTaskCreate(vTaskIntercalarMovimentos, "tarefa-intercalar-movmntos", 128, &controlador, 1, NULL);

	vTaskStartScheduler();

	return 0;
}