#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <vector>
#include <cmath>

# define M_PI 3.14159265358979323846
static GLfloat theta[] = { 0.0, 0.0, 0.0 }; // trzy k�ty obrotu
typedef float point3[3];
const int N = 70;
int model = 1;

struct p {
	float x, y, z;
};
p matrix[N][N];

float randColor()
{
	return static_cast<float> (rand()) / static_cast <float> (RAND_MAX); // 0.0 ; 1.0
}

void Axes(void)
{
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz�tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0,  5.0, 0.0 };
	// pocz�tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0,  5.0 };
	//  pocz�tek i koniec obrazu osi y

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
			matrix[(int)i][(int)j].x = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * cos(v * M_PI);
			matrix[(int)i][(int)j].y = 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2);
			matrix[(int)i][(int)j].z = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * sin(v * M_PI);
		}
	}

}
void EggNet(int n)
{
	glRotated(-40.0, 1.0, 0.0, 0.0);
	glTranslated(0.0f, -4.5f, 0.0f);
	glBegin(GL_LINES);

	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - 1; j++)
		{
			glColor3f(0.8f, 0.9f, 0.0f);
			glVertex3f(matrix[i][j].x, matrix[i][j].y, matrix[i][j].z);
			glVertex3f(matrix[i][j + 1].x, matrix[i][j + 1].y, matrix[i][j + 1].z);

			glColor3f(0.8f, 0.8f, 0.0f);
			glVertex3f(matrix[i][j].x, matrix[i][j].y, matrix[i][j].z);
			glVertex3f(matrix[i + 1][j].x, matrix[i + 1][j].y, matrix[i + 1][j].z);
		}
	}
	glEnd();
}
void Egg(int n)
{
	glRotated(-40.0, 1.0, 0.0, 0.0);
	glTranslated(0.0f, -4.5f, 0.0f);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			glBegin(GL_POINTS);
				glColor3f(0.8, 0.6f, 0.9f);
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
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - 1; j++)
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
		}
	}
}
void EggTriangleStrip(int n)
{
	glRotated(-40.0, 1.0, 0.0, 0.0);
	glTranslated(0.0f, -4.5f, 0.0f);
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 2; j < n; j++)
		{

			glBegin(GL_TRIANGLE_STRIP);
				glColor3f(1.0f, 0.0f, 0.7f);
				glVertex3f(matrix[(i)][j].x, matrix[i][j].y, matrix[i][j].z);
				glColor3f(1.0f, 0.0f, 0.7f);
				glVertex3f(matrix[(i + 1)][j].x, matrix[(i + 1)][j].y, matrix[i + 1][j].z);
				glColor3f(1.0f, 0.0f, 0.8f);
				glVertex3f(matrix[i][(j - 1)].x, matrix[i][(j - 1)].y, matrix[i][(j - 1)].z);
				glColor3f(1.0f, 0.4f, 0.9f);
				glVertex3f(matrix[(i + 1)][(j - 1)].x, matrix[(i + 1)][(j - 1)].y, matrix[(i + 1)][(j - 1)].z);
				glColor3f(1.0f, 0.4f, 0.9f);
				glVertex3f(matrix[i][(j - 2)].x, matrix[i][j - 2].y, matrix[i][(j - 2)].z);
				glColor3f(1.0f, 0.3f, 0.8f);
				glVertex3f(matrix[(i + 1)][(j - 2)].x, matrix[(i + 1)][(j - 2)].y, matrix[(i + 1)][(j - 2)].z);
			glEnd();
		}
	}
}

// Funkcja okre�laj�ca co ma by� rysowane (zawsze wywo�ywana gdy trzeba
// przerysowa� scen�)
void RenderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	glLoadIdentity();
	// Czyszczenie macierzy bie��cej

	//Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej wy�ej
	//Imbryczek();
	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glRotatef(theta[2], 0.0, 0.0, 1.0);

	if (model == 1) Egg(N);

	if (model == 2) EggNet(N);

	if (model == 3) EggTriangle(N);

	if (model == 4) EggTriangleStrip(N);

	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania
	glutSwapBuffers();
}

// Funkcja ustalaj�ca stan renderowania
void MyInit(void)
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszc�cy (wype�nienia okna) ustawiono na czarny

}

// Funkcja ma za zadanie utrzymanie sta�ych proporcji rysowanych
// w przypadku zmiany rozmiar�w okna.
// Parametry vertical i horizontal (wysoko�� i szeroko�� okna) s�
// przekazywane do funkcji za ka�dym razem gdy zmieni si� rozmiar okna.
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{

	GLfloat AspectRatio;
	// Deklaracja zmiennej AspectRatio  okre�laj�cej proporcj�
	// wymiar�w okna 

	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0

		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielko�ciokna okna widoku (viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)  

	glMatrixMode(GL_PROJECTION);
	// Prze��czenie macierzy bie��cej na macierz projekcji 

	glLoadIdentity();
	// Czyszcznie macierzy bie��cej            

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wsp�czynnika  proporcji okna
	// Gdy okno nie jest kwadratem wymagane jest okre�lenie tak zwanej
	// przestrzeni ograniczaj�cej pozwalaj�cej zachowa� w�a�ciwe
	// proporcje rysowanego obiektu.
	// Do okreslenia przestrzeni ograniczj�cej s�u�y funkcja
	// glOrtho(...)            

	if (horizontal <= vertical)

		glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);

	else

		glOrtho(-7.5 * AspectRatio, 7.5 * AspectRatio, -7.5, 7.5, 10.0, -10.0);

	glMatrixMode(GL_MODELVIEW);
	// Prze��czenie macierzy bie��cej na macierz widoku modelu                                    

	glLoadIdentity();
	// Czyszcenie macierzy bie��cej

}

void spinEgg()
{

	theta[0] -= 0.02;
	if (theta[0] > 360.0) theta[0] -= 360.0;

	theta[1] -= 0.02;
	if (theta[1] > 360.0) theta[1] -= 360.0;

	theta[2] -= 0.02;
	if (theta[2] > 360.0) theta[2] -= 360.0;

	glutPostRedisplay(); //od�wie�enie zawarto�ci aktualnego okna
}

void keys(unsigned char key, int x, int y)
{
	if (key == '1') model = 1;
	if (key == '2') model = 2;
	if (key == '3') model = 3;
	if (key == '4') model = 4;
	RenderScene(); // przerysowanie obrazu sceny
}

// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli
void main(void)
{
	generateMatrix();
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 300);

	glutCreateWindow("Modelowanie 3D");

	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn�
	// (callback function).  Bedzie ona wywo�ywana za ka�dym razem
	// gdy zajdzie potrzba przeryswania okna 

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn�
	// zazmiany rozmiaru okna      

	MyInit();
	// Funkcja MyInit() (zdefiniowana powy�ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst�pieniem do renderowania 

	glEnable(GL_DEPTH_TEST);
	// W��czenie mechanizmu usuwania powierzchni niewidocznych

	glutIdleFunc(spinEgg);
	glutKeyboardFunc(keys);
	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}