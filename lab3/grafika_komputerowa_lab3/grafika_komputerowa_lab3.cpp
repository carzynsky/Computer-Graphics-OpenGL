// grafika_komputerowa_lab3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
/*************************************************************************************/

//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacją osi 
//  układu współrzednych

/*************************************************************************************/

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <vector>
#include <cmath>

# define M_PI 3.14159265358979323846
static GLfloat theta[] = { 0.0, 0.0, 0.0 }; // trzy kąty obrotu
typedef float point3[3];
const int N = 50;
int model = 1;
/*************************************************************************************/
void spinEgg();
void keys(unsigned char key, int x, int y);

// Funkcja rysująca osie układu współrzędnych
struct p {
	float x, y, z;
};
p matrix[N][N];

float randColor()
{
	return static_cast< float> (rand()) / static_cast <float> (RAND_MAX); // 0.0 ; 1.0
}

void Axes(void)
{

	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// początek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0,  5.0, 0.0 };
	// początek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0,  5.0 };
	//  początek i koniec obrazu osi y

	//glRotated(60.0, .0, 1.0, 1.0);
	//glRotated(90.0, .0, 1.0, 0.0);

	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
	glBegin(GL_LINES); // rysowanie osi x

	glVertex3fv(x_min);
	glVertex3fv(x_max);

	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
	glBegin(GL_LINES);  // rysowanie osi y

	glVertex3fv(y_min);
	glVertex3fv(y_max);

	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z

	glVertex3fv(z_min);
	glVertex3fv(z_max);

	glEnd();

}
void Imbryczek()
{
	glColor3f(1.0f, 1.0f, 0.0f);
	glRotated(45.0f, 1.0f, 1.0f, 1.0f);
	glutWireTeapot(3.0);

}

void generateMatrix()
{
	float u, v;
	for (float i = 0; i < N; i++)
	{
		u = i / (N - 1);
		for (float j = 0; j < N; j++)
		{
			v = j / (N - 1);
			matrix[(int)i][(int)j].x = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*cos(v*M_PI);
			matrix[(int)i][(int)j].y = 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2);
			matrix[(int)i][(int)j].z = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*sin(v*M_PI);
		}
	}

}
void EggNet(int n)
{
	glRotated(-40.0, 1.0, 0.0, 0.0);
	glTranslated(0.0f, -4.5f, 0.0f);
	int i=0, j=0;
	glBegin(GL_LINES);
	
	for (i = 0; i < n-1; i++)
	{


		for (j = 0; j < n-1; j++)
		{

			glColor3f(0.8f, 0.9f, 0.0f);

			
			glVertex3f(matrix[i][j].x, matrix[i][j].y, matrix[i][j].z);
			glVertex3f(matrix[i][j+1].x, matrix[i][j+1].y, matrix[i][j+1].z);

			glColor3f(0.8f, 0.8f, 0.0f);


			glVertex3f(matrix[i][j].x, matrix[i][j].y, matrix[i][j].z);
			glVertex3f(matrix[i + 1][j].x, matrix[i + 1][j].y, matrix[i + 1][j].z);
			

		}
		
	}
	glEnd();

}
void Egg(int n)
{
	
	glRotated(-40.0, 1.0, 0.0, 0.0 );
	glTranslated(0.0f, -4.5f, 0.0f);
	float u, v;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{

			glColor3f(randColor(), 0.0f, 1.0f); 

			glBegin(GL_POINTS);
			glVertex3f(matrix[i][j].x, matrix[i][j].y, matrix[i][j].z);
			glEnd();
			
		}
	}
	
}
void EggTriangle(int n)
{

	float u, v;
	glRotated(-40.0, 1.0, 0.0, 0.0);
	glTranslated(0.0f, -4.5f, 0.0f);
	for (int i = 0; i < n-1; i++)
	{
		for (int j = 0; j < n-1; j++)
		{
			v = (float)(j / (N - 1));
			glBegin(GL_TRIANGLES);
			glColor3f(0.0f, 0.0f, 0.1f);
			glVertex3f(matrix[i][j].x, matrix[i][j].y, matrix[i][j].z);
			
			glVertex3f(matrix[i + 1][j].x, matrix[i + 1][j].y, matrix[i + 1][j].z);
			
			glVertex3f(matrix[i + 1][j + 1].x, matrix[i + 1][j + 1].y, matrix[i + 1][j + 1].z);

			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(matrix[i][j].x, matrix[i][j].y, matrix[i][j].z);
			
			glVertex3f(matrix[i][j + 1].x, matrix[i][j + 1].y, matrix[i][j + 1].z);
			
			glVertex3f(matrix[i + 1][j + 1].x, matrix[i + 1][j + 1].y, matrix[i + 1][j + 1].z);

			glEnd();
			// drugi trojkat




		}
		
	}
}
void EggTriangleStrip(int n)
{
	
	glRotated(-40.0, 1.0, 0.0, 0.0);
	glTranslated(0.0f, -4.5f, 0.0f);
	for (int i = 0; i < n-1; i++)
	{
		for (int j = 2; j < n; j++)
		{
						
			glBegin(GL_TRIANGLE_STRIP);
			glColor3f(1.0f, 0.0f, 0.7f);
			glVertex3f(matrix[(i)][j].x, matrix[i][j].y, matrix[i][j].z);			// *

			glColor3f(1.0f, 0.0f, 0.7f);
			glVertex3f(matrix[(i+1)][j].x, matrix[(i+1)][j].y, matrix[(i+1)%n][j%n].z);			//     *

			glColor3f(1.0f, 0.0f, 0.8f);
			glVertex3f(matrix[i][(j-1)].x, matrix[i][(j-1)].y, matrix[i][(j-1)].z);        // * 

			glColor3f(1.0f, 0.4f, 0.9f);
			glVertex3f(matrix[(i+1)][(j-1)].x, matrix[(i+1)][(j-1)].y, matrix[(i+1)][(j-1)].z);        //       * 

			glColor3f(1.0f, 0.4f, 0.9f);
			glVertex3f(matrix[i][(j - 2)].x, matrix[i%n][(j - 2)%n].y, matrix[i%n][(j - 2)].z);

			glColor3f(1.0f, 0.3f, 0.8f);
			glVertex3f(matrix[(i+1)][(j - 2)].x, matrix[(i+1)][(j - 2)].y, matrix[(i+1)][(j - 2)].z);

			glEnd();
		}
	}
}
/*************************************************************************************/

