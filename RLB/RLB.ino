/*
Nombre Proyecto: RecLaserBean
Version: 0.0
Autores: Ildefonso Macarro Pueyo, Javier Lamparero Lopez
Fecha: 20/01/2021
Placa empleada: Arduino Mega 2560
Shield: Arduino Ethernet SHIELD
Version de Arduino en diseño: 1.8.13
*/

//External libs
#include <SPI.h>
#include <SD.h>

//My libs
#include "structs.h"
#include "constants.h"
#include "vars.h"
#include "functions.h"
#include "move_motor.h"
#include "gcode.h"

void setup()
{
  pinMode(PIN_ERROR,OUTPUT);
  pinMode(PIN_DMOTOR_X,OUTPUT);
  pinMode(PIN_MMOTOR_X,OUTPUT);
  pinMode(PIN_DMOTOR_Y,OUTPUT);
  pinMode(PIN_MMOTOR_Y,OUTPUT);
  pinMode(PIN_LASER_PWM,OUTPUT);
  pinMode(PIN_PULSADOR_X,INPUT);
  pinMode(PIN_PULSADOR_Y,INPUT);
  pinMode(PIN_ACTION,INPUT);
  Serial.begin(9600);
  cleanAlarm();
  flagAction=false;
  posicion.x=0;
  posicion.y=0;
  laserP=0;
  laserE=false;
  timing=0;
  cleanBuff();
  while(!SD.begin(PIN_SD))
    alarm();
  attachInterrupt(0,setAction, RISING);
  home();
}

void loop()
{
  File fileCode;
  bool execState=true;
  Serial.println(WALL_RECORD);
  Serial.println(RECORD_NOT_START);
  if(flagAction) //Se detecta accion
  {
    setAction();
    cleanBuff();
    Serial.println(MSG_AT_START_RECORS);
    if(!SD.begin(PIN_SD)) //Lector y tarjeta disponible
    {
      Serial.println(SD_NOT_FOUND);
      alarm();
      execState=false; //Error
    }
    else
    {
      cleanAlarm();
      fileCode=SD.open(FILE_ROOT);
    }
    if(fileCode&&execState) //Acceso a la SD verificado
    {
      cleanAlarm();
      Serial.println(SD_FOUND);
      Serial.println(TITTLE_SD_LOOP_SEARCH);
      execState=false;
      do //Buscar primer fichero con la extension adecuada
      {
        fileCode=fileCode.openNextFile();
        Serial.println(fileCode.name());
        //if(!fileCode.isDirectory()) //Prime fichero
        if(!fileCode.isDirectory()&&charLastContains(fileCode.name(),(char*)GCODE_EXTENSION)) //Si es un fichero y tiene la extension adecuada
        {
          execState=true;
          break;
        }
      } while(fileCode);
      Serial.print(FILE_SELECTED);
      Serial.println(fileCode.name());
      Serial.print(FILE_SELECTED_CODE);
      if(execState)
      {
        Serial.println(FILE_IS_GOOD);
        Serial.println(FILE_IS_EXIST);
        //Fichero encontrado
        if(!fileCode.available()) //No se puede abrir o esta vacio
        {
          Serial.println(FILE_HAVE_PROBLEM);
          alarm();
          execState==255;
        }
        else
        {
          gcomand comando;
          initGcomand(&comando);
          while(fileCode.available())
          {
            execState=false;
            Serial.println(READ_FILE_GCODE);
            int iterationBuff=0;
            char readComand[]={0}; //Tiene tener un valor inicial para evitar no leer la instruccion a partir de la segunda iteracion
            if(flagAction) //accion -> si la ejecucion esta en proceso la detiene y viceversa
            {
              Serial.println("STOP");
              bool laserLastState=statLaser();
              if(laserLastState)
                disableLaser();
              while(flagAction)
                while(flagAction);
              if(laserLastState)
                enableLaser();
                Serial.println("START");
            }
            cleanBuff();
            while(readComand[0]!='\n') //Carga una sola instruccion
            {
              fileCode.read(readComand,1);
              if(readComand[0]<0||iterationBuff==BUFF_SIZE) //Termina la lectura, bien por falta de espacio en el buffer o por terminar la lectura de archivo
              {
                if(readComand[0]<0)
                {
                  execState=true;
                }
                break;
              }
              if(readComand[0]!='\n')
                buffer[iterationBuff++]=readComand[0];
            }
            Serial.print(TITTLE_LINE_GCODE);
            Serial.println(buffer);
            if(execState) //Salir de la ejecucion al terminar de leer el archivo
            {
              Serial.println(END_PREMATURE_FILE);
              break;
            }
            Serial.print(MY_POSITION);
            debugPos();
            switch(translate(buffer, &comando))
            {
              case 0: //Comando reconocido
              {
                Serial.println(FORMALIZED_COMMAND);
                debugGcomand(&comando);
                if(execute(&comando)<0) //Error al efectuar la ejecucion de la orden; poner a la espera de arranque
                {
                  Serial.println(ERROR_COMMAND);
                  alarm();
                  bool laserLastState=statLaser();
                  if(laserLastState)
                    disableLaser();
                  while(flagAction)
                    while(flagAction);
                  if(laserLastState)
                    enableLaser();
                  cleanAlarm();
                }
                else
                  Serial.println(GOOD_EXECUTE);
                break;
              }
              case 1: //Es un comentario, no hacer nada
                Serial.println(COMMAND_COMENTED);
                break;
              default: //Comando no reconocido; poner a la espera de arranque
              {
                Serial.println(BAD_COMAND);
                alarm();
                bool laserLastState=statLaser();
                if(laserLastState)
                  disableLaser();
                while(flagAction)
                  while(flagAction);
                if(laserLastState)
                  enableLaser();
                cleanAlarm();
              }
            }
            Serial.print(POSITION_AFTER_COMMAND);
            debugPos();
          }
          setPowerLaser(12);
          disableLaser();
        }
        Serial.println(END_FILE_NATURAL);
      }
      else
        Serial.println(FILE_IS_BAD);
      Serial.println(CLOSE_SD);
      fileCode.close();
      home();
    }
    else
      alarm();
  }
  delay(1000);
}
