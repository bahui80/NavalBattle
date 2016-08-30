#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "getnum.h"
#include "rands.h"
#include "estructura.h"
#include "batallaNavalBack.h"
#define BORRA_BUFFER while(getchar()!='\n')
#define BLOQUE 8
#define ERROR_TEMATICAS -1
#define ERROR_MEMORIA -2
#define ERROR_TIPOBARCO -3
#define ERROR_NUMBARCO -4
#define ERROR_LIMITES_TABLERO -5
#define ERROR_DIAGONAL -6
#define ERROR_LONGITUD -7 
#define ERROR_OCUPADO -8
#define ERROR_COMANDOS -9
#define ERROR_VALORES_BOMB -10
#define ERROR_ATACADO -11 
#define ERROR_CARGANDO -12
#define PRINCIPIO 1
#define SEGUNDO 2
#define TERCERO 3
#define CUARTO 4
#define QUINTO 5
#define SEXTO 6
#define SEPTIMO 7
#define OCTAVO 8
#define NOVENO 9
#define DECIMO 10
tJuego * validaTematicas(int * numTemas);
void imprimeTablero(int ** tablero,int filas,int cols);
int menu(void);
int imprimeTematicas(tJuego * juego,int canttemas);
int eligeUbicacion(void);
void imprimeEstado(tBarco * barcos,int tiposBarcos);
void ubicacionInteractiva(int ** tablero,int totalbarcos,tBarco * barcos,int tiposbarcos,int filas,int columnas);
void guardaJuego(char * nombre,int tema,int turno,int tiposbarcos,tBarco * barcosjug,tBarco * barcosPC,int ** tablero1jug,int ** tablero2jug,int filas,int cols,int ** tablero1PC,int ** tablero2PC,int undo);
void juegoNuevo(tJuego * juego,int numtemas);
void juega(int filas,int cols,int ** tablero2jug,int ** tablero1PC,int totalbarcosPC,int tiposbarcos,tBarco * barcosPC,int turno,int tema,int ** tablero1jug,int ** tablero2PC,tBarco * barcosjug,int totalbarcosjug,int undo);
void quiereGuardar(int tema,int turno,int tiposbarcos,tBarco * barcosjug,tBarco * barcosPC,int ** tablero1jug,int ** tablero2jug,int filas,int cols,int ** tablero1PC,int ** tablero2PC,int undo);
void cargarJuego(tJuego * juego,int numtemas);
int validaJuegoCargado(FILE * archivo,tJuego * juego,int numtemas,int * tema,int * turno,int * undo,int * totalbarcos,int * totalbarcos2);


/* Funcion principal del programa que llama al resto de las funciones.*/
int
main(void)
{
	tJuego * juego;
	int opcion,numtemas;
	
	randomize();
	do
	{
		juego=validaTematicas(&numtemas);
		opcion=menu();
		switch(opcion)
		{
			case 1:
			{			
				juegoNuevo(juego,numtemas);
				libera(juego,numtemas);
			}break;
			case 2:
			{
				cargarJuego(juego,numtemas);
				libera(juego,numtemas);
			}break;
			case 3:
			{
				libera(juego,numtemas);
				exit(1);
			}break;
		}
	}while(opcion!=3);

	return 0;
}	

/* Funcion general que valida el archivo inicial (tematicas)para verificar que
 * todas sean correctas y asi poder cargar todos los datos del juego. La funcion
 * retorna la un vector de estructuras de tipo tJuego donde cada posicion
 * del vector sera una tematica distinta con todos sus datos cargados. En caso
 * de que el archivo de tematicas sea corrupto, aborta el programa.
 */