// Funkcja określająca co ma być rysowane (zawsze wywoływana gdy trzeba
// przerysować scenę)



void RenderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszczącym

	glLoadIdentity();
	// Czyszczenie macierzy bieżącej

	//Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej wyżej
	//Imbryczek();
	glRotatef(theta[0], 1.0, 0.0, 0.0);

	glRotatef(theta[1], 0.0, 1.0, 0.0);

	glRotatef(theta[2], 0.0, 0.0, 1.0);
	if (model == 1)
	{
		EggNet(N);
	}
	if (model == 2)
	{
		EggTriangleStrip(N);
	}
	if (model == 3)
	{
		EggTriangle(N);
	}
	if (model == 4)
	{
		Egg(N);
	}
	glFlush();
	// Przekazanie poleceń rysujących do wykonania



	glutSwapBuffers();
	//

}

/*************************************************************************************/

// Funkcja ustalająca stan renderowania



void MyInit(void)
{

	glClearColor(1.0f, 1.0f, 0.9f, 0.3f);
	// Kolor czyszcący (wypełnienia okna) ustawiono na czarny

}

/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie stałych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokość i szerokość okna) są
// przekazywane do funkcji za każdym razem gdy zmieni się rozmiar okna.



void ChangeSize(GLsizei horizontal, GLsizei vertical)
{

	GLfloat AspectRatio;
	// Deklaracja zmiennej AspectRatio  określającej proporcję
	// wymiarów okna 

	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0

		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkościokna okna widoku (viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)  

	glMatrixMode(GL_PROJECTION);
	// Przełączenie macierzy bieżącej na macierz projekcji 

	glLoadIdentity();
	// Czyszcznie macierzy bieżącej            

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie współczynnika  proporcji okna
	// Gdy okno nie jest kwadratem wymagane jest określenie tak zwanej
	// przestrzeni ograniczającej pozwalającej zachować właściwe
	// proporcje rysowanego obiektu.
	// Do okreslenia przestrzeni ograniczjącej służy funkcja
	// glOrtho(...)            

	if (horizontal <= vertical)

		glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);

	else

		glOrtho(-7.5*AspectRatio, 7.5*AspectRatio, -7.5, 7.5, 10.0, -10.0);

	glMatrixMode(GL_MODELVIEW);
	// Przełączenie macierzy bieżącej na macierz widoku modelu                                    

	glLoadIdentity();
	// Czyszcenie macierzy bieżącej

}

/*************************************************************************************/

// Główny punkt wejścia programu. Program działa w trybie konsoli



void main(void)
{
	generateMatrix();
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 300);

	glutCreateWindow("Grafika lab3 - Autor Arkadiusz Carzynski");

	glutDisplayFunc(RenderScene);
	// Określenie, że funkcja RenderScene będzie funkcją zwrotną
	// (callback function).  Bedzie ona wywoływana za każdym razem
	// gdy zajdzie potrzba przeryswania okna 

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną
	// zazmiany rozmiaru okna      

	MyInit();
	// Funkcja MyInit() (zdefiniowana powyżej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przystąpieniem do renderowania 

	glEnable(GL_DEPTH_TEST);
	// Włączenie mechanizmu usuwania powierzchni niewidocznych

	glutIdleFunc(spinEgg);
	glutKeyboardFunc(keys);
	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}
void spinEgg()
{

	theta[0] -= 0.02;
	if (theta[0] > 360.0) theta[0] -= 360.0;

	theta[1] -= 0.02;
	if (theta[1] > 360.0) theta[1] -= 360.0;

	theta[2] -= 0.02;
	if (theta[2] > 360.0) theta[2] -= 360.0;

	glutPostRedisplay(); //odświeżenie zawartości aktualnego okna
}

void keys(unsigned char key, int x, int y)
{
	if (key == 'p') model = 1;
	if (key == 'w') model = 2;
	if (key == 's') model = 3;
	if (key == 'x') model = 4;
	RenderScene(); // przerysowanie obrazu sceny
}


/*************************************************************************************/


