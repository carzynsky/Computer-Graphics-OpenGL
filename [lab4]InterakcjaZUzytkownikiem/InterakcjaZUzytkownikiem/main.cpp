#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>

#define PI 3.14159265359
typedef float point3[3];

static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
static GLfloat object[] = { 0.0, 0.0, 0.0 };
static GLfloat zoom = 0.0;
static GLfloat theta[] = { 0.0, 0.0 };   // k�ty

static GLfloat pix2angle;     // przelicznik pikseli na stopnie
static GLfloat R = 10; // promie�

static GLint status = 0;       // stan klawiszy myszy
							   // 2 - wcisniety prawy
							   // 1 - wcisniety lewy
							   // 0 - nie wcisniety zaden

// poprzednia pozycja x kursora myszy
static int x_pos_old = 0;
// poprzednia pozycja y kursora myszy
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
static int delta_x = 0, delta_y = 0;        // r�nica pomi�dzy pozycj� bie��c�
									  // i poprzedni� kursora myszy

void Mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// przypisanie aktualnie odczytanej pozycji kursora jako pozycji poprzedniej
		x_pos_old = x;
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

	delta_y3 = y - y3_pos_old;
	delta_x3 = x - x_pos_old;
	delta_r = y - r_old;

	y3_pos_old = y;
	r_old = y;
	x_pos_old = x;            // podstawienie bie��cego po�o�enia jako poprzednie
	y_pos_old = y;
	glutPostRedisplay();     // przerysowanie obrazu sceny
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
	if (interaction == 1) // obracanie obiektem
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Czyszczenie okna aktualnym kolorem czyszcz�cym

		glLoadIdentity();
		// Czyszczenie macierzy bie??cej

		gluLookAt(viewer[0], viewer[1], viewer[2], object[0], object[1], object[2], 0.0, 1.0, 0.0);
		// Zdefiniowanie po�o�enia obserwatora

		Axes();
		// Narysowanie osi przy pomocy funkcji zdefiniowanej powy�ej 

		// lewy przycisk myszy wci�ni�ty
		if (status == 1)
		{
			theta[0] += delta_x * pix2angle;    // modyfikacja k�ta obrotu o kat proporcjonalny
			theta[1] += delta_y * pix2angle;
		}                                  // do r�nicy po�o�e� kursora myszy

		// prawy przycisk myszy wci�ni�ty
		if (status == 2)
		{
			zoom -= delta_y2 * pix2angle;
		}

		glTranslatef(0.0, 0.0, zoom);
		glRotatef(theta[0], 0.0, 1.0, 0.0);  //obr�t obiektu o nowy k�t
		glRotatef(theta[1], 1.0, 0.0, 0.0);
	}

	if (interaction == 2) // poruszanie kamera
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Czyszczenie okna aktualnym kolorem czyszcz�cym

		glLoadIdentity();

		// lewy przycisk myszy wci�ni�ty
		if (status == 1)
		{
			theta[0] -= (delta_x * pix2angle) * (PI / 180);

			theta[1] -= (delta_y * pix2angle) * (PI / 180);
			while (theta[1] > 360 * (PI / 180)) theta[1] -= 360 * (PI / 180);
			while (theta[1] < 0 * (PI / 180)) theta[1] += 360 * (PI / 180);
		}
		// prawy przycisk myszy wci�ni�ty
		if (status == 2)
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
			gluLookAt(viewer[0], viewer[1], viewer[2], object[0], object[1], object[2], 0.0, 1.0, 0.0);
		Axes();
	}
	if (interaction == 3) // poruszanie kamery i przesuwanie obiektu
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Czyszczenie okna aktualnym kolorem czyszcz�cym

		glLoadIdentity();

		// Ustawienie koloru rysowania
		if (status == 1)                     // je�li lewy klawisz myszy wci�ni�ty
		{
			object[0] += delta_x;
			object[1] += delta_y;
		}

		gluLookAt(viewer[0], viewer[1], viewer[2], object[0], object[1], object[2], 0.0, 1.0, 0.0);
		Axes();
	}
	glColor3f(0.96f, 0.38f, 0.26f);
	glutWireTeapot(3.0);
	// Narysowanie czajnika 

	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania

	glutSwapBuffers();
}

// Funkcja ustalaj�ca stan renderowania
void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszcz�cy (wype�nienia okna) ustawiono na czarny
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

// Funkcja obs�uguj�ca przyciski klawiatury
void keys(unsigned char key, int x, int y)
{
	if (key == '1') interaction = 1;
	if (key == '2') interaction = 2;
	if (key == '3') interaction = 3;
	RenderScene(); // przerysowanie obrazu sceny
}

// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli
void main(void)
{

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