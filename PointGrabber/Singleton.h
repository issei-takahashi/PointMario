#pragma once

template< class T >
class Singleton{
public:
	static shared_ptr<T> getInstance()
	{
		static shared_ptr<T> instance = (shared_ptr<T>)(new T());
		return instance;
	}
private:
	// ÉRÉsÅ[ã÷é~
	Singleton( const T& other ){}
	Singleton& operator = ( const Singleton& other ){}
protected:
	Singleton(){}
	virtual ~Singleton(){}
};
