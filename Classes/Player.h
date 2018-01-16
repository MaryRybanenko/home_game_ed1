#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"


NS_CC_BEGIN

class Player : public Node
{
public:

	Player::Player();
	Player::~Player();
	static Player* create();
	bool Player::init();

	float Player::getInitYpos();
	float Player::getInitXpos();
	float Player::getJumpHight();
	Sprite* Player::getPlayerSprite();

	void Player::playerActionRight();
	void Player::playerActionLeft();
	void Player::playerActionJump();
	void Player::playerActionJumpDown();

	
private:
	Sprite* mPlayer;

	float mInitYPos;
	float mInitXPos;
	float mJumpHight;

	float mSpeed;
	float mYValForJump;
};


NS_CC_END
#endif