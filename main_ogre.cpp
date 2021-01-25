// This file is part of the OGRE project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at https://www.ogre3d.org/licensing.
// SPDX-License-Identifier: MIT
/*
#include <iostream>
#include <Ogre.h>
#include <Bites/OgreApplicationContext.h>
#include <RTShaderSystem/OgreShaderGenerator.h>
#include <Terrain/OgreTerrain.h>
#include "myEventListener.hpp"

//#include "ExampleFrameListener.h"

using namespace Ogre;

class MyTestApp : public OgreBites::ApplicationContext
{
    public:
        MyTestApp();
        void setup();
        void createTerrain();
    private:
        Ogre::Camera* _cam = nullptr;
        int _mVitesse;
        bool _mToucheAppuyee;
        Ogre::SceneManager* scnMgr;

        Ogre::Terrain* mTerrain;
        Ogre::TerrainGlobalOptions* mGlobals;
        Ogre::Light* mLight;

        std::unordered_map<std::string, Ogre::SceneNode*> listScene;
};

//! [constructor]
MyTestApp::MyTestApp() : OgreBites::ApplicationContext("My Game")
{
    _mVitesse = 100;
    _mToucheAppuyee = false;
    mTerrain = nullptr;
}
//! [constructor]

void MyTestApp::createTerrain()
{
    Ogre::Vector3 lightdir(0.55f, -0.3f, 0.75f);
    mLight = scnMgr->createLight("terrainLight");
    mLight->setType(Ogre::Light::LT_DIRECTIONAL);
    mLight->setDirection(lightdir);
    mLight->setDiffuseColour(Ogre::ColourValue::White);
    mLight->setSpecularColour(Ogre::ColourValue(0.4f, 0.4f, 0.4f));

    mTerrain = OGRE_NEW Ogre::Terrain(scnMgr);

    // options globales
    mGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
    mGlobals->setMaxPixelError(10);
    mGlobals->setCompositeMapDistance(8000);
    mGlobals->setLightMapDirection(mLight->getDerivedDirection());
    mGlobals->setCompositeMapAmbient(scnMgr->getAmbientLight());
    mGlobals->setCompositeMapDiffuse(mLight->getDiffuseColour());

    Ogre::Image img;
    img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    // informations géométriques
    Ogre::Terrain::ImportData imp;
    imp.inputImage = &img;
    imp.terrainSize = img.getWidth();
    imp.worldSize = 8000;
    imp.inputScale = 600;
    imp.minBatchSize = 33;
    imp.maxBatchSize = 65;

    // textures
    imp.layerList.resize(3);
    imp.layerList[0].worldSize = 100;
    imp.layerList[0].textureNames.push_back("Ground23_normheight.dds");
    imp.layerList[0].textureNames.push_back("Ground23_normheight.dds");
    imp.layerList[1].worldSize = 30;
    imp.layerList[1].textureNames.push_back("Ground37_diffspec.dds");
    imp.layerList[1].textureNames.push_back("Ground37_normheight.dds");
    imp.layerList[2].worldSize = 200;
    imp.layerList[2].textureNames.push_back("Rock20_diffspec.dds");
    imp.layerList[2].textureNames.push_back("Rock20_normheight.dds");
    mTerrain->prepare(imp);
    mTerrain->load();

    // plaquage de texture
    Ogre::TerrainLayerBlendMap* blendMap1 = mTerrain->getLayerBlendMap(1);
    float* pBlend1 = blendMap1->getBlendPointer();

    for (Ogre::uint16 y = 0; y < mTerrain->getLayerBlendMapSize(); ++y)
    {
        for (Ogre::uint16 x = 0; x < mTerrain->getLayerBlendMapSize(); ++x)
        {
            *pBlend1++ = 150;
        }
    }

    blendMap1->dirty();
    blendMap1->update();

    mTerrain->freeTemporaryResources();
}



//! [setup]
void MyTestApp::setup(void)
{
    Ogre::ConfigFile configFile;
    configFile.load("resources.cfg");
    // do not forget to call the base first
    OgreBites::ApplicationContext::setup();
    Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
    Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(8);
    // get a pointer to the already created root
    Ogre::Root* root = getRoot();
    scnMgr = root->createSceneManager();

    // register our scene with the RTSS
    Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    // Light
    Ogre::Light* light = scnMgr->createLight("MainLight");
    Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(100, 500, 100);
    lightNode->attachObject(light);
    scnMgr->setAmbientLight(ColourValue(1, 1, 1));


    // also need to tell where we are
    Ogre::SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    camNode->setPosition(-100, 150, 200);
    camNode->lookAt(Ogre::Vector3(0, 100, 0), Ogre::Node::TS_PARENT);

    // create the camera
    _cam = scnMgr->createCamera("myCam");
    _cam->setNearClipDistance(5); // specific to this sample
    _cam->setAutoAspectRatio(true);
    _cam->setNearClipDistance(1);
    _cam->setFarClipDistance(1500);
    camNode->attachObject(_cam);
    
    // and tell it to render into the main window
    getRenderWindow()->addViewport(_cam);
   

    // finally something to render
    Ogre::Entity* ent = scnMgr->createEntity("robot", "robot.mesh");
    Ogre::SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode();
    Ogre::SceneNode* nodePlane = scnMgr->getRootSceneNode()->createChildSceneNode();
    Plane plan(Vector3::UNIT_Y, 0);
    MeshManager::getSingleton().createPlane("ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plan, 500, 500, 1, 1, true, 1, 1, 1, Vector3::UNIT_Z);
    Entity* sol = scnMgr->createEntity("EntiteSol", "ground");
    node->attachObject(ent);
    nodePlane->attachObject(sol);
    sol->setCastShadows(true);
    ent->setCastShadows(true);
    sol->setMaterialName("Examples/GrassFloor");
    node->setPosition(0, 30, 0.0);
    node->translate(0.0, 20.0, 0.0);
    nodePlane->setPosition(0, 0, 0.0); 
    listScene.insert(std::pair<std::string, Ogre::SceneNode*>("player", node));
    listScene.insert(std::pair<std::string, Ogre::SceneNode*>("plane", nodePlane));

    // Events
    Ogre::RenderWindow* window = getRenderWindow();
    myEventListener* myListener = new myEventListener(_cam, window, scnMgr, listScene);
    root->addFrameListener(myListener);

    // Terrain
    createTerrain();

    scnMgr->setSkyBox(true, "Examples/CloudySky", 600, false);
    //scnMgr->setSkyDome(true, "Examples/CloudySky", 2, 10.0);

}
//! [setup]

//! [main]
int main()
{
    try {
        MyTestApp app;
        app.initApp();
        app.getRoot()->startRendering();
        app.closeApp();
    }
    catch (Ogre::Exception & e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
//! [main] */