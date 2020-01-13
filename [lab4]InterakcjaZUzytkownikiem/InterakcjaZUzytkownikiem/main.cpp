#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>

#define PI 3.14159265359
typedef float point3[3];

static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
static GLfloat object[] = { 0.0, 0.0, 0.0 };
static GLfloat zoom = 0.0;
static GLfloat theta[] = { 0.0, 0.0 };   // k¹ty

static GLfloat pix2angle;     // przelicznik pikseli na stopnie
static GLfloat R = 10; // promieñ

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
static int delta_x = 0, delta_y = 0;        // ró¿nica pomiêdzy pozycj¹ bie¿¹c¹
									  // i poprzedni¹ kursora myszy

void Mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// przypisanie aktualnie odczytanej pozycji kursora jako pozycji poprzedniej
		x_pos_old = x;
		y_pos_old = y;
		y3_pos_old = y;
		x3_pos_old = x;
		status = 1;          // wciêniêty zosta³ lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		y2_pos_old = y;        // przypisanie aktualnie odczytanej pozycji kursora jako pozycji poprzedniej
		r_old = y;
		status = 2;
	}
	else 
		status = 0;          // nie zosta³ wciêniêty ¿aden klawisz
}

/*************************************************************************************/
// Funkcja "monitoruje" po³o¿enie kursora myszy i ustawia wartoœci odpowiednich
// zmiennych globalnych
void Motion(GLsizei x, GLsizei y)
{

	delta_x = x - x_pos_old;     // obliczenie ró¿nicy po³o¿enia kursora myszy
	delta_y = y - y_pos_old;

	delta_y3 = y - y3_pos_old;
	delta_x3 = x - x_pos_old;
	delta_r = y - r_old;

	y3_pos_old = y;
	r_old = y;
	x_pos_old = x;            // podstawienie bie¿¹cego po³o¿enia jako poprzednie
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

// Funkcja okreœlaj¹ca co ma byæ rysowane (zawsze wywo³ywana, gdy trzeba
// przerysowaæ scenê)
void RenderScene(void)
{
	if (interaction == 1) // obracanie obiektem
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Czyszczenie okna aktualnym kolorem czyszcz¹cym

		glLoadIdentity();
		// Czyszczenie macierzy bie??cej

		gluLookAt(viewer[0], viewer[1], viewer[2], object[0], object[1], object[2], 0.0, 1.0, 0.0);
		// Zdefiniowanie po³o¿enia obserwatora

		Axes();
		// Narysowanie osi przy pomocy funkcji zdefiniowanej powy¿ej 

		// lewy przycisk myszy wciœniêty
		if (status == 1)
		{
			theta[0] += delta_x * pix2angle;    // modyfikacja k¹ta obrotu o kat proporcjonalny
			theta[1] += delta_y * pix2angle;
		}                                  // do ró¿nicy po³o¿eñ kursora myszy

		// prawy przycisk myszy wciœniêty
		if (status == 2)
		{
			zoom -= delta_y2 * pix2angle;
		}

		glTranslatef(0.0, 0.0, zoom);
		glRotatef(theta[0], 0.0, 1.0, 0.0);  //obrót obiektu o nowy k¹t
		glRotatef(theta[1], 1.0, 0.0, 0.0);
	}

	if (interaction == 2) // poruszanie kamera
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Czyszczenie okna aktualnym kolorem czyszcz¹cym

		glLoadIdentity();

		// lewy przycisk myszy wciœniêty
		if (status == 1)
		{
			theta[0] -= (delta_x * pix2angle) * (PI / 180);

			theta[1] -= (delta_y * pix2angle) * (PI / 180);
			while (theta[1] > 360 * (PI / 180)) theta[1] -= 360 * (PI / 180);
			while (theta[1] < 0 * (PI / 180)) theta[1] += 360 * (PI / 180);
		}
		// prawy przycisk myszy wciœniêty
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
		// Czyszczenie okna aktualnym kolorem czyszcz¹cym

		glLoadIdentity();

		// Ustawienie koloru rysowania
		if (status == 1)                     // jeœli lewy klawisz myszy wciêniêty
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
	// Przekazanie poleceñ rysuj¹cych do wykonania

	glutSwapBuffers();
}

// Funkcja ustalaj¹ca stan renderowania
void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszcz¹cy (wype³nienia okna) ustawiono na czarny
}

// Funkcja ma za zadanie utrzymanie sta³ych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokoœæ i szerokoœæ okna) s¹ 
// przekazywane do funkcji za ka¿dym razem gdy zmieni siê rozmiar okna.
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie
	//pix2angle2 = 360.0 / (float)vertical;
	glMatrixMode(GL_PROJECTION);
	// Prze³¹czenie macierzy bie¿¹cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie¿¹cej

	gluPerspective(70, 1.0, 1.0, 300.0);
	// Ustawienie parametrów dla rzutu perspektywicznego

	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielkoœci okna okna widoku (viewport) w zale¿noœci
	// relacji pomiêdzy wysokoœci¹ i szerokoœci¹ okna

	glMatrixMode(GL_MODELVIEW);
	// Prze³¹czenie macierzy bie¿¹cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie¿¹cej

}

// Funkcja obs³uguj¹ca przyciski klawiatury
void keys(unsigned char key, int x, int y)
{
	if (key == '1') interaction = 1;
	if (key == '2') interaction = 2;
	if (key == '3') interaction = 3;
	RenderScene(); // przerysowanie obrazu sceny
}

// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli
void main(void)
{

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 300);

	glutCreateWindow("Rzutowanie perspektywiczne");

	glutDisplayFunc(RenderScene);
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹
	// (callback function).  Bêdzie ona wywo³ywana za ka¿dym razem
	// gdy zajdzie potrzeba przerysowania okna

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹
	// za zmiany rozmiaru okna                       

	MyInit();
	// Funkcja MyInit() (zdefiniowana powy¿ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst¹pieniem do renderowania 

	glEnable(GL_DEPTH_TEST);
	// W³¹czenie mechanizmu usuwania niewidocznych elementów sceny

	glutMouseFunc(Mouse);
	// Ustala funkcjê zwrotn¹ odpowiedzialn¹ za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcjê zwrotn¹ odpowiedzialn¹ za badanie ruchu myszy
	glutKeyboardFunc(keys);

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}