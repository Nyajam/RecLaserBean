//
//HEAD
//
void initGcomand(gcomand* com); //Inicializa el estado del comando
byte translate(char* buff, gcomand* com); //Convierte la cadena recivida en un comando, salida: 0=>correcto, 1=> comentado
byte execute(gcomand* com); //Ejecuta comando, invoca otras funciones
byte gCode00(gcomand* com); //mover a velocidad maxima G00
byte gCode01(gcomand* com); //mover con velocidad especifica G01
byte gCodeM3(gcomand* com); //encender laser M3
byte gCodeM5(gcomand* com); //apagar laser M5
byte gCode28(gcomand* com); //volver al origen (0,0) G28
long conversor(float data); //traduce las unidades del gcode (parametro) a pasos (return) -> p/mm
void debugGcomand(gcomand* com); //imprime por serial los valores del comando

//
//CODE
//
void initGcomand(gcomand* com) //Inicializa el estado del comando
{
  com->code=255;
  com->finalP.x=0;
  com->finalP.y=0;
  com->state=0;
  com->radio=0;
  com->speed=0;
  com->laserPower=0;
}
void debugGcomand(gcomand* com) //imprime por serial los valores del comando
{
  Serial.print("\tCodigo:");
  Serial.println(com->code);
  Serial.print("\tX:");
  Serial.println(com->finalP.x);
  Serial.print("\tY:");
  Serial.println(com->finalP.y);
  Serial.print("\tState:");
  Serial.println(com->state);
  Serial.print("\tRadio:");
  Serial.println(com->radio);
  Serial.print("\tSpeed:");
  Serial.println(com->speed);
  Serial.print("\tLaser Power:");
  Serial.println(com->laserPower);
}
byte translate(char* buff, gcomand* com) //Convierte la cadena recivida en un comando, salida: 0=>correcto; 1=>error;
{
  initGcomand(com);
  point tmp={0,0};
  byte parm=1;
  //CODE
  tmp=getParameter(buff,0);
  switch(charUp(buff[tmp.x]))
  {
    case ';':
    case '#': //Comando comentado
      return 1;
      break;
    case 'G':
      com->code=0;
      for(int i=tmp.x+1;i<=tmp.y;i++)
        com->code=byte(buff[i]-'0')+com->code*10;
      break;
    case 'M':
      com->code=0;
      for(int i=tmp.x+1;i<=tmp.y;i++)
        com->code=byte(buff[i]-'0')+com->code*10;
      com->code=com->code+128;
      break;
  }
  //Resto de parametros (si hubiera)
  tmp=getParameter(buff,parm);
  while(tmp.x>=0&&tmp.y>=0)
  {
    float valorParam=0;
    for(int i=tmp.x+1;i<=tmp.y;i++)
    {
      if(com->state>0)
        com->state++;
      if(buff[i]=='.')
        com->state=1;
      else
        if(buff[i]>='0'&&buff[i]<='9')
            valorParam=valorParam*10+float(buff[i]-'0');
    }
    valorParam=valorParam/pow(10,(com->state-1));
    if(com->state==0)
      valorParam=valorParam/10;
    com->state=0;
    switch (charUp(buff[tmp.x]))
    {
      case 'X':
        com->finalP.x=conversor(valorParam);
        break;
      case 'Y':
        com->finalP.y=conversor(valorParam);
        break;
      case 'R':
        com->radio=int(conversor(valorParam));
        break;
      case 'F': //Speed (motor rpm)
      case 'S': //Speed
        com->speed=byte(conversor(valorParam));
        break;
    }
    tmp=getParameter(buff,++parm);
  }
  return 0;
}

byte gCode28(gcomand* com) //volver al origen (0,0) G28
{
  if(com->code!=28)
    return 2;
  home();
  if(posicion.x==0&&posicion.y==0)
    return 0;
  else
    return 1;
}

