/* Funcion que permite saltear los comentarios de un archivo
 * para poder seguir trabajando sobre el mismo sin problemas
 */
void salteaComentario(FILE * archivo);


/* Funcion que chequea un string leido desde el archivo. La funcion recibe
 * el archivo y el string esperado en el archivo y los compara. En caso
 * de error de memoria devuelve una costante simbolica ERROR_MEMORIA y
 * si todo anduvo bien devuelve 0. Si alguno de los dos era mas largo 
 * que el otro devuelve 1 o -1 tal como lo hace el strcmp.
 */ 
int chequeaString(FILE * archivo,char * string);


/* Funcion que controla que el numero de tematicas sea correcto (mayor
 * que cero y sea numero). Devuelve la cantidad de temas que contiene
 * el archivo. En caso de error devuelve ERROR_TEMATICAS.
 */
int validaIdTematicas(FILE * archivo,int * numTemas);


/* Funcion que valida que cada numero de tematica del archivo
 * sea correcto, es decir, sea mayor que cero, menor que la cantidad 
 * de tematicas y sea un numero. Devuelve 0 si el numero era valido y 
 * ERROR_TEMATICAS en caso contrario. Tambien guarda en la estructura
 * juego el respectivo numero de tematica
 */
int validaIdTematica(FILE * archivo,int numeroTem,tJuego * juego);


/* Funcion que carga y valida que el nombre de la tematica este correcto.
 * Si el nombre de la tematica es vacio devuelve ERROR_TEMATICAS. En caso 
 * contrario, guarda el nombre de la tematica en la estructura y devuelve
 * 0.
 */
int validaIdNombre(FILE * archivo,int numeroTem,tJuego * juego);


/* Funcion que valida las filas y las columnas del tablero. Carga dichos
 * datos en la estructura para cada jugador y tambien a partir de los datos
 * correctos crea los tableros de cada jugador dinamicamente. En caso que
 * haya error de memoria devuelve ERROR_MEMORIA y si hubo algun error
 * en la validacion de las filas y las columnas devuelve ERROR_TEMATICAS.
 */
int validaIdTablero(FILE * archivo, int numTema,tJuego * juego);


/* Funcion que valida la cantidad de elementos que tiene una tematica.
 * Verifica si es un numero y mayor que cero. Devuelve ERROR_TEMATICAS
 * si era incorrecto y la cantidad de elementos en caso contrario
 */
int validaIdCantFlotas(FILE * archivo,tJuego * juego,int i);


/*Funcion que valida que el numero de cada elemento sea valido y este 
 * en orden. Para que sea valido debe ser un numero mayor que cero y 
 * menor o igual que la cantidad de tematicas. Devuelve 0 si era todo 
 * correcto y ERROR_TEMATICAS en caso contrario
 */
int validaIdFlotas(FILE * archivo,int numFlota);


/* Funcion que valida que el nombre del cada elemento no este vacio.
 * Guarda dicho nombre en cada elemento de la estructura. Si hubo
 * un problema de memoria devuelve ERROR_MEMORIA, si el nombre estaba
 * vacio devuelve ERROR_TEMATICAS y devuelve 0 en caso que todo fuese 
 * correcto
 */
int validaCampoNombre(FILE * archivo,int numTema,int numeroElem, tJuego * juego);


/* Funcion que valida que la longitud de los barcos sea mayor o igual 
 * que cero. Valida tambien que luego de la longitud haya un - el cual
 * responde al formato del archivo de tematicas. En caso de error
 * devuelve ERROR_TEMATICAS y si todo era correcto devuelve la longitud
 * del barco en cuestion.
 */
int validaIdLongitud(FILE * archivo);


/* Funcion que verifica que la cantidad de barcos de un tipo distinto
 * sea mayor que cero. Si hubo error devuelve ERROR_TEMATICAS y devuelve 
 * el numero de barcos de un tipo de barco en caso contrario.
 * Guarda tambien estos datos en la estructura del juego.
 */
