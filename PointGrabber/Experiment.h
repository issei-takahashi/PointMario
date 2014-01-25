#pragma once

#include "Coordinate.h"
#include "SerializableData.h"

namespace mario
{
	/* 実験の抽象クラス */
	class Experiment
	{
	public:
		virtual void experimentLoop() = 0;
	};

	/* 実験1 */
	class Experiment001 : public Experiment
	{
	public:
		void experimentLoop();
	private:
		bool inputContinueLoop();
		string inputFileNameLoop();
		Coordinate<typeR> inputRealPositionLoop();
		Coordinate<typeM> measureRedPointsLoop( vector<Coordinate<typeM> > * _pDst, Coordinate<typeM> const & _beforepM );
		Coordinate<typeD> showCrossAndRegisterCrossLoop( Coordinate<typeD> const & _beforepD );
	};
	
	/* 実験2(実験1データから変換行列を計算する) */
	class Experiment002 : public Experiment
	{
	public:
		void experimentLoop();
	private:
		string inputFileNameLoop();
		char inputCoordinateTypeLoop( string const & _message );
	};

	/* 実験101(赤色4点抽出テストモード) */
	class Experiment101 : public Experiment
	{
	public:
		void experimentLoop();
	};
};