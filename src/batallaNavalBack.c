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

void
salteaComentario(FILE * archivo)
{
	int c;
	while((c=fgetc(archivo))==' ' || c=='\t' || c==';')
	{
		if(c==';')
			while((c=fgetc(archivo))!='\n');
	}
	if(c!=EOF)
	{
		if(c!='\n')
			fseek(archivo,-1,SEEK_CUR);
	}
}

int 
chequeaString(FILE * archivo,char * string)
{
	int num;
	int i=0;
	char * aux=NULL;
	char * aux2;
	
	while(string[i]!=0)
	{
		if(i%BLOQUE==0)
		{
			aux2=realloc(aux,(i+BLOQUE)*sizeof(char));
			if(aux2==NULL)
			{
				free(aux);
				return ERROR_MEMORIA;
			}
			else
				aux=aux2;
		}
		aux[i]=fgetc(archivo);
		i++;
	}
	if(i%BLOQUE==0)
	{
		aux2=realloc(aux,(i+BLOQUE)*sizeof(char));
		if(aux2==NULL)
		{
			free(aux);
			return ERROR_MEMORIA;
		}
		else
			aux=aux2;
	}
	aux[i]=0;
	num=strcmp(aux,string);
	free(aux);
	return num;
}

int
validaIdTematicas(FILE * archivo,int * numTemas)
{
	int canttemas;
	int cantidad;
	
	cantidad=fscanf(archivo,"%2d",&canttemas);
	if(cantidad==0 && canttemas<=0)
		return ERROR_TEMATICAS;
	else
	{
		*(numTemas)=canttemas;
		salteaComentario(archivo);
		return canttemas;
	}
}

int
validaIdTematica(FILE * archivo,int numeroTem,tJuego * juego)
{
	int correcto,tematica;
	
	correcto=fscanf(archivo,"%d",&tematica);	

	if(correcto==0 || tematica!=(numeroTem+1))
		return ERROR_TEMATICAS;
	else
	{
		juego[numeroTem].numTema=numeroTem+1;
		salteaComentario(archivo);
	}
	return 0;
}

int 
validaIdNombre(FILE * archivo,int numeroTem,tJuego * juego)
{
	int c,largo,i=0;
	char * aux=NULL;
	char * aux2;
	
	while((c=fgetc(archivo))==' ' || c=='\t');
	if(c=='\n' || c==';')
		return ERROR_TEMATICAS;
	else
		fseek(archivo,-1,SEEK_CUR);
	
	while((c=fgetc(archivo))!='\n' && c!=';')
	{
		if(i%BLOQUE==0)
		{
			aux2=realloc(aux,(i+BLOQUE)*sizeof(char));
			if(aux2==NULL)
			{
				free(aux);
				return ERROR_MEMORIA;
			}
			else
				aux=aux2;
		}
		aux[i]=c;
		i++;
	}
	if(i%BLOQUE==0)
	{
		aux2=realloc(aux,(i+BLOQUE)*sizeof(char));
		if(aux2==NULL)
		{
			free(aux);
			return ERROR_MEMORIA;
		}
		else
			aux=aux2;
	}
	aux[i]=0;		

	largo=strlen(aux)+1;
	if((juego[numeroTem].nombreTema=malloc(largo*sizeof(char)))==NULL)
		return ERROR_MEMORIA;
	
	strcpy(juego[numeroTem].nombreTema,aux);
	
	if(c==';')
		while(fgetc(archivo)!='\n');
	salteaComentario(archivo);
	free(aux);
	return 0;
}

int
validaIdTablero(FILE * archivo, int numTema,tJuego * juego)	
{
	int cantidad,cantidad2;
	int leido;
	
	leido=fscanf(archivo,"%dx%d",&cantidad,&cantidad2);
	if(cantidad<=0 || cantidad2<=0 || leido!=2)
		return ERROR_TEMATICAS;
	if((juego[numTema].jugador[0].tablero=malloc(sizeof(tTablero)))==NULL)
		return ERROR_MEMORIA;
	if((juego[numTema].jugador[1].tablero=malloc(sizeof(tTablero)))==NULL)
		return ERROR_MEMORIA;
	
	juego[numTema].jugador[0].tablero->filas=cantidad;
	juego[numTema].jugador[0].tablero->cols=cantidad2;
	juego[numTema].jugador[1].tablero->filas=cantidad;
	juego[numTema].jugador[1].tablero->cols=cantidad2;
	if((juego[numTema].jugador[0].tablero->tablero1=crearTablero(juego[numTema].jugador[0].tablero->filas,juego[numTema].jugador[0].tablero->cols))==NULL)
		return ERROR_MEMORIA;
	if((juego[numTema].jugador[0].tablero->tablero2=crearTablero(juego[numTema].jugador[0].tablero->filas,juego[numTema].jugador[0].tablero->cols))==NULL)
		return ERROR_MEMORIA;
	if((juego[numTema].jugador[1].tablero->tablero1=crearTablero(juego[numTema].jugador[1].tablero->filas,juego[numTema].jugador[1].tablero->cols))==NULL)
		return ERROR_MEMORIA;
	if((juego[numTema].jugador[1].tablero->tablero2=crearTablero(juego[numTema].jugador[1].tablero->filas,juego[numTema].jugador[1].tablero->cols))==NULL)
		return ERROR_MEMORIA;
	
	salteaComentario(archivo);
	
	return 0;
}

int
validaIdCantFlotas(FILE * archivo,tJuego * juego,int i)
{
	int correcto;
	int cantidad;
	
	correcto=fscanf(archivo,"%d",&cantidad);
	
	if(correcto==0 || cantidad<=0)
		return ERROR_TEMATICAS;
	
	juego[i].jugador[0].tiposbarcos=cantidad;
	juego[i].jugador[1].tiposbarcos=cantidad;
	
	salteaComentario(archivo);
	
	return cantidad;
}