tJuego *
validaTematicas(int * numTemas)
{
	int canttemas,cantflotas,numerodebarcos,longitud,i,j,c;
	tJuego * juego;
	FILE * archivo;
	
	archivo=fopen("tematicas.dat","r");
	if(archivo==NULL)
	{
		printf("No se pudo abrir el archivo\n");
		exit(1);
	}
	
	if(!feof(archivo))
	{
		salteaComentario(archivo);
		if((chequeaString(archivo,"Numero de tematicas: "))!=0 || (canttemas=validaIdTematicas(archivo,numTemas))<0)
		{
			printf("Habia un error en el archivo de tematicas. Verifique el numero de tematicas\n");
			exit(1);
		}
		if((juego=malloc(sizeof(tJuego)*canttemas))==NULL)
		{
			printf("Hubo un error al reservar la memoria de la estructura del juego. Intente nuevamente\n");
			exit(1);
		}
		for(i=0;i<canttemas;i++)
		{
			if((juego[i].jugador=malloc(sizeof(tJugador)*2))==NULL)
			{
				printf("Hubo un error al reservar la memoria de la estructura del juego. Intente nuevamente\n");
				exit(1);
			}
		}
		for(i=0;i<canttemas;i++)
		{
			if((chequeaString(archivo,"Tematica "))!=0)
			{
				printf("Habia un error en el archivo de tematicas. Verifique la tematica %d\n",i+1);
				exit(1);
			}
			if((validaIdTematica(archivo,i,juego))!=0)
			{
				printf("Habia un error en el archivo de tematicas. Verifique el numero de la tematica %d\n",i+1);
				exit(1);
			}
			if((chequeaString(archivo,"Nombre: "))!=0)
			{
				printf("Habia un error en el archivo de tematicas. Verifique el campo nombre de la tematica %d\n",i+1);
				exit(1);
			}
			if((validaIdNombre(archivo,i,juego))!=0)
			{
				printf("Habia un error en el archivo de tematicas. Verifique el nombre de la tematica %d\n",i+1);
				exit(1);
			}
			if((chequeaString(archivo,"Tablero: "))!=0)
			{
				printf("Habia un error en el archivo de tematicas. Verifique el campot tablero de la tematica %d\n",i+1);
				exit(1);
			}
			if((validaIdTablero(archivo,i,juego))!=0)
			{
				printf("Habia un error en el archivo de tematicas. Verifique el numero del tablero de la tematica %d\n",i+1);
				exit(1);
			}
			if((chequeaString(archivo,"Numero de elementos de la flota: "))!=0)
			{
				printf("Habia un error en el archivo de tematicas. Verifique el campo de elementos de la flota de la tematica %d\n",i+1);
				exit(1);
			}
			if((cantflotas=validaIdCantFlotas(archivo,juego,i))<=0)
			{
				printf("Habia un error en el archivo de tematicas. Verifique el numero del campo de elementos de la flota de la tematica %d\n",i+1);
				exit(1);
			}
			if((juego[i].jugador[0].barcos=malloc(cantflotas*sizeof(tBarco)))==NULL)
			{
				printf("Hubo un error al reservar la memoria de la estructura del juego. Intente nuevamente\n");
				exit(1);
			}
			if((juego[i].jugador[1].barcos=malloc(cantflotas*sizeof(tBarco)))==NULL)
			{
				printf("Hubo un error al reservar la memoria de la estructura del juego. Intente nuevamente\n");
				exit(1);
			}
			for(j=0;j<cantflotas;j++)
			{
				if((chequeaString(archivo,"Elemento "))!=0)
				{
					printf("Habia un error en el archivo de tematicas. Verifique el campo elemento %d de la tematica %d\n",j+1,i+1);
					exit(1);
				}
				if((validaIdFlotas(archivo,j))!=0)
				{
					printf("Habia un error en el archivo de tematicas. Verifique el numero del elemento %d de la tematica %d\n",j+1,i+1);
					exit(1);
				}
				if((validaCampoNombre(archivo,i,j,juego))!=0)
				{
					printf("Habia un error en el archivo de tematicas. Verifique el campo nombre del elemento %d de la tematica %d\n",j+1,i+1);
					exit(1);
				}
				if((chequeaString(archivo," Longitud: "))!=0)
				{
					printf("Habia un error en el archivo de tematicas. Verifique el campo longitud del elemento %d de la tematica %d\n",j+1,i+1);
					exit(1);
				}
				if((longitud=validaIdLongitud(archivo))<=0)
				{
					printf("Habia un error en el archivo de tematicas. Verifique la longitud del elemento %d de la tematica %d\n",j+1,i+1);
					exit(1);
				}
				if((chequeaString(archivo," Cantidad: "))!=0)
				{
					printf("Habia un error en el archivo de tematcas. Verifique el campo cantidad del elemento %d de la tematica %d\n",j+1,i+1);
					exit(1);
				}
				if((numerodebarcos=validaIdCantidad(archivo,juego,i,j))<=0)
				{
					printf("Habia un error en el archivo de tematicas. Verifique la cantidad del elemento %d de la tematica %d\n",j+1,i+1);
					exit(1);
				}
				if((juego[i].jugador[0].barcos[j].cantbarcos=malloc(numerodebarcos*sizeof(tCantBarco)))==NULL)
				{
					printf("Hubo un error al reservar la memoria de la estructura del juego. Intente nuevamente\n");
					exit(1);
				}
				if((juego[i].jugador[1].barcos[j].cantbarcos=malloc(numerodebarcos*sizeof(tCantBarco)))==NULL)
				{
					printf("Hubo un error al reservar la memoria de la estructura del juego. Intente nuevamente\n");
					exit(1);
				}
				guardaLongitud(juego,i,j,numerodebarcos,longitud);
			}	
		}
		while(!feof(archivo))
		{	
			while((c=fgetc(archivo))==' ' || c=='\t');
			if(c!=';' && c!='\n' && !feof(archivo))
			{
				printf("Habia un error en el archivo de tematicas. Verifique que el archivo termine correctamente\n");
				exit(1);
			}
			else if(c==';')
				while((c=fgetc(archivo))!='\n');
		}
	}
	
	fclose(archivo);
	
	return juego;
}


