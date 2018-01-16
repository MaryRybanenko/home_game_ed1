#include "ResourceList.h"

USING_NS_CC;

ResourceList* ResourceList::mInstance = nullptr;

ResourceList::ResourceList()
{}

ResourceList::~ResourceList()
{}

ResourceList* ResourceList::getInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new ResourceList();
	}

	return mInstance;
}


bool ResourceList::LoadResourceList(const std::string filePath)
{
	bool result;
	StringMap localResMap;
	std::string content = FileUtils::getInstance()->getStringFromFile(filePath);

	rapidjson::Document document;
	document.Parse<0>(content.c_str());

	if (!document.HasParseError())
	{
		for (auto fieldIt = document.MemberBegin(); fieldIt != document.MemberEnd(); fieldIt++)
		{
			const std::string key = fieldIt->name.GetString();
			localResMap[key] = fieldIt->value.GetString();
		}

		mImageResources[filePath] = localResMap;
		result = true;
	}

	else
	{
		log("PARSE ERROR resource list!!!");
		result = false;
	}

	return result;
}

std::string ResourceList::getResourcePath(const std::string filePath, const std::string& resourceId)
{
	String result = "";
	auto mapIt = mImageResources.find(filePath);
	if (mapIt != mImageResources.end())
	{
		auto resRecordsIt = mImageResources[filePath].find(resourceId);

		if (resRecordsIt != mImageResources[filePath].end())
		{
			result = resRecordsIt->second;
		}
		else
		{
			log("getResourcePath: Unknow resource path!!!");
		}
	}
	else
	{
		log("getResourcePat: Unknow resource list!!!");
	}


	return result.getCString();
}

bool  ResourceList::UnloadResource(const std::string& resourceId, const std::string& filePath)
{
	bool result = true;

	auto mapIt = mImageResources.find(filePath);
	if (mapIt != mImageResources.end())
	{
		auto resRecordsIt = mImageResources[filePath].find(resourceId);

		if (resRecordsIt != mImageResources[filePath].end())
		{
			mImageResources[filePath].erase(resRecordsIt);
		}
		else
		{
			log("UnloadResource: Unknow resource path!!!");
			result = false;
		}
	}
	else
	{
		log("UnloadResource: Unknow resource list!!!");
		result = false;
	}
	return result;
}


bool  ResourceList::UnloadResourceList(const std::string& filePath)
{
	bool result = true;

	auto mapIt = mImageResources.find(filePath);
	if (mapIt != mImageResources.end())
	{
		mImageResources.erase(mapIt);
	}
	else
	{
		log("UnloadResourceList: Unknow resource list!!!");
		result = false;
	}
	return result;
}


