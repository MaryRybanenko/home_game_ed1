#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "GameConfig.h"

USING_NS_CC;


AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() {

	GC->init();

	const float width = GC->getParam("width").floatValue();
	const float height = GC->getParam("height").floatValue();

	Size designSize(width, height);

	auto director = Director::getInstance(); //creates and handles the main Window and manages how and when to execute the Scenes.
	auto glview = director->getOpenGLView(); //By GLView you can operate the frame information of EGL view
	if (!glview) {
		glview = GLViewImpl::create("Hello World");

		glview->setFrameSize(designSize.width, designSize.height);
		glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::SHOW_ALL);

		director->setOpenGLView(glview);
	}

	auto base = Scene::create();
	director->runWithScene(base);

	auto scene = LayerHelloWorld::createScene();
	director->pushScene(scene);

	return true;
}

void AppDelegate::applicationDidEnterBackground() {
}

void AppDelegate::applicationWillEnterForeground() {
}