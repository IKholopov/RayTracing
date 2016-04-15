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
        Scene(Camera camera, IView* view, IGeometryHierarchy* hierarchy);

        void RenderScene();
    private:
        IGeometryHierarchy* hierarchy_;
        IView* view_;
        Camera camera_;
};

#endif