int 
validaIdFlotas(FILE * archivo,int numFlota)
{
	int correcto,flota,c;
	
	correcto=fscanf(archivo,"%d",&flota);
	
	if(correcto==0 || flota!=(numFlota+1))
		return ERROR_TEMATICAS;
	if(fgetc(archivo)!=':')
		return ERROR_TEMATICAS;
	
	while((c=fgetc(archivo))==' ' || c=='\t');
	fseek(archivo,-1,SEEK_CUR);
	
	return 0;
}

int
validaCampoNombre(FILE * archivo,int numTema,int numeroElem, tJuego * juego)
{
	int c,i=0;
	char * aux=NULL;
	char * aux2;
	int largo;
	
	if((c=fgetc(archivo))=='-' || c==';')
		return ERROR_TEMATICAS;
	fseek(archivo,-1,SEEK_CUR);
	while((c=fgetc(archivo))!='-' && c!='\n')
	{
		if(i%BLOQUE==0)
		{
			aux2=realloc(aux,(i+BLOQUE)*sizeof(char));
			if(aux2==NULL)
			{	free(aux);
				return ERROR_MEMORIA;
			}
			else
				aux=aux2;
		}
		aux[i]=c;
		i++;
	}
	if(i%BLOQUE==0)
	{
		aux2=realloc(aux,(i+BLOQUE)*sizeof(char));
		if(aux2==NULL)
		{
			free(aux);
			return ERROR_MEMORIA;
		}
		else
			aux=aux2;
	}
	aux[i]=0;	
	largo=strlen(aux)+1;
	
	if((juego[numTema].jugador[0].barcos[numeroElem].nombre=malloc(largo*sizeof(char)))==NULL)
		return ERROR_MEMORIA;
	strcpy(juego[numTema].jugador[0].barcos[numeroElem].nombre,aux);
	if((juego[numTema].jugador[1].barcos[numeroElem].nombre=malloc(largo*sizeof(char)))==NULL)
		return ERROR_MEMORIA;
	strcpy(juego[numTema].jugador[1].barcos[numeroElem].nombre,aux);
	free(aux);
	
	return 0;
}

int
validaIdLongitud(FILE * archivo)
{
	int correcto, longitud;
	int c;
	
	correcto=fscanf(archivo,"%d ",&longitud);
	
	if(correcto==0 || longitud<=0)
		return ERROR_TEMATICAS;
	
	c=fgetc(archivo);
	if(c!='-')
		return ERROR_TEMATICAS;
		
	return longitud;
}

int 
validaIdCantidad(FILE * archivo,tJuego * juego,int i,int j)
{
	int correcto, numero;
	
	correcto=fscanf(archivo,"%d",&numero);
	
	if(correcto==0 || numero<=0)
		return ERROR_TEMATICAS;
	
	juego[i].jugador[0].barcos[j].cantidad=numero;
	juego[i].jugador[1].barcos[j].cantidad=numero;

	salteaComentario(archivo);
	return numero;
}

void
guardaLongitud(tJuego * juego,int i,int j,int numerodebarcos,int largo)
{
	int h;
	
	for(h=0;h<numerodebarcos;h++)
	{
		juego[i].jugador[0].barcos[j].cantbarcos[h].longitud=largo;
		juego[i].jugador[1].barcos[j].cantbarcos[h].longitud=largo;
		juego[i].jugador[0].barcos[j].cantbarcos[h].puesto=0;
		juego[i].jugador[1].barcos[j].cantbarcos[h].puesto=0;
		
	}
}

int **
crearTablero(int filas, int cols)
{
	int ** tablero;
	int i,j;
	
	if((tablero=malloc(filas*sizeof(int *)))==NULL)
		return NULL;
		
	for(i=0;i<filas;i++)
	{
		if((tablero[i]=malloc(cols*sizeof(int)))==NULL)
			return NULL;
	}
	for(i=0;i<filas;i++)
		for(j=0;j<cols;j++)
			tablero[i][j]='.';
	
	return tablero;
}

int
barcosTotales(tBarco * barcos, int tiposBarcos) 
{
	int i,totalBarcos=0;

	for(i=0;i<tiposBarcos;i++)
		totalBarcos+=barcos[i].cantidad;

	return totalBarcos;
}

int 
validaDatos(int tipoNave,int idNave,tPosicion inicial,tPosicion final,int ** tablero,int filas,int cols,tBarco * barcos,int tiposbarcos)
{
	int tamano;
	
	tamano=calculaLongitud(inicial,final);
	
	if(tipoNave>tiposbarcos || tipoNave<=0)
		return ERROR_TIPOBARCO;
	else if(idNave<=0 || idNave>barcos[tipoNave-1].cantidad)
		return ERROR_NUMBARCO;
	else if(inicial.fila<0 || inicial.fila>filas-1 || inicial.columna<0 || inicial.columna>cols-1 || final.fila<0 || final.fila>filas-1 || final.columna<0 || final.columna>cols-1)
		return ERROR_LIMITES_TABLERO;
	else if(!(inicial.fila==final.fila || inicial.columna==final.columna))
		return ERROR_DIAGONAL;
	else if(tamano!=barcos[tipoNave-1].cantbarcos[idNave-1].longitud)
		return ERROR_LONGITUD;
	else if(validaCasillero(tablero,inicial,final,filas,cols,barcos[tipoNave-1].cantbarcos[idNave-1]))
		return ERROR_OCUPADO;
	
	return 0;
}

int 
validaCasillero(int ** tablero,tPosicion inicial,tPosicion fin,int filas,int cols,tCantBarco barco)
{
	int i,j,minfil,mincol,maxfil,maxcol;
	int flag=0;
	
	asignaOrden(inicial,fin,&minfil,&maxfil,&mincol,&maxcol);
	if(barco.puesto==0)
	{
		for(i=minfil-1;i<=maxfil+1 && !flag;i++)
			for(j=mincol-1;j<=maxcol+1 && !flag;j++)
			{
				if(i>=0 && i<filas && j>=0 && j<cols)
					if(tablero[i][j]=='@')
						flag=1;
			}
	}
	else if(barco.puesto==1)
	{
		for(i=minfil-1;i<=maxfil+1 && !flag;i++)
			for(j=mincol-1;j<=maxcol+1 && !flag;j++)
			{
				if(i>=0 && i<filas && j>=0 && j<cols && !((i>=barco.comienzo.fila && i<=barco.final.fila) && (j>=barco.comienzo.columna && j<=barco.final.columna)))
					if(tablero[i][j]=='@' || tablero[i][j]=='X' || tablero[i][j]=='D')
						flag=1;
			}
	}
		
	return flag;
}

