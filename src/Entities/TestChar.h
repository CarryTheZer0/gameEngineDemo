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
#include "../Components/Fixture.h"
#include "../Components/Sensor.h"

class InputHandler;
class SpriteRenderer;
class Game;
class PhotographSystem;

class Player : public Entity
{
public:
	Player(Game* pGame, SpriteRenderer* pRenderer, InputHandler* pInput, PhotographSystem* pPhoto);
	~Player() = default;

	void init(b2World* pWorld, glm::vec2 pos);
	void update(float deltaTime) override;
	void render(float percent, glm::vec2 camera) override;

	void contactFloor();
	void contactEdge(bool right);
	void endContactEdge();
private:
	SpriteRenderer* m_pRenderer;
	InputHandler* m_pInput;
	Game* m_pGame;
	PhotographSystem* m_pPhoto;
	AnimatedSprite m_sprite;
	Sprite m_spriteHead;
	Body m_body;
	Fixture m_fixture;
	Sensor m_groundCheck;
	Sensor m_leftCheck;
	Sensor m_rightCheck;
	bool m_jumping;
	bool m_snap;
	bool m_facingRight;
	int m_contact;
};

#endif /* ENTITIES_TESTCHAR_H_ */
