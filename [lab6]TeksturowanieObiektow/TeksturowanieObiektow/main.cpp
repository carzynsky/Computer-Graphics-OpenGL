#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>
#include <cstdio>
#include <string>

#define PI 3.14159265359
typedef float point3[3];

static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
static GLfloat object[] = { 0.0, 0.0, 0.0 };
static GLfloat zoom = 0.0;
static GLfloat theta[] = { 0.0, 0.0 };   // k�ty

static GLfloat pix2angle;     // przelicznik pikseli na stopnie
//static GLfloat pix2angle2;
static GLfloat R = 10;

static GLint status = 0;       // stan klawiszy myszy
							   // 2 - wcisniety prawy
							   // 1 - wcisniety lewy
							   // 0 - nie wcisniety zaden
static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old = 0;

static int y2_pos_old = 0;
static int delta_y2 = 0;

static int y3_pos_old = 0;
static int x3_pos_old = 0;

static int r_old = 0;
static int delta_r = 0;
static int delta_y3 = 0;
static int delta_x3 = 0;
int interaction = 1;

int modelPart = 5;

static int delta_x = 0, delta_y = 0;        // r�nica pomi�dzy pozycj� bie��c�
									  // i poprzedni� kursora myszy

GLbyte* LoadTGAImage(const char* FileName, GLint* ImWidth, GLint* ImHeight, GLint* ImComponents, GLenum* ImFormat);

GLbyte* pBytes;
GLint ImWidth, ImHeight, ImComponents;
GLenum ImFormat;

const int N = 50;

// struktura dla wspolrzednych jajka
struct p {
	float x, y, z;
};

p matrix[N][N];

void keys(unsigned char key, int x, int y);

void generateMatrix()
{
	float u, v;
	for (float i = 0; i < N; i++)
	{
		u = i / (N - 1);
		for (float j = 0; j < N; j++)
		{
			v = j / (N - 1);
			matrix[(int)i][(int)j].x = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * cos(v * PI);
			matrix[(int)i][(int)j].y = 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2);
			matrix[(int)i][(int)j].z = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * sin(v * PI);
		}
	}

}

void EggTriangle(int n)
{
	float u, v;
	glRotated(0.0, 1.0, 0.0, 0.0);
	glTranslated(0.0f, -2.0f, 0.0f);
	for (int i = 0; i < n - 1; i++)
	{
		u = i / (N - 1);
		for (int j = 0; j < n - 1; j++)
		{
			v = j / (N - 1);
			glBegin(GL_TRIANGLES);

				glColor3f(1.0f, 1.0f, 1.0f);
				glVertex3f(matrix[i][j].x, matrix[i][j].y, matrix[i][j].z);
				glTexCoord2f(i,j);
				glVertex3f(matrix[i + 1][j].x, matrix[i + 1][j].y, matrix[i + 1][j].z);
				glVertex3f(matrix[i + 1][j + 1].x, matrix[i + 1][j + 1].y, matrix[i + 1][j + 1].z);


				glColor3f(0.0f, 0.0f, 1.0f);
				glVertex3f(matrix[i][j].x, matrix[i][j].y, matrix[i][j].z);
				glTexCoord2f(i, j);
				glVertex3f(matrix[i][j + 1].x, matrix[i][j + 1].y, matrix[i][j + 1].z);

				glVertex3f(matrix[i + 1][j + 1].x, matrix[i + 1][j + 1].y, matrix[i + 1][j + 1].z);

			glEnd();
		}

	}
}

void drawTriangle()
{
	glBegin(GL_TRIANGLES);

		// Ustawienie koloru na bia�y
		glColor3f(1.0f, 1.0f, 1.0f);

		//glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(-3.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);

		//glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(3.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.6f);

		//glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(0.0f, 7.0f, 0.0f);
		glTexCoord2f(0.5f, 1.0f);

	glEnd();
}

