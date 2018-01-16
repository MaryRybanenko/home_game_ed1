#include "HelloWorldScene.h"
#include "GameConfig.h"
#include "ResourceList.h"
#include "cocos\deprecated\CCString.h"

USING_NS_CC;

#define PLAYER_LAYER 10
#define BACKGROUND_LAYER 1
#define DEBUG_DRAW_ON 0

#define ENEMY_DISPATCH 1

LayerHelloWorld::LayerHelloWorld()
	: mLivesCounter(3)
	, mDebugDrawNode( nullptr )
	, mIsCanJump(true)
	, mIsGameFinish(false)
	, mIsAnimationEnd(false)
	, mIsTouched( false )
	, mIsLeftPressed(false)
	, mIsRightPressed(false)
	, mIsJumpPressed(false)
	, mPrevEnemyObj( nullptr )
{}

LayerHelloWorld::~LayerHelloWorld()
{
	unscheduleUpdate();
}


Scene* LayerHelloWorld::createScene()
{
	auto scene = Scene::create();
	auto layer = LayerHelloWorld::create();

	scene->addChild(layer);

	return scene;
}

bool LayerHelloWorld::init()
{

	if (!Parent::init())
	{
		return false;
	}

	scheduleUpdate();
	

	const Size designSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	const auto scrWidth = designSize.width;
	const auto scrHeight = designSize.height;
	Sprite* bcground;
	Sprite* bcground_green;

	if (RESLIST->LoadResourceList("resource_list.json"))
	{
		mPlayer = Sprite::create(RESLIST->getResourcePath("resource_list.json", "player_id"));
		bcground = Sprite::create(RESLIST->getResourcePath("resource_list.json", "sky_id"));
		bcground_green = Sprite::create(RESLIST->getResourcePath("resource_list.json", "land_id"));
		mGrass = Sprite::create(RESLIST->getResourcePath("resource_list.json", "grass_0_id"));
		mGrassClone = Sprite::create(RESLIST->getResourcePath("resource_list.json", "grass_1_id"));
	}

	String str = GC->getParam("player_pos").getCString();
	auto arr = str.componentsSeparatedByString("|");
	const float posX = static_cast< String* >(arr->getObjectAtIndex(0))->floatValue();
	const float posY = static_cast< String* >(arr->getObjectAtIndex(1))->floatValue();

	mPosYforEnemy = designSize.height / 3;

	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	setTouchEnabled(true);
	setKeyboardEnabled(true);

	bcground->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	bcground->setPosition(Vec2::ZERO);
	bcground->setName("bcground");
	addChild(bcground, BACKGROUND_LAYER);

	const auto bcWidth = bcground->getContentSize().width;
	const auto bcHeight = bcground->getContentSize().height;
	bcground->setScale(scrWidth / bcWidth, scrHeight / bcHeight);

	bcground_green->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	bcground_green->setPosition(Vec2::ZERO);
	bcground_green->setName("bc_green");
	addChild(bcground_green, BACKGROUND_LAYER);

	const auto oWidth = bcground_green->getContentSize().width;
	const auto oHeight = bcground_green->getContentSize().height;
	const auto vHeight = designSize.height / 3;

	bcground_green->setScale(scrWidth / oWidth, vHeight / oHeight); // backOrig scaled to screen size

	mGrass->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	mInitYForGrass = designSize.height / 3 -  (mPlayer->getContentSize().height / 2);
	mGrass->setPosition(Vec2(0.0f, mInitYForGrass));
	mGrass->setName("grass");
	addChild(mGrass, BACKGROUND_LAYER + 1);
	mGrass->setScaleX(scrWidth / mGrass->getContentSize().width);


	mGrassClone->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	mGrassClone->setPosition(Vec2(designSize.width, mInitYForGrass));
	mGrassClone->setName("grass2");
	addChild(mGrassClone, BACKGROUND_LAYER + 1);

	mGrassClone->setScaleX(scrWidth / mGrassClone->getContentSize().width);

	mPlayerObj = Player::create();
	mPlayerObj->setPosition(mPlayerObj->getInitXpos(), mPlayerObj->getInitYpos());
	addChild(mPlayerObj, PLAYER_LAYER);

#if  DEBUG_DRAW_ON
	auto playerDrawNode = DrawNode::create();
	playerDrawNode->clear();
	playerDrawNode->drawRect(Vec2::ZERO, mPlayer->getContentSize(), Color4F::MAGENTA);
	mPlayer->addChild( playerDrawNode, 1 );

	mDebugDrawNode = DrawNode::create();
	addChild(mDebugDrawNode, 99);
#endif
	printScore();
	createMenuBar();

	//landMovement();
	//landMovementClone();
	//playerMovement();
	callEnemyCreator();

	return true;
}



