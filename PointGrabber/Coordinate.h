#pragma once

namespace mario
{
	typedef double typeR;
	typedef double typeD;
	typedef double typeM;

	/*---------------------- Coordinate----------------------------

	3�������W�̔ėp�N���X

	--------------------------------------------------------------------------*/

	template <class T>
	class Coordinate
	{
		friend class boost::serialization::access;

	public:
		//���W�l
		T x,y,z;

		//�V���A���C�Y�p
		template<class Archive>
		void serialize( Archive& ar, unsigned int ver )
		{
			ar & boost::serialization::make_nvp("x", this->x);
			ar & boost::serialization::make_nvp("y", this->y);
			ar & boost::serialization::make_nvp("z", this->z);
		}

		double distance( Coordinate<T> const & p ) const 
		{
			return sqrt( powf( (float)( this->x - p.x ), 2 ) +
				powf( (float)( this->y - p.y ), 2 )  +
				powf( (float)( this->z - p.z ), 2 ) );
		}

		//�R���X�g���N�^
		Coordinate()
		{
			x = 0;
			y = 0;
			z = 0;
		}

		//�R���X�g���N�^
		Coordinate(const T& t1, const T& t2, const T& t3)
		{
			x = t1;
			y = t2;
			z = t3;
		}

		// +���Z�q�I�[�o�[���[�h
		Coordinate<T> operator+(const Coordinate<T>& obj) const
		{
			Coordinate tmp;
			tmp.x = x + obj.x;
			tmp.y = y + obj.y;
			tmp.z = z + obj.z;
			return tmp;
		}


		// -���Z�q�I�[�o�[���[�h
		Coordinate<T> operator-(const Coordinate<T>& obj) const
		{
			Coordinate tmp;
			tmp.x = x - obj.x;
			tmp.y = y - obj.y;
			tmp.z = z - obj.z;
			return tmp;
		}

		// *���Z�q�I�[�o�[���[�h
		Coordinate<T> operator*(const double& d) const
		{
			Coordinate tmp;
			tmp.x = (T)(this->x * d);
			tmp.y = (T)(this->y * d);
			tmp.z = (T)(this->z * d);
			return tmp;
		}

		// /���Z�q�I�[�o�[���[�h
		Coordinate<T> operator/(const double& d) const
		{
			Coordinate tmp;
			tmp.x = (T)(this->x / d);
			tmp.y = (T)(this->y / d);
			return tmp;
		}

		// +=���Z�q�I�[�o�[���[�h
		Coordinate<T>& operator+=(const Coordinate<T>& obj)
		{
			x += obj.x;
			y += obj.y;
			z += obj.z;
			return *this;
		}

		// -=���Z�q�I�[�o�[���[�h
		Coordinate<T>& operator-=(const Coordinate<T>& obj)
		{
			x -= obj.x;
			y -= obj.y;
			z -= obj.z;
			return *this;
		}

		// *=���Z�q�I�[�o�[���[�h
		Coordinate<T>& operator*=(const double d)
		{
			(T)(this->x *= d);
			(T)(this->y *= d);
			(T)(this->z *= d);
			return *this;
		}

		// /=���Z�q�I�[�o�[���[�h
		Coordinate<T>& operator/=(const double d)
		{
			(T)(this->x /= d);
			(T)(this->y /= d);
			(T)(this->z /= d);
			return *this;
		}

		// =���Z�q�I�[�o�[���[�h
		Coordinate<T>& operator=(const Coordinate<T>& obj)
		{
			x = obj.x;
			y = obj.y;
			z = obj.z;
			return *this;
		}

		// ==���Z�q�I�[�o�[���[�h
		bool operator==(const Coordinate<T>& obj) const
		{
			if( x == obj.x && y == obj.y && z == obj.z)
				return true;
			else
				return false;
		}

		// !=���Z�q�I�[�o�[���[�h
		bool operator!=(const Coordinate<T>& obj) const
		{
			if( x != obj.x || y != obj.y || z != obj.z )
				return true;
			else
				return false;
		}

		/* Eigen �Ƃ̌݊��� */

		// +���Z�q�I�[�o�[���[�h
		Coordinate<T> operator+(const Eigen::Vector3d& obj) const
		{
			Coordinate tmp;
			tmp.x = x + obj(0);
			tmp.y = y + obj(1);
			tmp.z = z + obj(2);
			return tmp;
		}


		// -���Z�q�I�[�o�[���[�h
		Coordinate<T> operator-(const Eigen::Vector3d& obj) const
		{
			Coordinate tmp;
			tmp.x = x - obj(0);
			tmp.y = y - obj(1);
			tmp.z = z - obj(2);
			return tmp;
		}

		// +=���Z�q�I�[�o�[���[�h
		Coordinate<T>& operator+=(const Eigen::Vector3d& obj)
		{
			x += obj(0);
			y += obj(1);
			z += obj(2);
			return *this;
		}

		// -=���Z�q�I�[�o�[���[�h
		Coordinate<T>& operator-=(const Eigen::Vector3d& obj)
		{
			x -= obj(0);
			y -= obj(1);
			z -= obj(2);
		}	
	};

};