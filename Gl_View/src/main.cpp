#include <GL/glut.h>
#include <cstdlib>
#include <iostream>

#include "GLView.h"
#include "Kernel.h"
#include "SimpleSerializer.h"

const unsigned int W = 640;
const unsigned int H = 480;

GLView* view;

void display()
{
    glClearColor( 0, 0, 0, 1 );
    glClear( GL_COLOR_BUFFER_BIT );


    glDrawPixels( W, H, GL_RGB, GL_FLOAT, view->GetData());

    glutSwapBuffers();
    glutPostRedisplay ();
}

int main( int argc, char **argv )
{
    view = new GLView(W, H);
    SimpleSerializer* serializer = new SimpleSerializer();
    if(argc < 2)
    {
        std::cerr << "No scene file to render" << std::endl;
        exit(1);
    }
    Kernel kernel(view, serializer, argv[1]);
    kernel.Run();
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
    glutInitWindowSize( W, H );
    glutCreateWindow( "RayTracer" );
    glutDisplayFunc( display );
    glutMainLoop();
    return 0;
}
