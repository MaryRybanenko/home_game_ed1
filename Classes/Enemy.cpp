#include "Enemy.h"
#include "ResourceList.h"
#include "HelloWorldScene.h"

USING_NS_CC;

Enemy::Enemy()
	: mSpeed(Director::getInstance()->getOpenGLView()->getDesignResolutionSize().width * 0.2f)
{}

Enemy::~Enemy()
{}

bool Enemy::init()
{
	bool ret = false;
	if (ret = Node::init())
	{

	}
	return ret;
}

Enemy* Enemy::create()
{
	auto ret = new (std::nothrow) Enemy;
	if (ret && ret->init()) {
		ret->autorelease();
		ret->mEnemy = Sprite::create(RESLIST->
			getResourcePath("resource_list.json", "enemy_id"));
		ret->addChild(ret->mEnemy);
		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}

Sprite* Enemy::getEnemySprite()
{
	return mEnemy;
}
void Enemy::enemyAction()
{
	float currentX = getPositionX();
	const float dt = Director::getInstance()->getDeltaTime();

	setPositionX(currentX - mSpeed * dt);

	if (currentX == (-getContentSize().width))
	{
		setPositionX(Director::getInstance()->getOpenGLView()->getDesignResolutionSize().width);
	}
}

bool Enemy::isSpritesIntersects(Rect playerBox, Rect enemyBox, Player* player)
{

	float delta = 10.0f;
	Vec2 playerCenterPoint = player->getPosition();
	float playerR = player->getPlayerSprite()->getContentSize().width / 2;

	Vec2 controlEnemyP1 = Vec2(enemyBox.getMinX() + delta, enemyBox.getMinY() + delta);
	Vec2 controlEnemyP2 = Vec2(enemyBox.getMidX() - delta, enemyBox.getMidY());
	Vec2 controlEnemyP3 = Vec2(enemyBox.getMidX(), enemyBox.getMidY() + delta);
	Vec2 controlEnemyP4 = Vec2(enemyBox.getMaxX(), enemyBox.getMaxY());

#if DEBUG_DRAW_ON
	mDebugDrawNode->drawCircle(controlEnemyP1, 5, 0, 10, 0, Color4F::RED);
	mDebugDrawNode->drawCircle(controlEnemyP2, 5, 0, 10, 0, Color4F::RED);
	mDebugDrawNode->drawCircle(controlEnemyP3, 5, 0, 10, 0, Color4F::RED);
	mDebugDrawNode->drawCircle(controlEnemyP4, 5, 0, 10, 0, Color4F::RED);
#endif

	return (playerCenterPoint.distance(controlEnemyP1) <= playerR
		|| playerCenterPoint.distance(controlEnemyP2) <= playerR
		|| playerCenterPoint.distance(controlEnemyP3) <= playerR
		|| playerCenterPoint.distance(controlEnemyP4) <= playerR);
}