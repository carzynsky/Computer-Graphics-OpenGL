/*************************************************************************************/

//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacją osi 
//  układu współrzędnych dla rzutowania perspektywicznego

/*************************************************************************************/

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>
#define PI 3.14159265359
typedef float point3[3];

static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
static GLfloat object[] = { 0.0, 0.0, 0.0 };
static GLfloat zoom = 0.0;
static GLfloat theta[] = { 0.0, 0.0 };   // kąty

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
static int delta_x = 0, delta_y = 0;        // różnica pomiędzy pozycją bieżącą
                                      // i poprzednią kursora myszy
// inicjalizacja położenia obserwatora


/*************************************************************************************/

// Funkcja rysująca osie układu wspó?rz?dnych

/*************************************************************************************/
// Funkcja "bada" stan myszy i ustawia wartości odpowiednich zmiennych globalnych

void keys(unsigned char key, int x, int y);

void Mouse(int btn, int state, int x, int y)
{

	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;        // przypisanie aktualnie odczytanej pozycji kursora jako pozycji poprzedniej
		y_pos_old = y;
		y3_pos_old = y;
		x3_pos_old = x;
		status = 1;          // wcięnięty został lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		y2_pos_old = y;        // przypisanie aktualnie odczytanej pozycji kursora jako pozycji poprzedniej
		r_old = y;
		status = 2;
	}
	else

		status = 0;          // nie został wcięnięty żaden klawisz
}

/*************************************************************************************/
// Funkcja "monitoruje" położenie kursora myszy i ustawia wartości odpowiednich
// zmiennych globalnych

void Motion(GLsizei x, GLsizei y)
{

	delta_x = x - x_pos_old;     // obliczenie różnicy położenia kursora myszy
	delta_y = y - y_pos_old;

	delta_y2 = y - y2_pos_old;
	delta_y3 = y - y3_pos_old;
	delta_x3 = x - x_pos_old;
	delta_r = y - r_old;

	y2_pos_old = y;
	y3_pos_old = y;
	r_old = y;
	x_pos_old = x;            // podstawienie bieżącego położenia jako poprzednie
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

// Funkcja określająca co ma być rysowane (zawsze wywoływana, gdy trzeba
// przerysować scenę)



void RenderScene(void)
{
	
	if (interaction == 1) // obracanie obiektem
	{

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Czyszczenie okna aktualnym kolorem czyszczącym

		glLoadIdentity();
		// Czyszczenie macierzy bie??cej

		gluLookAt(viewer[0], viewer[1], viewer[2], object[0], object[1], object[2], 0.0, 1.0, 0.0);
		// Zdefiniowanie położenia obserwatora

		Axes();
		// Narysowanie osi przy pomocy funkcji zdefiniowanej powyżej 


		if (status == 1)                     // jeśli lewy klawisz myszy wcięnięty
		{
			theta[0] += delta_x * pix2angle;    // modyfikacja kąta obrotu o kat proporcjonalny
			theta[1] += delta_y * pix2angle;
		}                                  // do różnicy położeń kursora myszy

		if (status == 2) // jesli prawy wcisniety
		{
			zoom -= delta_y2 * pix2angle;
		}

		glTranslatef(0.0, 0.0, zoom);

		glRotatef(theta[0], 0.0, 1.0, 0.0);  //obrót obiektu o nowy kąt
		glRotatef(theta[1], 1.0, 0.0, 0.0);
	}

	if (interaction == 2) // poruszanie kamera
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Czyszczenie okna aktualnym kolorem czyszczącym

		glLoadIdentity();

		// Ustawienie koloru rysowania
		
		if (status == 1)                     // jeśli lewy klawisz myszy wcięnięty
		{
			theta[0] -= (delta_x3*pix2angle)*(PI/180);
			
			theta[1] -= (delta_y3*pix2angle)*(PI/180);
			while(theta[1] > 360 * (PI / 180)) theta[1] -= 360*(PI/180);
			while (theta[1] < 0*(PI/180)) theta[1]+= 360*(PI/180);

		}                                  // do różnicy położeń kursora myszy

		if (status == 2) // jesli prawy wcisniety
		{
			R -= delta_r;
			if (R >= 1 && R <= 20) R;
			else if (R < 1) R = 1;
			else R = 20;
		}

		viewer[0] = R * cos(theta[0]) * cos(theta[1]);
		viewer[1] = R * sin(theta[1]);
		viewer[2] = R * sin(theta[0]) * cos(theta[1]);

		if(theta[1] >= (90*(PI/180)) && theta[1] <=(270 * (PI / 180)))
		gluLookAt(viewer[0], viewer[1], viewer[2], object[0], object[1], object[2], 0.0, -1.0, 0.0);

		else if(theta[1] < (90 * (PI / 180)) || theta[1] > (270 * (PI / 180)))
		gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		Axes();
	}
	if (interaction == 3) // poruszanie kamery i przesuwanie obiektu
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Czyszczenie okna aktualnym kolorem czyszczącym

		glLoadIdentity();

		// Ustawienie koloru rysowania

		if (status == 1)                     // jeśli lewy klawisz myszy wcięnięty
		{
			object[0] += delta_x;
			object[1] += delta_y;			
		}                                

		gluLookAt(viewer[0], viewer[1], viewer[2], object[0], object[1], object[2], 0.0, 1.0, 0.0);

		Axes();
	}
	glColor3f(0.9f, 0.2f, 0.9f);
	glutWireTeapot(3.0);
	// Narysowanie czajnika 

	glFlush();
	// Przekazanie poleceń rysujących do wykonania

	glutSwapBuffers();



}
/*************************************************************************************/

