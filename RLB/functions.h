//
//HEAD
//
void enableLaser(); //Encender laser
void disableLaser(); //Apagar laser
boolean statLaser(); //Obtener estado (on|off) del laser
void setPowerLaser(byte power); //Fijar potencia del laser
byte getPowerLaser(); //Ver potencia fijada del laser
void alarm(); //Activa la alarma fisica
void cleanAlarm(); //Apaga la alarma
boolean charClean(char c); //Verifica si el caracter es despreciable
char charUp(char c); //Retorna un caracter a mayuscula
point getParameter(char* buff, byte index); //Devuelve el ifirst y el ilast del parametro index de buff
long divRound(long a, long b); //Divide a entre b (enteros) y retorna la division redondeada
bool charLastContains(char* a, char* b); //comprueba si a termina con b
void debugPosMotors(); //Imprime por serial la posicion actual de los motores
void debugPos(); //Imprime por serial la posicion actual
void cleanBuff(); //Limpia el buffer
void setAction(); //Cambia el estado de la accion
//
//CODE
//
void enableLaser() //Encender laser
{
  analogWrite(PIN_LASER_PWM,laserP);
  laserE=true;
}
void disableLaser() //Apagar laser
{
  analogWrite(PIN_LASER_PWM,0);
  laserE=false;
}
boolean statLaser() //Obtener estado (on|off) del laser
{
  return laserE;
}
void setPowerLaser(byte power) //Fijar potencia del laser
{
  laserP=power;
}
byte getPowerLaser() //Ver potencia fijada del laser
{
  return laserP;
}
void alarm() //Activa la alarma fisica
{
  digitalWrite(PIN_ERROR,HIGH);
}
void cleanAlarm() //Apaga la alarma
{
  digitalWrite(PIN_ERROR,LOW);
}
boolean charClean(char c) //Verifica si el caracter es despreciable
{
  switch(c)
  {
    case 0:
      return true;
      break;
    case ' ':
      return true;
      break;
    case '\t':
      return true;
      break;
    case '\n':
      return true;
      break;
    default:
      return false;
  }
}
char charUp(char c) //Retorna un caracter a mayuscula
{
  if(c<=122&&c>=97)
    return (c-32);
  return c;
}
point getParameter(char* buff, byte index) //Devuelve el ifirst y el ilast del parametro index de buff
{
  point res={-1,-1};
  byte pos=0; //Parametro actual
  for(byte i=0;i<BUFF_SIZE;i++)
  {
    if(!charClean(buff[i])&&i!=BUFF_SIZE-1) //No es un caracter de separacion y no es el ultimo elemento
    {
      if(i>0)
        if(charClean(buff[i-1])) //Si es el primer caracter del parametro del buff
          pos++;
      if(pos==index&&res.x<0)
        res.x=long(i);
    }
    else //Caso de no ser un caracter de separacion
    {
      if(res.x>=0)
      {
        if(!charClean(buff[i])&&i==BUFF_SIZE-1) //Si es un caracter valido y es el ultimo
          res.y=long(i);
        else //Carater final de parametro
          res.y=long(i-1);
        i=BUFF_SIZE;
      }
    }
  }
  return res;
}
long divRound(long a, long b) //Divide a entre b (enteros) y retorna la division redondeada
{
  float tmp;
  tmp=(float(a)/float(b))-float(a/b);
  if(tmp>=0.5)
    return (a/b)+1;
  return a/b;
}
void setAction() //Cambia el estado de la accion
{
  if(millis()>timing+ACTION_TIMING) //Prevencion de efecto rebote
  {
    flagAction=(!flagAction);
    timing=millis();
  }
}
bool charLastContains(char* a, char* b) //comprueba si a termina con b
{
  //.gcode
  int i=0;
  int k=0;
  while(a[i]!=0)
  {
    if(charUp(a[i])==charUp(b[k]))
      k++;
    else
      k=0;
    i++;
  }
  return b[k]==0;
}
void debugPosMotors() //Imprime por serial la posicion actual de los motores
{
  if(!DEBUG_MOTORS)
    return;
  Serial.print('\t');
  Serial.print('(');
  Serial.print(posicion.x);
  Serial.print(',');
  Serial.print(posicion.y);
  Serial.println(')');
}
void cleanBuff() //Limpia el buffer
{
  for(int i=0;i<BUFF_SIZE;i++)
    buffer[i]=0;
}
void debugPos() //Imprime por serial la posicion actual
{
  Serial.print('\t');
  Serial.print('(');
  Serial.print(posicion.x);
  Serial.print(',');
  Serial.print(posicion.y);
  Serial.println(')');
}
