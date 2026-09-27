#include <16F877A.h>
#FUSES XT, NOWDT
#USE delay(clock=4MHz)
#USE STANDARD_IO(E)
#USE STANDARD_IO(C)
#USE STANDARD_IO(D)

   int1 State = 0; // Variable para detectar si la valvula esta abierta o cerrada
   int IN_OUT = 0;
   int1 JMV = 0;
   int delay;
   Int pasos[8] = {1,5,4,12,8,10,2,3};
   signed int16 x=0;
   int32 Npasos;
   
#INT_TIMER0
void Timer0_ISR() {

     set_timer0(22);
     
     if(IN_OUT == 0){
        output_low(pin_E1);
        output_low(pin_E0);
     }
    
     if(IN_OUT == 1){
        output_low(pin_E1);
        output_high(pin_E0);
        delay_ms(delay);
        output_low(pin_E0);
     }
     
     if(IN_OUT == 2){
        output_low(pin_E0);
        output_high(pin_E1);
        delay_ms(delay);
        output_low(pin_E1);
     }
     

}

    valvula(){
    
         
         
         if((State == 0)&&(JMV == 1)){
            disable_interrupts(GLOBAL);
            output_E(0);
            
            for (int i = 0; i < 200; i++) {
            
                output_d(pasos[x]);
                x--;  // Decrementa para giro antihorario
                if (x < 0)
                    x = 7;  // Reinicia la secuencia
                delay_ms(10);
            }
             
             JMV = 0;
             output_d(0);  
         } // STATE = 0
         
         if((JMV == 0)&&(State == 1)){
            disable_interrupts(GLOBAL);
            output_E(0);
            for(Npasos= 0;Npasos < 200;Npasos++){
               
               output_d(pasos[x]);         
               x++;
               
               if (x==8)
                  x=0;
               
               delay_ms(10);
               
             }// FOR      
             
               JMV = 1;
               output_d(0);  
         } // STATE = 1
         
         enable_interrupts(GLOBAL);
         
      } // VALVULA
    
    
    
    void main(){
         output_low(pin_E0);
         output_low(pin_E1);
         setup_timer_0(RTCC_INTERNAL| RTCC_DIV_256);  
         set_timer0(22); 
                                                    
         enable_interrupts(INT_TIMER0);                            
         enable_interrupts(GLOBAL);
         while(true){
            
            if(input(pin_C0) == 1){ // todo apagado
            State = 0;
            valvula();
            delay = 0;
            IN_OUT = 0;
            }
            
            if(input(pin_C1) == 1){ // más bajo bomba 1
            State = 0;
            valvula();
            delay = 35;
            IN_OUT = 1;
            }
            
            if(input(pin_C2) == 1){ // intermedio bomba 1
            State = 0;
            valvula();
            delay = 45;
            IN_OUT = 1;
            }
            
            if(input(pin_C3) == 1){ // intermedio bomba 1
            State = 0;
            valvula();
            delay = 55;
            IN_OUT = 1;
            }
            
            if(input(pin_C4) == 1){ // más bajo bomba 2
            State = 1;
            valvula();
            delay = 35;
            IN_OUT = 2;
            }
            
            if(input(pin_C5) == 1){ // más bajo bomba 2
            State = 1;
            valvula();
            delay = 45;
            IN_OUT = 2;
            }
            
            if(input(pin_C6) == 1){ // más bajo bomba 2
            State = 1;
            valvula();
            delay = 55;
            IN_OUT = 2;
            }
         }
         
         }
      
      

