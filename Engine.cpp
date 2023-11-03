#include "Engine.h"
#include "GlobalMacros.h"


//DEBUG
#include <SDL.h>
#include <iostream>
#include <SDL_ttf.h>


//STD
#include <chrono>//     chrono
#include <functional>// std::function, std::bind
//
#include "RenderableObjec.h"

//MODULES IMPLEMENTATIONS
#include "SDL_InputModule.h"
#include "SDL_InputModule2.h"
#include "SDL_RendererModule.h"
#include "SDL_ResourceModule.h"
#include "MathModule.h"

//MANAGERS
#include "EntityManager.h"
#include "SpriteManager.h"
#include "ColliderManager.h"
#include "LevelManager.h"
#include "ShapeManager.h"
#include "GameManager.h"
#include "TextManager.h"
#include "SoundManager.h"
#include "SaveManager.h"

//ENTITY
#include "Entity.h"

//ENGINE COMPONENTS
#include "EC_Transform.h"
#include "EC_Sprite.h"
#include "EC_Animator.h"
#include "EC_Collider.h"
#include "EC_Shape.h"
#include "EC_Button.h"
#include "EC_Text.h"
#include "EC_Sound.h"
#include "EC_ParticleSpawner.h"


//TOOLS
#include "PostFrameDrawer.h"
#include "EventBus.h"

//USER COMPONENTS
#include "UC_WASDController.h"
#include "UC_StaticTracker.h"
#include "UC_Cursor.h"
#include "UC_Tester.h"
#include "UC_Demo2_Deleter.h"
#include "UC_Demo2_Player.h"
#include "UC_CollisionInformer.h"
#include "UC_DummyComp.h"
#include "UC_ColTypeSwapper.h"
#include "UC_Demo4_Player.h"
#include "UC_Demo4_HealthMod.h"
#include "UC_Demo4_Wall.h"
#include "UC_Demo4_Enemy.h"
#include "UC_Demo4_Key.h"
#include "UC_Demo4_LightZone.h"
#include "UC_Demo4_EnemySpawner.h"
#include "UC_LevelLoader.h"
#include "UC_Demo5_HallwayManager.h"
#include "UC_Demo_6_AnyKeyLoader.h"
#include "UC_EventSenderTest.h"
#include "UC_EventListenerTest.h"
#include "UC_LoadListener.h"
#include "UC_SoundPlayer.h"
#include "UC_Bulltet.h"
#include "UC_Shooter.h"
#include "UC_Demo7_Player.h"
#include "UC_ShooterGameManager.h"
#include "UC_Demo7_OrbEnemy.h"
#include "UC_Demo7_Boss.h"
#include "UC_Demo4_Ghost.h"
#include "UC_Debug_ShieldPieceSpawner.h"
#include "UC_Demo8_SaveManager.h"
#include "UC_Demo8_Lvl1WinCon.h"
#include "UC_Demo8_Lvl2WinCon.h"
#include "UC_Demo9_FlameThrower.h"
#include "UC_Demo9_FlamableObject.h"
#include "UC_Demo9_Selectable.h"
#include "UC_Demo9_Chest.h"
#include "UC_Final_GameManager.h"
#include "UC_BtnToEntLinker.h"
#include "UC_Final_SelectedLevelDisplayer.h"
#include "UC_Btn_LoadOnClick.h"
#include "UC_Debug_Mover.h"

Engine::Engine()
    : m_pInput(new SDL_InputModule2())
    , m_pRenderer(new SDL_RendererModule())
    , m_pResource(new SDL_ResourceModule())
    , m_pEntityManager(new EntityManager())
    , m_pSpriteManager(new SpriteManager())
    , m_pColliderManager(new ColliderManager())
    , m_pLevelManager(new LevelManager(m_pRenderer, m_pEntityManager, m_pResource))
    , m_pGameManager(new GameManager())
    , m_pShapeManager(new ShapeManager())
    , m_pTextManager(new TextManager())
    , m_pSoundManager(new SoundManager())
    , m_pSaveManager(new SaveManager())
    , m_pEventBus(new EventBus())
    , m_postFrameDrawer(new PostFrameDrawer())
    , m_quit(false)
{}

Engine::~Engine()
{
    delete m_pSpriteManager;
    delete m_pEntityManager;
    delete m_pColliderManager;

    delete m_pInput;
    delete m_pRenderer;
    delete m_pResource;
}

//  Init modules and SDL (SDL Might get removed from here in the future)
void Engine::Init()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    //IMG_Init(IMG_InitFlags::IMG_INIT_JPG | IMG_InitFlags::IMG_INIT_PNG);

    m_pRenderer->Init();
    m_pResource->Init();
    m_pInput->Init();
}

//  De-Init modules and SDL (SDL Might get removed from here in the future)
void Engine::DeInit()
{
    m_pEntityManager->DeInit();
    m_pInput->DeInit();
    m_pResource->DeInit();
    m_pRenderer->DeInit();
    SDL_Quit();
}

//  Game loop
void Engine::Run()
{
    Init();
    LoadSandbox();

    while (!m_quit)
    {
        GetInput();
        Update();
        Render();
    }

    DeInit();
}

//  Updates Input
void Engine::GetInput()
{
    m_pInput->Update();

    if ((m_pInput->GetKeyState(KeyCode::k_Ctrl_L) == KeyState::k_KS_Hold &&
        m_pInput->GetKeyState(KeyCode::k_Q) == KeyState::k_KS_Press) ||
        m_pInput->GetKeyState(KeyCode::k_Win_Close) == KeyState::k_KS_Press)
        m_quit = true;
    else if (m_pInput->GetKeyState(KeyCode::k_P) == KeyState::k_KS_Press)
        std::cout << "PAUSE";
}

//  Updates Entities (Components are updated by Entities)
void Engine::Update()
{
    static auto lastFrame = std::chrono::high_resolution_clock::now();
    auto thisFrame = std::chrono::high_resolution_clock::now();
    static float colliderCounter = 0;
    static float colUpdateRate = 0.0166666f;

    std::chrono::duration<float> delta = thisFrame - lastFrame;

    m_pGameManager->m_sessionTime += delta.count();

    float finalDelta = delta.count() * m_pGameManager->m_deltaVel;


    //Update entities
    m_pEntityManager->UpdateEnts_Upd(finalDelta);
    //Delete destroyed entities
    m_pEntityManager->UpdateEnts_Del();
    //Add new entities
    m_pEntityManager->UpdateEnts_Add();


    //Update Colliders
    colliderCounter += finalDelta;
    if (colliderCounter >= colUpdateRate)
    {
        m_pColliderManager->UpdateColliders(colUpdateRate);
        m_pColliderManager->UpdateCollisions();

        colliderCounter = colliderCounter - colUpdateRate;
    }

    static float secondCounter = 0.0f;
    static int fpsCounter = 0;

    //-------------------------------------------------- DEBUG - DEBUG - DEBUG
#if(DEBUG_PRINT_TEXTURE_DATA || DEBUG_PRINT_SPRITE_DATA)
    LogDebugOnConsole(delta.count());
#endif
    lastFrame = thisFrame;

    if (m_pGameManager->m_quitRequest)
    {
        m_quit = true;
        return;
    }

    m_pEventBus->ProcessEvents();
}

//  Updates screen (Renders a frame)
void Engine::Render()
{
    m_pRenderer->DrawSprites(m_pSpriteManager);

#if DEBUG_DRAW_COLLIDERS
        m_pRenderer->DrawObjects(*m_pColliderManager->GetRenderableObjects());
#endif

    m_pRenderer->DrawObjects(*m_postFrameDrawer->GetObjects());
    m_postFrameDrawer->ClearContents();

    m_pRenderer->DrawText(m_pTextManager);

    m_pRenderer->DrawShapes(m_pShapeManager);

    //Render everything you got onto the screen
    m_pRenderer->PresentFrame();
}

