#include "DummySceneListener.h"

#include "Camera/MayaCamera.h"
#include "Ogre.h"
#include "Compositor/OgreCompositorManager2.h"

class 	DummySceneListener::Imp
{
public:

	Ogre::RenderWindow*	RT_{};
	MayaCameraSPtr 		CameraListener_{};
	Ogre::SceneManager*	Smgr_{};
	Ogre::CompositorWorkspace*	WorkSpce_{};
};

DummySceneListener::DummySceneListener( Ogre::RenderWindow *rt ):ImpUPtr_(new Imp)
{
	ImpUPtr_->RT_ = rt;
}

DummySceneListener::~DummySceneListener()
{

}

void DummySceneListener::_Load()
{
	auto& imp_ = *ImpUPtr_;

	Ogre::Vector3 v1(1.f, 1.f, 1.f);
	v1.normalise();

	imp_.Smgr_ = Ogre::Root::getSingletonPtr()->createSceneManager(Ogre::ST_GENERIC, 1, Ogre::INSTANCING_CULLING_SINGLETHREAD, "DummyScene");
	auto camera = imp_.Smgr_->createCamera("MainCamera");
	camera->setAutoAspectRatio(true);

	auto comMgr = Ogre::Root::getSingletonPtr()->getCompositorManager2();
	if ( !comMgr->hasWorkspaceDefinition("DummyWorkspaceDef") )
	{
		comMgr->createBasicWorkspaceDef("DummyWorkspaceDef", Ogre::ColourValue(.6f, 0.6f, .6f), {});
	}

	imp_.WorkSpce_ = comMgr->addWorkspace(imp_.Smgr_, imp_.RT_, camera, "DummyWorkspaceDef", true);

	auto light = imp_.Smgr_->createLight();
	imp_.Smgr_->setAmbientLight(Ogre::ColourValue(1.f, 1.f, 1.f));
	auto lightNode = imp_.Smgr_->getRootSceneNode()->createChildSceneNode();
	lightNode->setPosition({20.f, 80.f, 50.f});
	lightNode->attachObject(light);
	light->setDirection({-1.f, -1.f, -1.f});
	light->setDiffuseColour(1.f, 1.f, 1.f);

	auto head = imp_.Smgr_->createEntity("ogrehead.mesh");
	auto headNode = imp_.Smgr_->getRootSceneNode()->createChildSceneNode();
	headNode->attachObject(head);
	headNode->setPosition({0.f, 0.f, 0.f});

	auto mayaCamera = std::make_shared<MayaCamera>(camera);

	mayaCamera->SetPosAndTarget({30.f, 90.f, 150.f}, {0.f, 0.f, 0.f});

	camera->setNearClipDistance(10.f);
	camera->setFarClipDistance(1000.f);

	imp_.CameraListener_ = mayaCamera;
}

void DummySceneListener::_Unload()
{
	auto& imp_ = *ImpUPtr_;

	imp_.CameraListener_.reset();

	auto comMgr = Ogre::Root::getSingletonPtr()->getCompositorManager2();
	comMgr->removeWorkspace(imp_.WorkSpce_);
	Ogre::Root::getSingletonPtr()->destroySceneManager(imp_.Smgr_);
}