#ifndef KERNEL_H
#define KERNEL_H

#include <string>
#include <thread>

#include "ISerializer.h"

class Kernel
{
    public:
        Kernel(IView* view);
        Kernel(IView* view, ISerializer* serializer, std::string sceneFilePath);
        ~Kernel();
        void Run();
        void RunScene();
    private:
        IView* view_;
        Scene* scene_;
        std::thread* thr;
};

#endif
