/*
 * Charger.h
 *
 *  Created on: 23 Apr 2021
 *      Author: mchlp
 */

#ifndef ENTITIES_CHARGER_H_
#define ENTITIES_CHARGER_H_

#include "../Entity.h"
#include "../Components/AnimatedSprite.h"
#include "../Components/Body.h"
#include "../Components/BoxCollider.h"
#include "../Components/CircleCollider.h"
#include "../Components/Sensor.h"

class SpriteRenderer;
class DebugRenderer;
class Player;

class Charger : public Entity
{
public:
	Charger(SpriteRenderer* pRenderer, DebugRenderer* pDebug, Player* pPlayer);
	~Charger() = default;

	void init(b2World* pWorld, glm::vec2 pos, DebugRenderer* pDebug, bool facingRight = false);
	void update(float deltaTime) override;
	void camUpdate() override {}
	void render(float percent, glm::vec2 camera, float scale) override;

	void proc();
	void charge();
	void setFacingRight(bool facing);

	void contactEdge(Fixture* contact);
	void endContactEdge(Fixture* contact);
private:
	SpriteRenderer* m_pRenderer;
	Player* m_pPlayer;

	AnimatedSprite m_sprite;
	Body m_body;
	BoxCollider m_colliderMain;
	CircleCollider m_colliderCircle;
	Sensor m_frontCheck;
	Sensor m_hurtBox;

	glm::vec2 m_spawnPos;
	bool m_charging;
	bool m_facingRight;
	bool m_contact;
};

#endif /* ENTITIES_CHARGER_H_ */
