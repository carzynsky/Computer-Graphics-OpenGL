#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>

int model = 1; // 1 - prostok¹t, 2 - dywan sierpiñskiego
float randNoise() 
{
	float LO = 0.8f;
	float HI = 1.2f;
	return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO))); // LO; HI
}

float randColor() 
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // 0.0 ; 1.0
}

void dywan(int poziomy, float x1, float y1, float x2, float y2)
{
	if (poziomy == 3)
	{
		glBegin(GL_TRIANGLE_STRIP);
		glColor3f(randColor(), randColor(), randColor());
		glVertex2f(x1, y1);
		glColor3f(randColor(), randColor(), randColor());
		glVertex2f(x2, y1);
		glColor3f(randColor(), randColor(), randColor());
		glVertex2f(x1, y2);
		glColor3f(randColor(), randColor(), randColor()); 
		glVertex2f(x2, y2);

		glEnd();
		return;
	}

	float height = (x2 - x1) / 3;
	float width = (y2 - y1) / 3;

	dywan(poziomy + 1, x1 + 0 * height, y1 + 0 * width, x1 + 1 * height, y1 + 1 * width);//1          1 2 3
	dywan(poziomy + 1, x1 + 1 * height, y1 + 0 * width, x1 + 2 * height, y1 + 1 * width);//2          4   6
	dywan(poziomy + 1, x1 + 2 * height, y1 + 0 * width, x1 + 3 * height, y1 + 1 * width);//3          7 8 9
	dywan(poziomy + 1, x1 + 0 * height, y1 + 1 * width, x1 + 1 * height, y1 + 2 * width);//4
	dywan(poziomy + 1, x1 + 2 * height, y1 + 1 * width, x1 + 3 * height, y1 + 2 * width);//6
	dywan(poziomy + 1, x1 + 0 * height, y1 + 2 * width, x1 + 1 * height, y1 + 3 * width);//7
	dywan(poziomy + 1, x1 + 1 * height, y1 + 2 * width, x1 + 2 * height, y1 + 3 * width);//8
	dywan(poziomy + 1, x1 + 2 * height, y1 + 2 * width, x1 + 3 * height, y1 + 3 * width);//9
}

void prostokat(float x, float y, float a, float b) 
{
	a *= randNoise();
	b *= randNoise();
	x *= randNoise();
	y *= randNoise();


	glBegin(GL_TRIANGLES);

		// first triangle
		glColor3f(randColor(), randColor(), randColor());
		glVertex2f(x, y);
		glColor3f(randColor(), randColor(), randColor());
		glVertex2f(x + a, y);
		glColor3f(randColor(), randColor(), randColor());
		glVertex2f(x, y + b);

		//second triangle
		glColor3f(randColor(), randColor(), randColor());
		glVertex2f(x, y + b);
		glColor3f(randColor(), randColor(), randColor());
		glVertex2f(x + a, y);
		glColor3f(randColor(), randColor(), randColor());
		glVertex2f(x + a, y + b);

	glEnd();
}

// Funkcaja okreœlaj¹ca, co ma byæ rysowane
// (zawsze wywo³ywana, gdy trzeba przerysowaæ scenê)

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym
	if (model == 1)
	{
		prostokat(-50.0f, -20.0f, 100.0f, 50.0f);
	}
	if (model == 2)
	{
		dywan(0, -80.0f, -45.0f, 80.0f, 45.0f);
	}
	glFlush();
	// Przekazanie poleceñ rysuj¹cych do wykonania
}

/*************************************************************************************/

// Funkcja ustalaj¹ca stan renderowania



void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}


/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie sta³ych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokoœæ i szerokoœæ okna) s¹ 
// przekazywane do funkcji za ka¿dym razem gdy zmieni siê rozmiar okna.

void ChangeSize(GLsizei horizontal, GLsizei vertical)

// Parametry horizontal i vertical (szerokoœæ i wysokoœæ okna) s¹
// przekazywane do funkcji za ka¿dym razem, gdy zmieni siê rozmiar okna
{
	GLfloat AspectRatio;
	// Deklaracja zmiennej AspectRatio okreœlaj¹cej proporcjê wymiarów okna

	if (vertical == 0)
		// Zabezpieczenie pzred dzieleniem przez 0

		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkoœciokna okna urz¹dzenia (Viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)

	glMatrixMode(GL_PROJECTION);
	// Okreœlenie uk³adu wspó³rzêdnych obserwatora

	glLoadIdentity();
	// Okreœlenie przestrzeni ograniczaj¹cej

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wspó³czynnika proporcji okna

	// Gdy okno na ekranie nie jest kwadratem wymagane jest
	// okreœlenie okna obserwatora.
	// Pozwala to zachowaæ w³aœciwe proporcje rysowanego obiektu
	// Do okreœlenia okna obserwatora s³u¿y funkcja glOrtho(...)

	if (horizontal <= vertical) glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);

	else glOrtho(-100.0 * AspectRatio, 100.0 * AspectRatio, -100.0, 100.0, 1.0, -1.0);

	glFlush();

	glMatrixMode(GL_MODELVIEW);
	// Okreœlenie uk³adu wspó³rzêdnych     

	glLoadIdentity();

}

// Obs³uga klawiatury
void keys(unsigned char key, int x, int y)
{
	if (key == '1') model = 1;
	if (key == '2') model = 2;
	RenderScene(); // przerysowanie obrazu sceny
}

/*************************************************************************************/

// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli

void main(void)
{

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// Ustawienie trybu wyœwietlania
	// GLUT_SINGLE - pojedynczy bufor wyœwietlania
	// GLUT_RGBA - model kolorów RGB

	glutCreateWindow("Dywan sierpiñskiego");
	// Utworzenie okna i okreœlenie treœci napisu w nag³ówku okna

	glutDisplayFunc(RenderScene);
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹ (callback)
	// Biblioteka GLUT bêdzie wywo³ywa³a t¹ funkcjê za ka¿dym razem, gdy
	// trzeba bêdzie przerysowaæ okno

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹ za
	// zmiany rozmiaru okna

	MyInit();
	// Funkcja MyInit (zdefiniowana powy¿ej) wykonuje wszelkie 
	// inicjalizacje konieczneprzed przyst¹pieniem do renderowania

	glutKeyboardFunc(keys);

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}
