#include <16F877A.h>
#device PASS_STRINGS = IN_RAM
#FUSES XT, NOWDT
#USE delay(clock=4MHz)
#USE rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, errors)
#include <stdlib.h>

int32 fosc = 4000000; 
#define P_ECHO PIN_C0
#define P_TRIG PIN_C1
#include <HCSR04.c>
#USE STANDARD_IO(B)
#USE STANDARD_IO(C)
#USE STANDARD_IO(E)

#define PI 3.1416

// Variables globales
float altura, AM = 12.85;
float distancia = 0, suma = 0, prom = 0, porcent, volumen;
int contador_muestras = 0;
float setpoint = 50;

// Estados y control
int info = 0, estado = 0, botellas = 0, contador_botellas = 0;
int flag_medir_sensor = 0;

// Control de tiempo
int flag_llenado = 0;
unsigned int16 contador_tiempo = 0;

// Motor a pasos
int pasos[8] = {1, 5, 4, 12, 8, 10, 2, 3};
int x = 0;

// Mensajes de alerta
int flag_tanque_bajo = 0; // Para evitar impresión indefinida de alerta
int flag_emergencia_enviada = 0;

// Buffer UART
char buffer[20];
int buffer_index = 0, flag_recibido = 0;

#INT_RDA
void serial_ISR() {
    char c = getc();
    if (c == '\r') {
        buffer[buffer_index] = '\0';
        flag_recibido = 1;
        buffer_index = 0;
    } else if (isdigit(c)) {
        if (buffer_index < sizeof(buffer) - 1) buffer[buffer_index++] = c;
    }
}

#INT_TIMER0
void Timer0_ISR() {
    flag_medir_sensor = 1; 
    set_timer0(22);
    if (flag_llenado) contador_tiempo++;
}

// Función para paro de emergencia
void paro_emergencia() {
    if (!flag_emergencia_enviada) {
        output_e(0);
        output_b(0);
        flag_llenado = 0;
        contador_tiempo = 0;
        estado = 9; // Cambiar estado a emergencia
        printf("ALERTA: EMERGENCIA DETECTADA\r\n");
        flag_emergencia_enviada = 1;
    }
}

void mover_motor_pasos() {
    for (int i = 0; i < 200; i++) {
        output_b(pasos[x]);
        x = (x + 1) % 8;
        delay_ms(10);
        if (estado == 9) return; // Salir en caso de emergencia
    }
}

void botellas_proceso() {
    if (contador_botellas < botellas && !flag_llenado) {
        mover_motor_pasos();
        output_high(PIN_E0);
        flag_llenado = 1;
        contador_tiempo = 0;

        printf("INFO: Llenando botella %d/%d\r\n", contador_botellas + 1, botellas);
    }

    if (flag_llenado && contador_tiempo >= 150) {
        output_low(PIN_E0);
        flag_llenado = 0;
        contador_botellas++;

        printf("INFO: Botella %d/%d completada\r\n", contador_botellas, botellas);
    }

    if (contador_botellas >= botellas) {
        estado = 8; // Proceso completado
        printf("INFO: Proceso completado. Total botellas llenas: %d\r\n", botellas);
        output_b(0);
    }
}

void main() {
    HCSR04_init();
    setup_timer_0(RTCC_INTERNAL | RTCC_DIV_256);
    set_timer0(22);
    enable_interrupts(INT_TIMER0);
    enable_interrupts(INT_RDA);
    enable_interrupts(GLOBAL);

    while (true) {
        if (flag_recibido) {
            flag_recibido = 0;
            info = atoi(buffer);

            if (info >= 1 && info <= 6) {
                botellas = info;
                printf("INFO: Botellas actualizadas a %d\r\n", botellas);
                contador_botellas = 0;
            } else if (info == 7) {
                estado = 7;
                printf("INFO: Proceso iniciado\r\n");
                contador_botellas = 0;
            } else if (info == 9) {
                paro_emergencia();
            }
            memset(buffer, 0, sizeof(buffer));
        }

        if (flag_medir_sensor) {
            flag_medir_sensor = 0;
            distancia = HCSR04_get_distance() - 2.86;
            suma += distancia;
            contador_muestras++;

            if (contador_muestras == 10) {
                prom = suma / 10.0;
                suma = 0;
                contador_muestras = 0;

                altura = AM - prom;
                if (altura < 0) altura = 0;
                porcent = (altura * 100) / AM;
                volumen = (PI * 8.0 * 8.0 * altura) / 1000.0;

                printf("NIVEL: %.2f%%, VOLUMEN: %.2f L, ESTADO: %d\r\n", porcent, volumen, estado);

                // Alerta de tanque bajo 50%
                if (porcent < 50 && !flag_tanque_bajo) {
                    printf("ALERTA: Tanque por debajo del 50%%\r\n");
                    flag_tanque_bajo = 1;
                } else if (porcent >= 50) {
                    flag_tanque_bajo = 0; // Resetear alerta cuando el nivel suba
                }
            }
        }

        if (porcent < setpoint) output_high(PIN_E1);
        else output_low(PIN_E1);

        if (estado == 7) botellas_proceso();
        if (estado == 9) paro_emergencia();
    }
}

