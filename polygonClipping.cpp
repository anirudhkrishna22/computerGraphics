// sutherland-hodgman polygon clipping algorithm
// press 'c' to clip polygon

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

// window coordinates
int x_min, y_min, x_max, y_max;

vector<Point> points;
vector<Point> forLineLoop;
vector<Point>leftc;
vector<Point>rightc;
vector<Point>bottomc;
vector<Point>topc;

// to draw window or not
bool isWindow=true;

void drawPoint(Point p)
{
    glColor3d(1, 1, 1);
    glPointSize(3);
    int x=p.x, y=p.y;
    glBegin(GL_POINTS);
    glVertex2d(x, y);
    glEnd();
    glFlush();
}

void drawLineLoop()
{
    glColor3d(0.9, 0.2, 0.2);
    glBegin(GL_LINE_LOOP);
    glLineWidth(2);
    while(forLineLoop.size())
    {
        glVertex2d(forLineLoop[0].x, forLineLoop[0].y);
        forLineLoop.erase(forLineLoop.begin());
    }
    glEnd();
    glFlush();
}

void drawPolygon()
{
    glColor3d(0.2, 0.9, 0.4);
    glBegin(GL_POLYGON);
    while(topc.size())
    {
        glVertex2d(topc[0].x, topc[0].y);
        topc.erase(topc.begin());
    }
    glEnd();
    glFlush();
}

// clipping wrt left edge of window
void clip_left()
{
    Point first_point=points[0];
    Point p1;
    Point p2;
    while(points.size())
    {
        if(points.size()==1)
        {
            p1=points[0];
            p2=first_point;
        }
        else
        {
            p1=points[0];
            p2=points[1];
        }

        float x1=p1.x, x2=p2.x, y1=p1.y, y2=p2.y;

        // first outside, second inside
        if(x1<x_min && x2>=x_min)
        {
            int x=x_min;
            float m=(y2-y1)/(x2-x1);
            int y=y1+(x-x1)*m;
            Point intersection=Point(x, y);
            leftc.push_back(intersection);
            leftc.push_back(p2);
        }
        // both inside
        else if(x1>=x_min && x2>=x_min)
        {
            leftc.push_back(p2);
        }
        // first inside, second outside
        else if(x1>=x_min && x2<x_min)
        {
            int x=x_min;
            float m=(y2-y1)/(x2-x1);
            int y=y1+(x-x1)*m;
            Point intersection=Point(x, y);
            leftc.push_back(intersection);
        }
        // both outside
        else
        {
            ;
        }
        points.erase(points.begin());
    }
}

// clipping wrt right edge of window
void clip_right()
{
    Point first_point=leftc[0];
    Point p1;
    Point p2;
    while(leftc.size())
    {
        if(leftc.size()==1)
        {
            p1=leftc[0];
            p2=first_point;
        }
        else
        {
            p1=leftc[0];
            p2=leftc[1];
        }

        float x1=p1.x, x2=p2.x, y1=p1.y, y2=p2.y;

        // first outside, second inside
        if(x1>x_max && x2<=x_max)
        {
            int x=x_max;
            float m=(y2-y1)/(x2-x1);
            int y=y1+(x-x1)*m;
            Point intersection=Point(x, y);
            rightc.push_back(intersection);
            rightc.push_back(p2);
        }
        // both inside
        else if(x1<=x_max && x2<=x_max)
        {
            rightc.push_back(p2);
        }
        // first inside, second outside
        else if(x1<=x_max && x2>x_max)
        {
            int x=x_max;
            float m=(y2-y1)/(x2-x1);
            int y=y1+(x-x1)*m;
            Point intersection=Point(x, y);
            rightc.push_back(intersection);
        }
        // both outside
        else
        {
            ;
        }
        leftc.erase(leftc.begin());
    }
}

// clipping wrt bottom edge of window
void clip_bottom()
{
    Point first_point=rightc[0];
    Point p1;
    Point p2;
    while(rightc.size())
    {
        if(rightc.size()==1)
        {
            p1=rightc[0];
            p2=first_point;
        }
        else
        {
            p1=rightc[0];
            p2=rightc[1];
        }

        float x1=p1.x, x2=p2.x, y1=p1.y, y2=p2.y;

        // first outside, second inside
        if(y1<y_min && y2>=y_min)
        {
            int y=y_min;
            float m=(y-y1)/(y2-y1);
            int x=x1+(x2-x1)*m;
            Point intersection=Point(x, y);
            bottomc.push_back(intersection);
            bottomc.push_back(p2);
        }
        // both inside
        else if(y1>=y_min && y2>=y_min)
        {
            bottomc.push_back(p2);
        }
        // first inside, second outside
        else if(y1>=y_min && y2<y_min)
        {
            int y=y_min;
            float m=(y-y1)/(y2-y1);
            int x=x1+(x2-x1)*m;
            Point intersection=Point(x, y);
            bottomc.push_back(intersection);
        }
        // both outside
        else
        {
            ;
        }
        rightc.erase(rightc.begin());
    }
}

// clipping wrt top edge of window
void clip_top()
{
    Point first_point=bottomc[0];
    Point p1;
    Point p2;
    while(bottomc.size())
    {
        if(bottomc.size()==1)
        {
            p1=bottomc[0];
            p2=first_point;
        }
        else
        {
            p1=bottomc[0];
            p2=bottomc[1];
        }

        float x1=p1.x, x2=p2.x, y1=p1.y, y2=p2.y;

        // first outside, second inside
        if(y1>y_max && y2<=y_max)
        {
            int y=y_max;
            float m=(y-y1)/(y2-y1);
            int x=x1+(x2-x1)*m;
            Point intersection=Point(x, y);
            topc.push_back(intersection);
            topc.push_back(p2);
        }
        // both inside
        else if(y1<=y_max && y2<=y_max)
        {
            topc.push_back(p2);
        }
        // first inside, second outside
        else if(y1<=y_max && y2>y_max)
        {
            int y=y_max;
            float m=(y-y1)/(y2-y1);
            int x=x1+(x2-x1)*m;
            Point intersection=Point(x, y);
            topc.push_back(intersection);
        }
        // both outside
        else
        {
            ;
        }
        bottomc.erase(bottomc.begin());
    }
}

void sutherlandHodgman()
{
    clip_left();
    clip_right();
    clip_bottom();
    clip_top();
    drawPolygon();
    cout<<" executed fine";
}

void drawWindow()
{
    glColor3d(0.3, 0.4, 1);
    Point p1=points[0];
    Point p2=points[1];
    x_min=p1.x, x_max=p2.x, y_min=p1.y, y_max=p2.y;
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    glVertex2d(x_min, y_min);
    glVertex2d(x_min, y_max);
    glVertex2d(x_max, y_max);
    glVertex2d(x_max, y_min);
    glEnd();
    glFlush();
    points.clear();
}

void mouse(int button, int state, int x, int y)
{
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
        Point p=Point(x, 480-y);
        drawPoint(p);
        points.push_back(p);
        if(isWindow && points.size()==2)
        {
            drawWindow();
            isWindow=false;
        }
    }
    if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
    {
        forLineLoop=points;
        drawLineLoop();
    }
}

static void keyboard(unsigned char key,int x,int y)
{
    if(key=='c')
    {
        sutherlandHodgman();
    }
}

// if there is no display callback there will be error
static void display(void)
{
    ;
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

    glutCreateWindow("polygon clipping");

    glutDisplayFunc(display);

    init();

    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    return EXIT_SUCCESS;
}