int validaIdCantidad(FILE * archivo,tJuego * juego,int i,int j);


/* Funcion que guarda la longitud de cada barco en la estructura. Recibe
 * el numero de tema, el total de los distintos tipos de barcos, la longitud
 * para esos barcos y el tipo de barco que se esta tratando. Tambien
 * asigna cero a la variable puesto en la estructura, la cual sirve para
 * saber si un barco ya se puso en el tablero o no.
 */
void guardaLongitud(tJuego * juego,int i,int j,int numerodebarcos,int largo);


/* Funcion que permite crear un tablero dinamicamente a partir de un 
 * parametro de entrada que son las filas y las columnas. Devuelve el 
 * tablero creado y rellena dicho tablero con el caracter ".".
 */	
int ** crearTablero(int filas, int cols);


/* Funcion que cuenta la cantidad total de barcos que tengo para una cierta
 * tematica. Recibe los barcos de dicha tematica y el numero total
 * de barcos distintos que tengo para dicha tematica. Devuelve el numero
 * total de barcos.
 */
int barcosTotales(tBarco * barcos, int tiposBarcos);


/* Funcion que valida que todos los datos del comando set sean validos.
 * Retorna ERROR_TIPOBARCO si el tipo de nave ingresada era invalido. Devuelve 
 * ERROR_NUMBARCO si el id de la nave ingresada era invalido. Devuelve 
 * ERROR_LIMITES_TABLERO si las coordenadas ingresadas por el usuario no eran 
 * validas. Devuelve ERROR_DIAGONAL si el usuario intento ingresar una nave
 * en posicion diagonal. Retorna ERROR_OCUPADO si el lugar donde se queria 
 * ubicar el barco no estaba rodeado de agua o ya estaba ocupado por un barco
 * previamente. Devuelve 0 si el comando fue ingresado correctamente.
 */
int validaDatos(int tipoNave,int idNave,tPosicion inicial,tPosicion final,int ** tablero,int filas,int cols,tBarco * barcos,int tiposbarcos);


/* Funcion que valida que las coordenadas elegidas para poner un barco no se 
 * encuentre ocupada ni rodeada de barcos. Retorna 0 si los casilleros no se 
 * encontraban ocupados ni rodeados y 1 en caso contrario. La funcion recibe las 
 * filas, las columnas, los barcos de un jugador y las coordenadas iniciales
 * y finales del mismo. Tambien recibe el tablero.
 */
int validaCasillero(int ** tablero,tPosicion inicial,tPosicion fin,int filas,int cols,tCantBarco barco);


/* Funcion que ordena las coordenadas ingresadas por el usuario, tanto las 
 * columnas como las filas y devuelve esos datos en 4 parametros de entrada.
 * Esto sirve para poder ubicar las piezas en caso que las coordenadas sean
 * dadas al reves
 */	
void asignaOrden(tPosicion inicial,tPosicion final,int * minfil,int * maxfil,int * mincol,int * maxcol);


/* Funcion que pone en el tablero el barco solicitado por el usuario. Se distinguen
 * dos casos, cuando se pone uno nuevo o cuando se reubica algun elemento. Los algoritmos
 * varian en cada caso. Devuelve 0 si se esta reubicando un elemento y 1 si se pone uno nuevo.
 * Esto permite saber cuantos barcos quedan por ubicar.
 */
int poneEnTablero(int ** tablero,tPosicion inicial,tPosicion fin,tBarco * barcos,int tipoNave,int idNave);


/* Funcion que valida el string ingresado por el usuario para ingresar interactivamente
 * las piezas en el tablero. Para analizarlo se realizo una maquina de estado en caso
 * que el comando ingresado sea set. Tambien guarda los valores ingresados.
 * Devuelve 1 si el comando ingresado fue done, ERROR_COMANDOS si el comando ingresado
 * era incorrecto y 0 si el comando ingresado era set.
 */