long conversor(float data) //traduce las unidades del gcode (parametro, en mm) a pasos (return)
{
  return long(data*KAY_PASS);
}
byte execute(gcomand* com) //Ejecuta comando, invoca otras funciones
{
  switch(com->code)
  {
    case 0: //G00
      return gCode00(com);
    case 1: //G01
      return gCode01(com);
    case 28: //G28
      return gCode28(com);
    case 131: //M3
      return gCodeM3(com);
    case 133: //M5
      return gCodeM5(com);
    default:
      return 255;
  }
}
byte gCode00(gcomand* com) //mover a velocidad maxima G00
{
  byte tmp;
  if(com->code!=0)
    return 2;
  com->code=1;
  tmp=gCode01(com);
  com->code=0;
  return tmp;
}
byte gCode01(gcomand* com) //mover con velocidad especifica G01
{
  if(com->code!=1)
    return 2;
  long timeStop=0; //Tiempo entre pasos
  if(com->speed!=0)
  {
    timeStop=conversor(com->speed); //mm/S a pasos/S
    timeStop=long(float(1000000/timeStop)); //pasos/S a mS por paso (delay funciona en micro segundos)
    timeStop=timeStop-DELAY_MOTOR_PASS; //Descontar el tiempo por defecto
  }
  unsigned int r=0; //Relacion de crecimiento
  bool sentidoX=com->finalP.x<posicion.x; //Marca el sentido de la x
  bool sentidoY=com->finalP.y<posicion.y; //Marca el sentido de la y
  bool sentidoR=abs(com->finalP.x-posicion.x)>abs(com->finalP.y-posicion.y); //Define el sentido de la relacion de los ejes: x/y o y/x
  if(abs(com->finalP.x-posicion.x)==0||abs(com->finalP.y-posicion.y)==0) //Caso lineal de un solo eje o ningun movimiento
  {
    int pasos;
    pasos=abs(com->finalP.x-posicion.x);
    for(int i=0;i<pasos;i++)
    {
      delayMicroseconds(timeStop);
      if(sentidoX)
      {
        if(shiftNX()>0)
          return 1;
      }
      else
      {
        if(shiftPX()>0)
          return 1;
      }
    }
    pasos=abs(com->finalP.y-posicion.y);
    for(int i=0;i<pasos;i++)
    {
      delayMicroseconds(timeStop);
      if(sentidoY)
      {
        if(shiftNY()>0)
          return 1;
      }
      else
      {
        if(shiftPY()>0)
          return 1;
      }
    }
    return 0;
  }
  if(sentidoR)
    r=abs(divRound(abs(com->finalP.x-posicion.x),abs(com->finalP.y-posicion.y)));
  else
    r=abs(divRound(abs(com->finalP.y-posicion.y),abs(com->finalP.x-posicion.x)));
  while(com->finalP.x!=posicion.x||com->finalP.y!=posicion.y) //Minetras que finalP sea distinto de posicion
  {
    if(sentidoR) // x/y que es mayor o igual que 1
    {
      for(int i=0;i<r&&com->finalP.x!=posicion.x;i++) //Por cada y++, r+x (o menos)
      {
        delayMicroseconds(timeStop);
        if(sentidoX)
        {
          if(shiftNX()>0)
            return 1;
        }
        else
        {
          if(shiftPX()>0)
            return 1;
        }
      }
      if(sentidoY) //y++ (o menos)
      {
        if(com->finalP.y < posicion.y) //Caso creciente
        {
          delayMicroseconds(timeStop);
          if(shiftNY()>0)
            return 1;
        }
      }
      else
      {
        if(com->finalP.y > posicion.y) //Caso menguante
        {
          delayMicroseconds(timeStop);
          if(shiftPY()>0)
            return 1;
        }
      }
      if(sentidoX) //Corrige, en caso de faltar menos que la relacion o si fuera a pasar, los pasos restantes para el final
      {
        if(com->finalP.x > posicion.x) //Caso creciente
          if(abs(com->finalP.x-posicion.x)<r)
            for(int i=0;i<abs(com->finalP.x-posicion.x);i++)
            {
              delayMicroseconds(timeStop);
              if(shiftPX()>0)
                return 1;
            }
      }
      else
      {
        if(com->finalP.x < posicion.x) //Caso menguante
          if(abs(com->finalP.x-posicion.x)<r)
            for(int i=0;i<abs(com->finalP.x-posicion.x);i++)
            {
              delayMicroseconds(timeStop);
              if(shiftNX()>0)
                return 1;
            }
      }
    }
    else // y/x que es mayor o igual que 1
    {
      for(int i=0;i<r&&com->finalP.y!=posicion.y;i++) //Por cada x++, r+y (o menos)
      {
        delayMicroseconds(timeStop);
        if(sentidoY)
        {
          if(shiftNY()>0)
            return 1;
        }
        else
        {
          if(shiftPY()>0)
            return 1;
        }
      }
      if(sentidoX) //x++ (o menos)  sentidoX= com->finalP.x < posicion.x
      {
        if(com->finalP.x < posicion.x) //Caso creciente
        {
          delayMicroseconds(timeStop);
          if(shiftNX()>0)
            return 1;
        }
      }
      else
      {
        if(com->finalP.x > posicion.x) //Caso menguante
        {
          delayMicroseconds(timeStop);
          if(shiftPX()>0)
            return 1;
        }
      }
      if(sentidoY) //Corrige, en caso de faltar menos que la relacion o si fuera a pasar, los pasos restantes para el final
      {
        if(com->finalP.y < posicion.y) //Caso creciente
          if(abs(com->finalP.y-posicion.y)<r)
            for(int i=0;i<abs(com->finalP.y-posicion.y);i++)
            {
              delayMicroseconds(timeStop);
              if(shiftNY()>0)
                return 1;
            }
      }
      else
      {
        if(com->finalP.y > posicion.y) //Caso menguante
          if(abs(com->finalP.y-posicion.y)<r)
            for(int i=0;i<abs(com->finalP.y-posicion.y);i++)
            {
              delayMicroseconds(timeStop);
              if(shiftPY()>0)
                return 1;
            }
      }
    }
  }
  return 0;
}
byte gCodeM3(gcomand* com) //encender laser M3
{
  int tmp=0;
  if(com->code!=131)
    return 2;
  if(com->speed>0)
  {
    tmp=map(com->speed, 1, 100, 1, 255);
    if(com->speed>100)
      tmp=100;
    setPowerLaser(byte(tmp));
  }
  enableLaser();
  return 0;
}
byte gCodeM5(gcomand* com) //apagar laser M5
{
  if(com->code!=133)
    return 2;
  disableLaser();
  return 0;
}
