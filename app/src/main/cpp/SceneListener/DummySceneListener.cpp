#include "DummySceneListener.h"

#include "Camera/MayaCamera.h"
#include "Ogre.h"
#include "Compositor/OgreCompositorManager2.h"

class 	DummySceneListener::Imp
{
public:

	Ogre::RenderWindow*	RT_{};
};

DummySceneListener::DummySceneListener( Ogre::RenderWindow *rt ):ImpUPtr_(new Imp)
{
	ImpUPtr_->RT_ = rt;
}

DummySceneListener::~DummySceneListener()
{ }

void DummySceneListener::Load()
{
	auto& imp_ = *ImpUPtr_;

	auto smgr = Ogre::Root::getSingletonPtr()->createSceneManager(Ogre::ST_GENERIC, 1, Ogre::INSTANCING_CULLING_SINGLETHREAD, "DummyScene");
	auto camera = smgr->createCamera("MainCamera");
	camera->setAutoAspectRatio(true);
	auto comMgr = Ogre::Root::getSingletonPtr()->getCompositorManager2();
	comMgr->createBasicWorkspaceDef("DummyWorkspaceDef", Ogre::ColourValue(.6f, 0.6f, .6f), {});
	auto workspace = comMgr->addWorkspace(smgr, imp_.RT_, camera, "DummyWorkspaceDef", true);

	auto light = smgr->createLight();
	smgr->setAmbientLight(Ogre::ColourValue(1.f, 1.f, 1.f));
	auto lightNode = smgr->getRootSceneNode()->createChildSceneNode();
	lightNode->setPosition({20.f, 80.f, 50.f});
	lightNode->attachObject(light);
	light->setDirection({-1.f, -1.f, -1.f});
	light->setDiffuseColour(1.f, 1.f, 1.f);

	auto head = smgr->createEntity("ogrehead.mesh");
	auto headNode = smgr->getRootSceneNode()->createChildSceneNode();
	headNode->attachObject(head);
	headNode->setPosition({0.f, 0.f, 0.f});

	auto mayaCamera = std::make_shared<MayaCamera>(camera);

	mayaCamera->SetPosAndTarget({30.f, 90.f, 150.f}, {0.f, 0.f, 0.f});

	camera->setNearClipDistance(10.f);
	camera->setFarClipDistance(1000.f);

	AddChild(mayaCamera);
}