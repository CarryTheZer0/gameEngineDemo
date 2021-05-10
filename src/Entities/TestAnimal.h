/*
 * testAnimal.h
 *
 *  Created on: 23 Apr 2021
 *      Author: mchlp
 */

#ifndef ENTITIES_TESTANIMAL_H_
#define ENTITIES_TESTANIMAL_H_

#include "../Entity.h"
#include "../Components/AnimatedSprite.h"
#include "../Components/Body.h"
#include "../Components/Fixture.h"
#include "../Components/Sensor.h"

class SpriteRenderer;
class Player;

class Animal : public Entity
{
public:
	Animal(SpriteRenderer* pRenderer, Player* pPlayer);
	~Animal() = default;

	void init(b2World* pWorld, glm::vec2 pos);
	void update(float deltaTime) override;
	void render(float percent, glm::vec2 camera) override;

	void proc();
	void charge();

	void contactEdge(bool right);
	void endContactEdge();
private:
	SpriteRenderer* m_pRenderer;
	AnimatedSprite m_sprite;
	Sensor m_leftCheck;
	Sensor m_rightCheck;
	Player* m_pPlayer;
	Body m_body;
	bool m_charging;
	int m_contact;
	bool m_facingRight;
	Fixture m_fixture;
	glm::vec2 m_spawnPos;
};

#endif /* ENTITIES_TESTANIMAL_H_ */