//  We dont have levels, so we use this to test things out.
void Engine::LoadSandbox()
{
    //Useful data for positioning and sprite rects
    Rect rect64{ 0,0,64,64 };
    Rect rect128 { 0,0,128,128 };
    Rect rect256{ 0,0,256,256};

    Point posCenter{256, 256};
    Point posBottom{ 256, 512 };
    Point posTop{ 256, 0 };
    Point posRight{ 512, 256 };
    Point posLeft{ 0, 256 };

    Point posTL{ 256 / 2, 256 / 2 };
    Point posT{ 256, 256 / 2 };
    Point posTR{ 256 + 128 , 256 / 2 };
    Point posL{ 256 / 2, 256 };

    Point posR{ 256 + 128 , 256 };
    Point posBL{ 256 / 2, 256 + 128 };
    Point posB{ 256, 256 + 128 };
    Point posBR{ 256 + 128, 256 + 128 };


    //this bind allows me to create a std::function object that binds the function I passed
    //with the object I want to call the function of.
    auto demo_1 = std::bind(&Engine::LoadDemo_1, this);
    auto demo_2 = std::bind(&Engine::LoadDemo_2, this);
    auto demo_3 = std::bind(&Engine::LoadDemo_3, this);
    auto demo_4 = std::bind(&Engine::LoadDemo_4, this);
    auto demo_5 = std::bind(&Engine::LoadDemo_5, this);
    auto demo_6_Instructions = std::bind(&Engine::LoadDemo_6_Instructions, this);
    auto demo_6_GameOver = std::bind(&Engine::LoadDemo_6_GameOver, this);
    auto demo_6_YouWin = std::bind(&Engine::LoadDemo_6_YouWin, this);
    auto demo_7_ShooterTutorial = std::bind(&Engine::LoadDemo_7_ShooterTutorial, this);
    auto demo_DEBUG_DungeonMiniGame = std::bind(&Engine::LoadDemo_DEBUG_DungeonMiniGame, this);
    auto demo_DEBUG_Shields = std::bind(&Engine::LoadDemo_DEBUG_Shields, this);
    auto demo_8 = std::bind(&Engine::LoadDemo_8, this);
    auto demo_9 = std::bind(&Engine::LoadDemo_9_Particles, this);
    auto demo_9_WinScreen = std::bind(&Engine::LoadDemo_9_ParticlesWinScreen, this);
    
    auto demo_final_preLoad = std::bind(&Engine::LoadDemo_Final_Preload, this);
    auto demo_final_mainMenu = std::bind(&Engine::LoadDemo_Final_MainMenu, this);
    auto demo_final_controls = std::bind(&Engine::LoadDemo_Final_Controls, this);
    auto demo_final_pauseMenu = std::bind(&Engine::LoadMenu_Final_Pause, this);
    auto demo_final_saveLoadMenu = std::bind(&Engine::LoadMenu_Final_SaveLoad, this);
    auto demo_final_demos = std::bind(&Engine::LoadMenu_Final_Demos, this);



    //Adding levels to LevelManager so I can load them later on any time I want.
    m_pLevelManager->AddLevel("demo_1", demo_1);
    m_pLevelManager->AddLevel("demo_2", demo_2);
    m_pLevelManager->AddLevel("demo_3", demo_3);
    m_pLevelManager->AddLevel("demo_4", demo_4);
    m_pLevelManager->AddLevel("demo_5", demo_5);
    m_pLevelManager->AddLevel("demo_6_Instructions", demo_6_Instructions);
    m_pLevelManager->AddLevel("demo_6_GameOver", demo_6_GameOver);
    m_pLevelManager->AddLevel("demo_6_YouWin", demo_6_YouWin);
    m_pLevelManager->AddLevel("demo_7_ShooterTutorial", demo_7_ShooterTutorial);
    m_pLevelManager->AddLevel("demo_DEBUG_DungeonMiniGame", demo_DEBUG_DungeonMiniGame);
    m_pLevelManager->AddLevel("demo_DEBUG_Shields", demo_DEBUG_Shields);
    m_pLevelManager->AddLevel("demo_8", demo_8);
    m_pLevelManager->AddLevel("demo_9_lvl1", demo_9);
    m_pLevelManager->AddLevel("demo_9_lvl2", demo_9_WinScreen);

    m_pLevelManager->AddLevel("demo_final_preLoad", demo_final_preLoad);
    m_pLevelManager->AddLevel("demo_final_mainMenu", demo_final_mainMenu);
    m_pLevelManager->AddLevel("demo_final_controls", demo_final_controls);
    m_pLevelManager->AddLevel("demo_final_saveLoadMenu", demo_final_saveLoadMenu);
    m_pLevelManager->AddLevel("demo_final_pauseMenu", demo_final_pauseMenu);
    m_pLevelManager->AddLevel("demo_final_demos", demo_final_demos);


    //Loads requested level
    m_pLevelManager->LoadLevel("demo_final_preLoad");
}

//  DEBUG - Prints an entity and its components in the console.
void Engine::PrintEntity(Entity* ent)
{
    std::cout << ent->m_name << std::endl;

    for (Component* c : ent->m_components)
    {
        std::cout << "  " << typeid(*c).name() << std::endl;
    }
}

Entity* Engine::MakeEnt(const char* name, const char* tx, Point pos, Rect src, float scale, bool centerSprite)
{
    Entity* ent = m_pEntityManager->AddEntity(name);

    EC_Transform* t = ent->AddComponent(new EC_Transform(pos.x, pos.y));
    EC_Sprite* s = ent->AddComponent(new EC_Sprite(tx, src));
     t->SetScale(Point(scale, scale));
     if(centerSprite)
         s->SetPivotToCenter();

    return ent;
}

void Engine::LogDebugOnConsole(float delta)
{
    static float debug = 0.0f;
    if (debug >= 0.016666f)
    {
        system("cls");

#if DEBUG_PRINT_SPRITE_DATA
        m_pSpriteManager->DEBUG_CollecSpritetData();
        std::cout << m_pSpriteManager->DEBUG_spriteData;
#endif
#if DEBUG_PRINT_TEXTURE_DATA
        m_pSpriteManager->DEBUG_CollecTexturetData();
        std::cout << m_pSpriteManager->DEBUG_textureData;
#endif

        debug = debug - 0.016666f;
    }
        debug += delta;
}

void Engine::LoadDemo_1()
{
    /*ABOUT:
    This is the original demo that shows off the animations and the watcher.*/
    Rect rect64     { 0,0,64,64 };
    Rect rect128    { 0,0,128,128 };
    Rect rect256    { 0,0,256,256 };

    Point posCenter { 256, 256 };
    Point posBottom { 256, 512 };
    Point posTop    { 256, 0 };
    Point posRight  { 512, 256 };
    Point posLeft   { 0, 256 };

    Point posTL     { 256 / 2, 256 / 2 };
    Point posT      { 256, 256 / 2 };
    Point posTR     { 256 + 128 , 256 / 2 };
    Point posL      { 256 / 2, 256 };

    Point posR      { 256 + 128 , 256 };
    Point posBL     { 256 / 2, 256 + 128 };
    Point posB      { 256, 256 + 128 };
    Point posBR     { 256 + 128, 256 + 128 };

    //  TILED BACKGROUND ENTITY
    Entity* ent_background = m_pEntityManager->AddEntity("E_Background");
    ent_background->AddComponent(new EC_Transform(0, 0));
    auto sprite1 = ent_background->AddComponent(new EC_Sprite("TX_CheckersTexture.bmp", rect256));
    sprite1->SetTile(true, 4, 4);
    EC_Transform* t1 = ent_background->GetComponent<EC_Transform>();
    t1->SetScale(0.5f, 0.5f);

    // ENTITIES
    Entity* ent64_1 = MakeEnt("E_Jpg_64", "TX_JPG.jpg", Point{ 64 * 7,64 * 6 }, rect256, 0.25f);
    Entity* ent64_2 = MakeEnt("E_Bmp_64", "TX_BMP.bmp", Point{ 64 * 6,64 * 7 }, rect256, 0.25f);
    Entity* ent64_3 = MakeEnt("E_Gif_64", "TX_GIF.gif", Point{ 64 * 6,64 * 6 }, rect256, 0.25f);
    Entity* ent64_4 = MakeEnt("E_Png_64", "TX_PNG.png", Point{ 64 * 7,64 * 7 }, rect256, 0.25f);


    MakeEnt("E_Jpg_128", "TX_JPG.jpg", Point(512 - 128, 0), rect256, 0.5f);
    MakeEnt("E_Bmp_128", "TX_BMP.bmp", Point(0, 512 - 128), rect256, 0.5f);
    MakeEnt("E_Gif_128", "TX_GIF.gif", Point(0, 0), rect256, 0.5f);


    //  PNG ENTITY (Animated Entity ".PNG")
    Entity* ent_png128 = m_pEntityManager->AddEntity("E_Png_128");
    EC_Transform* ent_png128_t = ent_png128->AddComponent(new EC_Transform((512 / 2) - 128 / 2, (512 / 2) - 128 / 2));
    auto sprite5 = ent_png128->AddComponent(new EC_Sprite("SS_PNG.png", rect256));
    sprite5->SetPivotToCenter();
    EC_Animator* anm = ent_png128->AddComponent(new EC_Animator());
    anm->AddClip("IDLE", 6, Rect{ 0,0,256,256 });
    anm->SetSpeed(0.07f);
    anm->SetClip("IDLE");
    anm->Play(true);
    ent_png128_t->SetScale(0.5f, 0.5f);

    //  LOOKER ENTITY
    Entity* ent_looker = m_pEntityManager->AddEntity("looker");
    auto lookerT = ent_looker->AddComponent(new EC_Transform(512 / 2, 512 / 2));
    lookerT->SetScale(0.5f, 0.5f);
    ent_looker->AddComponent(new EC_Sprite());
    ent_looker->AddComponent(new UC_StaticTracker(ent_png128->GetComponent<EC_Transform>()));
    EC_Sprite* lookerSprite = ent_looker->GetComponent<EC_Sprite>();
    lookerSprite->SetPivotToCenter();
    EC_Transform* looker_transform = ent_looker->GetComponent<EC_Transform>();

    //  CURSOR ENTITY
    Entity* ent_cursor = m_pEntityManager->AddEntity("CURSOR");
    ent_cursor->AddComponent(new UC_Cursor(250.0f));

    ent_looker->GetComponent<UC_StaticTracker>()->SetTarget(ent_cursor->GetComponent<EC_Transform>());

    auto lvl1WinConEnt = m_pEntityManager->AddEntity("LVL1_WINCON");
    lvl1WinConEnt->AddComponent(new UC_Demo8_Lvl1WinCon(ent_cursor->GetComponent<EC_Transform>(), Point{0,0}, 30.0f));
}