/* Funcion que a partir de un tablero y su dimension (filas y columnas)
 * imprime el estado actual del mismo.
 */
void
imprimeTablero(int ** tablero,int filas,int cols)
{
	int i,j;
	
	for(i=0;i<=cols;i++)
	{
		if(i==0)
			printf("|  |");
		else
			printf("|%2d|",i-1);
	}
	
	putchar('\n');
	
	for(i=0;i<filas;i++)
	{
		for(j=0;j<=cols;j++)
		{
			if(j==0)
				printf("|%2d|",i);
			else
				printf("|%2c|",tablero[i][j-1]);
		}
		putchar('\n');
	}
}


/* Funcion que ofrece al usuario el menu inicial de forma interactiva.
 * Muestra el menu y un mensaje de error mientras el numero ingresado
 * sea incorrecto. Devuelve la opcion elegida por el usuario.
 */
int
menu(void)
{
	int opcion;
	int flag=0;

	do
	{
		if(flag)
			printf("\nOpcion invalida. Ingrese nuevamente:");				
		printf("\n1. Iniciar juego nuevo (jugador vs computadora)\n");
		printf("2. Recuperar un juego guardado\n");
		printf("3. Salir\n");
		opcion=getint("Opcion elegida: ");
		flag=1;
	}while(opcion<1 || opcion>3);

	return opcion;
}


/* Funcion que ofrece al usuario la lista completa de las tematicas.
 * Valida que la opcion ingresada por el usuario sea la correcta. Devuelve
 * la tematica elegida por el jugador.
 */
int
imprimeTematicas(tJuego * juego,int canttemas)
{
	int i,opcion,flag=0;
	
	do
	{
		if(flag)
			printf("\nValor invalido. Reingrese\n");
		printf("\nElija una tematica:\n");
		for(i=0;i<canttemas;i++)
			printf("%d.%s\n",i+1,juego[i].nombreTema);
		opcion=getint("Tematica numero: ");
		flag=1;
	}while(opcion<=0 || opcion>canttemas);
	return opcion;
}


/* Funcion que imprime al usuario un menu con las posibles maneras de 
 * ubicar sus barcos. Valida que dicho dato tenga sentido y devuelve
 * la opcion elegida por el usuario.
 */
int 
eligeUbicacion(void)
{
	int opcion;
	int flag=0;
	
	do
	{
		if(flag)
			printf("\nOpcion incorrecta. Reingrese por favor");
		printf("\nElija de que manera quiere ubicar los barcos:\n");
		printf("1.Ubicacion interactiva\n");
		printf("2.Ubicacion aleatoria\n");
		opcion=getint("Opcion numero: ");
		flag=1;
	}while(opcion>2 || opcion<1);
	
	return opcion;
}


/* Funcion que imprime los barcos que faltan ubicar en el tablero. Esta
 * funcion se utiliza para la ubicacion interactiva. Recibe los barcos
 * y la cantidad de barcos distintos que hay.
 */
void
imprimeEstado(tBarco * barcos,int tiposBarcos)
{
	int i,j;
	
	printf("\nPiezas pendientes:\n");
	for(i=0;i<tiposBarcos;i++)
	{
		for(j=0;j<barcos[i].cantidad;j++)
		{
			printf("%d.%d->%s ",i+1,j+1,barcos[i].nombre);
			printf("Longitud: %d",barcos[i].cantbarcos[j].longitud);
			if(barcos[i].cantbarcos[j].puesto==1)
				printf("\tUBICADO    FILA: %d,COLUMNA: %d\n",barcos[i].cantbarcos[j].comienzo.fila,barcos[i].cantbarcos[j].comienzo.columna);
			else
				putchar('\n');
		}
	}
	
}


/* Funcion que permite al usuario habiendo elegido la opcion de ubicacion 
 * interactiva ubicar los barcos de dicha manera. Recibe el tablero, el 
 * numero de los distintos tipos de barcos, los barcos, las filas y las
 * columnas. En caso de error de memoria aborta el programa imprimiendo un cartel.
 * Llama a otras funciones que validan que lo que ingresa el usuario sea
 * correcto. Imprime los distintos tipos de errores que pueden suceder.
 */
