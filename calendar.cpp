#include <stdio.h>

class Pantalla
{
	public:
		char pantalla[80][80];
		int filas;
		int columnas;
		
		virtual void imprimir_todo(void) = 0;
		virtual void imprimir_linea(int linea) = 0;
};

class PantallaMensual : public Pantalla
{
	private:
		int month;
		int year;

		int get_posicion_zeller(int day, int month, int year) {
			int a, y, m;

			a = (14 - month) / 12;
			y = year - a;
			m = month + 12 * a - 2;
		return ((day + y + y/4 - y/100 + y/400 + (31*m) / 12) %7);
		}

		/* Devuelve la posición donde debe emepzar a imprimirse el primer caracter de un día
			dado el día de la semana.
			La posición está calculada en un calendario de mes de 6x26 caracteres (ver enunciado)
		*/
		int from_dia_semana_to_pos(int dia_semana) {
			int pos;

			// Calculamos la posición del caracter donde debe imprimirse el primer dígito del día
			// según el formato fijo que nos han dado en el enunciado
			switch(dia_semana) {
				case 0: pos = 20; break;
				case 1: pos = 0; break;
				case 2: pos = 3; break;
				case 3: pos = 6; break;
				case 4: pos = 9; break;
				case 5: pos = 12; break;
				case 6: pos = 17; break;
				default: pos= -1;
			}
			return pos;
		}

		/* Devuelve >0 si es una posición donde debe imprimirse ',' según el enunciado.
			La posición está calculada en un calendario de mes de 6x26 caracteres (ver enunciado)
		*/
		int es_punto(int pos) {
			if(pos == 1|| pos==4 || pos == 7 || pos == 10 || pos==13|| pos==18 || pos==21)
				return 1;
			return 0;
		}

	public:
		PantallaMensual(int month, int year) {

			int dia;
			int dia_semana, i, j, col, fila, total_semanas;
			char buff[2];

			sprintf (pantalla[0], "                         ");  
			switch (month) {
      
				case 1:  sprintf (pantalla[1], "ENERO             %d   ", year);
					break;
				case 2:  sprintf (pantalla[1], "FEBRERO           %d   ", year);
					break;
				case 3:  sprintf (pantalla[1], "MARZO             %d   ", year);
					break;
				case 4:  sprintf (pantalla[1], "ABRIL             %d   ", year);
					break;
				case 5:  sprintf (pantalla[1], "MAYO              %d   ", year);
					break;
				case 6:  sprintf (pantalla[1], "JUNIO             %d   ", year);
					break;
				case 7:  sprintf (pantalla[1], "JULIO             %d   ", year);
					break;
				case 8:  sprintf (pantalla[1], "AGOSTO            %d   ", year);
					break;
				case 9:  sprintf (pantalla[1], "SEPTIEMBRE        %d   ", year);
					break;
				case 10: sprintf (pantalla[1], "OCTUBRE           %d   ", year);
					break;
				case 11: sprintf (pantalla[1], "NOVIEMBRE         %d   ", year);
					break;
				default: sprintf (pantalla[1], "DICIEMBRE         %d   ", year);  
			}//switch

			/*Dibujar la estructura del calendario*/
			sprintf (pantalla[2], "======================   ");  
      		sprintf (pantalla[3], "LU MA MI JU VI | SA DO   ");
			sprintf (pantalla[4], "======================   ");  

			// Rellenamos todos los días con blanco o con punto si es la posición del dígito
			for(i=5; i<11; i++)
				for(j=0; j<23+2; j++) {
					if(es_punto(j))
						pantalla[i][j] = '.';
					else
						pantalla[i][j] = ' ';
				}

			// Colocamos los separadores de fin de semana
			for(i=5; i<11;i ++)
				pantalla[i][15] = '|';

			// Rellenamos los dias dentro del calendario
			fila = 5;
			for(dia=1; dia<=31; dia++) {


				dia_semana = get_posicion_zeller(dia, month, year);


				// Calculamos la columna en la matriz
				col = from_dia_semana_to_pos(dia_semana);


				sprintf(buff, "%2d", dia);
				if(dia>=10) {

					/* 
						No es dia 30 y es Febrero
						No es el día 31 y es un mes de < 31 dias
						No es dia 29 y no siendo año bisiesto
					*/
					if(dia==30 && month == 2)
						break;
					if(dia==31 && (month == 2 || month==4 || month==6 || month==9 || month==11))
						break;
					if(dia==29 && month == 2 && !(year%4==0 && year%100!=0 || year%400==0))
						break;

					pantalla[fila][col] = buff[0];
					pantalla[fila][col+1] = buff[1];
				} else
					pantalla[fila][col+1] = buff[1];

				printf("mes %d dia %d dia_semana %d (col %d fila %d)\n", month, dia, dia_semana, col, fila);

				// Por cada domingo encontrado, cambiamos de semana (pasamos a la siguiente fila)
				if(!dia_semana) {
					fila++;
				}
			}

			// Dejamos calculadas las filas y columnas que ocupa el mes
			if(dia_semana)
				fila++;

			// Quitamos la linea de puntos si no tiene ningún dia
			if(pantalla[fila][1]=='.') {
				sprintf(pantalla[fila], "                         "); 
			}
			
			columnas = 23 + 2; // Añadimos 3 para los tres espacios requiridos entre dos meses
			filas = fila;

			printf("fila %d columna %d mes %d\n", filas, columnas, month);
        }

		void imprimir_todo() {
			int i,j;

			for(i=0;i<filas;i++) {
				for(j=0;j<columnas;j++)
					printf("%c", pantalla[i][j]);
				printf("\n");
			}
		}

		void imprimir_linea(int linea) {

			if(linea>filas) return;

			for(int j=0;j<columnas;j++)
				printf("%c", pantalla[linea][j]);
		}

};

class CalendarioMensual {

	private:
		int filas;
		int columnas;
		PantallaMensual* pantalla_meses[12][12];

	public:
		CalendarioMensual(int filas, int columnas, int year) {
			int month = 1;
			this->filas = filas;
			this->columnas = columnas;
			for(int i=0; i<filas;i++)
				for(int j=0; j<columnas;j++) {
					PantallaMensual * pm = new PantallaMensual(month, year);
					pantalla_meses[i][j] = pm;
					month++;
				}
		}

		void imprime() {
			for(int i=0; i<filas;i++) {
			for(int semana=0; semana<11; semana++) {
					for(int j=0; j<columnas;j++) {
						pantalla_meses[i][j]->imprimir_linea(semana);
					}
					printf("\n");
				}
			}
		}
};
 
class Principal
{
	private:
		CalendarioMensual* caledario_mensual;

	public:

		// Constructor
		Principal(int year) {
			this->caledario_mensual = new CalendarioMensual(4, 3, year);
		}

		// Muestra el calendario
		void imprime() {
			caledario_mensual->imprime();
		}
};

int main()
{
	int year;

	/* Solicitamos el año dentro de los que marca el enunciado*/
	printf ("A\xa4o (1601...3000)?");
// 	scanf ("%d", &year);
	year = 2014;
	if (year<1601 || year>3000) {
    	printf ("Los valores introducidos no son correctos\n");
		return -1;
	}

	Principal* principal = new Principal(year);
	principal->imprime();
	// delete &principal;

	// delete pm;

	return 0;

}