void firstTexture()
{

	// ..................................       
	//       Pozosta�a cz�� funkcji MyInit()

	// ..................................

/*************************************************************************************/

// Teksturowanie b�dzie prowadzone tyko po jednej stronie �ciany

	glEnable(GL_CULL_FACE);

	/*************************************************************************************/

	//  Przeczytanie obrazu tekstury z pliku o nazwie tekstura.tga

	pBytes = LoadTGAImage("F:/Grafika(OpenGL)/TeksturowanieObiektow/tekstury/P3_t.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);

	/*************************************************************************************/

   // Zdefiniowanie tekstury 2-D

	glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);

	/*************************************************************************************/

	// Zwolnienie pami�ci

	free(pBytes);

	/*************************************************************************************/

	// W��czenie mechanizmu teksturowania

	glEnable(GL_TEXTURE_2D);

	/*************************************************************************************/

	// Ustalenie trybu teksturowania

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	/*************************************************************************************/

	// Okre�lenie sposobu nak�adania tekstur

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void secondTexture()
{

	// ..................................       
	//       Pozosta�a cz�� funkcji MyInit()

	// ..................................

/*************************************************************************************/

// Teksturowanie b�dzie prowadzone tyko po jednej stronie �ciany

	glEnable(GL_CULL_FACE);

	/*************************************************************************************/

	//  Przeczytanie obrazu tekstury z pliku o nazwie tekstura.tga

	pBytes = LoadTGAImage("F:/Grafika(OpenGL)/TeksturowanieObiektow/tekstury/korwin.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);

	/*************************************************************************************/

   // Zdefiniowanie tekstury 2-D

	glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);

	/*************************************************************************************/

	// Zwolnienie pami�ci

	free(pBytes);

	/*************************************************************************************/

	// W��czenie mechanizmu teksturowania

	glEnable(GL_TEXTURE_2D);

	/*************************************************************************************/

	// Ustalenie trybu teksturowania

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	/*************************************************************************************/

	// Okre�lenie sposobu nak�adania tekstur

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void thirdTexture()
{
	// Teksturowanie b�dzie prowadzone tyko po jednej stronie �ciany
	glEnable(GL_CULL_FACE);

	//  Przeczytanie obrazu tekstury z pliku o nazwie tekstura.tga
	pBytes = LoadTGAImage("F:/Grafika(OpenGL)/TeksturowanieObiektow/tekstury/D7_t.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);

   // Zdefiniowanie tekstury 2-D
	glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);

	// Zwolnienie pami�ci
	free(pBytes);

	// W��czenie mechanizmu teksturowania
	glEnable(GL_TEXTURE_2D);

	// Ustalenie trybu teksturowania
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Okre�lenie sposobu nak�adania tekstur
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void drawPiramidPart5()
{
	glBegin(GL_TRIANGLES);

	// 1 trojkat
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-2.0f, 2.0f, 0.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(2.0f, 2.0f, 4.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-2.0f, 2.0f, 4.0f);

	// 2 trojk�t
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-2.0f, 2.0f, 0.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(2.0f, 2.0f, 0.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(2.0f, 2.0f, 4.0f);

	// druga sciana boczna
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-2.0f, 2.0f, 4.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(2.0f, 2.0f, 4.0f);

		glTexCoord2f(1.0f, 0.5f);
		glVertex3f(0.0f, 8.0f, 2.0f);

	glEnd();

	
	/*
	// pierwsza sciana boczna
	glBegin(GL_TRIANGLES);


		glVertex3f(-2.0f, 2.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);

		glVertex3f(2.0f, 2.0f, 0.0f);
		glTexCoord2f(0.8f, 1.0f);

		glVertex3f(0.0f, 8.0f, 2.0f);
		glTexCoord2f(0.5f, 1.0f);

	glEnd();



	// druga sciana boczna
	glBegin(GL_TRIANGLES);


	glVertex3f(-2.0f, 2.0f, 4.0f);
	glTexCoord2f(0.0f, 0.0f);

	glVertex3f(2.0f, 2.0f, 4.0f);
	glTexCoord2f(1.0f, 0.5f);

	glVertex3f(0.0f, 8.0f, 2.0f);
	glTexCoord2f(0.5f, 1.0f);

	glEnd();



	// trzecia sciana trojkatna
	glBegin(GL_TRIANGLES);

	glVertex3f(-2.0f, 2.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);

	glVertex3f(-2.0f, 2.0f, 4.0f);
	glTexCoord2f(1.0f, 0.5f);

	glVertex3f(0.0f, 8.0f, 2.0f);
	glTexCoord2f(0.5f, 1.0f);

	glEnd();


	// czwarta sciana trojkatna
	glBegin(GL_TRIANGLES);

		glVertex3f(2.0f, 2.0f, 4.0f);
		glTexCoord2f(0.0f, 0.0f);

		glVertex3f(2.0f, 2.0f, 0.0f);
		glTexCoord2f(1.0f, 0.5f);

		glVertex3f(0.0f, 8.0f, 2.0f);
		glTexCoord2f(0.5f, 1.0f);

	glEnd();
*/

}


void Mouse(int btn, int state, int x, int y)
{

	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;        // przypisanie aktualnie odczytanej pozycji kursora jako pozycji poprzedniej
		y_pos_old = y;
		y3_pos_old = y;
		x3_pos_old = x;
		status = 1;          // wci�ni�ty zosta� lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		y2_pos_old = y;        // przypisanie aktualnie odczytanej pozycji kursora jako pozycji poprzedniej
		r_old = y;
		status = 2;
	}
	else
		status = 0;          // nie zosta� wci�ni�ty �aden klawisz
}

/*************************************************************************************/
// Funkcja "monitoruje" po�o�enie kursora myszy i ustawia warto�ci odpowiednich
// zmiennych globalnych

void Motion(GLsizei x, GLsizei y)
{

	delta_x = x - x_pos_old;     // obliczenie r�nicy po�o�enia kursora myszy
	delta_y = y - y_pos_old;

	delta_y2 = y - y2_pos_old;
	delta_y3 = y - y3_pos_old;
	delta_x3 = x - x_pos_old;
	delta_r = y - r_old;

	y2_pos_old = y;
	y3_pos_old = y;
	r_old = y;
	x_pos_old = x;            // podstawienie bie��cego po�o�enia jako poprzednie
	y_pos_old = y;
	glutPostRedisplay();     // przerysowanie obrazu sceny
}

/*************************************************************************************/

void Axes(void)
{

	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz?tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0,  5.0, 0.0 };
	// pocz?tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0,  5.0 };
	//  pocz?tek i koniec obrazu osi y

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

/*************************************************************************************/

// Funkcja okre�laj�ca co ma by� rysowane (zawsze wywo�ywana, gdy trzeba
// przerysowa� scen�)

void RenderScene(void)
{

	if (interaction == 1) firstTexture();
	if (interaction == 2) secondTexture();
	if (interaction == 3) thirdTexture();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	glLoadIdentity();
	// Czyszczenie macierzy bie??cej

	gluLookAt(viewer[0], viewer[1], viewer[2], object[0], object[1], object[2], 0.0, 1.0, 0.0);
	// Zdefiniowanie po�o�enia obserwatora

	if (status == 1)                     // je�li lewy klawisz myszy wci�ni�ty
	{
		theta[0] += delta_x * pix2angle;    // modyfikacja k�ta obrotu o kat proporcjonalny
		theta[1] += delta_y * pix2angle;
	}                                  // do r�nicy po�o�e� kursora myszy

	if (status == 2) // jesli prawy wcisniety
	{
		zoom -= delta_y2 * pix2angle;
	}

	glTranslatef(0.0, -3.0, zoom);

	glRotatef(theta[0], 0.0, 1.0, 0.0);  //obr�t obiektu o nowy k�t
	glRotatef(theta[1], 1.0, 0.0, 0.0);

	
	glColor3f(1.0f, 1.0f, 1.0f);

	//glutSolidTeapot(3.0);
	// Narysowanie czajnika 

	//drawTriangle();
	if(modelPart == 5) drawPiramidPart5();
	if(modelPart == 0) EggTriangle(N);
	// Narysowanie tr�j�ta

	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania

	glutSwapBuffers();
}
/*************************************************************************************/

// Funkcja ustalaj�ca stan renderowania

void MyInit(void)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// Kolor czyszcz�cy (wype�nienia okna) ustawiono na cszary

	//  Definicja materia�u z jakiego zrobiony jest czajnik
	//  i definicja �r�d�a �wiat�a
	GLfloat mat_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	// wsp�czynniki ka =[kar,kag,kab] dla �wiat�a otoczenia

	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// wsp�czynniki kd =[kdr,kdg,kdb] �wiat�a rozproszonego

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// wsp�czynniki ks =[ksr,ksg,ksb] dla �wiat�a odbitego               

	GLfloat mat_shininess = { 128.0 };
	// wsp�czynnik n opisuj�cy po�ysk powierzchni

/*************************************************************************************/
// Definicja �r�d�a �wiat�a

	GLfloat light_position[] = { 0.0, 1.0, 5.0, 1.0 };
	GLfloat light1_position[] = { 15.0, 1.0, 0.0, 1.0 };
	GLfloat light2_position[] = { -2.0, 1.0, 0.0, 1.0 };
	// po�o�enie �r�d�a

	GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a otoczenia
	// Ia = [Iar,Iag,Iab]

	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
	// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
	// odbicie kierunkowe Is = [Isr,Isg,Isb]

	GLfloat att_constant = { 0.5 };
	// sk�adowa sta�a ds dla modelu zmian o�wietlenia w funkcji
	// odleg�o�ci od �r�d�a

	GLfloat att_linear = { 0.05 };
	// sk�adowa liniowa dl dla modelu zmian o�wietlenia w funkcji
	// odleg�o�ci od �r�d�a

	GLfloat att_quadratic = { 0.001 };
	// sk�adowa kwadratowa dq dla modelu zmian o�wietlenia w funkcji
	// odleg�o�ci od �r�d�a

/*************************************************************************************/
	// Ustawienie patrametr�w materia�u

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	/*************************************************************************************/
	// Ustawienie parametr�w �r�d�a1

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

	// Ustawienie parametr�w �r�d�a2

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_quadratic);

	// Ustawienie parametr�w �r�d�a3

	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);

	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, att_quadratic);


	/*************************************************************************************/
	// Ustawienie opcji systemu o�wietlania sceny

	glShadeModel(GL_SMOOTH); // w�aczenie �agodnego cieniowania
	glEnable(GL_LIGHTING);   // w�aczenie systemu o�wietlenia sceny
	glEnable(GL_LIGHT0);     // w��czenie �r�d�a o numerze 0
	glEnable(GL_LIGHT1);     // w��czenie �r�d�a o numerze 1
	glEnable(GL_LIGHT2);     // w��czenie �r�d�a o numerze 1
	glEnable(GL_DEPTH_TEST); // w��czenie mechanizmu z-bufora

