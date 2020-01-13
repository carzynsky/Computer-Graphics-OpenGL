#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>
#include <iostream>

#define PI 3.14159265359
typedef float point3[3];

static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
static GLfloat object[] = { 0.0, 0.0, 0.0 };
static GLfloat zoom = 0.0;
static GLfloat theta[] = { 0.0, 0.0, 0.0, 0.0};   // k�ty

static GLfloat pix2angle;     // przelicznik pikseli na stopnie
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

static int y4_pos_old = 0;
static int x4_pos_old = 0;

static int y5_pos_old = 0;
static int x5_pos_old = 0;

static int r_old = 0;
static int delta_r = 0;
static int delta_y3 = 0;
static int delta_x3 = 0;

static int delta_x4 = 0;
static int delta_y4 = 0;

static int delta_x5 = 0;
static int delta_y5 = 0;

int interaction = 1; // wybrana interakcja
float changeRed = 0.5f;
float changeGreen = 0.0f;
float changeBlue = 0.0f;

float changeRed2 = 0.0f;
float changeGreen2 = 0.0f;
float changeBlue2 = 0.5f;


static int lightXOld = 0;
static int lightYOld = 0;
static int deltaLightX = 0;
static int deltaLightY = 0;


static float xFirst = 0.0;
static float yFirst = 10.0;
static int delta_x = 0, delta_y = 0;        // r�nica pomi�dzy pozycj� bie��c�
									  // i poprzedni� kursora myszy

const int N = 50;


GLfloat lightPosition[] = { 0.0, 0.0, 10.0, 1.0 };
GLfloat lightPosition2[] = { 10.0, 0.0, 0.0, 1.0 };

GLfloat lightAmbient[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat lightAmbient2[] = { 0.1, 0.1, 0.1, 1.0 };
// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a otoczenia
// Ia = [Iar,Iag,Iab]

GLfloat lightDiffuse[] = { 1.0f, 0.1f, 0.0, 1.0 };
GLfloat lightDiffuse2[] = { 0.0f, 0.1f, 1.0, 1.0 };
// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

GLfloat lightSpecular[] = { 1.2, 1.5, 1.0, 1.0 };
GLfloat lightSpecular2[] = { 1.2, 1.5, 1.0, 1.0 };
// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
// odbicie kierunkowe Is = [Isr,Isg,Isb]

GLfloat attConstant = { 1.0 };
GLfloat attConstant2 = { 1.0 };
// sk�adowa sta�a ds dla modelu zmian o�wietlenia w funkcji
// odleg�o�ci od �r�d�a

GLfloat attLinear = { 0.3 };
GLfloat attLinear2= { 0.3 };
// sk�adowa liniowa dl dla modelu zmian o�wietlenia w funkcji
// odleg�o�ci od �r�d�a

GLfloat att_quadratic = { 0.001 };
GLfloat attQuadratic2 = { 0.001 };
// sk�adowa kwadratowa dq dla modelu zmian o�wietlenia w funkcji
// odleg�o�ci od �r�d�a

void EggTriangle(int n);

struct p {
	float x, y, z;
};

struct p2 {
	float xu, xv, yu, yv, zu, zv;
};
p matrix[N][N];
p2 matrix2[N][N];

point3 normal;

float randColor()
{
	return static_cast<float> (rand()) / static_cast <float> (RAND_MAX); // 0.0 ; 1.0
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
			matrix[(int)i][(int)j].x = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * cos(v * PI);
			matrix[(int)i][(int)j].y = 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2);
			matrix[(int)i][(int)j].z = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * sin(v * PI);
		}
	}
}

void Mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;        // przypisanie aktualnie odczytanej pozycji kursora jako pozycji poprzedniej
		y_pos_old = y;
		y3_pos_old = y;
		x3_pos_old = x;

		x4_pos_old = x;
		y4_pos_old = y;

		x5_pos_old = x;
		y5_pos_old = y;

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

