#include "RayTracerFactory.h"

#include <fstream>
#include <iostream>

#include "SimpleSerializer.h"
#include "STLBinarySerializer.h"
#include "CustomSerializer.h"
#include "RenderConfig.h"

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
    RenderConfig renderConfig;
    while(!config.eof())
    {
        config >> s;
        if(!s[0] == '#')
            continue;
        if(!s.compare("SimpleSerializer"))
            kernel->LoadScene(new SimpleSerializer(), scenePath);
        else if(!s.compare("STLBinarySerializer"))
            kernel->LoadScene(new STLBinarySerializer(), scenePath);
        else if(!s.compare("CustomSerializer"))
            kernel->LoadScene(new CustomSerializer(), scenePath);
        else if(!s.compare("disableLight"))
            renderConfig.setLight(false);
        else if(!s.compare("disableReflection"))
            renderConfig.setReflection(false);
        else if(!s.compare("disableRefraction"))
            renderConfig.setRefraction(false);
        else if(!s.compare("reflectionDepth"))
        {
            unsigned int depth;
            config >> depth;
            renderConfig.setReflectionDepth(depth);
        }
    }
    kernel->SetConfig(renderConfig);
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