void Engine::LoadDemo_2()
{

    /*ABOUT:    -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
    * Shows how to manage a vector of pointers to other entities using "Safe References" (special
    * pointers) to entities.
    * 
    * HOW "SAFE REFERENCES" WORK:   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
    * A safe reference is a normal pointer that points to an entity, however, this pointer is
    * automatically set to nullptr whenever the entity it points to starts its deletion process.
    * To get a "Safe Reference" to an entity, you must call its Entity::GetSafeRef(Entity*& holder);
    * function. You provide the address of the Entity* you wish to convert into a "Safe Reference"
    * and the function returns the address of the Entity object to you to assign to your Entity*.
    * 
    * Once you are done with the Safe Reference, you must always call
    * Entity::DestroySafeRef(Entity*& holder) on it, the reason is that each Entity keeps track of
    * each pointer that is pointing to it via the "Safe Reference" system, so calling that
    * DestroySafeRef tells the entity to remove that pointer from its list of referees.
    * 
    * HOW TO PLAY:  -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
    * This demo shows various entities that are connected to one another.
    * These entities can be deleted by using the NUM KEYPAD numbers, there is an entity attached
    * to each number.
    * 
    * OTHER:    -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
    * You can also see a new feature that lets you render lines and rects on top of the rendered
    * frame. Its being used to render the colored rects and lines. The main purpose of this tool
    * was to be used with debugging collision rects but you can use it to create a mouse
    * selection rect (right now we dont have way of reading mouse input but if we had you could)
    */


    Rect rect64     { 0,0,64,64 };
    Rect rect128    { 0,0,128,128 };
    Rect rect256    { 0,0,256,256 };

    Point posCenter { 256, 256 };
    Point posBottom { 256, 512 };
    Point posTop    { 256, 0 };
    Point posRight  { 512, 256 };
    Point posLeft   { 0, 256 };

    Point posTL     { 256 / 2, 256 / 2 };
    Point posT      { 256, 256 / 2 };
    Point posTR     { 256 + 128 , 256 / 2 };
    Point posL      { 256 / 2, 256 };

    Point posR      { 256 + 128 , 256 };
    Point posBL     { 256 / 2, 256 + 128 };
    Point posB      { 256, 256 + 128 };
    Point posBR     { 256 + 128, 256 + 128 };


    //  TILED BACKGROUND ENTITY
    Entity* ent_background = m_pEntityManager->AddEntity("E_Background");
    ent_background->AddComponent(new EC_Transform(0, 0));
    auto sprite1 = ent_background->AddComponent(new EC_Sprite("TX_CheckersTexture.bmp", rect256));
    sprite1->SetTile(true, 4, 4);
    EC_Transform* t1 = ent_background->GetComponent<EC_Transform>();
    t1->SetScale(0.5f, 0.5f);

    //PLATFORM
    Rect pltformRect{0,0,320,320};
    auto platformEnt = m_pEntityManager->AddEntity("PlatformEnt");
    auto platformT = platformEnt->AddComponent(new EC_Transform);
    auto platformS = platformEnt->AddComponent(new EC_Sprite);
    platformS->SetSourceImg("SP_Platform_2.png", pltformRect);
    platformT->SetPos(96, 96);


    Entity* eTL = MakeEnt("ent_Black", "SP_Soldier_S.png", posTL, Rect{ 0,0,70,117 }, 0.25f, true);
    Entity* eT =  MakeEnt("ent_Blue", "SP_Soldier_S.png", posT, Rect{ 0,0,70,117 }, 0.25f, true);
    Entity* eTR = MakeEnt("ent_Green", "SP_Soldier_S.png", posTR, Rect{ 0,0,70,117 }, 0.25f, true);
    Entity* eL =  MakeEnt("ent_Orange", "SP_Soldier_S.png", posL, Rect{ 0,0,70,117 }, 0.25f, true);
    Entity* eC =  MakeEnt("ent_Purple", "SP_Soldier_S.png", posCenter, Rect{ 0,0,70,117 }, 0.25f, true);
    Entity* eR =  MakeEnt("ent_Red", "SP_Soldier_S.png", posR, Rect{ 0,0,70,117 }, 0.25f, true);
    Entity* eBL = MakeEnt("ent_White", "SP_Soldier_S.png", posBL, Rect{ 0,0,70,117 }, 0.25f, true);
    Entity* eB =  MakeEnt("ent_Yellow", "SP_Soldier_S.png", posB, Rect{ 0,0,70,117 }, 0.25f, true);
    Entity* eBR = MakeEnt("ent_DarkBlue-ish", "SP_Soldier_S.png", posBR, Rect{ 0,0,70,117 }, 0.25f, true);

    UC_Tester* testerTL = eTL->AddComponent<UC_Tester>(new UC_Tester(KeyCode::k_Keypad_7, Color::PreDefined::k_Black));
    UC_Tester* testerT = eT->AddComponent<UC_Tester>(new UC_Tester(KeyCode::k_Keypad_8, Color::PreDefined::k_Blue));
    UC_Tester* testerTR = eTR->AddComponent<UC_Tester>(new UC_Tester(KeyCode::k_Keypad_9, Color::PreDefined::k_Green));
    UC_Tester* testerL = eL->AddComponent<UC_Tester>(new UC_Tester(KeyCode::k_Keypad_4, Color::PreDefined::k_Orange));
    UC_Tester* testerC = eC->AddComponent<UC_Tester>(new UC_Tester(KeyCode::k_Keypad_5, Color::PreDefined::k_Purple));
    UC_Tester* testerR = eR->AddComponent<UC_Tester>(new UC_Tester(KeyCode::k_Keypad_6, Color::PreDefined::k_Red));
    UC_Tester* testerBL = eBL->AddComponent<UC_Tester>(new UC_Tester(KeyCode::k_Keypad_1, Color::PreDefined::k_White));
    UC_Tester* testerB = eB->AddComponent<UC_Tester>(new UC_Tester(KeyCode::k_Keypad_2, Color::PreDefined::k_Yellow));
    UC_Tester* testerBR = eBR->AddComponent<UC_Tester>(new UC_Tester(KeyCode::k_Keypad_3, Color(5, 85, 105, 255)));

    testerTL->AddTesterToVector(testerBL);
    testerT->AddTesterToVector(testerC);
    testerT->AddTesterToVector(testerTL);
    testerTR->AddTesterToVector(testerT);
    testerTR->AddTesterToVector(testerL);
    testerC->AddTesterToVector(testerT);
    testerC->AddTesterToVector(testerTR);
    testerC->AddTesterToVector(testerL);
    testerC->AddTesterToVector(testerR);
    testerC->AddTesterToVector(testerB);
    testerC->AddTesterToVector(testerBL);
    testerR->AddTesterToVector(testerBR);
    testerBL->AddTesterToVector(testerL);
    testerBL->AddTesterToVector(testerT);
    testerBL->AddTesterToVector(testerBR);
    testerB->AddTesterToVector(testerBL);
    testerB->AddTesterToVector(testerTR);
    testerBR->AddTesterToVector(testerB);
    testerBR->AddTesterToVector(testerC);
    testerBR->AddTesterToVector(testerR);
    testerR->AddTesterToVector(testerC);
    testerB->AddTesterToVector(testerC);
    testerTR->AddTesterToVector(testerC);
    testerL->AddTesterToVector(testerBL);

    eTL->AddComponent(new UC_DummyComp());
    eT->AddComponent(new UC_DummyComp());
    eTR->AddComponent(new UC_DummyComp());
    eL->AddComponent(new UC_DummyComp());
    eC->AddComponent(new UC_DummyComp());
    eR->AddComponent(new UC_DummyComp());
    eBL->AddComponent(new UC_DummyComp());
    eB->AddComponent(new UC_DummyComp());
    eBR->AddComponent(new UC_DummyComp());

    auto winCon = m_pEntityManager->AddEntity("LVL2_WINCON");
    auto winConComp = winCon->AddComponent(new UC_Demo8_Lvl2WinCon);
    
    winConComp->AddTester(testerTL);
    winConComp->AddTester(testerT);
    winConComp->AddTester(testerTR);
    winConComp->AddTester(testerL);
    winConComp->AddTester(testerC);
    winConComp->AddTester(testerR);
    winConComp->AddTester(testerBL);
    winConComp->AddTester(testerB);
    winConComp->AddTester(testerBR);
}

void Engine::LoadDemo_3()
{
    //Testing layer ordering and the ability to decide where in the layer you want to be placed
    //(last or first)
    Rect srcRect{0,0,70,117};
    Point startPos{ 15,15 };
    int xOffset = 35;

    for (int i = 0; i < 10; ++i)
    {
        auto ent = m_pEntityManager->AddEntity("instance");
        auto entT = ent->AddComponent(new EC_Transform);
        auto entS = ent->AddComponent(new EC_Sprite(2U));

        entS->SetSourceImg("SP_Soldier_S.png", srcRect);
        entS->SetLayer(2U, false);
        entT->SetPos(startPos.x + (xOffset * i), startPos.y);
    }

}

