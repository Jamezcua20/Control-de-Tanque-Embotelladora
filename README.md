# Sistema de Control de Nivel de Tanque y Planta Embotelladora Automatizada

## Nota sobre la Versión del Repositorio

> **Aviso de Estado del Proyecto:**
>
> Los archivos adjuntos en este repositorio corresponden a las **fases de desarrollo, pruebas de arquitectura distribuida y prototipos de integración** registrados con fecha **18 de diciembre de 2024**.
>
> Esta recopilación representa el proceso de investigación y desarrollo (R&D) del sistema, incluyendo la implementación del lazo de control PID en hardware distribuido y la posterior migración hacia el secuenciador de la planta embotelladora. **No constituyen la versión final consolidada**, la cual integraba la trama directa de comunicación del PID completo hacia la aplicación SCADA en C#.

## Descripción General

El proyecto consiste en una planta a escala mecatrónica para el **control de nivel de fluido en tanque** y el **secuenciamiento de embotellado por lotes**. El sistema integra la adquisición de variables físicas (distancia/nivel ultrasónico), algoritmos de control de lazo cerrado (PID), actuadores de potencia (motor a pasos para posicionamiento/válvulas y bombas DC) y una interfaz HMI/SCADA de monitoreo en tiempo real.

```
                  ┌──────────────────────────────────────────┐
                  │          PLANTA FÍSICA / EMBOTELLADORA   │
                  │   • Sensor Ultrasónico (HC-SR04)         │
                  │   • Motor a Pasos (Banda / Válvula)     │
                  │   • Bombas de Dosificación / Recirculación│
                  └────────────────────┬─────────────────────┘
                                       │
                        ┌──────────────┴──────────────┐
                        │                             │
                        ▼                             ▼
              ┌───────────────────┐         ┌───────────────────┐
              │   PIC #1 (Master) │         │   PIC #2 (Slave)  │
              │  Cálculo PID /    │────────▶│ Control Directo   │
              │  Muestreo Sensor  │ Bus I/O │ de Actuadores     │
              └─────────┬─────────┘         └───────────────────┘
                        │
                        │ Enlace Serial RS232 (9600 Baudios)
                        ▼
              ┌──────────────────────────────────────────┐
              │          INTERFAZ HMI / SCADA (PC)       │
              │   • Aplicación C# WinForms               │
              │   • Gráficas de Respuesta Temporal       │
              │   • Base de Datos SQL Server             │
              └──────────────────────────────────────────┘

```

## Estructura del Proyecto

```
├── firmware/
│   ├── control_pid_distribuido/
│   │   ├── PIC1_PID_Master.c       # Algoritmo PID, sensor HC-SR04 y pantalla LCD
│   │   └── PIC2_Actuadores_Slave.c # Control de motor a pasos (válvula) y bombas
│   │
│   ├── planta_embotelladora/
│   │   ├── PIC_Embotelladora_CSharp.c # Mapeo de tramas ':' para interfaz WinForms
│   │   └── PIC_Embotelladora_Debug.c  # Versión asíncrona (Timer0) con logs legibles
│   │
│   └── drivers/
│       └── HCSR04.c                # Driver/Librería para sensor ultrasónico
│
├── hmi-desktop/
│   ├── Form1.cs                    # Lógica HMI, puerto serie y graficado
│   ├── Form1.Designer.cs           # Diseño de interfaz WinForms
│   └── ControlTanque.csproj        # Proyecto C#
│
└── docs/
    └── database_schema.sql         # Script SQL Server para registro de telemetría

```

## Componentes del Sistema

### 1. Sistema de Control PID Distribuido (Arquitectura 2 PICs)

Diseñado para desacoplar las tareas matemáticas pesadas en tiempo real de los tiempos de retardo que requieren los actuadores de potencia.

* **PIC Maestro (`PIC1_PID_Master.c`):**

  * Lee el sensor ultrasónico HC-SR04 en el puerto B.

  * Mantiene el cálculo del PID discreto:
    

    $$
    u(t) = K_p \cdot e(t) + K_i \int e(t)dt + K_d \frac{de(t)}{dt}
    $$

    
    *(Constantes configuradas:* $K_p = 0.5658$*,* $K_i = 0.9433$*,* $K_d = 0.05$*)*.

  * Muestra las variables `Ctrl`, `Altura` y `%` en una pantalla LCD 16x2.

  * Mapea la acción de control $u(t)$ en 7 rangos discretos transmitidos mediante el Puerto C (pines `C0`–`C6`) hacia el esclavo.

* **PIC Esclavo (`PIC2_Actuadores_Slave.c`):**

  * Lee las líneas discretas enviadas por el maestro.

  * Controla un motor a pasos de 4 fases (Puerto D) para abrir/cerrar mecánicamente la válvula proporcional.

  * Ejecuta la conmutación y temporización de las bombas de llenado (`PIN_E0`) y vaciado (`PIN_E1`).

### 2. Firmwares de Planta Embotelladora y Telemetría

* **Módulo para Interfaz C# (`PIC_Embotelladora_CSharp.c`):**

  * Mantiene el formateo estricto de tramas delimitadas por dos puntos:
    `[BOTELLAS_COMPLETADAS]:[PORCENTAJE]:[VOLUMEN]:[ESTADO]`

  * Gestiona la secuencia de transporte por motor a pasos y tiempo de llenado por lote.

* **Módulo de Depuración Asíncrono (`PIC_Embotelladora_Debug.c`):**

  * Sustituye retardos bloqueantes (`delay_ms`) por temporización basada en la interrupción por desbordamiento del **Timer0**.

  * Emite cadenas de texto legibles (`INFO:`, `ALERTA:`) para validación mediante terminal serie (Hercules/PuTTY).

### 3. Interfaz SCADA y Base de Datos (C# WinForms + SQL Server)

* **Comunicación RS232:** Escucha continua en segundo plano mediante el evento `DataReceived` a 9600 baudios.

* **Gestión de Datos:** Convierte la trama recibida, actualiza indicadores numéricos, barras de progreso y realiza el cálculo de error respecto al Setpoint introducido por el usuario.

* **Graficado Dinámico:** Renderiza la respuesta temporal del sistema (Setpoint vs. Nivel actual).

* **Persistencia SQL Server:** Guarda cada muestra recibida en la tabla `DatosTanque` para análisis posterior de datos e historial de operación.

## Especificaciones Técnicas

* **Microcontrolador:** Microchip PIC16F877A @ 4 MHz (Oscilador de Cristal XT).

* **Compilador C:** CCS C Compiler.

* **Velocidad de Comunicación Serial:** 9600 Baudios (8N1).

* **Entorno HMI:** .NET Framework / C# WinForms.

* **Motor a Pasos:** Unipolar / Bipolar accionado por secuencia de paso medio/completo `{1, 5, 4, 12, 8, 10, 2, 3}`.

* **Sensor de Nivel:** HC-SR04 (Ultrasónico, rango de medición ajustado con offset de 2.86 cm y altura máxima de 12.85 cm).

## Historial de Desarrollo

* **18/12/2024:** Pruebas de integración del lazo cerrado PID, desarrollo de la arquitectura de 2 PICs y estructura de recepción de la HMI en C#.

## El proyecto fue desarrollado por:

Jose Amezcua