void
ubicacionInteractiva(int ** tablero,int totalbarcos,tBarco * barcos,int tiposbarcos,int filas,int columnas)
{
	char * string;
	int flag=0;
	int numString,tipoNave,idNave,error,pone=0;
	tPosicion inicial,final;
	

	do
	{
		if((string=malloc(200*sizeof(char)))==NULL)
		{
			printf("No se pudo reservar memoria\n");
			exit(1);
		}
		imprimeTablero(tablero,filas,columnas);
		imprimeEstado(barcos,tiposbarcos);
		scanf("%s",string);
		BORRA_BUFFER;
		numString=analizaString(string,&tipoNave,&idNave,&inicial,&final);
		if(numString==ERROR_COMANDOS)
			printf("Comando invalido\n");
		else if(numString==0)
		{
			error=validaDatos(tipoNave,idNave,inicial,final,tablero,filas,columnas,barcos,tiposbarcos);
			switch(error)
			{
				case ERROR_TIPOBARCO: printf("\nEl tipo de pieza ingresado no existe\n\n");break;
				case ERROR_NUMBARCO: printf("\nNo existe esa cantidad de piezas\n\n");break;
				case ERROR_LIMITES_TABLERO: printf("\nIngreso coordenadas que estan fuera del tablero\n\n");break;
				case ERROR_DIAGONAL: printf("\nLas piezas no se pueden ubicar en diagonal\n\n");break;
				case ERROR_LONGITUD: printf("\nLa pieza solicitada no tiene la longitud ingresada\n\n");break;
				case ERROR_OCUPADO: printf("\nEl lugar donde quiere ubicar la pieza ya se encuentra ocupado o bien no esta rodeado de agua\n\n");break;
				default: 
				{
					pone=poneEnTablero(tablero,inicial,final,barcos,tipoNave,idNave);
					if(pone==1)
						totalbarcos--;
				}
			}
		}
		else
		{
			if(totalbarcos!=0)
				printf("\nAun quedan barcos por ubicar\n\n");
			else
				flag=1;
		}
	free(string);
	}while(!flag);

}

/* Funcion que permite al usuario guardar una partida. Si no se puede guardar el 
 * juego imprime una notificacion de error.
 */
void
guardaJuego(char * nombre,int tema,int turno,int tiposbarcos,tBarco * barcosjug,tBarco * barcosPC,int ** tablero1jug,int ** tablero2jug,int filas,int cols,int ** tablero1PC,int ** tablero2PC,int undo)
{
	FILE * saveGame;
	char * string="del jugador";
	char * string2="de la computadora";
	
	saveGame=fopen(nombre,"w+");
	
	if(saveGame==NULL)
		printf("No se pudo guardar el archivo\n");

	guardaDatosIniciales(tema,turno,saveGame,undo);
	guardaDatosJugador(tiposbarcos,barcosjug,saveGame,tablero1jug,tablero2jug,filas,cols,string);
	guardaDatosJugador(tiposbarcos,barcosPC,saveGame,tablero1PC,tablero2PC,filas,cols,string2);
	fclose(saveGame);

}

/* Funcion que permite jugar un juego nuevo. Esta funcion es invocada desde el 
 * main si el usuario eligio juego nuevo.
 */