void Engine::LoadDemo_4()
{
    /*ABOUT:
    * MID TERM GAME: DAREDUNGEON
    */
    Rect rect64     { 0,0,64,64 };
    Rect rect128    { 0,0,128,128 };
    Rect rect256    { 0,0,256,256 };

    Point posCenter { 256, 256 };
    Point posBottom { 256, 512 };
    Point posTop    { 256, 0 };
    Point posRight  { 512, 256 };
    Point posLeft   { 0, 256 };

    Point posTL     { 256 / 2, 256 / 2 };
    Point posT      { 256, 256 / 2 };
    Point posTR     { 256 + 128 , 256 / 2 };
    Point posL      { 256 / 2, 256 };

    Point posR      { 256 + 128 , 256 };
    Point posBL     { 256 / 2, 256 + 128 };
    Point posB      { 256, 256 + 128 };
    Point posBR     { 256 + 128, 256 + 128 };

    //FLOOR
    auto floor = m_pEntityManager->AddEntity("Floor");
    auto floorT = floor->AddComponent(new EC_Transform);
    auto floorS = floor->AddComponent(new EC_Sprite);
    Rect floorRect(16*1, 16*2, 16, 16);
    floorS->SetSourceImg("TX_Tile.png", floorRect);
    floorS->SetTile(true, 512 / 16, 512 / 16);

    //OUTTER WALLS
    auto wallTop = m_pEntityManager->AddEntity("Wall");
    wallTop->AddComponent(new UC_Demo4_Wall(Rect(0,16*-1,32,1)));
    auto wallBot = m_pEntityManager->AddEntity("Wall");
    wallBot->AddComponent(new UC_Demo4_Wall(Rect(0, 16*32, 32, 1)));
    auto wallRight = m_pEntityManager->AddEntity("Wall");
    wallRight->AddComponent(new UC_Demo4_Wall(Rect(16*32, 0, 1, 32)));
    auto wallLeft = m_pEntityManager->AddEntity("Wall");
    wallLeft->AddComponent(new UC_Demo4_Wall(Rect(16*-1, 0, 1, 32)));

    //INNER WALLS
    auto innerWall_1 = m_pEntityManager->AddEntity("Wall");
    innerWall_1->AddComponent(new UC_Demo4_Wall(Rect(16 * 3, 16 * 3, 1, 2)));

    auto innerWall_2 = m_pEntityManager->AddEntity("Wall");
    innerWall_2->AddComponent(new UC_Demo4_Wall(Rect(16 * 0, 16 * 9, 8, 2)));

    auto innerWall_3 = m_pEntityManager->AddEntity("Wall");
    innerWall_3->AddComponent(new UC_Demo4_Wall(Rect(16 * 7, 16 * 3, 1, 6)));

    auto innerWall_4 = m_pEntityManager->AddEntity("Wall");
    innerWall_4->AddComponent(new UC_Demo4_Wall(Rect(16 * 5, 16 * 15, 2, 17)));

    auto innerWall_5 = m_pEntityManager->AddEntity("Wall");
    innerWall_5->AddComponent(new UC_Demo4_Wall(Rect(16 * 11, 16 * 0, 1, 16 )));

    auto innerWall_6 = m_pEntityManager->AddEntity("Wall");
    innerWall_6->AddComponent(new UC_Demo4_Wall(Rect(16 * 17, 16 * 0, 2, 3)));

    auto innerWall_7 = m_pEntityManager->AddEntity("Wall");
    innerWall_7->AddComponent(new UC_Demo4_Wall(Rect(16 * 17, 16 * 9, 2, 15)));

    auto innerWall_8 = m_pEntityManager->AddEntity("Wall");
    innerWall_8->AddComponent(new UC_Demo4_Wall(Rect(16 * 19, 16 * 19, 13, 2)));

    auto innerWall_9 = m_pEntityManager->AddEntity("Wall");
    innerWall_9->AddComponent(new UC_Demo4_Wall(Rect(16 * 23, 16 * 25, 2, 7)));

    auto innerWall_10 = m_pEntityManager->AddEntity("Wall");
    innerWall_10->AddComponent(new UC_Demo4_Wall(Rect(16 * 28, 16 * 25, 1, 4)));

    auto innerWall_11 = m_pEntityManager->AddEntity("Wall");
    innerWall_11->AddComponent(new UC_Demo4_Wall(Rect(16 * 22, 16 * 0, 1, 16)));

    auto innerWall_12 = m_pEntityManager->AddEntity("Wall");
    innerWall_12->AddComponent(new UC_Demo4_Wall(Rect(16 * 23, 16 * 0, 3, 4)));

    auto innerWall_13 = m_pEntityManager->AddEntity("Wall");
    innerWall_13->AddComponent(new UC_Demo4_Wall(Rect(16 * 29, 16 * 0, 3, 4)));

    auto innerWall_14 = m_pEntityManager->AddEntity("Wall");
    innerWall_14->AddComponent(new UC_Demo4_Wall(Rect(16 * 23, 16 * 7, 3, 3)));

    auto innerWall_15 = m_pEntityManager->AddEntity("Wall");
    innerWall_15->AddComponent(new UC_Demo4_Wall(Rect(16 * 29, 16 * 7, 3, 3)));

    auto innerWall_16 = m_pEntityManager->AddEntity("Wall");
    innerWall_16->AddComponent(new UC_Demo4_Wall(Rect(16 * 23, 16 * 13, 3, 3)));

    auto innerWall_17 = m_pEntityManager->AddEntity("Wall");
    innerWall_17->AddComponent(new UC_Demo4_Wall(Rect(16 * 29, 16 * 13, 3, 3)));

    //IMMORTAL ENEMIES
    auto e1 = m_pEntityManager->AddEntity("Skeleton");
    auto e1C = e1->AddComponent(new UC_Demo4_Enemy(40.0f, Point(256, 256), true));
    e1C->m_currDir = Point(1, 0);
    auto e1T = e1->GetComponent<EC_Transform>();
    e1T->SetPos(16*24, 16*5);

    auto e2 = m_pEntityManager->AddEntity("Skeleton");
    auto e2C = e2->AddComponent(new UC_Demo4_Enemy(40.0f, Point(256, 256), true));
    e2C->m_currDir = Point(1, 0);
    auto e2T = e2->GetComponent<EC_Transform>();
    e2T->SetPos(16 * 26, 16 * 11);

    //DOOR
    auto door = m_pEntityManager->AddEntity("Door");
    auto doorT = door->AddComponent(new EC_Transform);
    doorT->SetPos(16 * 19, 16 * 9);
    doorT->SetScale(3, 3);
    auto doorS = door->AddComponent(new EC_Sprite);
    Rect doorSrcRect(0,0,16,16);
    doorS->SetSourceImg("TX_Door.png", doorSrcRect);
    auto doorC = door->AddComponent(new EC_Collider);
    doorC->AttachToSprite(true);

    //KEYS
    auto key1 = m_pEntityManager->AddEntity("Key");
    key1->AddComponent(new UC_Demo4_Key(16.0f * 1.5f, 16.0f * 7.0f));

    auto key2 = m_pEntityManager->AddEntity("Key");
    key2->AddComponent(new UC_Demo4_Key(16*29, 16*30));

    //ENEMY SPAWNER
    auto enemySpawner = m_pEntityManager->AddEntity("EnemySpawner");
    enemySpawner->AddComponent(new UC_Demo4_EnemySpawner(5.0f));

    //AREAS
    auto healingAreaEnt = m_pEntityManager->AddEntity("HealingArea");
    healingAreaEnt->AddComponent(new UC_Demo4_HealthMod(Point(128,128), 1, false));
    auto healingAreaTransform = healingAreaEnt->GetComponent<EC_Transform>();
    healingAreaTransform->SetPos(16 * 29, 16 * 21);
    healingAreaTransform->SetScale(0.25 * 3, 0.25 * 4);
    
    auto damageAreaEnt = m_pEntityManager->AddEntity("DamageArea");
    damageAreaEnt->AddComponent(new UC_Demo4_HealthMod(Point(1, 1), -1, false));
    auto dmgAreaTransform = damageAreaEnt->GetComponent<EC_Transform>();
    dmgAreaTransform->SetPos(16*15, 16*9);
    dmgAreaTransform->SetScale(0.25*2, 0.25*6);

    auto damageAreaEnt2 = m_pEntityManager->AddEntity("DamageArea");
    damageAreaEnt2->AddComponent(new UC_Demo4_HealthMod(Point(1, 1), -1, false));
    auto dmgAreaTransform2 = damageAreaEnt2->GetComponent<EC_Transform>();
    dmgAreaTransform2->SetPos(16 * 26, 16 * 07);
    dmgAreaTransform2->SetScale(0.25 * 3, 0.25 * 3);

    auto damageAreaEnt3 = m_pEntityManager->AddEntity("DamageArea");
    damageAreaEnt3->AddComponent(new UC_Demo4_HealthMod(Point(1, 1), -1, false));
    auto dmgAreaTransform3 = damageAreaEnt3->GetComponent<EC_Transform>();
    dmgAreaTransform3->SetPos(16 * 26, 16 * 13);
    dmgAreaTransform3->SetScale(0.25 * 3, 0.25 * 3);
    
    auto winAreaEnt = m_pEntityManager->AddEntity("WinArea");
    winAreaEnt->AddComponent(new UC_Demo4_HealthMod(Point(128, 128), 0, true));
    auto winAreaTransform = winAreaEnt->GetComponent<EC_Transform>();
    winAreaTransform->SetPos(16 * 26, 16 * 0);
    winAreaTransform->SetScale(0.25 * 3, 0.25 * 1);

    //PLAYER
    auto playerEnt = m_pEntityManager->AddEntity("Player");
    auto playerC = playerEnt->AddComponent(new UC_Demo4_Player(Point(16*2, 16*29)));


    for (int i = 0; i < 3; ++i)
    {
        auto ghostEnt = m_pEntityManager->AddEntity("Skeleton");
        auto ghostComp = ghostEnt->AddComponent(new UC_Demo4_Ghost(60));
    }

    //LIGHTZONE
    auto lightZone = m_pEntityManager->AddEntity("LightZone");
    auto lightZoneComp = lightZone->AddComponent(new UC_Demo4_LightZone(playerEnt));
}

void Engine::LoadDemo_5()
{
    Rect rect{0,0,16,16};
    Point tracingDir = {1.0f, -2.f};

    Quad2D q1;
    q1.p[0] = { 20, 40 };
    q1.p[1] = { 30, 20 };
    q1.p[2] = { 60, 30 };
    q1.p[3] = { 70, 70 };

    Quad2D q2;
    q2.p[0] = { -10, -10 };
    q2.p[1] = { 10, -10 };
    q2.p[2] = { 15, 15 };
    q2.p[3] = { -10, 10 };


    auto shapeEnt = m_pEntityManager->AddEntity("ShapeEnt");
    auto shapeTransform = shapeEnt->AddComponent(new EC_Transform);
    shapeTransform->SetScale(2.0f, 2.0f);
    shapeTransform->SetPos(0, 0);
    auto shape = shapeEnt->AddComponent(new EC_Shape);
    shape->AddQuad(q1);


    auto shapeEnt2 = m_pEntityManager->AddEntity("ShapeEnt");
    auto shapeTransform2 = shapeEnt2->AddComponent(new EC_Transform);
    shapeTransform2->SetPos(256, 256);
    shapeTransform2->SetScale(2, 2);
    auto shape2 = shapeEnt2->AddComponent(new EC_Shape);
    shape2->AddQuad(q2);
}

void Engine::LoadDemo_6_Instructions()
{
    auto gameManager = m_pEntityManager->AddEntity("GameManager");
    gameManager->AddComponent(new UC_LevelLoader);
}

