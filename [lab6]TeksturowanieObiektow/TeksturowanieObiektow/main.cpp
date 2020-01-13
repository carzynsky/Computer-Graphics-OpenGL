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
static GLfloat theta[] = { 0.0, 0.0 };   // k¹ty

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

int interaction = 1;
int model = 1;

static int delta_x = 0, delta_y = 0;        // ró¿nica pomiêdzy pozycj¹ bie¿¹c¹
									  // i poprzedni¹ kursora myszy

GLbyte* LoadTGAImage(const char* FileName, GLint* ImWidth, GLint* ImHeight, GLint* ImComponents, GLenum* ImFormat);
GLbyte* pBytes;
GLint ImWidth, ImHeight, ImComponents;
GLenum ImFormat;

void drawCube()
{
	glBegin(GL_TRIANGLES);
	// Pierwsza œciana DONE 
	{
		// Pierwszy trójkat
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-2.0f, 2.0f, 0.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(2.0f, 2.0f, 4.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-2.0f, 2.0f, 4.0f);

		// Drugi trójk¹t
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-2.0f, 2.0f, 0.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(2.0f, 2.0f, 0.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(2.0f, 2.0f, 4.0f);
	}

	// Druga œciana
	{
		// Pierwszy trójkat
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-2.0f, 2.0f, 0.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-2.0f, 6.0f, 0.0f);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(2.0f, 2.0f, 0.0f);
		
		// Drugi trójk¹t
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(2.0f, 2.0f, 0.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-2.0f, 6.0f, 0.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(2.0f, 6.0f, 0.0f);
	}

	// Trzecia œciana
	{
		// Pierwszy trójkat
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-2.0f, 2.0f, 4.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-2.0f, 6.0f, 4.0f);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-2.0f, 2.0f, 0.0f);

		// Drugi trójk¹t
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-2.0f, 2.0f, 0.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-2.0f, 6.0f, 4.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-2.0f, 6.0f, 0.0f);
	}

	// Czwarta œciana
	{
		// Pierwszy trójkat
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-2.0f, 2.0f, 4.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(2.0f, 2.0f, 4.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(2.0f, 6.0f, 4.0f);

		// Drugi trójk¹t
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(2.0f, 6.0f, 4.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-2.0f, 6.0f, 4.0f);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-2.0f, 2.0f, 4.0f);
	}

	// Pi¹ta œciana
	{
		// Pierwszy trójkat
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(2.0f, 2.0f, 0.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(2.0f, 6.0f, 0.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(2.0f, 6.0f, 4.0f);

		// Drugi trójk¹t
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(2.0f, 2.0f, 0.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(2.0f, 6.0f, 4.0f);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(2.0f, 2.0f, 4.0f);
	}
	// Szósta œciana
	{
		// Pierwszy trójkat
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-2.0f, 6.0f, 4.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(2.0f, 6.0f, 4.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(2.0f, 6.0f, 0.0f);

		// Drugi trójk¹t
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-2.0f, 6.0f, 4.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(2.0f, 6.0f, 0.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-2.0f, 6.0f, 0.0f);
	}
	glEnd();
}

void changeTexture()
{
	// Teksturowanie bêdzie prowadzone tyko po jednej stronie œciany
	glEnable(GL_CULL_FACE);

	// Zdefiniowanie tekstury 2-D
	glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);

	// Zwolnienie pamiêci
	free(pBytes);

	// W³¹czenie mechanizmu teksturowania
	glEnable(GL_TEXTURE_2D);

	// Ustalenie trybu teksturowania
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Okreœlenie sposobu nak³adania tekstur
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void drawPiramid()
{
	glBegin(GL_TRIANGLES);
	// Podstawa
	{
		// Pierwszt trójkat podstawy
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-2.0f, 2.0f, 0.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(2.0f, 2.0f, 4.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-2.0f, 2.0f, 4.0f);

		// Drugi trójk¹t podstawy
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-2.0f, 2.0f, 0.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(2.0f, 2.0f, 0.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(2.0f, 2.0f, 4.0f);
	}

	// Pierwsza œciana boczna
	{
		glTexCoord2f(1.0f, 0.5f);
		glVertex3f(0.0f, 7.0f, 2.0f);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(2.0f, 2.0f, 0.0f);

		glTexCoord2f(0.5f, 1.0f);
		glVertex3f(-2.0f, 2.0f, 0.0f);
	}

	// Druga sciana boczna
	{
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-2.0f, 2.0f, 4.0f);

		glTexCoord2f(0.5f, 1.0f);
		glVertex3f(2.0f, 2.0f, 4.0f);

		glTexCoord2f(1.0, 0.5f);
		glVertex3f(0.0f, 7.0f, 2.0f);
	}

	// Trzecia sciana trojkatna
	{
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-2.0f, 2.0f, 0.0f);

		glTexCoord2f(0.5f, 1.0f);
		glVertex3f(-2.0f, 2.0f, 4.0f);

		glTexCoord2f(1.0f, 0.5f);
		glVertex3f(0.0f, 7.0f, 2.0f);
	}

	// Czwarta sciana trojkatna
	{
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(2.0f, 2.0f, 4.0f);

		glTexCoord2f(0.5f, 1.0f);
		glVertex3f(2.0f, 2.0f, 0.0f);

		glTexCoord2f(1.0f, 0.5f);
		glVertex3f(0.0f, 7.0f, 2.0f);

	}
	glEnd();
}

void Mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;        // przypisanie aktualnie odczytanej pozycji kursora jako pozycji poprzedniej
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

	delta_y2 = y - y2_pos_old;
	delta_r = y - r_old;

	y2_pos_old = y;
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

/*************************************************************************************/

// Funkcja okreœlaj¹ca co ma byæ rysowane (zawsze wywo³ywana, gdy trzeba
// przerysowaæ scenê)

void RenderScene(void)
{
	if (interaction == 1)
	{
		pBytes = LoadTGAImage("F:/Grafika(OpenGL)/[lab6]TeksturowanieObiektow/tekstury/P3_t.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);
		changeTexture();
	}
	if (interaction == 2)
	{
		pBytes = LoadTGAImage("F:/Grafika(OpenGL)/[lab6]TeksturowanieObiektow/tekstury/korwin.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);
		changeTexture();
	}
	if (interaction == 3)
	{
		pBytes = LoadTGAImage("F:/Grafika(OpenGL)/[lab6]TeksturowanieObiektow/tekstury/D7_t.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);
		changeTexture();
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym

	glLoadIdentity();
	// Czyszczenie macierzy bie??cej

	gluLookAt(viewer[0], viewer[1], viewer[2], object[0], object[1], object[2], 0.0, 1.0, 0.0);
	// Zdefiniowanie po³o¿enia obserwatora

	if (status == 1)                     // jeœli lewy klawisz myszy wciêniêty
	{
		theta[0] += delta_x * pix2angle;    // modyfikacja k¹ta obrotu o kat proporcjonalny
		theta[1] += delta_y * pix2angle;
	}                                  // do ró¿nicy po³o¿eñ kursora myszy

	if (status == 2) // jesli prawy wcisniety
	{
		zoom -= delta_y2 * pix2angle;
	}

	glTranslatef(0.0, -3.0, zoom);

	glRotatef(theta[0], 0.0, 1.0, 0.0);  //obrót obiektu o nowy k¹t
	glRotatef(theta[1], 1.0, 0.0, 0.0);

	
	glColor3f(1.0f, 1.0f, 1.0f);

	//glutSolidTeapot(3.0);
	// Narysowanie czajnika 

	//drawTriangle();
	if(model == 1) drawPiramid();
	if (model == 2) drawCube();

	glFlush();
	// Przekazanie poleceñ rysuj¹cych do wykonania

	glutSwapBuffers();
}

// Funkcja ustalaj¹ca stan renderowania
void MyInit(void)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// Kolor czyszcz¹cy (wype³nienia okna) ustawiono na cszary

	//  Definicja materia³u z jakiego zrobiony jest czajnik
	//  i definicja Ÿród³a œwiat³a
	GLfloat mat_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	// wspó³czynniki ka =[kar,kag,kab] dla œwiat³a otoczenia

	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// wspó³czynniki kd =[kdr,kdg,kdb] œwiat³a rozproszonego

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// wspó³czynniki ks =[ksr,ksg,ksb] dla œwiat³a odbitego               

	GLfloat mat_shininess = { 128.0 };
	// wspó³czynnik n opisuj¹cy po³ysk powierzchni

/*************************************************************************************/
// Definicja Ÿród³a œwiat³a

	GLfloat light_position[] = { 0.0, 1.0, 5.0, 1.0 };
	GLfloat light1_position[] = { 15.0, 1.0, 0.0, 1.0 };
	GLfloat light2_position[] = { -2.0, 1.0, 0.0, 1.0 };
	// po³o¿enie Ÿród³a

	GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a otoczenia
	// Ia = [Iar,Iag,Iab]

	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
	// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
	// odbicie kierunkowe Is = [Isr,Isg,Isb]

	GLfloat att_constant = { 0.5 };
	// sk³adowa sta³a ds dla modelu zmian oœwietlenia w funkcji
	// odleg³oœci od Ÿród³a

	GLfloat att_linear = { 0.05 };
	// sk³adowa liniowa dl dla modelu zmian oœwietlenia w funkcji
	// odleg³oœci od Ÿród³a

	GLfloat att_quadratic = { 0.001 };
	// sk³adowa kwadratowa dq dla modelu zmian oœwietlenia w funkcji
	// odleg³oœci od Ÿród³a

/*************************************************************************************/
	// Ustawienie patrametrów materia³u

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	/*************************************************************************************/
	// Ustawienie parametrów Ÿród³a1

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

	// Ustawienie parametrów Ÿród³a2

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_quadratic);

	// Ustawienie parametrów Ÿród³a3

	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);

	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, att_quadratic);


	/*************************************************************************************/
	// Ustawienie opcji systemu oœwietlania sceny

	glShadeModel(GL_SMOOTH); // w³aczenie ³agodnego cieniowania
	glEnable(GL_LIGHTING);   // w³aczenie systemu oœwietlenia sceny
	glEnable(GL_LIGHT0);     // w³¹czenie Ÿród³a o numerze 0
	glEnable(GL_LIGHT1);     // w³¹czenie Ÿród³a o numerze 1
	glEnable(GL_LIGHT2);     // w³¹czenie Ÿród³a o numerze 1
	glEnable(GL_DEPTH_TEST); // w³¹czenie mechanizmu z-bufora
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

void keys(unsigned char key, int x, int y)
{
	// tekstury
	if (key == 't') interaction = 1;
	if (key == 'k') interaction = 2;
	if (key == 'b') interaction = 3;

	// modele
	if (key == '1') model = 1;
	if (key == '2') model = 2;

	RenderScene(); // przerysowanie obrazu sceny
}

// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli
int main(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(400, 400);
	glutInitWindowPosition(580, 200);

	glutCreateWindow("Teksturowanie obiektów");

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
	// Funkcja obs³uguj¹ca klawiaturê komputera
	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}

/*************************************************************************************/
 // Funkcja wczytuje dane obrazu zapisanego w formacie TGA w pliku o nazwie
 // FileName, alokuje pamiêæ i zwraca wskaŸnik (pBits) do bufora w którym
 // umieszczone s¹ dane.
 // Ponadto udostêpnia szerokoœæ (ImWidth), wysokoœæ (ImHeight) obrazu
 // tekstury oraz dane opisuj¹ce format obrazu wed³ug specyfikacji OpenGL
 // (ImComponents) i (ImFormat).
 // Jest to bardzo uproszczona wersja funkcji wczytuj¹cej dane z pliku TGA.
 // Dzia³a tylko dla obrazów wykorzystuj¹cych 8, 24, or 32 bitowy kolor.
 // Nie obs³uguje plików w formacie TGA kodowanych z kompresj¹ RLE.
/*************************************************************************************/
GLbyte* LoadTGAImage(const char* FileName, GLint* ImWidth, GLint* ImHeight, GLint* ImComponents, GLenum* ImFormat)
{

	/*************************************************************************************/

	// Struktura dla nag³ówka pliku  TGA


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

	// Wartoœci domyœlne zwracane w przypadku b³êdu
	*ImWidth = 0;
	*ImHeight = 0;
	*ImFormat = GL_BGR_EXT;
	*ImComponents = GL_RGB8;

	pFile = fopen(FileName, "rb");
	if (pFile == NULL)
		return NULL;

	// Przeczytanie nag³ówka pliku 
	fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);

	// Odczytanie szerokoœci, wysokoœci i g³êbi obrazu
	*ImWidth = tgaHeader.width;
	*ImHeight = tgaHeader.height;
	sDepth = tgaHeader.bitsperpixel / 8;

	// Sprawdzenie, czy g³êbia spe³nia za³o¿one warunki (8, 24, lub 32 bity)
	if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32)
		return NULL;


	// Obliczenie rozmiaru bufora w pamiêci
	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

	// Alokacja pamiêci dla danych obrazu
	pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));

	if (pbitsperpixel == NULL)
		return NULL;

	if (fread(pbitsperpixel, lImageSize, 1, pFile) != 1)
	{
		free(pbitsperpixel);
		return NULL;
	}

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