void
juegoNuevo(tJuego * juego,int numtemas)
{
	int tema,ubicacion,totalbarcos,turno,undo=1,numero;
	
	tema=imprimeTematicas(juego,numtemas);
	totalbarcos=barcosTotales(juego[tema-1].jugador[0].barcos,juego[tema-1].jugador[0].tiposbarcos);
	numero=ubicacionAleatoria(juego[tema-1].jugador[1].tablero->tablero1,juego[tema-1].jugador[1].tablero->filas,juego[tema-1].jugador[1].tablero->cols,juego[tema-1].jugador[1].barcos,totalbarcos,juego[tema-1].jugador[1].tiposbarcos);
	if(numero==1)
	{
		printf("\nNo se pudo crear aleatoriamente el tablero de la computadora. Intente nuevamente\n");
		return ;
	} 
	ubicacion=eligeUbicacion(); 
	turno=randInt(0,1);
	switch(ubicacion)
	{
		case 1:
		{
			ubicacionInteractiva(juego[tema-1].jugador[0].tablero->tablero1,totalbarcos,juego[tema-1].jugador[0].barcos,juego[tema-1].jugador[0].tiposbarcos,juego[tema-1].jugador[0].tablero->filas,juego[tema-1].jugador[0].tablero->cols);
			quiereGuardar(tema,turno,juego[tema-1].jugador[0].tiposbarcos,juego[tema-1].jugador[0].barcos,juego[tema-1].jugador[1].barcos,juego[tema-1].jugador[0].tablero->tablero1,juego[tema-1].jugador[0].tablero->tablero2,juego[tema-1].jugador[0].tablero->filas,juego[tema-1].jugador[0].tablero->cols,juego[tema-1].jugador[1].tablero->tablero1,juego[tema-1].jugador[1].tablero->tablero2,undo);
		}break;
		case 2:
		{
			numero=ubicacionAleatoria(juego[tema-1].jugador[0].tablero->tablero1,juego[tema-1].jugador[0].tablero->filas,juego[tema-1].jugador[0].tablero->cols,juego[tema-1].jugador[0].barcos,totalbarcos,juego[tema-1].jugador[0].tiposbarcos);
			if(numero==1)
			{
				printf("\nNo se pudo crear aleatoriamente el tablero del jugador. Intente nuevamente\n");
				return ;
			}
		}break;
	}
	juega(juego[tema-1].jugador[0].tablero->filas,juego[tema-1].jugador[0].tablero->cols,juego[tema-1].jugador[0].tablero->tablero2,juego[tema-1].jugador[1].tablero->tablero1,totalbarcos,juego[tema-1].jugador[1].tiposbarcos,juego[tema-1].jugador[1].barcos,turno,tema,juego[tema-1].jugador[0].tablero->tablero1,juego[tema-1].jugador[1].tablero->tablero2,juego[tema-1].jugador[0].barcos,totalbarcos,undo);
}		


/* Funcion que lleva a cabo el juego entre la computadora y el usuario. Esta funcion
 * recibe un numero (0 o 1) que se corresponde con el jugador que comienza a jugar. Si
 * el numero es 0 comienza el usuario, en caso contrario, comienza la computadora. Esta funcion
 * llama a funciones auxiliares que permiten el correcto juego de cada jugador.
 */
void
juega(int filas,int cols,int ** tablero2jug,int ** tablero1PC,int totalbarcosPC,int tiposbarcos,tBarco * barcosPC,int turno,int tema,int ** tablero1jug,int ** tablero2PC,tBarco * barcosjug,int totalbarcosjug,int undo)
{
	char * string;
	char * string2;
	int a,b,funcion,i=0,c,contadorjugadas=0,huboUndo=0;  /* a y b son las coordenadas de ataque ingresadas por el jugador sin validar.*/
	char * aux;
	tPosicion jugadaPC,jugadajug; /*jugadaPC y jugadajug son las coordenadas de ataque ya validadas y correctas.*/
	
	printf("\nComienza %s\n",turno==0?"el jugador":"la computadora");
	while(totalbarcosPC!=0 && totalbarcosjug!=0)
	{
		if(turno==0)
		{
			huboUndo=0;
			printf("\nTablero de ataques jugador\n");
			imprimeTablero(tablero2jug,filas,cols);
			printf("\nTablero propio\n");
			imprimeTablero(tablero1jug,filas,cols);
			do
			{	
				string=malloc(sizeof(char)*1);
				printf("\nIngrese un comando: ");
				i=0;
				while((c=getchar())!='\n')
				{
					if(i%BLOQUE==0)
					{
						aux=realloc(string,(i+BLOQUE)*sizeof(char));
						if(aux==NULL)
						{
							printf("Hubo un problema al reservar memoria. Intente nuevamente\n");
							exit(1);
						}
						else
							string=aux;
					}
					string[i++]=c;
				}
				string[i]=0;
				string2=calloc(sizeof(char),strlen(string));	
				funcion=validaStringJuego(string,&a,&b,string2,filas,cols,tablero2jug);
				switch(funcion)
				{
					case 0: 
					{
						totalbarcosjug-=ataqueJugador(a,b,tablero2jug,tablero1PC,barcosPC,tiposbarcos,&jugadajug);
						contadorjugadas++;
					}break;
					case 1:
					{
						if(contadorjugadas>=2 && undo<=3 && huboUndo==0)
						{
							comandoUndo(jugadaPC,jugadajug,tablero1PC,tablero2PC,tablero1jug,tablero2jug,tiposbarcos,barcosjug,barcosPC,&totalbarcosjug,&totalbarcosPC);
							undo++;
							huboUndo=1;
							printf("\nTablero de ataques jugador\n");
							imprimeTablero(tablero2jug,filas,cols);
							printf("\nTablero propio\n");
							imprimeTablero(tablero1jug,filas,cols);
						}
						else if(contadorjugadas<=2 && undo<=3)
							printf("\nNo hay jugadas suficientes para utilizar el comando undo. Debe haber atacado por lo menos una vez\n");
						else if(undo>3)
							printf("\nNo puede usar mas el comando undo. Solo se permite usarlo 3 veces\n");
						else
							printf("\nNo puede usar el undo dos veces en el mismo turno\n");
					}break;
					case 2:
					{
						guardaJuego(string2,tema,turno,tiposbarcos,barcosjug,barcosPC,tablero1jug,tablero2jug,filas,cols,tablero1PC,tablero2PC,undo);
						printf("Juego guardado correctamente\n");
					}break;
					case 3:
					{
						quiereGuardar(tema,turno,tiposbarcos,barcosjug,barcosPC,tablero1jug,tablero2jug,filas,cols,tablero1PC,tablero2PC,undo);
						free(string);
						free(string2);
						return;
					}
					case ERROR_COMANDOS: printf("El comando ingresado no es valido\n");break;
					case ERROR_ATACADO: printf("Eligio una celda que ya estaba atacada. Intente nuevamente\n");break;
					case ERROR_VALORES_BOMB: printf("Introdujo coordenadas no validas\n");break;
				}
			free(string);
			free(string2);	
			}while(funcion!=0);
			turno=1;
		}
		else
		{
			contadorjugadas++;
			totalbarcosPC-=estrategiaPC(filas,cols,tablero2PC,tablero1jug,barcosjug,tiposbarcos,&jugadaPC);
			printf("\nJugada de la PC: bomb(%d,%d)\n",jugadaPC.fila,jugadaPC.columna);
			turno=0;
			BORRA_BUFFER;
		}
	}
	if(totalbarcosPC==0)
		printf("\n\n\nLO SIENTO HA PERDIDO\n\n");
	else if(totalbarcosjug==0)
		printf("\n\n\nMUY BIEN HA GANADO!!\n\n");
	printf("\nEl tablero del jugador con los barcos era el siguiente:\n");
	imprimeTablero(tablero1jug,filas,cols);
	printf("\nEl tablero de la computadora con los barcos era el siguiente:\n");
	imprimeTablero(tablero1PC,filas,cols);
}


