#pragma once
#include "CoreStructs.h"//      Point

//MODULES
class InputModule;
class RendererModule;
class ResourceModule;

//MANAGERS
class EntityManager;
class SpriteManager;
class ColliderManager;
class LevelManager;
class GameManager;
class ShapeManager;
class TextManager;
class SoundManager;
class SaveManager;

//TOOLS
class PostFrameDrawer;
class EventBus;

//DEBUG
class Entity;
class EC_Shape;

class Engine
{
    public:
    Engine();
    ~Engine();
    void Run();

private:
    void Init();
    void DeInit();

    void LoadSandbox();

    void GetInput();
    void Update();
    void Render();

    //  DEMOS
    void LoadDemo_1();
    void LoadDemo_2();
    void LoadDemo_3();
    void LoadDemo_4();
    void LoadDemo_5();
    void LoadDemo_6_Instructions();
    void LoadDemo_6_GameOver();
    void LoadDemo_6_YouWin();
    void LoadDemo_7_ShooterTutorial();
    void LoadDemo_DEBUG_DungeonMiniGame();
    void LoadDemo_DEBUG_Shields();
    void LoadDemo_8();
    void LoadDemo_9_Particles();
    void LoadDemo_9_ParticlesWinScreen();
    void LoadDemo_Final_MainMenu();//Load, sel demo, exit
    void LoadDemo_Final_Controls();
    void LoadDemo_Final_Preload();//This scene is the one that loads the static entities
    void LoadMenu_Final_Pause();//cont, save, exit
    void LoadMenu_Final_SaveLoad();
    void LoadMenu_Final_Demos();


    void LogDebugOnConsole(float delta);
    void PrintEntity(Entity* ent);
    Entity* MakeEnt(const char* name, const char* tx, Point pos, Rect src, float scale, bool centerSprite = false);

private:
    InputModule* m_pInput;
    RendererModule* m_pRenderer;
    ResourceModule* m_pResource;

    EntityManager* m_pEntityManager;
    SpriteManager* m_pSpriteManager;
    ColliderManager* m_pColliderManager;
    LevelManager* m_pLevelManager;
    GameManager* m_pGameManager;
    ShapeManager* m_pShapeManager;
    TextManager* m_pTextManager;
    SoundManager* m_pSoundManager;
    SaveManager* m_pSaveManager;

    EventBus* m_pEventBus;
    PostFrameDrawer* m_postFrameDrawer;
    bool m_quit;
};