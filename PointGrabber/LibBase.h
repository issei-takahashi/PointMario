#pragma once

class LibBase
{
public:
	static void initLib();
	static void quitLib();
private:
	static bool initFlag;
	static bool quitFlag;
};