void
asignaOrden(tPosicion inicial,tPosicion final,int * minfil,int * maxfil,int * mincol,int * maxcol)
{
	if((inicial.fila)>=(final.fila))
	{
		*minfil=final.fila;
		*maxfil=inicial.fila;
	}
	else
	{	
		*minfil=inicial.fila;
		*maxfil=final.fila;
	}
	if((inicial.columna)>=(final.columna))
	{
		*mincol=final.columna;
		*maxcol=inicial.columna;
	}
	else
	{
		*mincol=inicial.columna;
		*maxcol=final.columna;
	}

}

int
poneEnTablero(int ** tablero,tPosicion inicial,tPosicion fin,tBarco * barcos,int tipoNave,int idNave)
{
	int minfil,maxfil;
	int mincol,maxcol;
	int i,j;
	
	asignaOrden(inicial,fin,&minfil,&maxfil,&mincol,&maxcol);
	if(barcos[tipoNave-1].cantbarcos[idNave-1].puesto==0)
	{
		for(i=minfil;i<=maxfil;i++)
			for(j=mincol;j<=maxcol;j++)
				tablero[i][j]='@';
	
		barcos[tipoNave-1].cantbarcos[idNave-1].puesto=1;
		barcos[tipoNave-1].cantbarcos[idNave-1].comienzo.fila=minfil;
		barcos[tipoNave-1].cantbarcos[idNave-1].comienzo.columna=mincol;
		barcos[tipoNave-1].cantbarcos[idNave-1].final.fila=maxfil;
		barcos[tipoNave-1].cantbarcos[idNave-1].final.columna=maxcol;
		return 1;
	}
	else if(barcos[tipoNave-1].cantbarcos[idNave-1].puesto==1)
	{
		for(i=barcos[tipoNave-1].cantbarcos[idNave-1].comienzo.fila;i<=barcos[tipoNave-1].cantbarcos[idNave-1].final.fila;i++)
			for(j=barcos[tipoNave-1].cantbarcos[idNave-1].comienzo.columna;j<=barcos[tipoNave-1].cantbarcos[idNave-1].final.columna;j++)
				tablero[i][j]='.';
		for(i=minfil;i<=maxfil;i++)
			for(j=mincol;j<=maxcol;j++)
				tablero[i][j]='@';
		barcos[tipoNave-1].cantbarcos[idNave-1].comienzo.fila=minfil;
		barcos[tipoNave-1].cantbarcos[idNave-1].comienzo.columna=mincol;
		barcos[tipoNave-1].cantbarcos[idNave-1].final.fila=maxfil;
		barcos[tipoNave-1].cantbarcos[idNave-1].final.columna=maxcol;
		return 0;
	}
	return 0;
}

int
analizaString(char * string,int * tipoNave,int * idNave,tPosicion * inicial,tPosicion * final)
{
	int i=0,j=1;
	int estado=PRINCIPIO;
	int contador1=0;
	int contador2=0;
	
	*tipoNave=0;
	*idNave=0;
	inicial->fila=0;
	inicial->columna=0;
	final->fila=0;
	final->columna=0;
	
	
	if(!(strcmp(string,"done")))
		return 1;
	else
	{
		while(string[i])
		{
			switch(estado)
			{
				case PRINCIPIO:
				{
					if(string[i]!='s')
						return ERROR_COMANDOS;
					else
					{
						i++;
						estado=SEGUNDO;
					}
				}break;
				case SEGUNDO: 
				{
					if(string[i]!='e')
						return ERROR_COMANDOS;
					else
					{
						i++;
						estado=TERCERO;
					}				
				}break;
				case TERCERO:
				{
					if(string[i]!='t')
						return ERROR_COMANDOS;
					else
					{
						i++;
						estado=CUARTO;
					}
				}break;
				case CUARTO:
				{
					if(string[i]!='(' || contador1==3)
						return ERROR_COMANDOS;
					else
					{
						estado=QUINTO;
						i++;
					}
				}break;
				case QUINTO:
				{
					switch(contador1)
					{
						case 0:
						{
							if(isdigit(string[i]))
							{
								*tipoNave=(string[i]-'0')+((*tipoNave)*j);
								j*=10;
								i++;
							}
							else
							{
								j=1;
								contador1++;
								estado=SEXTO;
							}
						}break;
						case 1:
						{
							if(isdigit(string[i]))
							{
								(inicial->fila)=(string[i]-'0')+((inicial->fila)*j);
								j*=10;
								i++;
							}
							else
							{
								j=1;
								contador1++;
								estado=SEXTO;
							}
						}break;
						case 2:
						{
							if(isdigit(string[i]))
							{
								(final->fila)=(string[i]-'0')+((final->fila)*j);
								j*=10;
								i++;
							}
							else
							{
								j=1;
								contador1++;
								estado=SEXTO;
							}
						}break;
					}
				}break;
				case SEXTO:
				{
					if(string[i]!=',')
						return ERROR_COMANDOS;
					else
					{
						estado=SEPTIMO;
						i++;
					}
				}break;
				case SEPTIMO:
				{
					switch(contador2)
					{
						case 0:
						{
							if(isdigit(string[i]))
							{
								*idNave=(string[i]-'0')+((*idNave)*j);
								j*=10;
								i++;
							}
							else
							{
								j=1;
								contador2++;
								estado=OCTAVO;
							}
						}break;
						case 1:
						{
							if(isdigit(string[i]))
							{
								(inicial->columna)=(string[i]-'0')+((inicial->columna)*j);
								j*=10;
								i++;
							}
							else
							{
								j=1;
								contador2++;
								estado=OCTAVO;
							}
						}break;
						case 2:
						{
							if(isdigit(string[i]))
							{
								(final->columna)=(string[i]-'0')+((final->columna)*j);
								j*=10;
								i++;
							}
							else
							{
								j=1;
								contador2++;
								estado=OCTAVO;
							}
						}break;
					}
				}break;
				case OCTAVO:
				{
					if(string[i]!=')')
						return ERROR_COMANDOS;
					else
					{
						estado=CUARTO;
						i++;
					}
				}break;
			}
		}
	}
	if(contador1!=3 && contador2!=3)
		return ERROR_COMANDOS;
	else
		return 0;
}