void Engine::LoadDemo_6_GameOver()
{

    Rect bgRect{ 0,0,512,512 };
    m_pGameManager->ShowCursor(true);


    //Background
    auto bgEnt = m_pEntityManager->AddEntity("PlatformEnt");
    auto btT = bgEnt->AddComponent(new EC_Transform);
    auto bgS = bgEnt->AddComponent(new EC_Sprite);
    bgS->SetSourceImg("SP_DefaultBG.png", bgRect);

    //Texts
    auto titleEnt = m_pEntityManager->AddEntity("titleEnt");
    auto titleText = titleEnt->AddComponent(new EC_Text);
    auto titleTransform = titleEnt->GetComponent<EC_Transform>();

    titleText->SetFont("Font/arial.ttf", 24);
    titleText->SetColor(Color::PreDefined::k_Black);
    titleText->SetText("Game Over");
    titleTransform->SetPos(192, 50);


    //***********
    Point mainMenuPos(256-32, 256);
    Point replayPos(256-32, 256 + 70);
    Point quitPos(256-32, 256 + 140);

    Rect rect(0, 0, 64, 64);

    auto mainMenuEnt = m_pEntityManager->AddEntity("SenderEnt");
    auto mainMenuBtn = mainMenuEnt->AddComponent(new EC_Button);

    auto mainMenuListener = m_pEntityManager->AddEntity("LevelListener");
    auto mainMenuListenerComp = mainMenuListener->AddComponent(new UC_LoadListener("demo_final_mainMenu", dynamic_cast<IEventSender*>(mainMenuBtn)));
    mainMenuBtn->SetPos(mainMenuPos);
    mainMenuBtn->SetPressImg("SP_Button.png", rect);
    auto mainMenuBtnText = mainMenuEnt->GetComponent<EC_Text>();
    mainMenuBtnText->SetText("Main menu");
    mainMenuBtnText->SetColor(Color::PreDefined::k_Blue);
    mainMenuBtnText->SetFont("Fonts/arial.ttf", 12);
    //---------------
    auto replayEnt = m_pEntityManager->AddEntity("SenderEnt");
    auto replayBtn = replayEnt->AddComponent(new EC_Button);

    auto replayListener = m_pEntityManager->AddEntity("LevelListener");
    auto replayListenerComp = replayListener->AddComponent(new UC_LoadListener("demo_4", dynamic_cast<IEventSender*>(replayBtn)));
    replayBtn->SetPos(replayPos);
    replayBtn->SetPressImg("SP_Button.png", rect);
    auto replayBtnText = replayEnt->GetComponent<EC_Text>();
    replayBtnText->SetText("Replay");
    replayBtnText->SetColor(Color::PreDefined::k_Blue);
    replayBtnText->SetFont("Fonts/arial.ttf", 12);
    //------------------
    auto quitEnt = m_pEntityManager->AddEntity("SenderEnt");
    auto quitBtn = quitEnt->AddComponent(new EC_Button);

    auto quitListener = m_pEntityManager->AddEntity("LevelListener");
    auto quitListenerComp = quitListener->AddComponent(new UC_LoadListener("demo_1", dynamic_cast<IEventSender*>(quitBtn), true));
    quitBtn->SetPos(quitPos);
    quitBtn->SetPressImg("SP_Button.png", rect);
    auto quitBtnText = quitEnt->GetComponent<EC_Text>();
    quitBtnText->SetText("Quit");
    quitBtnText->SetColor(Color::PreDefined::k_Blue);
    quitBtnText->SetFont("Fonts/arial.ttf", 12);


    auto creditsEnt = m_pEntityManager->AddEntity("CreditsEnt");
    auto creditsText = creditsEnt->AddComponent(new EC_Text);
    creditsText->SetText("Game by: Alfredo Rodriguez");
    creditsText->SetColor(Color::PreDefined::k_Black);
    creditsText->SetFont("Fonts/arial.ttf", 12);
    auto creditsTransform = creditsEnt->GetComponent<EC_Transform>();
    creditsTransform->SetPos(170, 470);
}

void Engine::LoadDemo_6_YouWin()
{
    Rect bgRect{ 0,0,512,512 };
    m_pGameManager->ShowCursor(true);


    //Background
    auto bgEnt = m_pEntityManager->AddEntity("PlatformEnt");
    auto btT = bgEnt->AddComponent(new EC_Transform);
    auto bgS = bgEnt->AddComponent(new EC_Sprite);
    bgS->SetSourceImg("SP_DefaultBG.png", bgRect);

    //Texts
    auto titleEnt = m_pEntityManager->AddEntity("titleEnt");
    auto titleText = titleEnt->AddComponent(new EC_Text);
    auto titleTransform = titleEnt->GetComponent<EC_Transform>();

    titleText->SetFont("Fonts/arial.ttf", 24);
    titleText->SetColor(Color::PreDefined::k_Black);
    titleText->SetText("Demos completed !");
    titleTransform->SetPos(150, 50);

    //*******************
    Point mainMenuPos(256-32, 256);
    Point replayPos(256-32, 256 + 70);

    Rect rect(0, 0, 64, 64);

    auto mainMenuEnt = m_pEntityManager->AddEntity("SenderEnt");
    auto mainMenuBtn = mainMenuEnt->AddComponent(new EC_Button);

    auto mainMenuListener = m_pEntityManager->AddEntity("LevelListener");
    auto mainMenuListenerComp = mainMenuListener->AddComponent(new UC_LoadListener("demo_final_mainMenu", dynamic_cast<IEventSender*>(mainMenuBtn)));
    mainMenuBtn->SetPos(mainMenuPos);
    mainMenuBtn->SetPressImg("SP_Button.png", rect);
    auto mainMenuBtnText = mainMenuEnt->GetComponent<EC_Text>();
    mainMenuBtnText->SetText("Main menu");
    mainMenuBtnText->SetColor(Color::PreDefined::k_Blue);
    mainMenuBtnText->SetFont("Fonts/arial.ttf", 12);
    //---------------
   
    //------------------
    auto quitEnt = m_pEntityManager->AddEntity("SenderEnt");
    auto quitBtn = quitEnt->AddComponent(new EC_Button);

    auto quitListener = m_pEntityManager->AddEntity("LevelListener");
    auto quitListenerComp = quitListener->AddComponent(new UC_LoadListener("demo_1", dynamic_cast<IEventSender*>(quitBtn), true));
    quitBtn->SetPos(replayPos);
    quitBtn->SetPressImg("SP_Button.png", rect);
    auto quitBtnText = quitEnt->GetComponent<EC_Text>();
    quitBtnText->SetText("Quit");
    quitBtnText->SetColor(Color::PreDefined::k_Blue);
    quitBtnText->SetFont("Fonts/arial.ttf", 12);

    auto creditsEnt = m_pEntityManager->AddEntity("CreditsEnt");
    auto creditsText = creditsEnt->AddComponent(new EC_Text);
    creditsText->SetText("Game by: Alfredo Rodriguez");
    creditsText->SetColor(Color::PreDefined::k_Black);
    creditsText->SetFont("Fonts/arial.ttf", 12);
    auto creditsTransform = creditsEnt->GetComponent<EC_Transform>();
    creditsTransform->SetPos(170, 470);
}

void Engine::LoadDemo_7_ShooterTutorial()
{
    //  TILED BACKGROUND ENTITY
    Rect rect256{0,0,256,256};
    Entity* ent_background = m_pEntityManager->AddEntity("E_Background");
    ent_background->AddComponent(new EC_Transform(0, 0));
    auto sprite1 = ent_background->AddComponent(new EC_Sprite("TX_CheckersTexture_Grayscale2.png", rect256));
    sprite1->SetTile(true, 16, 16);
    EC_Transform* t1 = ent_background->GetComponent<EC_Transform>();
    t1->SetScale(0.25f/2.0f, 0.25f/2.0f);

    auto shooterManager = m_pEntityManager->AddEntity("SHOOTER_MANAGER_ENT");
    auto shooterManagerComp = shooterManager->AddComponent(new UC_ShooterGameManager);

    auto bossEnt = m_pEntityManager->AddEntity("ENEMY_BOSS_ENT");
    bossEnt->AddComponent(new UC_Demo7_Boss);

    auto player = m_pEntityManager->AddEntity("PLAYER_ENT");
    player->AddComponent(new UC_Demo7_Player);
}


