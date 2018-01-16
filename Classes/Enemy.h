#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
#include "Player.h"
NS_CC_BEGIN

class Enemy : public Node
{
public:

	Enemy::Enemy();
	Enemy::~Enemy();
	static Enemy* create();
	bool init();

	Sprite* Enemy::getEnemySprite();

	void enemyAction();
	bool Enemy::isSpritesIntersects(Rect playerBox, Rect enemyBox, Player* player);

private:
	Sprite* mEnemy;
	float mSpeed;

};

NS_CC_END
#endif