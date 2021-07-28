/*
 * MushroomMonster.h
 *
 *  Created on: 7 Jun 2021
 *      Author: mchlp
 */

#ifndef ENTITIES_MUSHROOMMONSTER_H_
#define ENTITIES_MUSHROOMMONSTER_H_

#include "../Entity.h"
#include "../Components/AnimatedSprite.h"
#include "../Components/Body.h"
#include "../Components/BoxCollider.h"
#include "../Components/CircleCollider.h"
#include "../Components/Sensor.h"
#include "../Components/Photographable.h"

class SpriteRenderer;
class DebugRenderer;
class Player;

class MushroomMonster : public Entity
{
public:
	MushroomMonster(GameplayScene* pParentScene, SpriteRenderer* pRenderer, DebugRenderer* pDebug);
	~MushroomMonster() = default;

	void init(b2World* pWorld, glm::vec2 pos, DebugRenderer* pDebug, bool facingRight = false);
	void update(float deltaTime) override;
	void camUpdate() override {}
	void render(float percent, glm::vec2 camera, float scale) override;

	void flipX();

	void idle();
	void walk();
	void graze();

	void contactEdge(Fixture* contact);
	void endContactEdge(Fixture* contact);

	void contactJump(Fixture* contact);
	void endContactJump(Fixture* contact);
private:
	AnimatedSprite m_sprite;
	Body m_body;
	BoxCollider m_colliderMain;
	CircleCollider m_colliderCircle;
	Sensor m_frontCheck;
	Sensor m_colliderJump;
	Photographable m_photographable;

	bool m_facingRight;
	bool m_contact;

	float m_timeElapsed;

	enum State {IDLE, WALK, GRAZE};
	State m_state;

	Player* m_jumpTarget;
};

#endif /* ENTITIES_MUSHROOMMONSTER_H_ */
