#ifndef KERNEL_H
#define KERNEL_H

#include <string>
#include <thread>

#include "ISerializer.h"
#include "RenderConfig.h"

class Kernel
{
    public:
        Kernel(IView* view=nullptr);
        Kernel(IView* view, ISerializer* serializer, std::string sceneFilePath);
        ~Kernel();
        void Run();
        void RunScene();
        void LoadScene(ISerializer* serializer, std::string sceneFilePath);
        void SetView(IView* view);
        void SetConfig(RenderConfig config);
    private:
        IView* view_;
        Scene* scene_;
        std::thread* thr;
};

#endif