int
calculaLongitud(tPosicion inicial,tPosicion final)
{
	int mayor,num1,num2;
	
	num1=abs((inicial.fila)-(final.fila));
	num2=abs((inicial.columna)-(final.columna));
		
	if(num1>=num2)
		mayor=num1;
	else
		mayor=num2;
		
	return (mayor+1);
}

int
ubicacionAleatoria(int ** tablero,int filas,int cols,tBarco * barcos,int totalbarcos,int tiposbarcos)
{
	tPosicion inicial,final;
	int i,j,a,flag=1,flag2=0,contador=0;
	
	for(i=1;i<=tiposbarcos;i++)
	{
		for(j=1;j<=barcos[i-1].cantidad;j++)
		{
			while(flag!=0 && contador!=6)
			{
				a=randInt(0,1);
				switch(a)
				{
					case 0: aleatoriaVertical(filas,cols,barcos[i-1].cantbarcos[j-1].longitud,&inicial,&final);break;
					case 1: aleatoriaHorizontal(filas,cols,barcos[i-1].cantbarcos[j-1].longitud,&inicial,&final);break;
				}	
				flag=validaDatos(i,j,inicial,final,tablero,filas,cols,barcos,tiposbarcos);
				if(!flag)
					poneEnTablero(tablero,inicial,final,barcos,i,j);
				contador++;
			}
			flag=1;
			contador=0;
		}
	}
	for(i=0;i<tiposbarcos && !flag2;i++)
		for(j=0;j<barcos[i].cantidad && !flag2;j++)
			if(barcos[i].cantbarcos[j].puesto==0)
				flag2=1;
	
	return flag2;
}

void
aleatoriaHorizontal(int filas,int cols,int longitud,tPosicion * inicial,tPosicion * final)
{
	int a,b;
	
	a=randInt(0,cols-1);
	b=randInt(0,filas-1);
	
	inicial->fila=b;
	final->fila=b;
	inicial->columna=a;
	if(!((a+longitud-1)>=cols))
		final->columna=a+longitud-1;
	else
		final->columna=a-longitud+1;
}

void
aleatoriaVertical(int filas,int cols,int longitud,tPosicion * inicial,tPosicion * final)
{
	int a,b;
	
	a=randInt(0,filas-1);
	b=randInt(0,cols-1);
	
	inicial->columna=b;
	final->columna=b;
	inicial->fila=a;
	if(!((a+longitud-1)>=filas))
		final->fila=a+longitud-1;
	else
		final->fila=a-longitud+1;
}

int
validaStringJuego(char * string,int * a,int * b,char * string2,int filas,int cols,int ** tablerojug)
{
	int i=0,j=1,contador=0;
	int estado=PRINCIPIO;
	char * aux="save ";

	*a=0;
	*b=0;
	
	if(!(strcmp(string,"undo")))
		return 1;
	else if(string[i]=='s')
	{
		for(i=0;aux[i];i++)
			if(string[i]!=aux[i])
				return ERROR_COMANDOS;
		strcpy(string2,string+i);
		return 2;
	}
	else if(!(strcmp(string,"quit")))
		return 3;
	else
	{
		while(string[i])
		{
			switch(estado)
			{
				case PRINCIPIO:
				{
					if(string[i]!='b')
						return ERROR_COMANDOS;
					else
					{
						i++;
						estado=SEGUNDO;
					}
				}break;
				case SEGUNDO:
				{
					if(string[i]!='o')
						return ERROR_COMANDOS;
					else
					{
						i++;
						estado=TERCERO;
					}
				}break;
				case TERCERO:
				{
					if(string[i]!='m')
						return ERROR_COMANDOS;
					else
					{
						i++;
						estado=CUARTO;
					}
				}break;
				case CUARTO:
				{
					if(string[i]!='b')
						return ERROR_COMANDOS;
					else
					{
						i++;
						estado=QUINTO;
					}
				}break;
				case QUINTO:
				{
					if(string[i]!='(')
						return ERROR_COMANDOS;
					else
					{
						i++;
						estado=SEXTO;
					}
				}break;
				case SEXTO:
				{
					if(isdigit(string[i]))
					{
						*a=(string[i]-'0')+((*a)*j);
						j*=10;
						i++;
					}
					else
					{
						j=1;
						contador++;
						estado=SEPTIMO;
					}
				}break;				
				case SEPTIMO:
				{
					if(string[i]!=',')
						return ERROR_COMANDOS;
					else
					{
						i++;
						estado=OCTAVO;
					}
				}break;
				case OCTAVO:
				{
					if(isdigit(string[i]))
					{
						*b=(string[i]-'0')+((*b)*j);
						j*=10;
						i++;
					}
					else
					{
						j=1;
						contador++;
						estado=NOVENO;
					}
				}break;
				case NOVENO:
				{
					if(string[i]!=')')
						return ERROR_COMANDOS;
					else
					{
						estado=DECIMO;
						i++;
					}
				}break;
				case DECIMO:
				{
					if(string[i])
						return ERROR_COMANDOS;
				}break;
			}
		}
	}										
	if(contador!=2)
		return ERROR_COMANDOS;
	else if(*a<0 || *b<0 || *a>=filas || *b>=cols) 
		return ERROR_VALORES_BOMB;
	else if(tablerojug[*a][*b]!='.')
		return ERROR_ATACADO;
	
	return 0;
}	

