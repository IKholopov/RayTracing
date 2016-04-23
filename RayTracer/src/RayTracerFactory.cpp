#include "RayTracerFactory.h"

#include <fstream>
#include <iostream>

#include "SimpleSerializer.h"
#include "STLBinarySerializer.h"

std::mutex RayTracerFactory::mutex_;

Kernel*RayTracerFactory::GetKernel(std::__cxx11::string configPath, std::string scenePath)
{
    auto kernel = new Kernel();
    std::ifstream config(configPath);
    if(!config.is_open())
    {
        std::cerr << "Can't open config file " << configPath << std::endl;
        exit(1);
    }
    std::string s;
    config >> s;
    if(!s.compare("SimpleSerializer"))
        kernel->LoadScene(new SimpleSerializer(), scenePath);
    else if(!s.compare("STLBinarySerializer"))
        kernel->LoadScene(new STLBinarySerializer(), scenePath);
    return kernel;
}

std::shared_ptr<RayTracerFactory>& RayTracerFactory::Instance()
{
    static std::shared_ptr<RayTracerFactory> instance = nullptr;
    if(!instance)
    {
        std::lock_guard<std::mutex> guard(mutex_);
        if(!instance)
            instance.reset(new RayTracerFactory());
    }
    return instance;
}
