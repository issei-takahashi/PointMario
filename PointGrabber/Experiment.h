#pragma once

#include "Coordinate.h"
#include "SerializableData.h"

namespace mario
{
	/* �����̒��ۃN���X */
	class Experiment
	{
	public:
		virtual void experimentLoop() = 0;
	};

	/* ����1 */
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
	
	/* ����2 */
	class Experiment002 : public Experiment
	{
	public:
		void experimentLoop();
	};

	/* ����101(�ԐF4�_���o�e�X�g���[�h) */
	class Experiment101 : public Experiment
	{
	public:
		void experimentLoop();
	};
};