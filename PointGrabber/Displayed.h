#pragma once

class Displayed abstract
{
public:
	virtual int getPriority() const;
	virtual void oneLoop();
};