//
//HEAD
//
//(X,Y)
void home(); //Ir al (0,0)
byte shiftPX(); //Desplazamiento (+1,0) x positiva
byte shiftPY(); //Desplazamiento (0,+1) y positiva
byte shiftPD(); //Desplazamiento (+1,+1) diagonal positiva
byte shiftNX(); //Desplazamiento (-1,0) x negativa
byte shiftNY(); //Desplazamiento (0,-1) y negativa
byte shiftND(); //Desplazamiento (-1,-1) diagonal negativa
byte shiftIPD(); //Desplazamiento (-1,+1) diagonal postiva hacia x=0
byte shiftIND(); //Desplazamiento (+1,-1) diagonal negativa hacia y=0
boolean isEndX(); //Check max x
boolean isStartX(); //Check 0 x
boolean isEndY(); //Check max y
boolean isStartY(); //Check 0 y
boolean chkPulsadorX(); //Comprueba si el pulsador del eje X esta pulsado
boolean chkPulsadorY(); //Comprueba si el pulsador del eje Y esta pulsado

//
//CODE
//
void home() //Ir al (0,0)
{
  while( !chkPulsadorX() || !chkPulsadorY() ) //Mientras que ninguno de los pulsadores este pulsado
  {
    if(!chkPulsadorX())
    {
      posicion.x=1;
      shiftNX();
    }
    if(!chkPulsadorY())
    {
      posicion.y=1;
      shiftNY();
    }
  }
  posicion.x=0;
  posicion.y=0;
}
byte shiftPX() //Desplazamiento (+1,0) x positiva
{
  if(isEndX())
    return 1;
  posicion.x++;
  digitalWrite(PIN_DMOTOR_X,DERECHA);//Fijar sentido
  delayMicroseconds(DELAY_MOTOR_G);//Tiempo de controlador
  digitalWrite(PIN_MMOTOR_X,HIGH);//Inicio giro
  delayMicroseconds(DELAY_MOTOR_PASS);//Tiempo de controlador
  digitalWrite(PIN_MMOTOR_X,LOW);//Parada de giro
  debugPosMotors();
  return 0;
}
byte shiftPY() //Desplazamiento (0,+1) y positiva
{
  if(isEndY())
    return 1;
  posicion.y++;
  digitalWrite(PIN_DMOTOR_Y,DERECHA);//Fijar sentido
  delayMicroseconds(DELAY_MOTOR_G);//Tiempo de controlador
  digitalWrite(PIN_MMOTOR_Y,HIGH);//Inicio giro
  delayMicroseconds(DELAY_MOTOR_PASS);//Tiempo de controlador
  digitalWrite(PIN_MMOTOR_Y,LOW);//Parada de giro
  debugPosMotors();
  return 0;
}
byte shiftPD() //Desplazamiento (+1,+1) diagonal positiva
{
  if(isEndX()&&isEndY())
    return 1;
  posicion.x++;
  posicion.y++;
  digitalWrite(PIN_DMOTOR_X,DERECHA);//Fijar sentido
  digitalWrite(PIN_DMOTOR_Y,DERECHA);//Fijar sentido
  delayMicroseconds(DELAY_MOTOR_G);//Tiempo de controlador
  digitalWrite(PIN_MMOTOR_X,HIGH);//Inicio giro
  digitalWrite(PIN_MMOTOR_Y,HIGH);//Inicio giro
  delayMicroseconds(DELAY_MOTOR_PASS);//Tiempo de controlador
  digitalWrite(PIN_MMOTOR_X,LOW);//Parada de giro
  digitalWrite(PIN_MMOTOR_Y,LOW);//Parada de giro
  debugPosMotors();
  return 0;
}
byte shiftNX() //Desplazamiento (-1,0) x negativa
{
  if(isStartX())
    return 1;
  posicion.x--;
  digitalWrite(PIN_DMOTOR_X,IZQUIERDA);//Fijar sentido
  delayMicroseconds(DELAY_MOTOR_G);//Tiempo de controlador
  digitalWrite(PIN_MMOTOR_X,HIGH);//Inicio giro
  delayMicroseconds(DELAY_MOTOR_PASS);//Tiempo de controlador
  digitalWrite(PIN_MMOTOR_X,LOW);//Parada de giro
  debugPosMotors();
  return 0;
}
byte shiftNY() //Desplazamiento (0,-1) y negativa
{
  if(isStartY())
    return 1;
  posicion.y--;
  digitalWrite(PIN_DMOTOR_Y,IZQUIERDA);//Fijar sentido
  delayMicroseconds(DELAY_MOTOR_G);//Tiempo de controlador
  digitalWrite(PIN_MMOTOR_Y,HIGH);//Inicio giro
  delayMicroseconds(DELAY_MOTOR_PASS);//Tiempo de controlador
  digitalWrite(PIN_MMOTOR_Y,LOW);//Parada de giro
  debugPosMotors();
  return 0;
}
byte shiftND() //Desplazamiento (-1,-1) diagonal negativa
{
  if(isStartX()&&isStartY())
    return 1;
  posicion.x--;
  posicion.y--;
  digitalWrite(PIN_DMOTOR_X,IZQUIERDA);//Fijar sentido
  digitalWrite(PIN_DMOTOR_Y,IZQUIERDA);//Fijar sentido
  delayMicroseconds(DELAY_MOTOR_G);//Tiempo de controlador
  digitalWrite(PIN_MMOTOR_X,HIGH);//Inicio giro
  digitalWrite(PIN_MMOTOR_Y,HIGH);//Inicio giro
  delayMicroseconds(DELAY_MOTOR_PASS);//Tiempo de controlador
  digitalWrite(PIN_MMOTOR_X,LOW);//Parada de giro
  digitalWrite(PIN_MMOTOR_Y,LOW);//Parada de giro
  debugPosMotors();
  return 0;
}
byte shiftIPD() //Desplazamiento (-1,+1) diagonal postiva hacia x=0
{
  if(isStartX()&&isStartY())
    return 1;
  posicion.x--;
  posicion.y++;
  digitalWrite(PIN_DMOTOR_X,IZQUIERDA);//Fijar sentido
  digitalWrite(PIN_DMOTOR_Y,DERECHA);//Fijar sentido
  delayMicroseconds(DELAY_MOTOR_G);//Tiempo de controlador
  digitalWrite(PIN_MMOTOR_X,HIGH);//Inicio giro
  digitalWrite(PIN_MMOTOR_Y,HIGH);//Inicio giro
  delayMicroseconds(DELAY_MOTOR_PASS);//Tiempo de controlador
  digitalWrite(PIN_MMOTOR_X,LOW);//Parada de giro
  digitalWrite(PIN_MMOTOR_Y,LOW);//Parada de giro
  debugPosMotors();
  return 0;
}
byte shiftIND() //Desplazamiento (+1,-1) diagonal negativa hacia y=0
{
  if(isStartX()&&isStartY())
    return 1;
  posicion.x++;
  posicion.y--;
  digitalWrite(PIN_DMOTOR_X,DERECHA);//Fijar sentido
  digitalWrite(PIN_DMOTOR_Y,IZQUIERDA);//Fijar sentido
  delayMicroseconds(DELAY_MOTOR_G);//Tiempo de controlador
  digitalWrite(PIN_MMOTOR_X,HIGH);//Inicio giro
  digitalWrite(PIN_MMOTOR_Y,HIGH);//Inicio giro
  delayMicroseconds(DELAY_MOTOR_PASS);//Tiempo de controlador
  digitalWrite(PIN_MMOTOR_X,LOW);//Parada de giro
  digitalWrite(PIN_MMOTOR_Y,LOW);//Parada de giro
  debugPosMotors();
  return 0;
}
boolean isEndX() //Check max x
{
  return posicion.x==X_MAX;
}
boolean isStartX() //Check 0 x
{
  return posicion.x==0;
}
boolean isEndY() //Check max y
{
  return posicion.y==Y_MAX;
}
boolean isStartY() //Check 0 y
{
  return posicion.y==0;
}
boolean chkPulsadorX() //Comprueba si el pulsador del eje X esta pulsado
{
  return digitalRead(PIN_PULSADOR_X)==HIGH;
}
boolean chkPulsadorY() //Comprueba si el pulsador del eje Y esta pulsado
{
  return digitalRead(PIN_PULSADOR_Y)==HIGH;
}
