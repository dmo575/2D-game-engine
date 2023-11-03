#include "Engine.h"

int main(int argc, char* argv[])
{
    Engine* engine = new Engine();
    
    engine->Run();
    
    delete engine;


    return 0;
}