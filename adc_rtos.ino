// +++++++++ TIEMPO REAL +++++++++++++++++
// Codigo para usarse en ATMEGA328P - Arduino Nano
#include "FreeRTOS_AVR.h"
//#include "FreeRTOS_ARM.h"
#include "basic_io_avr.h"
// ++++++++++++++ Parametros +++++++++++++++
//Funciones de las tareas
void medirTask(void *pvParameters);
void medir(void);
void transTask(void *pvParameters);
void trans(void);
//Se crean los Strings que seran pasados como parametros a las tareas.
const char *pcTextForTask1 = "MEDIR en Ejecucion\r\n";
const char *pcTextForTask2 = "TRANS en Ejecucion\t\n";
// ++++++++++++++ Variables ++++++++++++++++
int vmedio = 0;
int vrms = 0;
int muestra =0;
bool flag = true;
int LedPin1=13;
int LedPin2=12;
void setup() {
  Serial.begin(115200);
  Serial.println("Arranque...");
  //++++++++++++ Trend +++++++++++++++++++++++++++++
  delay(2000);
  //+++++++++++ TIEMPO REAL ++++++++++++++++++++++
  //Creando una de las tareas
  //xTaskCreate(Puntero de la funcion de la tarea, nombre para la tarea, tamaño de la pila, Null, Prioridad, null)
  xTaskCreate(medirTask, (signed char*)"medirTask ", 200, (void*)pcTextForTask1, 1, NULL);  
  //Creando otra tarea
  xTaskCreate(transTask, (signed char*)"transTask ", 200, (void*)pcTextForTask2, 2, NULL);
  //Inicia el planificador, las tareas empiezan a ejecutarse
  vTaskStartScheduler();
}

void loop() {
  //Todo se ejecuta por RT
  }
//++++++++++++++++++ Tareas +++++++++++++++++++++++++++
void medirTask(void *pvParameters){                   //Se le asigna un parametro a la tarea.
  char *pcTaskName;
  pcTaskName = (char*) pvParameters;                  //Este parametro es de tipo Caracter y corresponde al nombre de la tarea.
  TickType_t xLastWakeTime;                           //Fijar la periocidad de ejecucion de las tareas (Frecuencia fija)
  xLastWakeTime =xTaskGetTickCount();                 //Obtiene el numero de ticks de interrupcion para la periocidad de las tareas.
  for(;;){
    medir();                                          //Esta tarea debe ejecutarse exactamente cada 600ms (el tiempo es medido en ticks)   
    vTaskDelayUntil(&xLastWakeTime, (600/portTICK_PERIOD_MS));
    flag = false;
  }
}
void transTask(void *pvParameters){                   //Se le asigna un parametro a la tarea.
  char *pcTaskName;
  pcTaskName = (char*) pvParameters;                  //Este parametro es de tipo Caracter y corresponde al nombre de la tarea.
  TickType_t xLastWakeTime;                           //Fijar la periocidad de ejecucion de las tareas (Frecuencia fija)
  xLastWakeTime =xTaskGetTickCount();                 //Obtiene el numero de ticks de interrupcion para la periocidad de las tareas.
  for(;;){
    trans();                                          //Esta tarea debe ejecutarse exactamente cada 60ms (el tiempo es medido en ticks)   
    vTaskDelayUntil(&xLastWakeTime, (60/portTICK_PERIOD_MS));
    flag = true;
  }
}
//+++++++++++++++ Metodos +++++++++++++++++++++++++++++
void medir(){
  //
  vmedio=0;
  muestra=0;
  int samples = 0;
  for (samples =0; samples <1000; samples++){
      int sensorValue = (5*(analogRead(A0)))/1023;
      Serial.println(sensorValue);
      digitalWrite(LedPin1, !digitalRead(LedPin1));
      vmedio=sensorValue + vmedio; 
      muestra++;
      }
  vrms= (vmedio/muestra)*1.11;
 }
 void trans(){
  // Transmitir este dato por Http para tendencia
  digitalWrite(LedPin2, !digitalRead(LedPin2));
  Serial.println(vrms);
 }
