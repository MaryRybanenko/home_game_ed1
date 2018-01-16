#include "Player.h"
#include "ResourceList.h"
#include "HelloWorldScene.h"


USING_NS_CC;


Player::Player()
	: mSpeed( Director::getInstance()->getOpenGLView()->getDesignResolutionSize().width * 0.3f )
	, mInitYPos(0.0f)
	, mInitXPos(0.0f)
	, mYValForJump(0.0f)
	, mJumpHight(0.0f)
{}

Player::~Player()
{}

bool Player::init()
{
	bool ret = false;
	if (ret = Node::init())
	{
		const Size designSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
		mInitYPos = designSize.height / 3;
		mInitXPos = designSize.width / 5;

		mYValForJump = mInitYPos;
	}
	return ret;
}

Sprite* Player::getPlayerSprite()
{
	return mPlayer;
}

float Player::getInitYpos()
{
	return mInitYPos;
}

float Player::getInitXpos()
{
	return mInitXPos;
}

float Player::getJumpHight()
{
	return mJumpHight;
}

Player* Player::create()
{
	auto ret = new (std::nothrow) Player;
	if (ret && ret->init()) {
		ret->autorelease();
		ret->mPlayer = Sprite::create(RESLIST->
			getResourcePath("resource_list.json", "player_id"));
		ret->addChild(ret->mPlayer);
		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}

void Player::playerActionRight()
{
	const Size designSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	float currentX = getPositionX();
	if ( currentX < (designSize.width - (mPlayer->getContentSize().width / 2)) )
	{
		const float dt = Director::getInstance()->getDeltaTime();
		
		setPositionX(currentX + mSpeed * dt);
		setRotation(getRotation() + mSpeed * dt);
	}
}

void Player::playerActionLeft()
{
	const Size designSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	float currentX = getPositionX();
	if (currentX > (mPlayer->getContentSize().width / 2))
	{
		const float dt = Director::getInstance()->getDeltaTime();

		setPositionX(currentX - mSpeed * dt );
		setRotation(getRotation() - mSpeed * dt);
	}
}

void Player::playerActionJump()
{
	const float dt = Director::getInstance()->getDeltaTime();
	mJumpHight = mInitYPos + (3 * mPlayer->getContentSize().height);
	if (mYValForJump <= mJumpHight)
	{
		setPositionX(getPositionX() + dt);
		setPositionY(mYValForJump += mSpeed * dt);
		DelayTime::create(3.0f);
	}
}

void Player::playerActionJumpDown()
{
	const float dt = Director::getInstance()->getDeltaTime();

	if (mYValForJump > mInitYPos)
	{
		setPositionY(mYValForJump -= mSpeed * dt);
		DelayTime::create(3.0f);
	}

	if (mYValForJump == mInitYPos)
	{
		setPositionY(mInitYPos);
	}

}

