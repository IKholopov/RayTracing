#ifndef KERNEL_H
#define KERNEL_H

#include <string>
#include <thread>

#include "Scene.h"

class Kernel
{
    public:
        Kernel();
        Kernel(std::string configFilePath);
        ~Kernel();
        void Run();
        void RunScene();
        IView* GetView();
    private:
        IView* view_;
        std::thread* thr;
};

#endif
