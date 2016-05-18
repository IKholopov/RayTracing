#include <iostream>

#include "RayTracerFactory.h"
#include "CustomSerializer.h"


int main( int argc, char **argv )
{
    if(argc < 4)
    {
        std::cerr << "Not enough arguments" << std::endl;
        exit(1);
    }
    Kernel* kernel = RayTracerFactory::Instance()->GetKernel(argv[1], argv[2]);
    auto serializer = new CustomSerializer();
    kernel->ExportScene(serializer, argv[3]);
    delete serializer;
    delete kernel;
    return 0;
}
