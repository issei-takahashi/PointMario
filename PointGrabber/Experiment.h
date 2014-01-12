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
	};
	class Experiment002 : public Experiment
	{
	public:
		void experimentLoop();
	};
};