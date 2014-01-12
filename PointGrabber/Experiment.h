#pragma once

namespace mario
{
	class Experiment
	{
	public:
		virtual void experimentLoop() = 0;
	};
	class Experiment001 : public Experiment
	{
	public:
		void experimentLoop();
	private:
		void inputRealPositionLoop();
		void measureRedPointsLoop();
		void showCrossAndRegisterCrossLoop();
	};
	class Experiment002 : public Experiment
	{
	public:
		void experimentLoop();
	};
};