/* Funcion que ofrece al usuario un menu con la opcion de guardar. Esta funcion
 * se invoca cuando el usuario invoca el comando quit mientras esta jugando y 
 * luego de ubicar los barcos de manera interactiva. En caso que el usuario responda
 * que desea guardar su juego, lo guarda.
 */
void
quiereGuardar(int tema,int turno,int tiposbarcos,tBarco * barcosjug,tBarco * barcosPC,int ** tablero1jug,int ** tablero2jug,int filas,int cols,int ** tablero1PC,int ** tablero2PC,int undo)
{
	int opcion,flag=0,i=0,c;
	char * aux,* string;
	
	do
	{
		if(flag)
			printf("\nOpcion incorrecta\n");
		printf("\n¿Desea guardar el juego?\n");
		printf("1.Si\n");
		printf("2.No\n");
		opcion=getint("Opcion elegida: ");
		flag=1;
	}while(opcion<0 || opcion>2);
	
	if(opcion==1)
	{
		printf("Ingrese el nombre con el que desea guardar el juego: ");
		string=malloc(sizeof(char)*1);
		while((c=getchar())!='\n')
		{
			if(i%BLOQUE==0)
			{
				aux=realloc(string,(i+BLOQUE)*sizeof(char));
				if(aux==NULL)
					exit(1);
				else
					string=aux;
			}
			string[i++]=c;
		}
		string[i]=0;
	guardaJuego(string,tema,turno,tiposbarcos,barcosjug,barcosPC,tablero1jug,tablero2jug,filas,cols,tablero1PC,tablero2PC,undo);
	free(string);
	}
}


/* Funcion que carga el juego en caso que el usuario haya ingresado dicha opcion.
 * Esta funcion le pide al usuario el nombre del archivo que desea cargar. En 
 * caso que no exista dicho archivo imprime un mensaje de advertencia y vuelve 
 * al menu principal. Si el archivo existia se procede a la validacion del mismo
 * y a la carga.
 */