int
marcaTablero(int num,int a,int b,int ** tablerojug,int ** tableroPC,tBarco * barcos,int tiposbarcos)
{
	int flag=0;
	int flag2=1;
	int i,j,h,k;
	
	if(num)
	{
		tablerojug[a][b]='X';
		for(i=0;i<tiposbarcos && !flag;i++)
			for(j=0;j<barcos[i].cantidad && !flag;j++)
				if((a>=barcos[i].cantbarcos[j].comienzo.fila && a<=barcos[i].cantbarcos[j].final.fila) && (b>=barcos[i].cantbarcos[j].comienzo.columna && b<=barcos[i].cantbarcos[j].final.columna))
					flag=1;
		for(h=barcos[i-1].cantbarcos[j-1].comienzo.fila;h<=barcos[i-1].cantbarcos[j-1].final.fila && flag;h++)
			for(k=barcos[i-1].cantbarcos[j-1].comienzo.columna;k<=barcos[i-1].cantbarcos[j-1].final.columna && flag;k++)
				if(tablerojug[h][k]!='X')
					flag2=0;
		if(flag2)
			for(h=barcos[i-1].cantbarcos[j-1].comienzo.fila;h<=barcos[i-1].cantbarcos[j-1].final.fila;h++)
				for(k=barcos[i-1].cantbarcos[j-1].comienzo.columna;k<=barcos[i-1].cantbarcos[j-1].final.columna;k++)		
						tablerojug[h][k]='D';
		tableroPC[a][b]='X';
	}			
	else
	{
			tablerojug[a][b]='A';
			flag2=0;
	}
	return flag2;
}

int
ataqueJugador(int a,int b,int ** tablerojug,int ** tableroPC,tBarco * barcos,int tiposbarcos,tPosicion * jugadajug)
{
	int num;
	
	jugadajug->fila=a;
	jugadajug->columna=b;
	
	if(tableroPC[a][b]!='@')
		num=0;
	else
		num=1;
		
	return marcaTablero(num,a,b,tablerojug,tableroPC,barcos,tiposbarcos);
}

void
guardaDatosIniciales(int tema,int turno,FILE * archivo,int undo)
{
	fprintf(archivo,"Tematica: %d\n",tema);
	fprintf(archivo,"Proximo turno: %s\n",(turno==0)?"Jugador":"Computadora");
	fprintf(archivo,"Undos: %d\n",undo-1);
	
}

int
estrategiaPC(int filas,int cols,int ** tablero2PC,int ** tablero1jug,tBarco * barcosjug,int tiposbarcos,tPosicion * jugadaPC)
{
	int num,num2;
	
	num=eligeCoordenadaAtaque(jugadaPC,filas,cols,tablero2PC,tablero1jug);
	num2=marcaTablero(num,jugadaPC->fila,jugadaPC->columna,tablero2PC,tablero1jug,barcosjug,tiposbarcos);
	if(num2==1)
		rellenaAgua(tablero2PC,barcosjug,tiposbarcos,jugadaPC,filas,cols);
		
	return num2;
}

void
guardaDatosJugador(int tiposbarcos,tBarco * barcos,FILE * archivo,int ** tablero1jug,int ** tablero2jug,int filas,int cols,char * string)
{
	int i,j,k,l;
	
	fprintf(archivo,"Datos %s\n",string);
	for(i=0;i<tiposbarcos;i++)
	{
		fprintf(archivo,"Elemento %d\n",i+1);
		for(j=0;j<barcos[i].cantidad;j++)
		{
			fprintf(archivo,"Miembro %d\n",j+1);
			for(k=barcos[i].cantbarcos[j].comienzo.fila;k<=barcos[i].cantbarcos[j].final.fila;k++)
				for(l=barcos[i].cantbarcos[j].comienzo.columna;l<=barcos[i].cantbarcos[j].final.columna;l++)
				{
					fprintf(archivo,"(%d,%d) -> ",k,l);
					if(tablero1jug[k][l]=='@' || tablero1jug[k][l]=='.')
						fputs("NO IMPACTADA\n",archivo);
					else
						fputs("IMPACTADA\n",archivo);
				}
		}
	}
	for(i=0;i<filas;i++)
		for(j=0;j<cols;j++)
		{
			if(tablero2jug[i][j]!='.')
			{
				fprintf(archivo,"(%d,%d) -> ",i,j);
				if(tablero2jug[i][j]=='A')
					fputs("AGUA\n",archivo);
				else if(tablero2jug[i][j]=='X')
					fputs("IMPACTO\n",archivo);
				else if(tablero2jug[i][j]=='D')
					fputs("DESTRUCCION\n",archivo);
			}
		}
}

void
comandoUndo(tPosicion jugadaPC,tPosicion jugadajug,int ** tablero1PC,int ** tablero2PC,int ** tablero1jug,int ** tablero2jug,int tiposbarcos,tBarco * barcosjug,tBarco * barcosPC,int * totalbarcosjug,int * totalbarcosPC)
{
	if(tablero1jug[jugadaPC.fila][jugadaPC.columna]=='X')
		tablero1jug[jugadaPC.fila][jugadaPC.columna]='@';
	if(tablero1PC[jugadajug.fila][jugadajug.columna]=='X')
		tablero1PC[jugadajug.fila][jugadajug.columna]='@';
	desmarcaTableroAtaqueJugador(tablero2jug,jugadajug,barcosPC,tiposbarcos,totalbarcosjug);
	desmarcaTableroAtaqueJugador(tablero2PC,jugadaPC,barcosjug,tiposbarcos,totalbarcosPC);

}

void
eligeEspacioMayor(int filas,int cols,tPosicion * coordenada,int ** tablero2PC)
{
	int a,b,i,j,colin,filin,max=0,k=0;
	tPosicion inicial,final;
	
	for(i=0;i<filas;i++)
	{
		k=0;
		for(j=0;j<cols;j++)
		{
			if(tablero2PC[i][j]=='.')
			{
				if(k==0)
					colin=j;
				k++;
				if(k>max)
				{
					max=k;
					inicial.fila=i;
					final.fila=i;
					inicial.columna=colin;
					final.columna=j;
				}
			}
			else
				k=0;
		}
	}
	k=0;
	
	for(i=0;i<cols;i++)
	{
		k=0;
		for(j=0;j<filas;j++)
		{
			if(tablero2PC[j][i]=='.')
			{
				if(k==0)
					filin=j;
				k++;
				if(k>max)
				{
					max=k;
					inicial.fila=filin;
					final.fila=j;
					inicial.columna=i;
					final.columna=i;
				}
				
			}
			else
				k=0;
		}
	}
	a=randInt(inicial.fila,final.fila);
	b=randInt(inicial.columna,final.columna);
	coordenada->fila=a;
	coordenada->columna=b;
}

