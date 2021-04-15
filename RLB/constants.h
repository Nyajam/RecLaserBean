//
//CONSTANTS
//
const unsigned long X_MAX=1789; //Maximo del lienzo en el eje x
const unsigned long Y_MAX=1100; //Maximo del lienzo en el eje y
const unsigned int DELAY_MOTOR_PASS=1500; //Tiempo en micro segundos que tarda el controlador del motor paso paso en recoger la instruccion del paso
const unsigned int DELAY_MOTOR_G=5; //Tiempo en micro segundos que tarda el controlador del motor paso paso en recoger la instruccion del sentodo de giro
const float KAY_PASS=4.878; //Relacion paso-distancia para convertir los mm en pasos (unidad en paso/mm)
const byte PIN_DMOTOR_X=25; //Pin de control (direccion) del motor paso a paso del eje x
const byte PIN_MMOTOR_X=27; //Pin de control (movimiento) del motor paso a paso del eje x
const byte PIN_DMOTOR_Y=24; //Pin de control (direccion) del motor paso a paso del eje y
const byte PIN_MMOTOR_Y=26; //Pin de control (movimiento) del motor paso a paso del eje y
const byte PIN_PULSADOR_X=23; //Pin del pulsador de fin de carrera del eje X
const byte PIN_PULSADOR_Y=22; //Pin del pulsador de fin de carrera del eje Y
const byte PIN_ERROR=13; //Pin de led de error
const byte PIN_ACTION=2; //Pin del pulsador de "accion"
const byte PIN_SD=4; //Pin especificado por el hardware para tarjeta SD
const byte PIN_LASER_PWM=5; //Pin de control de potencia del laser
const boolean DERECHA=HIGH; //Valor asociado a la derecha en el giro de los motores
const boolean IZQUIERDA=LOW; //Valor asociado a la izquierda en el giro de los motores
const byte BUFF_SIZE=128; //Longitud del array de char
const char* GCODE_EXTENSION=".GCO"; //Nombre de la extension de los gcode (en mayusculas), SOLO LAS TRES PRIMERAS LETRAS DE LA EXTENSION CON EL PUNTO
const int ACTION_TIMING=500; //Tiempo de desecho (mS) para prevenir el efecto rebote en las interrupciones
const bool DEBUG_MOTORS=false; //Habilita (o no) que los driver impriman por señal la nueva posicion (x,y)
const char* FILE_ROOT="/"; //Directorio inicial de busqueda de ficheros

//
//MESSAGES LOOP FUNCTION
//

const char* WALL_RECORD=">-------------------------------------<";
const char* RECORD_NOT_START="Espera de accion";
const char* MSG_AT_START_RECORS="Accion iniciada";
const char* SD_NOT_FOUND="Error en SD";
const char* SD_FOUND="SD operativa";
const char* TITTLE_SD_LOOP_SEARCH="Files list";
const char* FILE_SELECTED="File selected name: ";
const char* FILE_SELECTED_CODE="File selected exit state: ";
const char* FILE_IS_GOOD="good";
const char* FILE_IS_EXIST="Fichero encontrado";
const char* FILE_HAVE_PROBLEM="Error al abrir el fichero";
const char* READ_FILE_GCODE="Bucle de ejecucion de instruccion";
const char* TITTLE_LINE_GCODE="Linea procesada: ";
const char* END_PREMATURE_FILE="Fin de archivo, salida anticipada";
const char* MY_POSITION="Posicion actual: ";
const char* FORMALIZED_COMMAND="Comando reconocido";
const char* ERROR_COMMAND="Comando mal ejecutado, grabacion pausada";
const char* GOOD_EXECUTE="Comando bien ejecutado";
const char* COMMAND_COMENTED="Comando comentado";
const char* BAD_COMAND="Comando erroneo, grabacion pausada";
const char* POSITION_AFTER_COMMAND="Posicion post-comando: ";
const char* END_FILE_NATURAL="Fin de archivo, salida principal";
const char* FILE_IS_BAD="bad";
const char* CLOSE_SD="Cierre de la SD";