void LayerHelloWorld::landMovement()
{
	const Size desSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();

	auto mGrassAct = Sequence::create(
		MoveTo::create(4.0f, Vec2(desSize.width * -1.0f, mInitYForGrass)),
		CallFunc::create(CC_CALLBACK_0(Node::setPositionX, mGrass, desSize.width)),
		CallFunc::create(CC_CALLBACK_0(Node::setPositionY, mGrass, mInitYForGrass)),
		MoveTo::create(4.0f, Vec2(0.0f, mInitYForGrass)),
		nullptr
		);

	Action* landMoveCallBack = Sequence::create(
		mGrassAct,
		CallFunc::create(CC_CALLBACK_0(LayerHelloWorld::landMovement, this)),
		nullptr
		);
	mGrass->runAction(landMoveCallBack);

}

void LayerHelloWorld::landMovementClone()
{
	const Size desSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();

	auto mGrassAct = Sequence::create(
		MoveTo::create(8.0f, Vec2(desSize.width * -1.0f, mInitYForGrass)),
		CallFunc::create(CC_CALLBACK_0(Node::setPositionX, mGrassClone, desSize.width)),
		CallFunc::create(CC_CALLBACK_0(Node::setPositionY, mGrassClone, mInitYForGrass)),
		nullptr
		);

	Action* landMoveCallBack = Sequence::create(
		mGrassAct,
		CallFunc::create(CC_CALLBACK_0(LayerHelloWorld::landMovementClone, this)),
		nullptr
		);

	mGrassClone->runAction(landMoveCallBack);
}


void LayerHelloWorld::update(float delta)
{

#if DEBUG_DRAW_ON 
	mDebugDrawNode->clear();
#endif

	if (mIsLeftPressed)
	{
		mPlayerObj->playerActionLeft();
	}
	if (mIsRightPressed)
	{
		mPlayerObj->playerActionRight();
	}
	if (mIsJumpPressed)
	{
		mPlayerObj->playerActionJump();
		if (mPlayerObj->getPositionY() >= mPlayerObj->getJumpHight())
		{
			mIsJumpPressed = false;
		}
	}
	else if (!mIsJumpPressed)
	{
		mPlayerObj->playerActionJumpDown();
	}

	for (auto enemy : mEnemyList)
	{
		enemy->enemyAction();
		const Rect characterBox = mPlayerObj->getBoundingBox();
		const Rect enemyBox = enemy->getBoundingBox();

#if DEBUG_DRAW_ON 
		
		mDebugDrawNode->drawRect(enemyBox.origin, enemyBox.origin + enemyBox.size, Color4F::RED);
		mDebugDrawNode->drawRect(characterBox.origin, characterBox.origin + characterBox.size, Color4F::BLUE);

		const Rect rect = mGrass->getBoundingBox();
		mDebugDrawNode->drawRect(rect.origin, rect.origin + rect.size, Color4F::YELLOW);

		const Rect rect1 = mGrassClone->getBoundingBox();
		mDebugDrawNode->drawRect(rect1.origin, rect1.origin + rect1.size, Color4F::BLACK);
#endif
		if (enemyBox.getMinX() >= characterBox.getMaxX())
		{
			if (mPrevEnemyObj != enemy)
			{
				mIsTouched = false;
			}
			if (!mIsTouched && enemy->isSpritesIntersects(characterBox, enemyBox, mPlayerObj))
			{
				mIsTouched = true;
				mPrevEnemyObj = enemy;
				mLivesCounter--;
				changeScore();

				if (!mLivesCounter)
				{
					unscheduleUpdate();

					mPlayerObj->stopAllActions();

					mGrass->stopAllActions();
					mGrassClone->stopAllActions();

					for (auto enemyIt : mEnemyList)
					{
						enemyIt->stopAllActions();
					}
					mIsGameFinish = true;
				}
			}
		}
	}

}


