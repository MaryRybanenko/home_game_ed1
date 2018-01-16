#ifndef __RES_LIST_H__
#define __RES_LIST_H__

#include "cocos2d.h"
#include "cocos2d\external\json\rapidjson.h"
#include "cocos2d\external\json\reader.h"
#include "cocos2d\external\json\document.h"

NS_CC_BEGIN

/* ResourceList is Singleton */
class ResourceList
{
private:
	
	std::map< std::string, StringMap > mImageResources;
	static ResourceList* mInstance;

	ResourceList();
	~ResourceList();

public:
	static ResourceList* getInstance();
	bool LoadResourceList(const std::string filename);
	std::string getResourcePath(const std::string filePath, const std::string& resourceId);
	bool UnloadResource(const std::string& resourceId, const std::string& filePath);
	bool UnloadResourceList(const std::string& filePath);

};

#define RESLIST ResourceList::getInstance()
#define GET_IMAGE_PATH ResourceList::getInstance()->


NS_CC_END
#endif