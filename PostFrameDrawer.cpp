#include "PostFrameDrawer.h"
#include "RenderableObjec.h"

PostFrameDrawer* PostFrameDrawer::m_self = nullptr;
PostFrameDrawer::PostFrameDrawer()
{
    m_self = this;
}

PostFrameDrawer::~PostFrameDrawer()
{

}

void PostFrameDrawer::DrawRenderableObject(RenderableObject& obj)
{
    m_objects.push_back(new RenderableObject(obj));
}

std::vector<RenderableObject*>* PostFrameDrawer::GetObjects()
{
    return &m_objects;
}

void PostFrameDrawer::ClearContents()
{
    for (RenderableObject* obj : m_objects)
        delete obj;

    m_objects.clear();
}