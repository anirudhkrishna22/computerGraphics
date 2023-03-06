// press 'b' for boundary fill 'f' for flood fill

#include<windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include<vector>
#include <stdlib.h>
#include<iostream>
using namespace std;

void init()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3d(0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 640, 0, 480);
    glClearColor(0, 0, 0, 1);
}

class Point
{
    public:
    int x;
    int y;
    Point(int a=0, int b=0)
    {
        x=a;
        y=b;
    }
};

class Color
{
    public:
    float r;
    float g;
    float b;
    Color(float R=0, float G=0, float B=0)
    {
        r=R;
        g=G;
        b=B;
    }
};

vector<Point> points;
vector<Point> forLineLoop;


void drawPoint(Point p)
{
    glColor3d(0.0f, 1.0f, 0.0f);
    //glPointSize(2.0f);
    int x=p.x, y=p.y;
    glBegin(GL_POINTS);
    glVertex2d(x, y);
    glEnd();
    glFlush();
}

void drawLineLoop()
{
    glColor3d(1.0f, 0.0f, 0.0f);
    //glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    while(forLineLoop.size())
    {
        glVertex2d(forLineLoop[0].x, forLineLoop[0].y);
        forLineLoop.erase(forLineLoop.begin());
    }
    glEnd();
    glFlush();
}

bool isequal(Color c1, Color c2)
{
    if(c1.r==c2.r && c1.g==c2.g && c1.b==c2.b)
        return true;
    return false;
}

Color getPixelColor(GLint x, GLint y) {
    Color color;
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &color);
    return color;
}

void setPixelColor(GLint x, GLint y, Color color) {

    glColor3d(color.r, color.g, color.b);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

void boundaryFill4 (int x, int y, Color fillColor, Color borderColor)
{
    Color interiorColor;
    interiorColor = getPixelColor (x, y);
    if (!isequal(interiorColor, borderColor) && !isequal(interiorColor, fillColor))
    {
        setPixelColor (x, y, fillColor); // Set color of pixel to fillColor.
        boundaryFill4 (x + 1, y , fillColor, borderColor);
        boundaryFill4 (x - 1, y , fillColor, borderColor);
        boundaryFill4 (x , y + 1, fillColor, borderColor);
        boundaryFill4 (x , y - 1, fillColor, borderColor);
    }
}

void floodFill4 (int x, int y, Color fillColor, Color interiorColor)
{
    Color currentColor;
    currentColor = getPixelColor (x, y);
    if (isequal(currentColor, interiorColor))
    {
        setPixelColor (x, y, fillColor); // Set color of pixel to fillColor.
        floodFill4 (x + 1, y, fillColor, interiorColor);
        floodFill4 (x - 1, y, fillColor, interiorColor);
        floodFill4 (x, y + 1, fillColor, interiorColor);
        floodFill4 (x, y - 1, fillColor, interiorColor);
    }
}

// this is used to check whether to draw a point or not
string lastClick="left";

void mouse(int button, int state, int x, int y)
{
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
        Point p=Point(x, 480-y);
        // otherwise when last click is right the current left click will be for seed point, so we don't need to plot that point
        if(lastClick=="left")
            drawPoint(p);
        points.push_back(p);
    }
    if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
    {
        lastClick="right";
        forLineLoop=points;
        // draw the polygon to fill
        drawLineLoop();
    }
}

static void keyboard(unsigned char key,int x,int y)
{
    if(key=='b')
    {
        Color red=Color(1.0f, 0.0f, 0.0f);
        Color blue=Color(0.0f, 0.0f, 1.0f);
        boundaryFill4(points[points.size()-1].x, points[points.size()-1].y, blue, red);
    }
    if(key=='f')
    {
        Color blue=Color(0.0f, 0.0f, 1.0f);
        Color black=Color(0.0f, 0.0f, 0.0f);
        floodFill4(points[points.size()-1].x, points[points.size()-1].y, blue, black);
    }
    else
    {
        cout<<"invalid key";
    }
}

static void display(void)
{

}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

    glutCreateWindow("polygon filling");

    glutDisplayFunc(display);

    init();

    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    //glClearColor(0, 0, 0, 0);

    glutMainLoop();

    return EXIT_SUCCESS;
}