// Teksturowanie b�dzie prowadzone tyko po jednej stronie �ciany

	glEnable(GL_CULL_FACE);

	/*************************************************************************************/

	//  Przeczytanie obrazu tekstury z pliku o nazwie tekstura.tga

	pBytes = LoadTGAImage("F:/Grafika(OpenGL)/TeksturowanieObiektow/tekstury/P3_t.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);

	/*************************************************************************************/

   // Zdefiniowanie tekstury 2-D

	glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);

	/*************************************************************************************/

	// Zwolnienie pami�ci

	free(pBytes);

	/*************************************************************************************/

	// W��czenie mechanizmu teksturowania

	glEnable(GL_TEXTURE_2D);

	/*************************************************************************************/

	// Ustalenie trybu teksturowania

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	/*************************************************************************************/

	// Okre�lenie sposobu nak�adania tekstur

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// ..................................
	//       Pozosta�a cz�� funkcji MyInit()

	// ..................................

}


/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie sta�ych proporcji rysowanych
// w przypadku zmiany rozmiar�w okna.
// Parametry vertical i horizontal (wysoko�� i szeroko�� okna) s� 
// przekazywane do funkcji za ka�dym razem gdy zmieni si� rozmiar okna.
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie
	//pix2angle2 = 360.0 / (float)vertical;
	glMatrixMode(GL_PROJECTION);
	// Prze��czenie macierzy bie��cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie��cej

	gluPerspective(70, 1.0, 1.0, 300.0);
	// Ustawienie parametr�w dla rzutu perspektywicznego


	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielko�ci okna okna widoku (viewport) w zale�no�ci
	// relacji pomi�dzy wysoko�ci� i szeroko�ci� okna

	glMatrixMode(GL_MODELVIEW);
	// Prze��czenie macierzy bie��cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie��cej

}

