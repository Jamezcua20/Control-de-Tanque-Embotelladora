#include <16F877A.h>
#FUSES XT, NOWDT
#USE delay(clock=4MHz)
int32 fosc = 4000000;
#define P_ECHO PIN_B0
#define P_TRIG PIN_B1
#include <HCSR04.c>
#include <lcd_d.c>
#USE STANDARD_IO(B)
#USE STANDARD_IO(C)

// Variables globales
float altura, AM = 12.85; // Altura máxima
float distancia = 0, suma = 0, prom = 0, porcent, alt_deseada;
int contador_muestras = 0;
int16 delay = 0;
float integral = 0, derivada = 0, Error = 0, Error_anterior = 0, DT = 0.06;
float Kp = 0.5658, Ki = 0.9433, Kd = 0.05, salida_control; // Ajustar según pruebas
float setpoint = 40;  // Nivel deseado en porcentaje
int flag_medir_sensor = 0; // Flag para realizar la medición del sensor



#INT_TIMER0
void Timer0_ISR() {
    // Marca un flag cada 60 ms
    flag_medir_sensor = 1;
    set_timer0(22); // Reinicia el Timer0 para 60 ms
    
}

void main() {
    lcd_init();
    HCSR04_init();

    // Configuración del Timer0 para 60 ms
    setup_timer_0(RTCC_INTERNAL | RTCC_DIV_256); 
    set_timer0(22);
    enable_interrupts(INT_TIMER0);
    enable_interrupts(GLOBAL);        
    

    while (true) {
        // Realiza la medición del sensor cada 60 ms
        if (flag_medir_sensor) {
            flag_medir_sensor = 0; // Limpia el flag

            // Medición con el sensor ultrasónico
            distancia = HCSR04_get_distance();
            distancia = distancia - 2.86;
            suma += distancia;
            contador_muestras++;

            // Calcular promedio después de 10 mediciones (~600 ms)
            if (contador_muestras == 10) {
                prom = suma / 10.0;
                suma = 0;
                contador_muestras = 0;

                // Calcular altura y porcentaje
                altura = AM - prom;
                if (altura < 0) altura = 0;
                porcent = (altura * 100) / AM;

                // Calcular control PID
                alt_deseada = (setpoint * AM) / 100;
                Error = alt_deseada - altura;
                integral = Error * DT;
                derivada = (Error - Error_anterior) / DT;
                salida_control = (Kp * Error) + (Ki * integral) + (Kd * derivada);
                Error_anterior = Error;

                // Actualizar lógica de salida
                    if(salida_control > 5){;
                     output_c(0);
                     output_high(pin_c3);
                   }
                   
                   if((salida_control <= 5) && (salida_control > 2)){
                     output_c(0);
                     output_high(pin_c2);
                   }
                   
                   if((salida_control <= 2) && (salida_control > 0.15)){
                     output_c(0);
                     output_high(pin_c1);
                   }
                   
                   if((salida_control < 0.15)&&(salida_control > - 0.3)){
                     output_c(0);
                     output_high(pin_c0);
                   }
                   
                   if((salida_control <= -0.3)&&(salida_control > -2)){
                     output_c(0);
                     output_high(pin_c4);
                   }
                   
                   if((salida_control <= -2 )&&(salida_control > -5)){
                     output_c(0);
                     output_high(pin_c5);
                   }
                   
                   if((salida_control <= -5 )){
                     output_c(0);
                     output_high(pin_c6);
                   }

       
            } // contador muestras
                   
            
        } // flag sensor

        // Actualización del LCD
        lcd_gotoxy(1, 1);
        printf(lcd_putc, "Ctrl:%.2f %Lu         ", salida_control, delay);
        lcd_gotoxy(1, 2);
        printf(lcd_putc, "Al:%.2f %%:%f", altura, porcent);
    }
}

