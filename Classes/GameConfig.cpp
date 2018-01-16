#include "GameConfig.h"

USING_NS_CC;

GameConfig* GameConfig::mInstance = nullptr;

GameConfig::GameConfig()
	: mIsInitCall(false)
{}

GameConfig::~GameConfig()
{}

GameConfig* GameConfig::getInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new GameConfig();
	}

	return mInstance;
}




void GameConfig::init()
{
	if (!mIsInitCall)
	{
		mIsInitCall = true;

		std::string content = FileUtils::getInstance()->getStringFromFile("main_config.json");
		
		rapidjson::Document document;
		document.Parse<0>(content.c_str());
		
		if (!document.HasParseError())
		{
			for (auto fieldIt = document.MemberBegin(); fieldIt != document.MemberEnd(); fieldIt++)
			{
				const std::string key = fieldIt->name.GetString();
				mParamMap[key] = fieldIt->value.GetString();
			}
		}
		else
		{
			log("PARSE ERROR main config!!!");
		}

	}
}


String GameConfig::getParam(std::string fieldName)
{
	String result;

	auto paramIt = mParamMap.find(fieldName);
	if (paramIt != mParamMap.end())
		result = paramIt->second.getCString();
	else
	{
		log("could not find key!!!");
	}
	return result;
}