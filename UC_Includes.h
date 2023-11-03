#pragma once

//This is to be used with the .CPP of any User component

//MODULES
#include "InputModule.h"//Used to get input
#include "MathModule.h"//Commom math functions

//MANAGERS
#include"EntityManager.h"//Entity management (create ent, destroy ent)
#include "LevelManager.h"//Level management (load level, unload level)
#include "GameManager.h"//Delta management (set delta vel)
#include "SoundManager.h"
#include "ColliderManager.h"
#include "SaveManager.h"

//ECS
#include "Entity.h"

//COMPONENTS
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
#include "Logger.h"
#include "PostFrameDrawer.h"

//TYPES
#include "CoreStructs.h"
#include "RenderableObjec.h"