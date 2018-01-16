#ifndef __GAME_CONFIG_H__
#define __GAME_CONFIG_H__

#include "cocos2d.h"
#include "cocos2d\external\json\rapidjson.h"
#include "cocos2d\external\json\reader.h"
#include "cocos2d\external\json\document.h"

NS_CC_BEGIN

/* GameConfig is Singleton */
class GameConfig
{
private:
	bool mIsInitCall;
	static GameConfig* mInstance;
	StringMap mParamMap;
	GameConfig();
	~GameConfig();

public:
	static GameConfig* getInstance();
	void init();
	String getParam(std::string str);
};

#define GC GameConfig::getInstance()

NS_CC_END
#endif