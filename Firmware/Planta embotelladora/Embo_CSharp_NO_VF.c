#include <16F877A.h>
#device PASS_STRINGS = IN_RAM
#FUSES XT, NOWDT
#USE delay(clock=4MHz)
#USE rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, errors)
#include <stdlib.h>

int32 fosc = 4000000; // Frecuencia del oscilador
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

int info = 0, estado = 0, botellas = 0, contador_botellas = 0;
int flag_medir_sensor = 0;

// Variables para motor a pasos
int Npasos = 0, x = 0;
int pasos[8] = {1, 5, 4, 12, 8, 10, 2, 3}; // Secuencia del motor

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
}

// Función para mover el motor a pasos
void mover_motor_pasos() {
    for (Npasos = 0; Npasos < 200; Npasos++) { // Simula 200 pasos
        output_b(pasos[x]);
        x = (x + 1) % 8;
        delay_ms(10);

        if (estado == 9) return; // Parar si hay emergencia
    }
}

// Proceso de llenado de botellas
void botellas_proceso() {
    if (contador_botellas < botellas) {
        mover_motor_pasos();
        output_high(PIN_E0); // Activa bomba de llenado

        for (int j = 0; j < 100; j++) { 
            delay_ms(150); 
            if (estado == 9) {
                output_low(PIN_E0);
                return;
            }
        }
        output_low(PIN_E0);

        contador_botellas++;
        // Enviar datos actualizados: BotellaActual, Porcentaje, Volumen, Estado 7
        printf("%d:%.2f:%.2f:7\r\n", contador_botellas, porcent, volumen);
    }

    if (contador_botellas >= botellas) {
        estado = 8; // Proceso completado
        printf("%d:%.2f:%.2f:8\r\n", contador_botellas, porcent, volumen);
    }
}

void paro_emergencia() {
    output_e(0);
    output_b(0);
    estado = 9;
    printf("%d:%.2f:%.2f:9\r\n", contador_botellas, porcent, volumen); // Estado 9
}

void main() {
    HCSR04_init();
    setup_timer_0(RTCC_INTERNAL | RTCC_DIV_256);
    set_timer0(22);
    enable_interrupts(INT_TIMER0);
    enable_interrupts(INT_RDA);
    enable_interrupts(GLOBAL);

    while (true) {
        // Procesar datos UART
        if (flag_recibido) {
            flag_recibido = 0;
            info = atoi(buffer);

            if (info >= 1 && info <= 6) { 
                botellas = info;
                contador_botellas = 0; 
            } else if (info == 7) { 
                estado = 7;
                contador_botellas = 0;
            } else if (info == 9) { 
                paro_emergencia();
            }
            memset(buffer, 0, sizeof(buffer));
        }

        // Medición ultrasónica
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
            }
        }

        // Control del nivel de agua
        if (porcent < 50.0) output_high(PIN_E1);
        else output_low(PIN_E1);

        // Procesar llenado de botellas
        if (estado == 7) botellas_proceso();
    }
}