// Funkcja "monitoruje" po�o�enie kursora myszy i ustawia warto�ci odpowiednich
// zmiennych globalnych
void Motion(GLsizei x, GLsizei y)
{

	delta_x = x - x_pos_old;     // obliczenie r�nicy po�o�enia kursora myszy
	delta_y = y - y_pos_old;

	deltaLightX = x - lightXOld;
	deltaLightY = y - lightYOld;

	delta_y2 = y - y2_pos_old;
	delta_y3 = y - y3_pos_old;
	delta_x3 = x - x_pos_old;
	delta_r = y - r_old;

	delta_x4 = x - x4_pos_old;
	delta_y4 = y - y4_pos_old;

	delta_x5 = x - x5_pos_old;
	delta_y5 = y - y5_pos_old;

	x4_pos_old = x;
	y4_pos_old = y;

	x5_pos_old = x;
	y5_pos_old = y;
	y2_pos_old = y;
	y3_pos_old = y;
	r_old = y;
	x_pos_old = x;            // podstawienie bie��cego po�o�enia jako poprzednie
	y_pos_old = y;
	glutPostRedisplay();     // przerysowanie obrazu sceny
}

void EggTriangle(int n)
{
	glRotated(-20.0, 1.0, 0.0, 0.0);
	glTranslated(0.0f, -5.0f, 0.0f);
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - 1; j++)
		{
			glBegin(GL_TRIANGLES);
				glColor3f(1.0f, 1.0f, 1.0f);
				glVertex3f(matrix[i][j].x, matrix[i][j].y, matrix[i][j].z);
				glVertex3f(matrix[i + 1][j].x, matrix[i + 1][j].y, matrix[i + 1][j].z);
				glVertex3f(matrix[i + 1][j + 1].x, matrix[i + 1][j + 1].y, matrix[i + 1][j + 1].z);
			
				glColor3f(0.0f, 1.0f, 0.0f);
				glVertex3f(matrix[i][j].x, matrix[i][j].y, matrix[i][j].z);
				glVertex3f(matrix[i][j + 1].x, matrix[i][j + 1].y, matrix[i][j + 1].z);
				glVertex3f(matrix[i + 1][j + 1].x, matrix[i + 1][j + 1].y, matrix[i + 1][j + 1].z);
			glEnd();
		}
	}
}

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