// Funkcja ustalająca stan renderowania



void MyInit(void)
{

	glClearColor(0.0f, 0.0f, 0.0f, 0.1f);
	// Kolor czyszczący (wypełnienia okna) ustawiono na czarny

}

/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie stałych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokość i szerokość okna) są 
// przekazywane do funkcji za każdym razem gdy zmieni się rozmiar okna.



void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie
	//pix2angle2 = 360.0 / (float)vertical;
	glMatrixMode(GL_PROJECTION);
	// Przełączenie macierzy bieżącej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bieżącej

	gluPerspective(70, 1.0, 1.0, 300.0);
	// Ustawienie parametrów dla rzutu perspektywicznego


	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielkości okna okna widoku (viewport) w zależności
	// relacji pomiędzy wysokością i szerokością okna

	glMatrixMode(GL_MODELVIEW);
	// Przełączenie macierzy bieżącej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bieżącej

}

/*************************************************************************************/

// Główny punkt wejścia programu. Program działa w trybie konsoli



void main(void)
{

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 300);

	glutCreateWindow("Rzutowanie perspektywiczne");

	glutDisplayFunc(RenderScene);
	// Określenie, że funkcja RenderScene będzie funkcją zwrotną
	// (callback function).  Będzie ona wywoływana za każdym razem
	// gdy zajdzie potrzeba przerysowania okna



	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną
	// za zmiany rozmiaru okna                       



	MyInit();
	// Funkcja MyInit() (zdefiniowana powyżej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przystąpieniem do renderowania 

	glEnable(GL_DEPTH_TEST);
	// Włączenie mechanizmu usuwania niewidocznych elementów sceny

	glutMouseFunc(Mouse);
	// Ustala funkcję zwrotną odpowiedzialną za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcję zwrotną odpowiedzialną za badanie ruchu myszy
	glutKeyboardFunc(keys);

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT


}

void keys(unsigned char key, int x, int y)
{
	if (key == '1') interaction = 1;
	if (key == '2') interaction = 2;
	if (key == '3') interaction = 3;
	RenderScene(); // przerysowanie obrazu sceny
}


/*************************************************************************************/