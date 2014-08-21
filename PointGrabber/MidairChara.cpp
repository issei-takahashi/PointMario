#include "MidairChara.h"
#include "Animation.h"
#include "Application.h"
#include "DownOutMeasure.h"
#include "utils.h"
#include "FileIO.h"

using namespace mario;

MidairChara::MidairChara( shared_ptr<Application> _owner, shared_ptr<DownOutMeasure> _base, string const & _folderPath )
	:PointBody(_owner), measureBase(_base),
	frameToJump(0), jumpingFrame(0)
{
	list<string> fileList;
	utils::getFileList(_folderPath,fileList); // フォルダのリストを取得
	if( fileList.size() > 0 ){
		foreach(it,fileList){ // 各フォルダに対して
			if( !IS_IMAGE_FILE(*it) ){
				this->motions[*it] = Animation::makeShared(_folderPath+*it+"/"); // アニメーションを作って登録
			}
		}
		this->currentMotion = this->motions.begin();
	}
}

shared_ptr<MidairChara> MidairChara::makeShared( shared_ptr<Application> _owner, shared_ptr<DownOutMeasure> _base, string const & _folderPath )
{
	return (shared_ptr<MidairChara>)(new MidairChara( _owner, _base, _folderPath) );
}

priority MidairChara::getPriority() const
{
	return 1;
}

void MidairChara::oneLoop( uint _x, uint _y ) 
{
	/* 計測データを元に移動処理 */
	if( auto sp = this->measureBase.lock() ){
		// 定数
		once_double resolution = FileIO::getConst("OCTREE_RESOLUTION");
		once_double radius = FileIO::getConst("OCTREE_RADIUS");
		once_double K = FileIO::getConst("OCTREE_K");
		once_double distance = FileIO::getConst("SIMPLE_SEARCH_DISTANCE");
		once_double GRAVITY = FileIO::getConst("GRAVITY");
		once_int WAIT_FRAME = FileIO::getConst("WAIT_FRAME");
		once_int FLYING_FRAME = FileIO::getConst("FLYING_FRAME");
		once_int NO_COLLISION_FRAME = FileIO::getConst("NO_COLLISION_FRAME");
		// 計測データから最高点データとキャラクター近傍データを抜き出し（1回のみ）
		Coordinate<typeD> highest;
		bool hit = sp->simpleCollisionDetectionAndGetHighestPoint(*this,distance,highest);
		/* 最高点目指して移動 */
		if( ++this->frameToJump > WAIT_FRAME ){
			cout << "highest : " << highest.x << "," << highest.y << "," << highest.z << endl;
			this->frameToJump = 0;
			this->jumpingFrame = 0;
			auto thispos = this->getPoint();
			Eigen::Vector3d vel;
			vel.x() = (highest.x-thispos.x)/FLYING_FRAME;
			vel.y() = (GRAVITY*FLYING_FRAME)/2 + (highest.y-thispos.y)/FLYING_FRAME;
			vel.z() = (highest.z-thispos.z)/FLYING_FRAME;
			this->setVelocity( vel );
		}else{
			auto thisvel = this->getVelocity();
			if( ++this->jumpingFrame > NO_COLLISION_FRAME && thisvel.y() <= 0 && hit ){
				this->setVelocity(0,0,0);
			}else{
				thisvel.y() -= GRAVITY;
				this->setVelocity(thisvel);
			}
		}
	}
	// Body側のループ
	PointBody::oneLoop();
	// 画面外にいたら座標設定
	auto p = this->PointBody::getPoint();
	p.x = min(330.0,max(20.0,p.x));
	p.y = min(290.0,max(10.0,p.y));
	p.z = min(290.0,max(10.0,p.z));
	this->PointBody::setPoint(p);
	// Body座標を描画座標に同期して描画
	if( this->currentMotion != this->motions.end() ){
		this->setDisplayPoint(this->PointBody::getPoint());
		this->currentMotion->second->oneLoop(_x,_y);
	}
}

void MidairChara::setWindow( shared_ptr<Window> _owner )
{
	foreach(it,this->motions){
		it->second->setWindow(_owner);
	}
}