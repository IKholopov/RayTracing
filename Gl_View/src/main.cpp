#include <GL/glut.h>
#include <cstdlib>
#include <iostream>

#include "GLView.h"
#include "RayTracerFactory.h"
#include "SimpleSerializer.h"
#include "STLBinarySerializer.h"

unsigned int W;
unsigned int H;

GLView* view;

void display()
{
    glClearColor( 0, 0, 0, 1 );
    glClear( GL_COLOR_BUFFER_BIT );


    glDrawPixels( W, H, GL_RGB, GL_FLOAT, view->GetData());

    glutSwapBuffers();
    glutPostRedisplay();
}

int main( int argc, char **argv )
{
    W = atoi(argv[1]);
    H = atoi(argv[2]);
    view = new GLView(W, H);
    if(argc < 4)
    {
        std::cerr << "No scene file to render" << std::endl;
        exit(1);
    }
    Kernel* kernel = RayTracerFactory::Instance()->GetKernel(argv[3], argv[4]);
    kernel->SetView(view);
    kernel->Run();
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
    glutInitWindowSize(W, H);
    glutCreateWindow( "RayTracer" );
    glutDisplayFunc( display );
    glutMainLoop();
    delete kernel;
    return 0;
}
