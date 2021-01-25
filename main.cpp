#include <iostream>
#include <Ogre.h>
#include "GameEngine.hpp"
#include "MyGame.hpp"

using namespace std;

int main()
{
    try {
        GameEngine gameEngine("My Game Engine");
        gameEngine.initApp();
        MyGame game(&gameEngine);
        game.start();
        gameEngine.closeApp();
    }
    catch (Ogre::Exception & e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}