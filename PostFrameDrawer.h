#pragma once
#include <vector>//         std::vectir

struct RenderableObject;

class PostFrameDrawer
{
public:
    PostFrameDrawer();
    ~PostFrameDrawer();

    static PostFrameDrawer* Get()
    {
        return m_self;
    }

    void DrawRenderableObject(RenderableObject& obj);
    std::vector<RenderableObject*>* GetObjects();
    void ClearContents();

private:
    std::vector<RenderableObject*> m_objects;
    static PostFrameDrawer* m_self;
};