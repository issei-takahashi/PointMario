#pragma once 


namespace mario
{
	class Main{
	public:
		static int get_mode() { return Main::mode; }
	private:
		static int mode;
	public:
		static void init();
		static int main();
	};
};