void LayerHelloWorld::printScore()
{
	const Size desSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();

	mScore = Label::createWithTTF("Your lives: 3", "fonts/Marker Felt.ttf", 24);
	mScore->enableOutline(Color4B::BLACK, 2);

	mScore->setPosition(Vec2(desSize.width / 2,
		desSize.height - mScore->getContentSize().height));

	this->addChild(mScore, 1);
}

void LayerHelloWorld::changeScore()
{
	if (mLivesCounter == 2)
	{
		mScore->setString("Your lives: 2");
	}
	else if (mLivesCounter == 1)
	{
		mScore->setString("Your lives: 1");
	}
	else if (mLivesCounter == 0)
	{
		mScore->setString("You are die!!!");
	}
}


void LayerHelloWorld::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{

	switch (keyCode){
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_A:
	{
		mIsLeftPressed = true;
		break;
	}
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_D:
	{
		mIsRightPressed = true;
		break;
	}
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_W:
	{
		mIsJumpPressed = true;
		break;
	}
	}
}

void LayerHelloWorld::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_A:
	{
		mIsLeftPressed = false;
		break;
	}
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_D:
	{
		mIsRightPressed = false;
		break;
	}
	}
}

bool LayerHelloWorld::onTouchBegan(Touch *touch, Event *unused_event)
{
	bool isTouched = false;
	return isTouched;
}

void LayerHelloWorld::onTouchEnded(Touch *touch, Event *unused_event)
{

}

MenuItemImage* LayerHelloWorld::createRestartIteam()
{
	MenuItemImage* restartItem = MenuItemImage::create(
		RESLIST->getResourcePath("resource_list.json", "b_restart_id"),
		RESLIST->getResourcePath("resource_list.json", "b_restart_small_id"),
		CC_CALLBACK_1(LayerHelloWorld::menuRestartCallback, this));

	const Size desSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	const Size itemSize = restartItem->getContentSize();

	float x = desSize.width - itemSize.width * 2;
	float y = desSize.height - itemSize.height / 2;
	restartItem->setPosition(Vec2(x, y));

	return restartItem;
}

void LayerHelloWorld::menuRestartCallback(Ref* pSender)
{
	Director::getInstance()->popScene();

	auto scene = LayerHelloWorld::createScene();
	Director::getInstance()->pushScene(scene);
}

MenuItemImage* LayerHelloWorld::createCloseIteam()
{
	MenuItemImage* closeItem = MenuItemImage::create(
		RESLIST->getResourcePath("resource_list.json", "b_close_id"),
		RESLIST->getResourcePath("resource_list.json", "b_close_small_id"),
		CC_CALLBACK_1(LayerHelloWorld::menuCloseCallback, this));

	const Size desSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	const Size itemSize = closeItem->getContentSize();

	float x = desSize.width - itemSize.width / 2;
	float y = desSize.height - itemSize.height / 2;
	closeItem->setPosition(Vec2(x, y));

	return closeItem;
}

void LayerHelloWorld::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();
}

void LayerHelloWorld::createMenuBar()
{
	Vector<MenuItem*> menu;

	menu.pushBack(createRestartIteam());
	menu.pushBack(createCloseIteam());
	
	auto menuBar = Menu::createWithArray(menu);
	menuBar->setPosition(Vec2::ZERO);
	this->addChild(menuBar, 1);
}

void LayerHelloWorld::enemyCreator()
{
	const Size desSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();

	Enemy* enemy = Enemy::create();
	enemy->setPosition(desSize.width - enemy->getContentSize().width, mPosYforEnemy);
	addChild(enemy, PLAYER_LAYER + 1);

	registrationEnemy(enemy);
}

void LayerHelloWorld::registrationEnemy(Enemy* node)
{
	mEnemyList.push_back(node);
}

void LayerHelloWorld::deregistrationEnemy(Enemy* node)
{
	auto it = std::find(mEnemyList.begin(), mEnemyList.end(), node);
	if (it != mEnemyList.end())
	{
		mEnemyList.erase(it);
		
	}

}

void  LayerHelloWorld::callEnemyCreator()
{
	if (!mIsGameFinish)
	{
		float delay = RandomHelper::random_real(3.0f, 7.0f); 
		enemyCreator();

		this->runAction(Sequence::create(
			DelayTime::create(delay),
			CallFunc::create(CC_CALLBACK_0(LayerHelloWorld::callEnemyCreator, this)),
			nullptr
			));
	}
}