int analizaString(char * string,int * tipoNave,int * idNave,tPosicion * inicial,tPosicion * final);


/* Funcion que calcula la longitud a partir de dos coordenadas ingresadas
 * por el usuario. Devuelve dicho valor.
 */
int calculaLongitud(tPosicion inicial,tPosicion final);


/* Funcion que permite ubicar de manera aleatoria las piezas. Se usa para crear
 * el tablero de la computadora y en caso que el usuario desee el suyo tambien.
 * Devuelve 0 si el tablero se pudo crear correctamente y 1 en caso contrario.
 */
int ubicacionAleatoria(int ** tablero,int filas,int cols,tBarco * barcos,int totalbarcos,int tiposbarcos);


/* Funcion que creas las coordenadas para ubicar un barco de manera horizontal.
 * Recibe dos parametros de entrada/salida (inicial y final) donde guarda las coordenadas
 * generadas al azar.
 */
void aleatoriaHorizontal(int filas,int cols,int longitud,tPosicion * inicial,tPosicion * final);


/* Funcion que crea las coordenadas para ubicar el barco de manera aleatoria 
 * en posicion vertical. Recibe dos parametros de entrada/salida (inicial y final)
 * donde se guardan las coordenadas de ubicacion del barco.
 */
void aleatoriaVertical(int filas,int cols,int longitud,tPosicion * inicial,tPosicion * final);


/* Funcion que valida el string ingresado por el usuario para jugar en su turno.
 * Lo valida mediante una maquina de estados. Devuelve 0 si el comando ingresado fue
 * bomb, 1 si el comando ingresado fue undo, 2 si el comando ingresado fue save y 
 * 3 si el comando ingresado fue quit. Devuelve ERROR_COMANDOS si el comando ingresado
 * no existia. Devuelve ERROR_VALORES_BOMB si las coordenadas del bomb eran invalidas y
 * ERROR_ATACADO si el usuario ataco en una coordenada previamente atacada.
 */	
int validaStringJuego(char * string,int * a,int * b,char * string2,int filas,int cols,int ** tablerojug);


/* Funcion que marca en el tablero (que ve el jugador luego de cada ataque)
 * el resultado del ataque realizado. Devuelve 1 si el usuario hundio un barco 
 * y 0 si la coordenada ingresada era agua o se toco un barco. Tambien se usa esta
 * funcion para marcar los ataques que realiza la computadora en su tablero de ataques
 */
int marcaTablero(int num,int a,int b,int ** tablerojug,int ** tableroPC,tBarco * barcos,int tiposbarcos);


/* Funcion que realiza el ataque del jugador. Valida si el casillero que se solicito
 * atacar tenia agua o barco. Ese valor se le pasa a la funcion marcaTablero que se 
 * encarga de ver si el barco fue hundido o tocado en caso que se hubiera atacado un barco.
 * La funcion devuelve 0 si el barco no fue hundido y 1 en caso contrario.
 */
int ataqueJugador(int a,int b,int ** tablerojug,int ** tableroPC,tBarco * barcos,int tiposbarcos,tPosicion * jugadajug);


/* Funcion que permite al usuario al haber ingresado el comando guardar, guardar 
 * los datos iniciales del juego (numero de tematica, turno siguiente y undos realizados).
 */
void guardaDatosIniciales(int tema,int turno,FILE * archivo,int undo);


/* Funcion que realiza la estrategia para que la computadora pueda atacar al
 * jugador en su turno. La estrategia se basa en buscar siempre si hay algun
 * barco tocado. En caso de haberlo busca en los casilleros aledaños hasta destruirlo
 * por completo. En caso que no haya ningun barco tocado busca el espacio mayor y ataca
 * aleatoriamente en dicho espacio. Va a devolver 1 si destruyo un barco y 0 en caso
 * contrario. Tambien va a llamar a otra funcion que rellena los tableros de acuerdo
 * al ataque.
 */
