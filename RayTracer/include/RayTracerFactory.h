#ifndef RAY_TRACER_FACTORY_H
#define RAY_TRACER_FACTORY_H

#include <memory>
#include <mutex>
#include <string>
#include "Kernel.h"

class RayTracerFactory
{
    public:
        ~RayTracerFactory() {};
        static std::shared_ptr<RayTracerFactory>& Instance();
        Kernel* GetKernel(std::string configPath, std::__cxx11::string scenePath);
    private:
        static std::mutex mutex_;

        RayTracerFactory() {};



};

#endif
