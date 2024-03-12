#pragma once

#include <Engine.h>

class FlappyBirdApp : public Engine::App
{
public:
	FlappyBirdApp();
	~FlappyBirdApp();
	virtual void Start();
	virtual void Update();
};
