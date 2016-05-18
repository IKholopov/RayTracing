#include <GL/glut.h>
#include <cstdlib>
#include <iostream>

#include "TGAView.h"
#include "RayTracerFactory.h"
#include "SimpleSerializer.h"
#include "STLBinarySerializer.h"

TGAView* view;

int main( int argc, char **argv )
{
    unsigned int W = atoi(argv[1]);
    unsigned int H = atoi(argv[2]);
    view = new TGAView(W, H);
    if(argc < 5)
    {
        std::cerr << "No scene file to render" << std::endl;
        std::cout << "Usage: TGAView WIDTH HEIGHT CONFIG_FILE SCENE_FILE [RESULT_PATH]\n" << std::endl;
        exit(1);
    }
    Kernel* kernel = RayTracerFactory::Instance()->GetKernel(argv[3], argv[4]);
    kernel->SetView(view);
    kernel->Run();
    kernel->WaitForRender();
    if(argc > 5)
        view->Export(argv[5]);
    else
        view->Export("rendered.tga");
    delete kernel;
    return 0;
}