void Engine::LoadDemo_DEBUG_DungeonMiniGame()
{
    /*ABOUT:
    * MID TERM GAME: DAREDUNGEON
    */
    Rect rect64{ 0,0,64,64 };
    Rect rect128{ 0,0,128,128 };
    Rect rect256{ 0,0,256,256 };

    Point posCenter{ 256, 256 };
    Point posBottom{ 256, 512 };
    Point posTop{ 256, 0 };
    Point posRight{ 512, 256 };
    Point posLeft{ 0, 256 };

    Point posTL{ 256 / 2, 256 / 2 };
    Point posT{ 256, 256 / 2 };
    Point posTR{ 256 + 128 , 256 / 2 };
    Point posL{ 256 / 2, 256 };

    Point posR{ 256 + 128 , 256 };
    Point posBL{ 256 / 2, 256 + 128 };
    Point posB{ 256, 256 + 128 };
    Point posBR{ 256 + 128, 256 + 128 };

    //FLOOR
    auto floor = m_pEntityManager->AddEntity("Floor");
    auto floorT = floor->AddComponent(new EC_Transform);
    auto floorS = floor->AddComponent(new EC_Sprite);
    Rect floorRect(16 * 1, 16 * 2, 16, 16);
    floorS->SetSourceImg("TX_Tile.png", floorRect);
    floorS->SetTile(true, 512 / 16, 512 / 16);

    //OUTTER WALLS
    auto wallTop = m_pEntityManager->AddEntity("Wall");
    wallTop->AddComponent(new UC_Demo4_Wall(Rect(0, 16 * -1, 32, 1)));
    auto wallBot = m_pEntityManager->AddEntity("Wall");
    wallBot->AddComponent(new UC_Demo4_Wall(Rect(0, 16 * 32, 32, 1)));
    auto wallRight = m_pEntityManager->AddEntity("Wall");
    wallRight->AddComponent(new UC_Demo4_Wall(Rect(16 * 32, 0, 1, 32)));
    auto wallLeft = m_pEntityManager->AddEntity("Wall");
    wallLeft->AddComponent(new UC_Demo4_Wall(Rect(16 * -1, 0, 1, 32)));

    //INNER WALLS
    auto innerWall_1 = m_pEntityManager->AddEntity("Wall");
    innerWall_1->AddComponent(new UC_Demo4_Wall(Rect(16 * 3, 16 * 3, 1, 2)));

    auto innerWall_2 = m_pEntityManager->AddEntity("Wall");
    innerWall_2->AddComponent(new UC_Demo4_Wall(Rect(16 * 0, 16 * 9, 8, 2)));

    auto innerWall_3 = m_pEntityManager->AddEntity("Wall");
    innerWall_3->AddComponent(new UC_Demo4_Wall(Rect(16 * 7, 16 * 3, 1, 6)));

    auto innerWall_4 = m_pEntityManager->AddEntity("Wall");
    innerWall_4->AddComponent(new UC_Demo4_Wall(Rect(16 * 5, 16 * 15, 2, 17)));

    auto innerWall_5 = m_pEntityManager->AddEntity("Wall");
    innerWall_5->AddComponent(new UC_Demo4_Wall(Rect(16 * 11, 16 * 0, 1, 16)));

    auto innerWall_6 = m_pEntityManager->AddEntity("Wall");
    innerWall_6->AddComponent(new UC_Demo4_Wall(Rect(16 * 17, 16 * 0, 2, 3)));

    auto innerWall_7 = m_pEntityManager->AddEntity("Wall");
    innerWall_7->AddComponent(new UC_Demo4_Wall(Rect(16 * 17, 16 * 9, 2, 15)));

    auto innerWall_8 = m_pEntityManager->AddEntity("Wall");
    innerWall_8->AddComponent(new UC_Demo4_Wall(Rect(16 * 19, 16 * 19, 13, 2)));

    auto innerWall_9 = m_pEntityManager->AddEntity("Wall");
    innerWall_9->AddComponent(new UC_Demo4_Wall(Rect(16 * 23, 16 * 25, 2, 7)));

    auto innerWall_10 = m_pEntityManager->AddEntity("Wall");
    innerWall_10->AddComponent(new UC_Demo4_Wall(Rect(16 * 28, 16 * 25, 1, 4)));

    auto innerWall_11 = m_pEntityManager->AddEntity("Wall");
    innerWall_11->AddComponent(new UC_Demo4_Wall(Rect(16 * 22, 16 * 0, 1, 16)));

    auto innerWall_12 = m_pEntityManager->AddEntity("Wall");
    innerWall_12->AddComponent(new UC_Demo4_Wall(Rect(16 * 23, 16 * 0, 3, 4)));

    auto innerWall_13 = m_pEntityManager->AddEntity("Wall");
    innerWall_13->AddComponent(new UC_Demo4_Wall(Rect(16 * 29, 16 * 0, 3, 4)));

    auto innerWall_14 = m_pEntityManager->AddEntity("Wall");
    innerWall_14->AddComponent(new UC_Demo4_Wall(Rect(16 * 23, 16 * 7, 3, 3)));

    auto innerWall_15 = m_pEntityManager->AddEntity("Wall");
    innerWall_15->AddComponent(new UC_Demo4_Wall(Rect(16 * 29, 16 * 7, 3, 3)));

    auto innerWall_16 = m_pEntityManager->AddEntity("Wall");
    innerWall_16->AddComponent(new UC_Demo4_Wall(Rect(16 * 23, 16 * 13, 3, 3)));

    auto innerWall_17 = m_pEntityManager->AddEntity("Wall");
    innerWall_17->AddComponent(new UC_Demo4_Wall(Rect(16 * 29, 16 * 13, 3, 3)));

    //IMMORTAL ENEMIES
    auto e1 = m_pEntityManager->AddEntity("Skeleton");
    auto e1C = e1->AddComponent(new UC_Demo4_Enemy(40.0f, Point(256, 256), true));
    e1C->m_currDir = Point(1, 0);
    auto e1T = e1->GetComponent<EC_Transform>();
    e1T->SetPos(16 * 24, 16 * 5);

    auto e2 = m_pEntityManager->AddEntity("Skeleton");
    auto e2C = e2->AddComponent(new UC_Demo4_Enemy(40.0f, Point(256, 256), true));
    e2C->m_currDir = Point(1, 0);
    auto e2T = e2->GetComponent<EC_Transform>();
    e2T->SetPos(16 * 26, 16 * 11);

    //DOOR
    auto door = m_pEntityManager->AddEntity("Door");
    auto doorT = door->AddComponent(new EC_Transform);
    doorT->SetPos(16 * 19, 16 * 9);
    doorT->SetScale(3, 3);
    auto doorS = door->AddComponent(new EC_Sprite);
    Rect doorSrcRect(0, 0, 16, 16);
    doorS->SetSourceImg("TX_Door.png", doorSrcRect);
    auto doorC = door->AddComponent(new EC_Collider);
    doorC->AttachToSprite(true);

    //KEYS
    auto key1 = m_pEntityManager->AddEntity("Key");
    key1->AddComponent(new UC_Demo4_Key(16.0f * 1.5f, 16.0f * 7.0f));

    auto key2 = m_pEntityManager->AddEntity("Key");
    key2->AddComponent(new UC_Demo4_Key(16 * 29, 16 * 30));

    //ENEMY SPAWNER
    auto enemySpawner = m_pEntityManager->AddEntity("EnemySpawner");
    enemySpawner->AddComponent(new UC_Demo4_EnemySpawner(5.0f));


    //AREAS
    auto healingAreaEnt = m_pEntityManager->AddEntity("HealingArea");
    healingAreaEnt->AddComponent(new UC_Demo4_HealthMod(Point(128, 128), 1, false));
    auto healingAreaTransform = healingAreaEnt->GetComponent<EC_Transform>();
    healingAreaTransform->SetPos(16 * 29, 16 * 21);
    healingAreaTransform->SetScale(0.25 * 3, 0.25 * 4);

    auto damageAreaEnt = m_pEntityManager->AddEntity("DamageArea");
    damageAreaEnt->AddComponent(new UC_Demo4_HealthMod(Point(1, 1), -1, false));
    auto dmgAreaTransform = damageAreaEnt->GetComponent<EC_Transform>();
    dmgAreaTransform->SetPos(16 * 15, 16 * 9);
    dmgAreaTransform->SetScale(0.25 * 2, 0.25 * 6);

    auto damageAreaEnt2 = m_pEntityManager->AddEntity("DamageArea");
    damageAreaEnt2->AddComponent(new UC_Demo4_HealthMod(Point(1, 1), -1, false));
    auto dmgAreaTransform2 = damageAreaEnt2->GetComponent<EC_Transform>();
    dmgAreaTransform2->SetPos(16 * 26, 16 * 07);
    dmgAreaTransform2->SetScale(0.25 * 3, 0.25 * 3);

    auto damageAreaEnt3 = m_pEntityManager->AddEntity("DamageArea");
    damageAreaEnt3->AddComponent(new UC_Demo4_HealthMod(Point(1, 1), -1, false));
    auto dmgAreaTransform3 = damageAreaEnt3->GetComponent<EC_Transform>();
    dmgAreaTransform3->SetPos(16 * 26, 16 * 13);
    dmgAreaTransform3->SetScale(0.25 * 3, 0.25 * 3);

    auto winAreaEnt = m_pEntityManager->AddEntity("WinArea");
    winAreaEnt->AddComponent(new UC_Demo4_HealthMod(Point(128, 128), 0, true));
    auto winAreaTransform = winAreaEnt->GetComponent<EC_Transform>();
    winAreaTransform->SetPos(16 * 26, 16 * 0);
    winAreaTransform->SetScale(0.25 * 3, 0.25 * 1);

    //PLAYER
    auto playerEnt = m_pEntityManager->AddEntity("Player");
    auto playerC = playerEnt->AddComponent(new UC_Demo4_Player(Point(16 * 2, 16 * 29)));
    playerC->SetNewHP(100);


    //DEBUG----------------------------------------------------
    bool addColToGhost = true;
    //DEBUG----------------------------------------------------

    for (int i = 0; i < 90; ++i)
    {
        auto ghostEnt = m_pEntityManager->AddEntity("Skeleton");
        auto ghostComp = ghostEnt->AddComponent(new UC_Demo4_Ghost(60, addColToGhost));
    }
}

void Engine::LoadDemo_DEBUG_Shields()
{
    auto ent = m_pEntityManager->AddEntity("SHIELDSPAWNER_ENT");
    ent->AddComponent(new UC_Debug_ShieldPieceSpawner);

    auto player = m_pEntityManager->AddEntity("PLAYER_ENT");
    player->AddComponent(new UC_Demo7_Player);
}

void Engine::LoadDemo_8()
{
    auto saveManager = m_pEntityManager->AddEntity("Demo8_SaveManager");
    saveManager->AddComponent(new UC_Demo8_SaveManager);

    m_pLevelManager->QueueLoadLevel("demo_1");
}

void Engine::LoadDemo_9_Particles()
{
    auto player = m_pEntityManager->AddEntity("PLAYER_ENT");
    auto flamethrower = player->AddComponent(new UC_Demo9_Flamethrower);


    for (int y = 0; y < 512 / (64 * 0.25f); ++y)
    {
        for (int x = 0; x < 512 / (64 * 0.25f); ++x)
        {
            if ((32 + 64 * x) * 0.25f > 350 && (32 + 64 * y) * 0.25f > 350)
                continue;

            auto obj = m_pEntityManager->AddEntity("HiddenBoxEnt");
            obj->AddComponent(new UC_Demo9_FlamableObject(flamethrower, Point{(32 + 64*x)*0.25f,(32 + 64*y)*0.25f}));
            auto transform = obj->GetComponent<EC_Transform>();
            transform->SetScale(0.25f, 0.25f);
        }
    }
}

