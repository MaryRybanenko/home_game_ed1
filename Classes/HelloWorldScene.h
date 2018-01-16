#ifndef __LAYER_HELLO_WORLD_H__
#define __LAYER_HELLO_WORLD_H__

#include "cocos2d.h"
#include "cocos2d\external\json\rapidjson.h"
#include "cocos2d\external\json\reader.h"
#include "cocos2d\external\json\document.h"

#include "Player.h"
#include "Enemy.h"


#include <map>

NS_CC_BEGIN

class LayerHelloWorld : public Layer
{
	typedef Layer Parent;
private:

	Player* mPlayerObj;
	Enemy* mEnemyObj;
	Enemy* mPrevEnemyObj;


	int mLivesCounter;
	float mPosY;
	float mPosYforEnemy;

	float mdelayTime;
	bool mIsCanJump;
	bool mIsGameFinish;
	bool mIsAnimationEnd;
	bool mIsTouched;
	float mInitYForGrass;

	DrawNode* mDebugDrawNode;

	Sprite* mPlayer;
	Sprite* mGrass;
	Sprite* mGrassClone;



	Label* mScore;

	//std::list< Node* > mEnemyList;
	std::list< Enemy* > mEnemyList;

	Action* seq;

	Vec2 mGamePlayerPos;

	LayerHelloWorld();

	virtual bool onTouchBegan(Touch *touch, Event *unused_event) override;
	virtual void onTouchEnded(Touch *touch, Event *unused_event) override;

	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) override;
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;

	void allowJump();
	void jumpActionUP();

	void printScore();

	void playerMovement();
	void animationState();
	void landMovement();
	void landMovementClone();
	//bool isSpritesIntersects(Rect sprite1, Rect sprite2);
	void changeScore();
	MenuItemImage* createCloseIteam();
	MenuItemImage* createRestartIteam();
	virtual void update(float delta) override;

	void enemyCreator();
	void currentEnemyMovement(Sprite* Enemy);
	void callEnemyCreator();

	void registrationEnemy(Enemy* node);
	void deregistrationEnemy(Enemy* node);

	void createMenuBar();

	void readJson();

	bool mIsLeftPressed;
	bool mIsRightPressed;
	bool mIsJumpPressed;

public:
	virtual ~LayerHelloWorld();

	static Scene* createScene();
	virtual bool init();
	void menuCloseCallback(Ref* pSender);
	void menuRestartCallback(Ref* pSender);
	CREATE_FUNC(LayerHelloWorld);



};

NS_CC_END

#endif