// Funkcja okre�laj�ca co ma by� rysowane (zawsze wywo�ywana, gdy trzeba
// przerysowa� scen�)
void RenderScene(void)
{
	if (interaction == 1) // rotacja obiektem
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Czyszczenie okna aktualnym kolorem czyszcz�cym

		glLoadIdentity();
		// Czyszczenie macierzy bie��cej

		gluLookAt(viewer[0], viewer[1], viewer[2], object[0], object[1], object[2], 0.0, 1.0, 0.0);
		// Zdefiniowanie po�o�enia obserwatora

		Axes();
		// Narysowanie osi przy pomocy funkcji zdefiniowanej powy�ej 

		// je�li lewy przycisk myszy wci�ni�ty
		if (status == 1)
		{
			theta[0] += delta_x * pix2angle;
			theta[1] += delta_y * pix2angle;
		}

		// jesli prawy przycisk myszy wcisniety
		if (status == 2)
		{
			zoom -= delta_y2 * pix2angle;
		}

		glTranslatef(0.0, 0.0, zoom);

		//obr�t obiektu o nowy k�t
		glRotatef(theta[0], 0.0, 1.0, 0.0);
		glRotatef(theta[1], 1.0, 0.0, 0.0);
	}

	if (interaction == 2) // modyfikacja po�o�enia obserwatora
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Czyszczenie okna aktualnym kolorem czyszcz�cym

		glLoadIdentity();

		// je�li lewy przycisk myszy wci�ni�ty
		if (status == 1)
		{
			theta[0] -= (delta_x3 * pix2angle) * (PI / 180);

			theta[1] -= (delta_y3 * pix2angle) * (PI / 180);
			while (theta[1] > 360 * (PI / 180)) theta[1] -= 360 * (PI / 180);
			while (theta[1] < 0 * (PI / 180)) theta[1] += 360 * (PI / 180);

		}

		if (status == 2) // jesli prawy przycisk myszy wcisni�ty
		{
			R -= delta_r;
			if (R >= 1 && R <= 20) R;
			else if (R < 1) R = 1;
			else R = 20;
		}

		viewer[0] = R * cos(theta[0]) * cos(theta[1]);
		viewer[1] = R * sin(theta[1]);
		viewer[2] = R * sin(theta[0]) * cos(theta[1]);

		if (theta[1] >= (90 * (PI / 180)) && theta[1] <= (270 * (PI / 180)))
			gluLookAt(viewer[0], viewer[1], viewer[2], object[0], object[1], object[2], 0.0, -1.0, 0.0);

		else if (theta[1] < (90 * (PI / 180)) || theta[1] > (270 * (PI / 180)))
			gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		Axes();
	}
	if (interaction == 3) // zmiana po�o�enia obiektu, na kt�ry patrzy obserwator
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Czyszczenie okna aktualnym kolorem czyszcz�cym

		glLoadIdentity();

		// je�li lewy przycisk myszy wci�ni�ty
		if (status == 1)
		{
			object[0] += delta_x;
			object[1] += delta_y;
		}

		gluLookAt(viewer[0], viewer[1], viewer[2], object[0], object[1], object[2], 0.0, 1.0, 0.0);
		Axes();
	}


	// Wizualizacja �r�de� �wiat�a
	if (interaction == 4)
	{
		// Pierwsze �r�d�o �wiat�a
		lightDiffuse[0] = changeRed;
		lightDiffuse[1] = changeGreen;
		lightDiffuse[2] = changeBlue;

		lightDiffuse2[0] = changeRed2;
		lightDiffuse2[1] = changeGreen2;
		lightDiffuse2[2] = changeBlue2;

		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);


		// Drugie �r�d�o �wiat��
		lightDiffuse2[0] = changeRed2;
		lightDiffuse2[1] = changeGreen2;
		lightDiffuse2[2] = changeBlue2;

		glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse2);
		glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient2);
		glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular2);
		glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Czyszczenie okna aktualnym kolorem czyszcz�cym

		glLoadIdentity();
		gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

		if (status == 1)               // je�li lewy klawisz myszy wci�ni�ty
		{
			theta[0] -= (delta_x4 * pix2angle) * (PI / 180);

			theta[1] -= (delta_y4 * pix2angle) * (PI / 180);
			while (theta[1] > 360 * (PI / 180)) theta[1] -= 360 * (PI / 180);
			while (theta[1] < 0 * (PI / 180)) theta[1] += 360 * (PI / 180);

			lightPosition[0] = R * cos(theta[0]) * cos(theta[1]);
			lightPosition[1] = R * sin(theta[1]);
			lightPosition[2] = R * sin(theta[0]) * cos(theta[1]);
		}
		else if (status == 2)
		{
			theta[2] -= (delta_x5 * pix2angle) * (PI / 180);

			theta[3] -= (delta_y5 * pix2angle) * (PI / 180);
			while (theta[3] > 360 * (PI / 180)) theta[3] -= 360 * (PI / 180);
			while (theta[3] < 0 * (PI / 180)) theta[3] += 360 * (PI / 180);

			lightPosition2[0] = R * cos(theta[3]) * cos(theta[2]);
			lightPosition2[1] = R * sin(theta[3]);
			lightPosition2[2] = R * sin(theta[3]) * cos(theta[3]);
		}

		glTranslated(lightPosition[0], lightPosition[1], lightPosition[2]);
		glutSolidSphere(1, 200, 2);
		glTranslated(-lightPosition[0], -lightPosition[1], -lightPosition[2]);

		glTranslated(lightPosition2[0], lightPosition2[1], lightPosition2[2]);
		glutWireTorus(1, 1, 1, 100);
		glTranslated(-lightPosition2[0], -lightPosition2[1], -lightPosition2[2]);
	}

	glColor3f(1.0f, 1.0f, 1.0f);
	//glutSolidTeapot(3.0);
	// Narysowanie czajnika 
	EggTriangle(N);

	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania

	glutSwapBuffers();
}