void Engine::LoadDemo_9_ParticlesWinScreen()
{
    //  TILED BACKGROUND ENTITY
    Rect rect256{ 0,0,256,256 };
    Entity* ent_background = m_pEntityManager->AddEntity("E_Background");
    ent_background->AddComponent(new EC_Transform(0, 0));
    auto sprite1 = ent_background->AddComponent(new EC_Sprite("TX_CheckersTexture_Grayscale2.png", rect256));
    sprite1->SetTile(true, 16, 16);
    sprite1->SetLayer(0U, false);
    EC_Transform* t1 = ent_background->GetComponent<EC_Transform>();
    t1->SetScale(0.25f / 2.0f, 0.25f / 2.0f);



    auto spawner_1 = m_pEntityManager->AddEntity("ParticleSpawner");
    auto spawnerComp_1 = spawner_1->AddComponent(new EC_ParticleSpawner(EC_ParticleSpawner::SpawnerType::k_Linear));
    auto transform_1 = spawner_1->GetComponent<EC_Transform>();
    transform_1->SetPos(512, 512);
    
    std::vector<std::string> imgs_1;
    imgs_1.push_back("SP_Ghost.png");
    imgs_1.push_back("SP_Soldier_S.png");
    
    std::vector<Rect> rects_1;
    rects_1.push_back({ 0,0,75,64 });//ghost srcRect
    rects_1.push_back({ 0,0,70,117 });//knight srcRect
    
    std::vector<Color> colors;
    colors.push_back(Color::PreDefined::k_Green);
    colors.push_back(Color::PreDefined::k_Yellow);
    
    spawnerComp_1->ParticleSetUp_Sprites(&imgs_1, &rects_1);
    spawnerComp_1->SetParticleStartScale(0.15f);
    spawnerComp_1->SetAccelerationMod(0.0f);
    spawnerComp_1->SetScaleMod(0.5f);
    spawnerComp_1->SetDistToTravel(200.0f);
    spawnerComp_1->SetVel(200.0f);
    spawnerComp_1->SetAppertureLinear(90.0f*2);
    spawnerComp_1->SetDirectionLinear(MathModule::Normalize({-1, -1}));
    spawnerComp_1->SetSpawnRatePerSpawnCall(3, 5);

    //------------------------------------------------------------------------------------------

    auto spawner_2 = m_pEntityManager->AddEntity("ParticleSpawner");
    auto spawnerComp_2 = spawner_2->AddComponent(new EC_ParticleSpawner(EC_ParticleSpawner::SpawnerType::k_Linear));
    auto transform_2 = spawner_2->GetComponent<EC_Transform>();
    transform_2->SetPos(0, 512);
    
    std::vector<std::string> imgs_2;
    imgs_2.push_back("SP_Ghost.png");
    imgs_2.push_back("SP_Soldier_S.png");
    
    std::vector<Rect> rects_2;
    rects_2.push_back({ 0,0,75,64 });//ghost srcRect
    rects_2.push_back({ 0,0,70,117 });//knight srcRect
    
    std::vector<Color> colors_2;
    colors_2.push_back(Color::PreDefined::k_Green);
    colors_2.push_back(Color::PreDefined::k_Yellow);
    
    spawnerComp_2->ParticleSetUp_Rects(&rects_2, &colors_2);
    spawnerComp_2->SetParticleStartScale(0.15f);
    spawnerComp_2->SetAccelerationMod(0.0f);
    spawnerComp_2->SetScaleMod(0.5f);
    spawnerComp_2->SetDistToTravel(200.0f);
    spawnerComp_2->SetVel(200.0f);
    spawnerComp_2->SetAppertureLinear(90.0f * 2);
    spawnerComp_2->SetDirectionLinear(MathModule::Normalize({ 1, -1 }));
    spawnerComp_2->SetSpawnRatePerSpawnCall(3, 5);

    //------------------------------------------------------------------------------------------

    auto selectable = m_pEntityManager->AddEntity("SelectableEnt");
    auto selectableComp = selectable->AddComponent(new UC_Demo9_Selectable);

    auto chest_1 = m_pEntityManager->AddEntity("chest");
    chest_1->AddComponent(new UC_Demo9_Chest(selectableComp, Point{50,50}));

    auto chest_2 = m_pEntityManager->AddEntity("chest");
    chest_2->AddComponent(new UC_Demo9_Chest(selectableComp, Point{ 512-50,512-50 }));

    auto chest_3 = m_pEntityManager->AddEntity("chest");
    chest_3->AddComponent(new UC_Demo9_Chest(selectableComp, Point{ 512-50,50 }));

    auto chest_4 = m_pEntityManager->AddEntity("chest");
    chest_4->AddComponent(new UC_Demo9_Chest(selectableComp, Point{ 50,512-50 }));
}

void Engine::LoadDemo_Final_Preload()
{
    //Load UC_Final_Manager
    auto managerEnt = m_pEntityManager->AddEntity("GameManager");
    managerEnt->AddComponent(new UC_Final_GameManager);
}

void Engine::LoadDemo_Final_MainMenu()
{
    Rect bgRect{ 0,0,512,512};
    m_pGameManager->ShowCursor(true);


    //Background
    auto bgEnt = m_pEntityManager->AddEntity("PlatformEnt");
    auto btT = bgEnt->AddComponent(new EC_Transform);
    auto bgS = bgEnt->AddComponent(new EC_Sprite);
    bgS->SetSourceImg("SP_DefaultBG.png", bgRect);

    //Texts
    auto titleEnt = m_pEntityManager->AddEntity("titleEnt");
    auto titleText = titleEnt->AddComponent(new EC_Text);
    auto titleTransform = titleEnt->GetComponent<EC_Transform>();

    titleText->SetFont("Fonts/arial.ttf", 24);
    titleText->SetColor(Color::PreDefined::k_Black);
    titleText->SetText("GAP 275: Game Engine");
    titleTransform->SetPos(125, 50);


    auto creditsEnt = m_pEntityManager->AddEntity("creditsEnt");
    auto creditsText = creditsEnt->AddComponent(new EC_Text);
    auto creditsTransform = creditsEnt->GetComponent<EC_Transform>();

    creditsText->SetFont("Fonts/arial.ttf", 12);
    creditsText->SetColor(Color::PreDefined::k_Black);
    creditsText->SetText("By: Alfredo Rodriguez");
    creditsTransform->SetPos(200, 480);


    //Buttons
    auto btnStartEnt = m_pEntityManager->AddEntity("ButtonEnt");
    btnStartEnt->AddComponent(new UC_Btn_LoadOnClick("     Start", "demo_final_controls"));
    auto btnStartTransform = btnStartEnt->GetComponent<EC_Transform>();
    btnStartTransform->SetPos(256 - 32, 200);

    auto btnLoadDemoEnt = m_pEntityManager->AddEntity("ButtonEnt");
    btnLoadDemoEnt->AddComponent(new UC_Btn_LoadOnClick(" Load demo", "demo_final_demos"));
    auto btnLoadDemoTransform = btnLoadDemoEnt->GetComponent<EC_Transform>();
    btnLoadDemoTransform->SetPos(256 - 32, 200 + 40);

    auto btnExitEnt = m_pEntityManager->AddEntity("ButtonEnt");
    btnExitEnt->AddComponent(new UC_Btn_LoadOnClick("      Exit", "cmnd_exit"));
    auto btnExitTransform = btnExitEnt->GetComponent<EC_Transform>();
    btnExitTransform->SetPos(256 - 32, 200 + 80);
}

void Engine::LoadDemo_Final_Controls()
{
    Rect btnRect{0,0,512,512};

    //load img
    auto imgEnt = m_pEntityManager->AddEntity("imgEnt");
    auto imgT = imgEnt->AddComponent(new EC_Transform);
    auto imgSprite = imgEnt->AddComponent(new EC_Sprite);
    imgSprite->SetSourceImg("SP_Controls.png", btnRect);
    
    auto btnEnt = m_pEntityManager->AddEntity("ButtonEnt");
    btnEnt->AddComponent(new UC_Btn_LoadOnClick("   continue", "demo_1"));
    auto btnTransform = btnEnt->GetComponent<EC_Transform>();
    btnTransform->SetPos(256 - 32, 512 - 64);
}

void Engine::LoadMenu_Final_Demos()
{
    Rect btnRect{ 0,0,512,512 };
    Rect bgRect{ 0,0,512,512 };


    //Background img
    auto bgEnt = m_pEntityManager->AddEntity("PlatformEnt");
    auto btT = bgEnt->AddComponent(new EC_Transform);
    auto bgS = bgEnt->AddComponent(new EC_Sprite);
    bgS->SetSourceImg("SP_DefaultBG.png", bgRect);

    //Text
    auto titleEnt = m_pEntityManager->AddEntity("titleEnt");
    auto titleText = titleEnt->AddComponent(new EC_Text);
    auto titleTransform = titleEnt->GetComponent<EC_Transform>();

    titleText->SetFont("Fonts/arial.ttf", 18);
    titleText->SetColor(Color::PreDefined::k_Black);
    titleText->SetText("Select demo");
    titleTransform->SetPos(200, 50);

    //Buttons
    auto demo1Ent = m_pEntityManager->AddEntity("ButtonEnt");
    demo1Ent->AddComponent(new UC_Btn_LoadOnClick("     01_OG", "demo_1"));
    auto demo1T = demo1Ent->GetComponent<EC_Transform>();
    demo1T->SetPos(256 - 32 - 70, 256-20);
    
    auto demo2Ent = m_pEntityManager->AddEntity("ButtonEnt");
    demo2Ent->AddComponent(new UC_Btn_LoadOnClick("     02_NUM", "demo_2"));
    auto demo2T = demo2Ent->GetComponent<EC_Transform>();
    demo2T->SetPos(256 - 32, 256 -20);
    
    auto demo3Ent = m_pEntityManager->AddEntity("ButtonEnt");
    demo3Ent->AddComponent(new UC_Btn_LoadOnClick("     03_DD", "demo_4"));
    auto demo3T = demo3Ent->GetComponent<EC_Transform>();
    demo3T->SetPos(256 + 32 + 6, 256 - 20);
    
    auto demo4Ent = m_pEntityManager->AddEntity("ButtonEnt");
    demo4Ent->AddComponent(new UC_Btn_LoadOnClick("     04_FPS", "demo_7_ShooterTutorial"));
    auto demo4T = demo4Ent->GetComponent<EC_Transform>();
    demo4T->SetPos(256 -30-70, 256+20);
    
    auto demo5Ent = m_pEntityManager->AddEntity("ButtonEnt");
    demo5Ent->AddComponent(new UC_Btn_LoadOnClick("     05_FLM", "demo_9_lvl1"));
    auto demo5T = demo5Ent->GetComponent<EC_Transform>();
    demo5T->SetPos(256 - 32, 256+20);
    
    auto demo6Ent = m_pEntityManager->AddEntity("ButtonEnt");
    demo6Ent->AddComponent(new UC_Btn_LoadOnClick("     06_KEY", "demo_9_lvl2"));
    auto demo6T = demo6Ent->GetComponent<EC_Transform>();
    demo6T->SetPos(256 + 32 + 6, 256 + 20);
    
    auto backEnt = m_pEntityManager->AddEntity("ButtonEnt");
    backEnt->AddComponent(new UC_Btn_LoadOnClick("     Back", "demo_final_mainMenu"));
    auto backT = backEnt->GetComponent<EC_Transform>();
    backT->SetPos(256-32, 512 - 60);
}

