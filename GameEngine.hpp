#pragma once

#include <iostream>
#include <memory>
#include <iostream>
#include <Ogre.h>
#include <Bites/OgreApplicationContext.h>
#include <RTShaderSystem/OgreShaderGenerator.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include "Entity.hpp"
#include "Component.hpp"
#include "myEventListener.hpp"

using namespace std;
class GameEngine : public OgreBites::ApplicationContext
{
public:
    GameEngine(const std::string& name);
    ~GameEngine();
    void setup();
    void startRendering();
    void createPlayer(std::shared_ptr<Entity>);
    void createEntity(std::shared_ptr<Entity>);

    void createComponentSprite(const std::string name, const std::string mesh);
    void createComponentPosition(const std::string name, float, float, float);
    void createComponentMoove(const std::string name, int speed);
    void createComponentAnimation(const std::string name, std::string, std::string, std::string);
    void createComponentWall(const std::string name, float);
    void createComponentCollider(const std::string name);
    void createComponentScale(const std::string name, float, float, float);

private:
    void setConfigFile();
    void setApplicationSetup();
    void setOgreGlobalVariables();
    void initObjects(); // Init physics
    void initGround(); // Need to be change
    void initWall(); // Need to be change
    
    shared_ptr<Component> getComponent(MyComponentEnum value, const std::string &name);

    // start Component generators
    
    void makePosition(std::shared_ptr<Entity>);
    void makeEvents(std::shared_ptr<Entity>);
    void makeWall(std::shared_ptr<Entity>);
    void makeCollider(std::shared_ptr<Entity>, float);
    
    // end Component generators

    Ogre::Root *_root;
    Ogre::SceneManager* _scnMgr;
    Ogre::Light* _mLight;
    Ogre::Camera* _cam;
    Ogre::RenderWindow* _window;

    Ogre::Plane* _mPlane;
    Ogre::Plane* _mWall;


    shared_ptr<myEventListener> myListener;

    unordered_map<string, shared_ptr<Entity>> _entity;
    array<vector<pair<string, shared_ptr<Component>>>, MyComponentEnum::Last> _components;

    vector<pair<string, shared_ptr<myEventListener>>> _listener;

    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* overlappingPairCache;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
    //std::vector<btCollisionShape*> collisionShapes;
    std::map<std::string, btRigidBody*> physicsAccessors;

    btAlignedObjectArray<btCollisionShape*> collisionShapes;

    std::shared_ptr<std::vector<btRigidBody*>> _walls;

};