/*************************************************************************************/

// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli

int main(void)
{
	generateMatrix();
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(400, 400);
	glutInitWindowPosition(580, 200);

	glutCreateWindow("Rzutowanie perspektywiczne");

	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn�
	// (callback function).  B�dzie ona wywo�ywana za ka�dym razem
	// gdy zajdzie potrzeba przerysowania okna

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn�
	// za zmiany rozmiaru okna                       

	MyInit();
	// Funkcja MyInit() (zdefiniowana powy�ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst�pieniem do renderowania 

	glEnable(GL_DEPTH_TEST);
	// W��czenie mechanizmu usuwania niewidocznych element�w sceny

	glutMouseFunc(Mouse);
	// Ustala funkcj� zwrotn� odpowiedzialn� za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcj� zwrotn� odpowiedzialn� za badanie ruchu myszy

	glutKeyboardFunc(keys);
	// Funkcja obs�uguj�ca klawiatur� komputera
	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}

void keys(unsigned char key, int x, int y)
{
	if (key == 't') interaction = 1;
	if (key == 'k') interaction = 2;
	if (key == 'b') interaction = 3;

	if (key == '1') modelPart = 1;
	if (key == '2') modelPart = 2;
	if (key == '3') modelPart = 3;
	if (key == '4') modelPart = 4;
	if (key == '5') modelPart = 5;

	if (key == '0') modelPart = 0;

	RenderScene(); // przerysowanie obrazu sceny
}

