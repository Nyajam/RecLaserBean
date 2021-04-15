struct point //Vector de representación espacial (x,y)
{
  long x;
  long y;
};

struct gcomand //Comando para los gcode
{
  byte code; //Codigo del gcode, los 128 primero numeros son G, el resto es M
  point finalP; //Posicion inicial
  byte state; //Estado del comando, el significado varia segun la ultima funcion que lo procesara
  int radio; //Si se tiene que hacer una curva, se indica el radio
  byte speed; //Velocidad que tiene que ir (mm/s)
  byte laserPower; //Potencia de grabado del laser
};