void
cargarJuego(tJuego * juego,int numtemas)
{
	int c,i=0;
	int tema,turno,undo,totalbarcos,totalbarcos2;
	char * string,* aux;
	FILE * archivo;
	
	printf("\nIngrese el nombre del archivo que quiere cargar: ");

	string=malloc(sizeof(char)*1);
	while((c=getchar())!='\n')
	{
		if(i%BLOQUE==0)
		{
			aux=realloc(string,(i+BLOQUE)*sizeof(char));
			if(aux==NULL)
				exit(1);
			else
				string=aux;
		}
		string[i++]=c;
	}
	string[i]=0;
	
	if((archivo=fopen(string,"r"))==NULL)
		printf("\nNo existe un archivo con ese nombre\n");
	
	if(archivo!=NULL)
	{
		if((validaJuegoCargado(archivo,juego,numtemas,&tema,&turno,&undo,&totalbarcos,&totalbarcos2))==1)
			juega(juego[tema-1].jugador[0].tablero->filas,juego[tema-1].jugador[0].tablero->cols,juego[tema-1].jugador[0].tablero->tablero2,juego[tema-1].jugador[1].tablero->tablero1,totalbarcos2,juego[tema-1].jugador[0].tiposbarcos,juego[tema-1].jugador[1].barcos,turno,tema,juego[tema-1].jugador[0].tablero->tablero1,juego[tema-1].jugador[1].tablero->tablero2,juego[tema-1].jugador[0].barcos,totalbarcos,undo);
		fclose(archivo);
	}

	free(string);
}


/* Funcion que se llama para validar que el archivo que solicito el usuario cargar
 * sea valido, es decir, todas las coordenadas ingresadas y los datos ingresados
 * sean correctos. Tambien se encarga de devolver en parametros de entrada/salida
 * los datos suficientes para poder jugar. Devuelve 0 si hubo algun error y 1 en caso
 * contrario.
 */
