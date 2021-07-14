/*
 * TestChar.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mchlp
 */

#ifndef ENTITIES_TESTCHAR_H_
#define ENTITIES_TESTCHAR_H_

#include <glm/glm.hpp>
#include "../Entity.h"
#include "../Components/AnimatedSprite.h"
#include "../Components/Sprite.h"
#include "../Components/Body.h"
#include "../Components/BoxCollider.h"
#include "../Components/CircleCollider.h"
#include "../Components/Sensor.h"
#include "../Camera.h"

class InputHandler;
class SpriteRenderer;
class DebugRenderer;
class GameplayScene;
class Game;
class PhotographSystem;

enum CharState
{
	idle, run, freefall, photo
};

class Player : public Entity
{
public:
	Player(Game* pGame, GameplayScene* pScene, SpriteRenderer* pRenderer, DebugRenderer* pDebug, InputHandler* pInput, PhotographSystem* pPhoto);
	~Player() = default;

	void init(b2World* pWorld, glm::vec2 pos, DebugRenderer* pDebug);
	void update(float deltaTime) override;
	void camUpdate() override;
	void render(float percent, glm::vec2 camera, float scale) override;

	void jump(float yForce = 300.0f);
	void reset();
	void flipX(bool facingRight);
	void takePhoto();

	void contactFloor(Fixture* contact);
	void endContactFloor(Fixture* contact);
	void contactEdge(Fixture* contact, bool right);
	void endContactEdge(Fixture* contact, bool right);
private:
	InputHandler* m_pInput;
	Game* m_pGame;
	PhotographSystem* m_pPhoto;

	AnimatedSprite m_sprite;
	Sprite m_spriteHead;
	Body m_body;
	BoxCollider m_colliderMain;
	CircleCollider m_colliderCircle;
	Sensor m_groundCheck;
	Sensor m_leftCheck;
	Sensor m_rightCheck;

	bool m_grounded;
	bool m_snap;
	bool m_facingRight;
	bool m_shouldReset;
	int m_contactRight;
	int m_contactLeft;

	glm::vec2 m_mouseVector;
	glm::vec2 m_lookDirection;
};

#endif /* ENTITIES_TESTCHAR_H_ */
