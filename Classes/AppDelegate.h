//#ifndef  _APP_DELEGATE_H_
//#define  _APP_DELEGATE_H_
//
//#include "cocos2d.h"						instead pragma once

//#endif // _APP_DELEGATE_H_
//


#pragma once

#include "cocos2d.h"

class  AppDelegate : private cocos2d::Application
{
public:
	AppDelegate();
	virtual ~AppDelegate();

	virtual bool applicationDidFinishLaunching(); //Implement Director and Scene init code here
	virtual void applicationDidEnterBackground(); //This function will be called when the application enters background.
	virtual void applicationWillEnterForeground(); //This function will be called when the application enters foreground.
};