int 
eligeCoordenadaAtaque(tPosicion * coordenada,int filas,int cols,int ** tablero2PC,int ** tablero1jug)
{
	int num,i,j,flag=0;
	/*Este ciclo busca si hay algun tocado en el tablero. En caso de haberlo
	* busca primero si habia algun tocado alrededor y sino elige un casillero de
    * los aledaños al tocado para atacar.
    */
	for(i=0;i<filas && !flag;i++) 
		for(j=0;j<cols && !flag;j++)
		{
			if(tablero2PC[i][j]=='X')			
			{
				if(coordenada1(coordenada,i,j,cols,tablero2PC))
					flag=1;
				else if(coordenada2(coordenada,i,j,filas,tablero2PC))
					flag=1;
				else if(coordenada3(i,j,tablero2PC,coordenada,filas,cols))
					flag=1;
			}
		}
	/* Si no habia ningun tocado procede entonces a buscar el agujero mas grande del tablero para atacar aleatoriamente ahi*/
	if(flag==0)
		eligeEspacioMayor(filas,cols,coordenada,tablero2PC);
	
	if(tablero1jug[coordenada->fila][coordenada->columna]=='.')
		num=0;
	else
		num=1;
	
	return num;
}
			
int
coordenada1(tPosicion * coordenada,int i,int j,int cols,int ** tablero2PC)
{
	int k,l;

	if((j+1)<cols && tablero2PC[i][j+1]=='X')
	{
		k=i;
		l=j+1;
		while(l<cols && tablero2PC[k][l]!='.' && tablero2PC[k][l]!='A')
			l++;
		if(l<cols && tablero2PC[k][l]=='.')
		{
			coordenada->fila=k;
			coordenada->columna=l;
			return 1;
		}
		else
		{
			do
			{
				l--;
			}while(tablero2PC[k][l]!='.');
			coordenada->fila=k;
			coordenada->columna=l; 
			return 1;
		 }
	}
	
	return 0;
}

int
coordenada2(tPosicion * coordenada,int i,int j,int filas,int ** tablero2PC)
{
	int k,l;
	
	if((i+1)<filas && tablero2PC[i+1][j]=='X')
	{
		k=i+1;
		l=j;
		while(k<filas && tablero2PC[k][l]!='.' && tablero2PC[k][l]!='A')
			k++;
		if(k<filas && tablero2PC[k][l]=='.')
		{
			coordenada->fila=k;
			coordenada->columna=l;
			return 1;
		}
		else
		{
			do
			{
				k--;
			}while(tablero2PC[k][l]!='.');
			coordenada->fila=k;
			coordenada->columna=l;
			return 1;
		}
	}
	return 0;
}

int
coordenada3(int i,int j,int ** tablero2PC,tPosicion * coordenada,int filas,int cols)
{
	int flag,a,b;
	
	do
	{
		flag=0;
		a=randInt(0,1);
		b=randInt(-1,1);
		if(a==1 && (j+b)>=0 && (j+b)<cols && (j+b)!=j)
		{
			coordenada->fila=i;
			coordenada->columna=j+b;
			flag=1;
		}
		else if(a==0 && (i+b)>=0 && (i+b)<filas && (i+b)!=i)
		{
			coordenada->fila=i+b;
			coordenada->columna=j;
			flag=1;
		}
	}while(!flag || tablero2PC[coordenada->fila][coordenada->columna]!='.');
	
	return 1;
}

void
rellenaAgua(int ** tablero2PC,tBarco * barcosjug,int tiposbarcos,tPosicion * coordenada,int filas,int cols)
{
	int i,j,h,k,a,b,flag=0;
	
	for(i=0;i<tiposbarcos && !flag;i++)
		for(j=0;j<barcosjug[i].cantidad && !flag;j++)
			if((coordenada->fila>=barcosjug[i].cantbarcos[j].comienzo.fila && coordenada->fila<=barcosjug[i].cantbarcos[j].final.fila) && (coordenada->columna>=barcosjug[i].cantbarcos[j].comienzo.columna && coordenada->columna<=barcosjug[i].cantbarcos[j].final.columna))
				flag=1;
	for(h=barcosjug[i-1].cantbarcos[j-1].comienzo.fila;h<=barcosjug[i-1].cantbarcos[j-1].final.fila;h++)
		for(k=barcosjug[i-1].cantbarcos[j-1].comienzo.columna;k<=barcosjug[i-1].cantbarcos[j-1].final.columna;k++)
			for(a=-1;a<=1;a++)
				for(b=-1;b<=1;b++)
					if((h+a)>=0 && (h+a)<filas && (k+b)>=0 && (k+b)<cols && !((h+a)>=barcosjug[i-1].cantbarcos[j-1].comienzo.fila && (h+a)<=barcosjug[i-1].cantbarcos[j-1].final.fila && (k+b)>=barcosjug[i-1].cantbarcos[j-1].comienzo.columna && (k+b)<=barcosjug[i-1].cantbarcos[j-1].final.columna))
						tablero2PC[h+a][k+b]='A';
		
}

