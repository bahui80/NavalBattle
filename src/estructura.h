/* tPosicion es una estructura que contiene dos variables enteras,
 * fila y columna. Esta estructura sirve para generar las coordenadas 
 * de ataque, de posicion de los barcos, etc
 */
typedef struct
{
	int fila;
	int columna;
}tPosicion;


/* tTablero es una estructura que contiene los tableros de cada jugador.
 * Los tableros son ambos matrices de enteros. Tambien contiene las filas
 * y las columnas de dichas matrices. Esas filas y columnas se extraen 
 * del archivo tematicas.
 */
typedef struct
{
	int ** tablero1;
	int ** tablero2;
	int cols;
	int filas;
}tTablero;



/* tCantBarco es una estructura de cada uno de los barcos de cada tipo de 
 * barco disinto que hay. Es decir, es un barco de los del tablero. Dicha
 * estructura se guarda la longitud del barco, las coordenadas de comienzo
 * y final del mismo y una variable entera (puesto) que se pone en uno
 * una vez que el barco fue insertado en el tablero.
 */
typedef struct
{
	tPosicion comienzo;
	tPosicion final;
	int longitud;
	int puesto;
}tCantBarco;



/* tBarco es una estructura de cada uno de los distintos tipos de barcos 
 * que hay. Dentro contiene el nombre de cada tipo de barco como asi tambien
 * la cantidad de barcos que hay de dicho tipo. A su vez contiene un puntero
 * a una estructura tCantBarco que es la que contiene cada barco por separado.
 */
typedef struct
{
	char * nombre;
	int cantidad;
	tCantBarco * cantbarcos;
}tBarco;



/* tJugador es la estructura de cada uno de los dos jugadores. Dentro contiene 
 * la cantidad de barcos distintos que hay (por ejemplo Caza, Destructor, etc), 
 * los tableros (dos uno de ubicacion personal de los barcos y otro de ataque)
 * de cada jugador y un puntero a la estructura tBarco que tiene todos los 
 * barcos del jugador.
 */
typedef struct
{
	tBarco * barcos;
	tTablero * tablero;
	int tiposbarcos;
}tJugador;


/* tJuego es la estructura general del juego que contiene todos los datos. 
 * En el main defino un puntero a tJuego que me va a servir para crear un tJuego
 * por cada tematica distinta leida del archivo. Dentro de tJuego tengo guardado
 * el numero de cada tema, el nombre de cada tema y tengo un puntero (vector)
 * a tJugador. Dicho vector jugador sera de dimension dos. Eso representara al
 * jugador humano y al jugador computadora. La posicion 0 se corresponde con la
 * del humano y la 1 con la de la computadora.
 */
typedef struct
{
	char * nombreTema;
	tJugador * jugador;
	int numTema;
}tJuego;