int estrategiaPC(int filas,int cols,int ** tablero2PC,int ** tablero1jug,tBarco * barcosjug,int tiposbarcos,tPosicion * jugadaPC);


/* Funcion que guarda en un archivo el estado de las coordenadas de los tableros
 * de cada jugador. Se la llama dos veces una para el jugador y otra para la computadora.
 */
void guardaDatosJugador(int tiposbarcos,tBarco * barcos,FILE * archivo,int ** tablero1jug,int ** tablero2jug,int filas,int cols,char * string);


/* Funcion que permite al usuario usar el comando undo. Esta funcion llama a otras
 * que realizan lo necesario para retroceder tanto la jugada del jugador como la de la 
 * computadora.
 */
void comandoUndo(tPosicion jugadaPC,tPosicion jugadajug,int ** tablero1PC,int ** tablero2PC,int ** tablero1jug,int ** tablero2jug,int tiposbarcos,tBarco * barcosjug,tBarco * barcosPC,int * totalbarcosjug,int * totalbarcosPC);


/* Funcion que realiza el algoritmo para buscar siempre el agujero mayor en el tablero
 * de ataques de la computadora y asi poder atacar. Dentro de ese agujero mayor elegido
 * se elige una posicion aleatoriamente. Todo esto lo realiza siempre y cuando no haya ningun
 * barco tocado ya que sino ataca hasta hundirlo.
 */
void eligeEspacioMayor(int filas,int cols,tPosicion * coordenada,int ** tablero2PC);


/* Funcion que elige una coordenada de ataque para la computadora. Elige esa coordenada de manera
 * mas conveniente. Devuelve 0 si en la coordenada elegida habia un "." y 1 si en la coordenada
 * elegida se impacto un barco.
 */
int eligeCoordenadaAtaque(tPosicion * coordenada,int filas,int cols,int ** tablero2PC,int ** tablero1jug);


/* Funcion que a partir de haber encontrado un tocado en el tablero permite a la
 * computadora buscar en la columna siguiente si hay un tocado. En caso de haber busca
 * en esa columna hasta encontrar una celda vacia contigua a un tocado y asi atacar
 * dicha celda. Devuelve 1 si encontro una coordenada vacia para atacar y 0 en caso
 * contrario.
 */
int coordenada1(tPosicion * coordenada,int i,int j,int cols,int ** tablero2PC);


/* Funcion que a partir de haber encontrado un tocado en el tablero permite a la
 * computadora buscar en la fila siguiente si hay un tocado. En caso de haber busca
 * en esa fila hasta encontrar una celda vacia contigua a un tocado y asi atacar
 * dicha celda. Devuelve 1 si encontro una coordenada vacia para atacar y 0 en caso
 * contrario.
 */
int coordenada2(tPosicion * coordenada,int i,int j,int filas,int ** tablero2PC);


/* Funcion que a partir de haber encontrado un tocado en el tablero permite a la
 * computadora buscar en los casilleros aledaños si hay un tocado. En caso de no 
 * haberlo, elige al azar uno de los 4 lugares posibles y lo guarda en el parametro
 * de entrada/salida coordenada. Devuelve 1 ya que siempre que entre a esta funcion
 * va a encontrar un lugar al cual atacar.
 */
int coordenada3(int i,int j,int ** tablero2PC,tPosicion * coordenada,int filas,int cols);


/* Funcion que sirve para rellenar el tablero de ataques de la computadora con agua
 * alrededor de un barco que fue hundido. En caso de que se impacte un barco y no se hunda,
 * esta funcion se obviara.
 */
void rellenaAgua(int ** tablero2PC,tBarco * barcosjug,int tiposbarcos,tPosicion * coordenada,int filas,int cols);


