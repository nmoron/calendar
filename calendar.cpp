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

		int from_dia_semana_to_pos(int dia, int dia_semana) {
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

		int es_punto(int pos) {
			if(pos == 1|| pos==4 || pos == 7 || pos == 10 || pos==13|| pos==18 || pos==21)
				return 1;
			return 0;
		}
	public:
		PantallaMensual(int month, int year) {

			int dia;
			int dia_semana, i, j, col, fila, total_semanas;
			// char pantalla[6][23];
			char buff[2];

			// Rellenamos todos los días con blanco o con punto si es la posición del dígito
			for(i=0;i<6;i++)
				for(j=0;j<23;j++) {
				if(es_punto(j))
					pantalla[i][j] = '.';
				else
					pantalla[i][j] = ' ';
			}

			// Colocamos los separadores de fin de semana
			for(i=0;i<6;i++)
				pantalla[i][15] = '|';

			// Rellenamos los dias dentro del calendario
			fila = 0;
			for(dia=1;dia<32;dia++) {

				dia_semana = get_posicion_zeller(dia, month, year);

				// Calculamos la columna en la matriz
				col = from_dia_semana_to_pos(dia, dia_semana);

				// printf("fila %d col %d dia %d diasem %d \n", fila, col, dia, dia_semana);

				// Saltar bisiestos y dias 31 en meses de 20

				sprintf(buff, "%2d", dia);
				if(dia>=10) {

					/* 
						No es dia 30 y es Febrero
						No es el día 31 y es un mes de < 31 dias
						No es dia 29 y no siendo año bisiesto (BUG1 : no funciona. probar con 2014)
					*/
					if( (!(dia==30 && month == 2)) &&
						(!(dia==31 && (month == 2 || month==4 || month==6 || month==9 || month==11))) &&
						(!(dia==29 && month ==2 && (year%4==0 && year%100!=0 || year%400==0))) ) {


						pantalla[fila][col] = buff[0];
						pantalla[fila][col+1] = buff[1];
					}
				} else
					pantalla[fila][col+1] = buff[1];

				// Por cada domingo encontrado, cambiamos de semana (pasamos a la siguiente fila)
				if(!dia_semana) {
					fila++;
				}

			}

			// Dejamos calculadas las filas y columnas que ocupa el mes
			filas = fila+1;
			columnas = 23;
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
				for(int semana=0; semana<6; semana++) {
					for(int j=0; j<columnas;j++) {
						pantalla_meses[i][j]->imprimir_linea(semana);
						printf("   ");
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
		Principal(int month, int year) {
			this->caledario_mensual = new CalendarioMensual(4, 3, year);
		}

		// Muestra el calendario
		void imprime() {
			caledario_mensual->imprime();
		}
};

int main()
{
	int year, month;

	/* Solicitamos el año dentro de los que marca el enunciado*/
	printf ("Mes (1...12)?");
	scanf ("%d", &month);
	printf ("A\xa4o (1601...3000)?");
	scanf ("%d", &year);
	if (year<1601 || year>3000) {
    	printf ("Los valores introducidos no son correctos\n");
		return -1;
	}

	Principal* principal = new Principal(month, year);
	principal->imprime();
	// delete &principal;

	// delete pm;

	return 0;

}