// Funkcja ustalaj�ca stan renderowania
void MyInit(void)
{

	glClearColor(0.0f, 0.0f, 0.0f, 0.1f);
	// Kolor czyszcz�cy (wype�nienia okna) ustawiono na czarny


	//  Definicja materia�u z jakiego zrobiony jest czajnik
	//  i definicja �r�d�a �wiat�a
	// Definicja materia�u z jakiego zrobiony jest czajnik

	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	// wsp�czynniki ka =[kar,kag,kab] dla �wiat�a otoczenia

	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// wsp�czynniki kd =[kdr,kdg,kdb] �wiat�a rozproszonego

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// wsp�czynniki ks =[ksr,ksg,ksb] dla �wiat�a odbitego               

	GLfloat mat_shininess = { 128.0 };
	// wsp�czynnik n opisuj�cy po�ysk powierzchni

/*************************************************************************************/
// Definicja �r�d�a �wiat�a

	GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 }; // pierwsze zrodlo
	//GLfloat light_position2[] = { 0.0, 10.0, 0.0, 1.0 }; // drugie zrodlo
	// po�o�enie �r�d�a


	GLfloat light_ambient[] = { 0.1, 0.1, 5.0, 1.0 };
	//GLfloat light_ambient2[] = { 1.0, 0.1, 0.1, 1.0 };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a otoczenia
	// Ia = [Iar,Iag,Iab]

	GLfloat light_diffuse[] = { 1.0, 0.1, 2.0, 1.0 };
	//GLfloat light_diffuse2[] = { 1.0, 2.0, 1.0, 1.0 };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
	// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

	GLfloat light_specular[] = { 1.2, 1.5, 1.0, 1.0 };
	//GLfloat light_specular2[] = { 1.0, 1.1, 1.0, 1.0 };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
	// odbicie kierunkowe Is = [Isr,Isg,Isb]

	GLfloat att_constant = { 1.0 };
	//GLfloat att_constant2 = { 1.0 };
	// sk�adowa sta�a ds dla modelu zmian o�wietlenia w funkcji
	// odleg�o�ci od �r�d�a

	GLfloat att_linear = { 0.3 };
	//GLfloat att_linear2 = { 0.3 };
	// sk�adowa liniowa dl dla modelu zmian o�wietlenia w funkcji
	// odleg�o�ci od �r�d�a

	GLfloat att_quadratic = { 0.001 };
	//GLfloat att_quadratic2 = { 0.001 };
	// sk�adowa kwadratowa dq dla modelu zmian o�wietlenia w funkcji
	// odleg�o�ci od �r�d�a

	// Ustawienie parametr�w materia�u i �r�d�a �wiat�a
	// Ustawienie patrametr�w materia�u


	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	/*************************************************************************************/
	// Ustawienie parametr�w �r�d�a 1 

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

	//Ustawienie parametr�w �r�d�a 2
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular2);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, attConstant2);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, attLinear2);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, attQuadratic2);

	// Ustawienie opcji systemu o�wietlania sceny
	glShadeModel(GL_SMOOTH); // w�aczenie �agodnego cieniowania
	glEnable(GL_LIGHTING);   // w�aczenie systemu o�wietlenia sceny
	glEnable(GL_LIGHT0);     // w��czenie �r�d�a o numerze 0
	glEnable(GL_LIGHT1);	// w��czenie �r�d�a o numerze 1
	glEnable(GL_DEPTH_TEST); // w��czenie mechanizmu z-bufora
}


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

	gluPerspective(70, (float)horizontal / vertical, 1.0, 30.0);
	// Ustawienie parametr�w dla rzutu perspektywicznego


	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielko�ci okna okna widoku (viewport) w zale�no�ci
	// relacji pomi�dzy wysoko�ci� i szeroko�ci� okna

	glMatrixMode(GL_MODELVIEW);
	// Prze��czenie macierzy bie��cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie��cej
}

// Obs�uga klawiatury
void keys(unsigned char key, int x, int y)
{
	if (key == '1') interaction = 1;
	if (key == '2') interaction = 2;
	if (key == '3') interaction = 3;
	if (key == '4') interaction = 4;

	// Pierwsze �r�d�o �wiat�a
	if (key == 'z') changeRed += 0.1;
	if (key == 'x')
	{
		if (changeRed >= 0.1)
			changeRed -= 0.1;
	}

	if (key == 'c') changeGreen += 0.1;
	if (key == 'v')
	{
		if (changeGreen >= 0.1)
			changeGreen -= 0.1;
	}

	if (key == 'b') changeBlue += 0.1;
	if (key == 'n')
	{
		if (changeBlue >= 0.1)
			changeBlue -= 0.1;
	}

	// Drugie �r�d�o �wiat�a
	if (key == 'a') changeRed2 += 0.1;
	if (key == 's')
	{
		if (changeRed2 >= 0.1)
			changeRed2 -= 0.1;
	}

	if (key == 'd') changeGreen2 += 0.1;
	if (key == 'f')
	{
		if (changeGreen2 >= 0.1)
			changeGreen2 -= 0.1;
	}

	if (key == 'g') changeBlue2 += 0.1;
	if (key == 'h')
	{
		if (changeBlue2 >= 0.1)
			changeBlue2 -= 0.1;
	}

	RenderScene(); // przerysowanie obrazu sceny
}


// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli
void main(void)
{
	generateMatrix();
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 300);

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

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}