void
desmarcaTableroAtaqueJugador(int ** tablero2jug,tPosicion jugadajug,tBarco * barcos,int tiposbarcos,int * totalbarcosjug)
{
	int i,j,h,k;
	int flag=0;
	
	if(tablero2jug[jugadajug.fila][jugadajug.columna]=='A' || tablero2jug[jugadajug.fila][jugadajug.columna]=='X')
		tablero2jug[jugadajug.fila][jugadajug.columna]='.';
	else
	{
		for(i=0;i<tiposbarcos && !flag;i++)
			for(j=0;j<barcos[i].cantidad && !flag;j++)
				if((jugadajug.fila>=barcos[i].cantbarcos[j].comienzo.fila && jugadajug.fila<=barcos[i].cantbarcos[j].final.fila) && (jugadajug.columna>=barcos[i].cantbarcos[j].comienzo.columna && jugadajug.columna<=barcos[i].cantbarcos[j].final.columna))
					flag=1;
		for(h=barcos[i-1].cantbarcos[j-1].comienzo.fila;h<=barcos[i-1].cantbarcos[j-1].final.fila;h++)
			for(k=barcos[i-1].cantbarcos[j-1].comienzo.columna;k<=barcos[i-1].cantbarcos[j-1].final.columna;k++)
				tablero2jug[h][k]='X';
		
		tablero2jug[jugadajug.fila][jugadajug.columna]='.';
		(*totalbarcosjug)=(*totalbarcosjug)+1;
	}
}

int
validaIdTem(FILE * archivo,int numtemas)
{
	int correcto,tematica;
	
	correcto=fscanf(archivo,"%d",&tematica);	

	if(correcto==0 || tematica>numtemas || tematica<=0)
		return ERROR_CARGANDO;
	else
		salteaComentario(archivo);
	
	return tematica;
}

int
validaIdTurno(FILE * archivo,int * turno)
{
	char * str1="ugador";
	char * str2="omputadora";
	int i,c,flag=0;
	
	c=fgetc(archivo);
	
	if(c=='J')
	{
		for(i=0;str1[i]!=0 && !flag;i++)
			if((c=fgetc(archivo))!=str1[i])
				flag=1;
		if(flag==0)
		{
			*turno=0;
			salteaComentario(archivo);
		}
		else
			return ERROR_CARGANDO;
	}
	else if(c=='C')
	{
		for(i=0;str2[i]!=0 && !flag;i++)
			if(fgetc(archivo)!=str2[i])
				flag=1;
		if(flag==0)
		{
			*turno=1;
			salteaComentario(archivo);
		}
		else
			return ERROR_CARGANDO;
	}
	else
		return ERROR_CARGANDO;
		
	return 0;
		
}

int
validaIdUndo(FILE * archivo,int * undo)
{
	int correcto,valor;
	
	correcto=fscanf(archivo,"%d",&valor);	

	if(correcto==0 || valor<0 || valor>3)
		return ERROR_CARGANDO;
	else
	{
		*undo=valor+1;
		salteaComentario(archivo);
	}
	
	return 0;
}

int 
validaIdTematica2(FILE * archivo,int numeroTem)
{
	int correcto,tematica;
	
	correcto=fscanf(archivo,"%d",&tematica);
	
	if(correcto==0 || tematica!=(numeroTem+1))
		return ERROR_CARGANDO;
	else
		salteaComentario(archivo);
	
	return 0;
}

int
validaCoordenadasBarcos(FILE * archivo,int filas,int cols,int ** tablero1jug,int longitud,tBarco * barcos,int tipobarco,int numbarco)
{
	int i,correcto,fila,columna,j;
	
	for(i=0;i<longitud;i++)
	{
		correcto=fscanf(archivo,"(%d,%d",&fila,&columna);
		if(correcto!=2 || fila<0 || fila>=filas || columna<0 || columna>=cols)
			return ERROR_CARGANDO;
		if(fgetc(archivo)!=')')
			return ERROR_CARGANDO;
		if(fgetc(archivo)!=' ')
			return ERROR_CARGANDO;
		if(fgetc(archivo)!='-')
			return ERROR_CARGANDO;
		if(fgetc(archivo)!='>')
			return ERROR_CARGANDO;
		if(fgetc(archivo)!=' ')
			return ERROR_CARGANDO;
		if(tablero1jug[fila][columna]=='.')
		{
			switch(validaStringCarga(archivo))
			{
				case 0: return ERROR_CARGANDO;break;
				case 1:	tablero1jug[fila][columna]='X';break;
				case 2:	tablero1jug[fila][columna]='@';break;
			}
		}
		else
			return ERROR_CARGANDO;
		if(i==0)
		{
			barcos[tipobarco].cantbarcos[numbarco].comienzo.fila=fila;
			barcos[tipobarco].cantbarcos[numbarco].comienzo.columna=columna;
		}
		if(i==longitud-1)
		{
			barcos[tipobarco].cantbarcos[numbarco].final.fila=fila;
			barcos[tipobarco].cantbarcos[numbarco].final.columna=columna;
		}
	}				
	for(i=barcos[tipobarco].cantbarcos[numbarco].comienzo.fila;i<=barcos[tipobarco].cantbarcos[numbarco].final.fila;i++)
		for(j=barcos[tipobarco].cantbarcos[numbarco].comienzo.columna;j<=barcos[tipobarco].cantbarcos[numbarco].final.columna;j++)
			if(tablero1jug[i][j]=='.')
				return ERROR_CARGANDO;
				
	barcos[tipobarco].cantbarcos[numbarco].puesto=1;
	
	if(validaCasillero(tablero1jug,barcos[tipobarco].cantbarcos[numbarco].comienzo,barcos[tipobarco].cantbarcos[numbarco].final,filas,cols,barcos[tipobarco].cantbarcos[numbarco]))
		return ERROR_CARGANDO;

	return 0;
}

int
validaStringCarga(FILE * archivo)
{
	char * string="MPACTADA";
	char * string2="O IMPACTADA";
	int c,i=0;
	
	if((c=fgetc(archivo))=='I')
	{
		while(string[i]!=0)
		{
			if(fgetc(archivo)!=string[i])
				return 0;
			i++;
		}
		salteaComentario(archivo);
		return 1;		
	}
	else if(c=='N')
	{
		while(string2[i]!=0)
		{
			if(fgetc(archivo)!=string2[i])
				return 0;
			i++;
		}
		salteaComentario(archivo);
		return 2;
	}
	else
		return 0;
}