/* Funcion que desmarca el agua alrdededor de un barco en un tablero de ataques. Esta funcion sirve 
 * cuando se invoca al comando undo ya que puede suceder que se utilice el undo luego de haber
 * hundido un barco por lo cual hay que desmarcar el agua de alrededor del barco que habia 
 * sido destruido previo al undo. Tambien recibe un parametro de entrada/salida que modifica
 * el numero de barcos hundidos luego del undo si fuese necesario.
 */
void desmarcaTableroAtaqueJugador(int ** tablero2jug,tPosicion jugadajug,tBarco * barcos,int tiposbarcos,int * totalbarcosjug);


/* Funcion que valida que el numero de la tematica leido en un archivo cargado
 * exista. Devuelve dicha tematica y ERROR_CARGANDO si hubo error.
 */
int validaIdTem(FILE * archivo,int numtemas);


/* Funcion que valida el turno siguiente en un archivo cargado. Si no habia
 * error en la lectura devuelve 0 y ERROR_CARGANDO en caso contrario. Devuelve
 * a su vez en un parametro de entrada/salida el turno siguiente (0 para jugador,
 * 1 para computadora).
 */
int validaIdTurno(FILE * archivo,int * turno);


/* Funcion que valida la cantidad de undos hechos por el jugador en un archivo cargado.
 * Devuelve dicho valor en un parametro de entrada/salida. Devuelve 0 si el numero era
 * correcto y ERROR_CARGANDO en caso contrario.
 */
int validaIdUndo(FILE * archivo,int * undo);


/* Funcion que valida que el numero de miembro y elemento en un archivo cargado exista.
 * Es decir, que el numero de barco y el tipo de barco exista en la tematica solicitada.
 * Retorna 0 si todo era correcto y ERROR_CARGANDO en caso contrario
 */
int validaIdTematica2(FILE * archivo,int numeroTem);


/* Funcion que valida las coordenadas leidas del archivo para el tablero1 (el
 * de los barcos propios) de un jugador. Tambien carga todos los datos en el tablero.
 * Retorna 0 si no hubo ningun problema y ERROR_CARGANDO en caso contrario.
 */
int validaCoordenadasBarcos(FILE * archivo,int filas,int cols,int ** tablero1jug,int longitud,tBarco * barcos,int tipobarco,int numbarco);


/* Funcion que valida cuando un usuario carga el archivo si una parte del barco 
 * estaba impactada o no. Tambien verifica que sea correcto lo que se esta leyendo.
 * Retorna 0 si habia algo incorrecto, 1 si la parte era NO IMPACTADA y 2 si era IMPACTADA.
 */ 
int validaStringCarga(FILE * archivo);


/* Funcion que carga las coordenadas del tablero de ataques de un jugador en dicho tablero.
 * Retorna ERROR_CARGANDO si hubo algun problema y 0 en caso contrario
 */
int validaCoordenadasTablero2(FILE * archivo,int ** tablero2jug,int filas,int cols);


/* Funcion que valida de un archivo cargado si en el tablero de ataques hubo 
 * impacto agua o destruccion en una coordenada. Valida dicho string y devuelve
 * 1 si hubo destruccion 2 si hubo impacto 3 si hubo agua y 0 en caso de error
 */
int validaStringCarga2(FILE * archivo);


/* Funcion que valida las coordenadas del tablero de ataques de un jugador una vez
 * que ya fueron ingresadas, viendo que tengan sentido con el tablero real del jugador.
 * Devuelve la cantidad de barcos hundidos y ERROR_CARGANDO si algun dato no era valido
 */
int validaCoordenadasBarcos2(int filas,int cols,int ** tablero1jug,int ** tablero2jug,int tiposbarcos,tBarco * barcos);


/* Funcion que libera la estructura juego creada dinamicamente para poder jugar.
 * Libera primero internamente todas las subestructuras dinamicas que habian y 
 * una vez realizado esto, libera la estructura juego.
 */
void libera(tJuego * juego,int numtemas);
