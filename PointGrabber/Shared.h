#pragma once

template<class T>
class Shared
{
	friend T;
	friend class shared_ptr<T>;
	friend class unique_ptr<T>;
public:
	typedef shared_ptr<T> Ptr;
	typedef weak_ptr<T> wPtr;
	typedef unique_ptr<T> uPtr;
private:
	Shared(){}
};