int
validaCoordenadasTablero2(FILE * archivo,int ** tablero2jug,int filas,int cols)
{
	int c,correcto,fila,columna;
	
	while((c=fgetc(archivo))!='D' && c!=EOF)
	{
		if(c!='(')
			return ERROR_CARGANDO;
		correcto=fscanf(archivo,"%d,%d",&fila,&columna);
		if(correcto!=2 || fila>=filas || fila<0 || columna<0 || columna>=cols)
			return ERROR_CARGANDO;
		if(fgetc(archivo)!=')')
			return ERROR_CARGANDO;
		if(fgetc(archivo)!=' ')
			return ERROR_CARGANDO;
		if(fgetc(archivo)!='-')
			return ERROR_CARGANDO;
		if(fgetc(archivo)!='>')
			return ERROR_CARGANDO;
		if(fgetc(archivo)!=' ')
			return ERROR_CARGANDO;
		if(tablero2jug[fila][columna]=='.')
		{
			switch(validaStringCarga2(archivo))
			{
				case 0: return ERROR_CARGANDO;break;
				case 1: tablero2jug[fila][columna]='D';break;
				case 2: tablero2jug[fila][columna]='X';break;
				case 3: tablero2jug[fila][columna]='A';break;
			}
		}
		else
			return ERROR_CARGANDO;
	}
	fseek(archivo,-1,SEEK_CUR);
	salteaComentario(archivo);
	return 0;
}
	
int
validaStringCarga2(FILE * archivo)
{
	char * str1="ESTRUCCION";
	char * str2="MPACTO";
	char * str3="GUA";
	int c,i=0;
	
	if((c=fgetc(archivo))=='D')
	{
		while(str1[i]!=0)
		{
			if(fgetc(archivo)!=str1[i])
				return 0;
			i++;
		}
		salteaComentario(archivo);
		return 1;
	}
	else if(c=='I')
	{
		while(str2[i]!=0)
		{
			if(fgetc(archivo)!=str2[i])
				return 0;
			i++;
		}
		salteaComentario(archivo);
		return 2;
	}
	else if(c=='A')
	{
		while(str3[i]!=0)
		{
			if(fgetc(archivo)!=str3[i])
				return 0;
			i++;
		}
		salteaComentario(archivo);
		return 3;
	}
	else
		return 0;
}

int
validaCoordenadasBarcos2(int filas,int cols,int ** tablero1jug,int ** tablero2jug,int tiposbarcos,tBarco * barcos)
{
	int i,j,k,l,estado,contador,barcostablero,hundidos=0;
	
	for(i=0;i<filas;i++)
		for(j=0;j<cols;j++)
		{
			if(tablero2jug[i][j]=='A')
			{
				if(tablero1jug[i][j]!='.')
					return ERROR_CARGANDO;
			}
			else if(tablero2jug[i][j]=='X' || tablero2jug[i][j]=='D')
			{
				if(tablero1jug[i][j]!='X')
					return ERROR_CARGANDO;
			}
		}
	for(i=0;i<tiposbarcos;i++)
		for(j=0;j<barcos[i].cantidad;j++)
		{
			estado=0;
			contador=0;
			for(k=barcos[i].cantbarcos[j].comienzo.fila;k<=barcos[i].cantbarcos[j].final.fila;k++)
			{
				for(l=barcos[i].cantbarcos[j].comienzo.columna;l<=barcos[i].cantbarcos[j].final.columna;l++)
				{
					switch(estado)
					{
						case 0: 
						{
							if(tablero2jug[k][l]=='D')
							{
								if(tablero1jug[k][l]!='X')
									return ERROR_CARGANDO;
								else
									estado=1;
							}
							else if(tablero2jug[k][l]=='X')
							{
								if(tablero1jug[k][l]!='X')
									return ERROR_CARGANDO;
								else
									estado=2;
							}
							else
							{
								if(tablero1jug[k][l]!='@')
									return ERROR_CARGANDO;
								contador++;
								estado=2;
							}
						}break;
						case 1:
						{
							if(!(tablero2jug[k][l]=='D' && tablero1jug[k][l]=='X'))
								return ERROR_CARGANDO;
						}break;
						case 2:
						{
							if(tablero2jug[k][l]=='X')
							{
								if(tablero1jug[k][l]!='X')
									return ERROR_CARGANDO;
							}
							else if(tablero2jug[k][l]=='D')
								return ERROR_CARGANDO;
							else 
								contador++;
						}break;
					}
				}
			}
			if(barcos[i].cantbarcos[j].longitud==1 && tablero2jug[k-1][l-1]=='X')
				return ERROR_CARGANDO;
			if(estado==2 && contador==0)
				return ERROR_CARGANDO;
			if(estado==1)
				hundidos++;
		}
	barcostablero=barcosTotales(barcos,tiposbarcos);
	
	return (barcostablero-hundidos);
}


void
libera(tJuego * juego,int numtemas)
{
	int i,j,k;
	
	for(i=0;i<numtemas;i++)
	{
		for(k=0;k<juego[i].jugador[0].tablero->filas;k++)
		{
			free(juego[i].jugador[0].tablero->tablero1[k]);
			free(juego[i].jugador[0].tablero->tablero2[k]);
			free(juego[i].jugador[1].tablero->tablero1[k]);
			free(juego[i].jugador[1].tablero->tablero2[k]);
		}
												
		free(juego[i].jugador[0].tablero->tablero1);
		free(juego[i].jugador[1].tablero->tablero1);
		free(juego[i].jugador[0].tablero->tablero2);
		free(juego[i].jugador[1].tablero->tablero2);
		for(j=0;j<juego[i].jugador[0].tiposbarcos;j++)
		{
			free(juego[i].jugador[0].barcos[j].cantbarcos);
			free(juego[i].jugador[1].barcos[j].cantbarcos);
			free(juego[i].jugador[0].barcos[j].nombre);
			free(juego[i].jugador[1].barcos[j].nombre);
		}
		free(juego[i].jugador[0].barcos);
		free(juego[i].jugador[1].barcos);
		free(juego[i].jugador[0].tablero);
		free(juego[i].jugador[1].tablero);
		free(juego[i].nombreTema);
		free(juego[i].jugador);
	}
	free(juego);
}
