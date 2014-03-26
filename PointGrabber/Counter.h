#pragma once

template<class T>
class Counter
{
public:
	Counter()
		:instanceCount(Counter<T>::totalCount++){}
	Counter<T> getCount() const
	{
		return *this;
	}
	// <���Z�q�I�[�o�[���[�h
	bool operator<(const Counter<T>& obj) const
	{
		return this->instanceCount < obj.instanceCount;
	}
	// >���Z�q�I�[�o�[���[�h
	bool operator>(const Counter<T>& obj) const
	{
		return this->instanceCount > obj.instanceCount;
	}
	static Counter<T> Zero()
	{
		return Counter<T>(0);
	}
private:
	Counter(int _count):instanceCount(_count){}
	int const instanceCount;
	static int totalCount;
};

template<class T>
int Counter<T>::totalCount = 1;