#pragma once

class Application
{
public:
	virtual ~Application() = default;
	virtual void Tick(float dt) = 0;
	virtual void Render() = 0;
	virtual void Inspect() = 0;

	virtual void Start() = 0;
	virtual void End() = 0;
};