int 
validaJuegoCargado(FILE * archivo,tJuego * juego,int numtemas,int * tema,int * turno,int * undo,int * totalbarcos,int * totalbarcos2)
{
	int i,j,c;
	
	if(!feof(archivo))
	{
		salteaComentario(archivo);
		if((chequeaString(archivo,"Tematica: "))!=0)
		{
			printf("El archivo que quiere cargar estaba corrupto. Chequee el campo tematica\n");
			return 0;
		}
		if((*tema=validaIdTem(archivo,numtemas))<0)
		{
			printf("El archivo que quiere cargar estaba corrupto. Chequee el numero de la tematica\n");
			return 0;
		}
		if((chequeaString(archivo,"Proximo turno: "))!=0)
		{
			printf("El archivo que quiere cargar estaba corrupto. Chequee el campo de proximo turno\n");
			return 0;
		}
		if((validaIdTurno(archivo,turno))!=0)
		{
			printf("El archivo que quiere cargar estaba corrupto. Chequee que el turno sea Jugador o Computadora\n");
			return 0;
		}
		if((chequeaString(archivo,"Undos: "))!=0)
		{
			printf("El archivo que quiere cargar estaba corrupto. Chequee el campo de undo\n");
			return 0;
		}
		if((validaIdUndo(archivo,undo))<0)
		{
			printf("El archivo que quiere cargar estaba corrupto. Chequee la cantidad de undos\n");
			return 0;
		}
		if((chequeaString(archivo,"Datos del jugador"))!=0)
		{
			printf("El archivo que quiere cargar estaba corrupto. Chequee el campo datos del jugador\n");
			return 0;
		}
		salteaComentario(archivo);
		for(i=0;i<juego[(*tema)-1].jugador[0].tiposbarcos;i++)
		{
			if((chequeaString(archivo,"Elemento "))!=0)
			{
				printf("El archivo que quiere cargar estaba corrupto. Chequee el campo elemento %d del jugador\n",i+1);
				return 0;
			}
			if((validaIdTematica2(archivo,i))<0)
			{
				printf("El archivo que quiere cargar estaba corrupto. Chequee el numero del elemento %d del jugador\n",i+1);
				return 0;
			}
			for(j=0;j<juego[(*tema)-1].jugador[0].barcos[i].cantidad;j++)
			{
			
				if((chequeaString(archivo,"Miembro "))!=0)
				{
					printf("El archivo que quiere cargar estaba corrupto. Chequee el campo miembro %d del elemento %d del jugador\n",j+1,i+1);
					return 0;
				}
				if((validaIdTematica2(archivo,j))<0)
				{
					printf("El archivo que quiere cargar estaba corrupto. Chequee el numero del miembro %d del elemento %d del jugador\n",j+1,i+1);
					return 0;
				}
				if((validaCoordenadasBarcos(archivo,juego[(*tema)-1].jugador[0].tablero->filas,juego[(*tema)-1].jugador[0].tablero->cols,juego[(*tema)-1].jugador[0].tablero->tablero1,juego[(*tema)-1].jugador[0].barcos[i].cantbarcos[j].longitud,juego[(*tema)-1].jugador[0].barcos,i,j))<0)
				{
					printf("El archivo que quiere cargar estaba corrupto. Chequee las coordenadas del miembro %d del elemento %d del jugador. Recuerde que los barcos no pueden estar en diagonal, que deben quedar dentro de los limites del tablero y que deben estar rodeados de agua y nunca por un barco\n",j+1,i+1);
					return 0;
				}
			}
		}
		salteaComentario(archivo);
		if((validaCoordenadasTablero2(archivo,juego[(*tema)-1].jugador[0].tablero->tablero2,juego[(*tema)-1].jugador[0].tablero->filas,juego[(*tema)-1].jugador[0].tablero->cols))<0)
		{
			printf("El archivo que quiere cargar estaba corrupto. Chequee las coordenadas del tablero de ataques del jugador\n");
			return 0;
		}	
		if((chequeaString(archivo,"Datos de la computadora"))!=0)
		{
			printf("El archivo que quiere cargar estaba corrupto. Chequee el campo datos de la computadora\n");
			return 0;
		}
		salteaComentario(archivo);	
		for(i=0;i<juego[(*tema)-1].jugador[1].tiposbarcos;i++)
		{
			if((chequeaString(archivo,"Elemento "))!=0)
			{
				printf("El archivo que quiere cargar estaba corrupto. Chequee el campo elemento %d de la computadora\n",i+1);
				return 0;
			}
			if((validaIdTematica2(archivo,i))<0)
			{
				printf("El archivo que quiere cargar estaba corrupto. Chequee el numero del elemento %d de la computadora\n",i+1);
				return 0;
			}
			for(j=0;j<juego[(*tema)-1].jugador[1].barcos[i].cantidad;j++)
			{
				if((chequeaString(archivo,"Miembro "))!=0)
				{
					printf("El archivo que quiere cargar estaba corrupto. Chequee el campo miembro %d del elemento %d de la computadora\n",j+1,i+1);
					return 0;
				}
				if((validaIdTematica2(archivo,j))<0)
				{
					printf("El archivo que quiere cargar estaba corrupto. Chequee el numero del miembro %d del elemento %d de la computadora\n",j+1,i+1);
					return 0;
				}
				if((validaCoordenadasBarcos(archivo,juego[(*tema)-1].jugador[1].tablero->filas,juego[(*tema)-1].jugador[1].tablero->cols,juego[(*tema)-1].jugador[1].tablero->tablero1,juego[(*tema)-1].jugador[1].barcos[i].cantbarcos[j].longitud,juego[(*tema)-1].jugador[1].barcos,i,j))<0)
				{
					printf("El archivo que quiere cargar estaba corrupto. Chequee las coordenadas del miembro %d del elemento %d de la computadora. Recuerde que los barcos no pueden estar en diagonal, que deben quedar dentro de los limites del tablero y que deben estar rodeados de agua y nunca por un barco\n",j+1,i+1);
					return 0;
				}	
			}
		}
		c=fgetc(archivo);
		if(c!=EOF)
		{
			fseek(archivo,-1,SEEK_CUR);
			salteaComentario(archivo);
			if((validaCoordenadasTablero2(archivo,juego[(*tema)-1].jugador[1].tablero->tablero2,juego[(*tema)-1].jugador[1].tablero->filas,juego[(*tema)-1].jugador[1].tablero->cols))<0)
			{
				printf("El archivo que quiere cargar estaba corrupto. Chequee las coordenadas del tablero de ataques de la computadora\n");
				return 0;
			}	
		}
		while(!feof(archivo))
		{
			while((c=fgetc(archivo))==' ' || c=='\t');
			if(c!='\n' && !feof(archivo))
			{
				printf("El archivo que quiere cargar estaba corrupto. Chequee el fin de archivo\n");
				return 0;
			}
		}
		if(((*totalbarcos)=validaCoordenadasBarcos2(juego[(*tema)-1].jugador[0].tablero->filas,juego[(*tema)-1].jugador[0].tablero->cols,juego[(*tema)-1].jugador[1].tablero->tablero1,juego[(*tema)-1].jugador[0].tablero->tablero2,juego[(*tema)-1].jugador[1].tiposbarcos,juego[(*tema)-1].jugador[1].barcos))<0)
		{
			printf("Verifique que las coordenadas del tablero de ataques y del tablero propio del jugador coincidan entre si\n");
			return 0;
		}
		if(((*totalbarcos2)=validaCoordenadasBarcos2(juego[(*tema)-1].jugador[1].tablero->filas,juego[(*tema)-1].jugador[1].tablero->cols,juego[(*tema)-1].jugador[0].tablero->tablero1,juego[(*tema)-1].jugador[1].tablero->tablero2,juego[(*tema)-1].jugador[0].tiposbarcos,juego[(*tema)-1].jugador[0].barcos))<0)
		{
			printf("Verifique que las coordenadas del tablero de ataques y del tablero propio de la computadora coincidan entre si\n");
			return 0;
		}	
	}
	return 1;	
}