/*************************************************************************************/
 // Funkcja wczytuje dane obrazu zapisanego w formacie TGA w pliku o nazwie
 // FileName, alokuje pami�� i zwraca wska�nik (pBits) do bufora w kt�rym
 // umieszczone s� dane.
 // Ponadto udost�pnia szeroko�� (ImWidth), wysoko�� (ImHeight) obrazu
 // tekstury oraz dane opisuj�ce format obrazu wed�ug specyfikacji OpenGL
 // (ImComponents) i (ImFormat).
 // Jest to bardzo uproszczona wersja funkcji wczytuj�cej dane z pliku TGA.
 // Dzia�a tylko dla obraz�w wykorzystuj�cych 8, 24, or 32 bitowy kolor.
 // Nie obs�uguje plik�w w formacie TGA kodowanych z kompresj� RLE.
/*************************************************************************************/


GLbyte* LoadTGAImage(const char* FileName, GLint* ImWidth, GLint* ImHeight, GLint* ImComponents, GLenum* ImFormat)
{

	/*************************************************************************************/

	// Struktura dla nag��wka pliku  TGA


#pragma pack(1)           
	typedef struct
	{
		GLbyte    idlength;
		GLbyte    colormaptype;
		GLbyte    datatypecode;
		unsigned short    colormapstart;
		unsigned short    colormaplength;
		unsigned char     colormapdepth;
		unsigned short    x_orgin;
		unsigned short    y_orgin;
		unsigned short    width;
		unsigned short    height;
		GLbyte    bitsperpixel;
		GLbyte    descriptor;
	}TGAHEADER;
#pragma pack(8)

	FILE* pFile;
	TGAHEADER tgaHeader;
	unsigned long lImageSize;
	short sDepth;
	GLbyte* pbitsperpixel = NULL;


	/*************************************************************************************/

	// Warto�ci domy�lne zwracane w przypadku b��du

	*ImWidth = 0;
	*ImHeight = 0;
	*ImFormat = GL_BGR_EXT;
	*ImComponents = GL_RGB8;

	pFile = fopen(FileName, "rb");
	if (pFile == NULL)
		return NULL;

	/*************************************************************************************/
	// Przeczytanie nag��wka pliku 


	fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);


	/*************************************************************************************/

	// Odczytanie szeroko�ci, wysoko�ci i g��bi obrazu

	*ImWidth = tgaHeader.width;
	*ImHeight = tgaHeader.height;
	sDepth = tgaHeader.bitsperpixel / 8;


	/*************************************************************************************/
	// Sprawdzenie, czy g��bia spe�nia za�o�one warunki (8, 24, lub 32 bity)

	if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32)
		return NULL;

	/*************************************************************************************/

	// Obliczenie rozmiaru bufora w pami�ci


	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;


	/*************************************************************************************/

	// Alokacja pami�ci dla danych obrazu


	pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));

	if (pbitsperpixel == NULL)
		return NULL;

	if (fread(pbitsperpixel, lImageSize, 1, pFile) != 1)
	{
		free(pbitsperpixel);
		return NULL;
	}


	/*************************************************************************************/

	// Ustawienie formatu OpenGL


	switch (sDepth)

	{

	case 3:

		*ImFormat = GL_BGR_EXT;

		*ImComponents = GL_RGB8;

		break;

	case 4:

		*ImFormat = GL_BGRA_EXT;

		*ImComponents = GL_RGBA8;

		break;

	case 1:

		*ImFormat = GL_LUMINANCE;

		*ImComponents = GL_LUMINANCE8;

		break;

	};



	fclose(pFile);
	return pbitsperpixel;

}

/*************************************************************************************/


/*************************************************************************************/