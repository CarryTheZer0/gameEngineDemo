/*
 * AnimatedSprite.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mchlp
 */

#ifndef COMPONENTS_ANIMATEDSPRITE_H_
#define COMPONENTS_ANIMATEDSPRITE_H_


#include <unordered_map>
#include <string>

#include "Sprite.h"

struct Animation
{
	int frameCount;
	glm::vec4 origin;
	glm::vec2 offset;
};

class AnimatedSprite : public Sprite
{
public:
	AnimatedSprite() = default;
	AnimatedSprite(Entity* pOwner, SpriteRenderer* pRenderer, const char* name, glm::vec4 srcRect,
		float timeToUpdate, float scale = 0.0f);
	~AnimatedSprite() = default;

	void update(float deltaTime);
	void render(float percent, glm::vec2 camera) override;

	void setCurrentAnimation(const char* name);
	void addAnimation(glm::vec4 origin, int frameCount, std::string name);
	void playAnimation(const char* name, bool loop = true);

	glm::vec2 getDimensions() override;
private:
	int m_frame;
	float m_elapsedTime;
	float m_timeToUpdate;
	bool m_loop;
	std::string m_currentAnimation;

	std::unordered_map<std::string, Animation> m_animations;
};


#endif /* COMPONENTS_ANIMATEDSPRITE_H_ */
