#ifndef SCENE_H
#define SCENE_H

#include "Base.h"
#include "Camera.h"
#include "IView.h"
#include "IGeometryHierarchy.h"
#include "ThreadPool.h"

class Scene
{
    public:
        Scene(Camera camera, IView* view, IGeometryHierarchy* hierarchy, std::vector<PointLight*> lights);
        void RenderScene();
        void SetView(IView* view);
    private:
        IGeometryHierarchy* hierarchy_;
        IView* view_;
        Camera camera_;
        std::vector<PointLight*> lights_;
};

#endif