void Engine::LoadMenu_Final_Pause()
{
    Rect panelRect{0,0, 76, 184};
    Rect btnImgSrcRect{ 0,0,64,64 };
    Rect btnSmallImgSrcRect{ 0,0,29,64 };


    Point panelPos{256 - 76/2, 256 - 148 / 2};
    Point resumeBtnPos{ panelPos.x + 7, panelPos.y + 3};
    Point saveBtnPos{ panelPos.x + 7, resumeBtnPos.y + 4 + 32};
    Point loadBtnPos{ panelPos.x + 7, saveBtnPos.y + 4 + 32 };
    Point VolUpBtnPos{ panelPos.x + 7, loadBtnPos.y + 4 + 32 };
    Point VolDownBtnPos{ panelPos.x + 7 + 33, loadBtnPos.y + 4 + 32 };
    Point quitToMenuBtnPos{ panelPos.x + 7, VolUpBtnPos.y + 4 + 32 };


    auto panelEnt = m_pEntityManager->AddEntity("SaveLoadMenuPanel", true);
    auto panelTransform = panelEnt->AddComponent(new EC_Transform);
    auto panelSprite = panelEnt->AddComponent(new EC_Sprite);
    panelTransform->SetPos(panelPos);
    panelSprite->SetSourceImg("SP_PausePanel_4.png", panelRect);
    panelSprite->SetLayer(1100U);

    //------------------------------------------------------------------------
    auto ent_resume = m_pEntityManager->AddEntity("Btn_Resume", true);
    auto btn_resume = ent_resume->AddComponent(new EC_Button);

    auto ent_save = m_pEntityManager->AddEntity("Btn_Save", true);
    auto btn_save = ent_save->AddComponent(new EC_Button);

    auto ent_load = m_pEntityManager->AddEntity("Btn_Load", true);
    auto btn_load = ent_load->AddComponent(new EC_Button);

    auto ent_volDown = m_pEntityManager->AddEntity("Btn_Vol-", true);
    auto btn_volDown = ent_volDown->AddComponent(new EC_Button);

    auto ent_volUp = m_pEntityManager->AddEntity("Btn_Vol+", true);
    auto btn_volUp = ent_volUp->AddComponent(new EC_Button);

    auto ent_quitToMainMenu = m_pEntityManager->AddEntity("Btn_QuitToMainMenu", true);
    auto btn_quitToMainMenu = ent_quitToMainMenu->AddComponent(new EC_Button);

    //------------------------------------------------------------------------
    btn_resume->SetPos(resumeBtnPos);
    btn_resume->SetPressImg("SP_Button_2.png", btnImgSrcRect);
    btn_resume->SetLayer(1101U);

    btn_save->SetPos(saveBtnPos);
    btn_save->SetPressImg("SP_Button_2.png", btnImgSrcRect);
    btn_save->SetLayer(1101U);

    btn_load->SetPos(loadBtnPos);
    btn_load->SetPressImg("SP_Button_2.png", btnImgSrcRect);
    btn_load->SetLayer(1101U);

    btn_volDown->SetPos(VolUpBtnPos);
    btn_volDown->SetPressImg("SP_Button_2.png", btnSmallImgSrcRect);
    btn_volDown->SetLayer(1101U);
    btn_volDown->UseSmallHL();

    btn_volUp->SetPos(VolDownBtnPos);
    btn_volUp->SetPressImg("SP_Button_2.png", btnSmallImgSrcRect);
    btn_volUp->SetLayer(1101U);
    btn_volUp->UseSmallHL();

    btn_quitToMainMenu->SetPos(quitToMenuBtnPos);
    btn_quitToMainMenu->SetPressImg("SP_Button_2.png", btnImgSrcRect);
    btn_quitToMainMenu->SetLayer(1101U);

    //------------------------------------------------------------------------
    auto txt_resume = ent_resume->GetComponent<EC_Text>();
    txt_resume->SetText("   Resume");
    txt_resume->SetColor(Color::PreDefined::k_Blue);
    txt_resume->SetFont("Fonts/arial.ttf", 12);

    auto txt_save = ent_save->GetComponent<EC_Text>();
    txt_save->SetText("     Save");
    txt_save->SetColor(Color::PreDefined::k_Blue);
    txt_save->SetFont("Fonts/arial.ttf", 12);

    auto txt_load = ent_load->GetComponent<EC_Text>();
    txt_load->SetText("     Load");
    txt_load->SetColor(Color::PreDefined::k_Blue);
    txt_load->SetFont("Fonts/arial.ttf", 12);

    auto txt_volDown = ent_volDown->GetComponent<EC_Text>();
    txt_volDown->SetText("vol-");
    txt_volDown->SetColor(Color::PreDefined::k_Blue);
    txt_volDown->SetFont("Fonts/arial.ttf", 12);

    auto txt_volUp = ent_volUp->GetComponent<EC_Text>();
    txt_volUp->SetText("vol+");
    txt_volUp->SetColor(Color::PreDefined::k_Blue);
    txt_volUp->SetFont("Fonts/arial.ttf", 12);

    auto txt_quitToMainMenu = ent_quitToMainMenu->GetComponent<EC_Text>();
    txt_quitToMainMenu->SetText(" Main menu");
    txt_quitToMainMenu->SetColor(Color::PreDefined::k_Blue);
    txt_quitToMainMenu->SetFont("Fonts/arial.ttf", 12);

    //---------------------------------------------------------------
    auto linkerEnt = m_pEntityManager->AddEntity("linker", true);
    auto linkerComp = linkerEnt->AddComponent(new UC_BtnToEntLinker);

    linkerComp->LinkBtnToEnt("GameManager", btn_resume, Event::CLICKABLE_CLICK_UP);
    linkerComp->LinkBtnToEnt("GameManager", btn_save, Event::CLICKABLE_CLICK_UP);
    linkerComp->LinkBtnToEnt("GameManager", btn_load, Event::CLICKABLE_CLICK_UP);
    linkerComp->LinkBtnToEnt("GameManager", btn_quitToMainMenu, Event::CLICKABLE_CLICK_UP);
    linkerComp->LinkBtnToEnt("GameManager", btn_volDown, Event::CLICKABLE_CLICK_UP);
    linkerComp->LinkBtnToEnt("GameManager", btn_volUp, Event::CLICKABLE_CLICK_UP);
}

void Engine::LoadMenu_Final_SaveLoad()
{
    Rect btnImgSrcRect{ 0,0,64,64 };
    Rect panelRect{0,0,142, 110};

    Point panelPos{200,200};
    Point slotBtnPos_1{ panelPos.x + 3, panelPos.y + 3};
    Point slotBtnPos_2{ slotBtnPos_1.x, slotBtnPos_1.y + 5 + 32 };
    Point slotBtnPos_3{ slotBtnPos_1.x, slotBtnPos_2.y + 5 + 32 };

    Point levelInfoTextPos1{panelPos.x + 64 + 15, panelPos.y+5};
    Point levelInfoTextPos2{levelInfoTextPos1.x, levelInfoTextPos1.y + 40};


    auto panelEnt = m_pEntityManager->AddEntity("SaveLoadMenuPanel", true);
    auto panelTransform = panelEnt->AddComponent(new EC_Transform);
    auto panelSprite = panelEnt->AddComponent(new EC_Sprite);
    panelTransform->SetPos(panelPos);
    panelSprite->SetSourceImg("SP_SaveLoadPanel_2.png", panelRect);
    panelSprite->SetLayer(1100U);


    auto levelInfoTextEnt = m_pEntityManager->AddEntity("LevelInfoText_Ent", true);
    auto levelInfoTextComp = levelInfoTextEnt->AddComponent(new UC_Final_SelectedLevelDisplayer(levelInfoTextPos1));


    //---------------------------------------------------------------
    auto ent_saveSlot1 = m_pEntityManager->AddEntity("Btn_SaveSlot1", true);
    auto btn_saveSlot1 = ent_saveSlot1->AddComponent(new EC_Button);

    auto ent_saveSlot2 = m_pEntityManager->AddEntity("Btn_SaveSlot2", true);
    auto btn_saveSlot2 = ent_saveSlot2->AddComponent(new EC_Button);

    auto ent_saveSlot3 = m_pEntityManager->AddEntity("Btn_SaveSlot3", true);
    auto btn_saveSlot3 = ent_saveSlot3->AddComponent(new EC_Button);

    //---------------------------------------------------------------
    btn_saveSlot1->SetPos(slotBtnPos_1);
    btn_saveSlot1->SetPressImg("SP_Button_2.png", btnImgSrcRect);
    btn_saveSlot1->SetLayer(1101U);

    btn_saveSlot2->SetPos(slotBtnPos_2);
    btn_saveSlot2->SetPressImg("SP_Button_2.png", btnImgSrcRect);
    btn_saveSlot2->SetLayer(1101U);

    btn_saveSlot3->SetPos(slotBtnPos_3);
    btn_saveSlot3->SetPressImg("SP_Button_2.png", btnImgSrcRect);
    btn_saveSlot3->SetLayer(1101U);

    //---------------------------------------------------------------
    auto txt_saveSlot1 = ent_saveSlot1->GetComponent<EC_Text>();
    txt_saveSlot1->SetText("     Slot 1");
    txt_saveSlot1->SetColor(Color::PreDefined::k_Blue);
    txt_saveSlot1->SetFont("Fonts/arial.ttf", 12);

    auto txt_saveSlot2 = ent_saveSlot2->GetComponent<EC_Text>();
    txt_saveSlot2->SetText("     Slot 2");
    txt_saveSlot2->SetColor(Color::PreDefined::k_Blue);
    txt_saveSlot2->SetFont("Fonts/arial.ttf", 12);

    auto txt_saveSlot3 = ent_saveSlot3->GetComponent<EC_Text>();
    txt_saveSlot3->SetText("     Slot 3");
    txt_saveSlot3->SetColor(Color::PreDefined::k_Blue);
    txt_saveSlot3->SetFont("Fonts/arial.ttf", 12);

    //---------------------------------------------------------------
    auto linkerEnt = m_pEntityManager->AddEntity("linker", true);
    auto linkerComp = linkerEnt->AddComponent(new UC_BtnToEntLinker);
    linkerComp->LinkBtnToEnt("GameManager", btn_saveSlot1, Event::CLICKABLE_HOVER_IN);
    linkerComp->LinkBtnToEnt("GameManager", btn_saveSlot1, Event::CLICKABLE_CLICK_UP);

    linkerComp->LinkBtnToEnt("GameManager", btn_saveSlot2, Event::CLICKABLE_HOVER_IN);
    linkerComp->LinkBtnToEnt("GameManager", btn_saveSlot2, Event::CLICKABLE_CLICK_UP);

    linkerComp->LinkBtnToEnt("GameManager", btn_saveSlot3, Event::CLICKABLE_HOVER_IN);
    linkerComp->LinkBtnToEnt("GameManager", btn_saveSlot3, Event::CLICKABLE_CLICK_UP);
}