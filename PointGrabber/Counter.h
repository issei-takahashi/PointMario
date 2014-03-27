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
	// <演算子オーバーロード
	bool operator<(const Counter<T>& obj) const
	{
		return this->instanceCount < obj.instanceCount;
	}
	// >演算子オーバーロード
	bool operator>(const Counter<T>& obj) const
	{
		return this->instanceCount > obj.instanceCount;
	}
	// =演算子オーバーロード
	Counter<T>& operator=(const Counter<T>& obj)
	{
		this->instanceCount = obj.instanceCount;
		return *this;
	}

	// ==演算子オーバーロード
	bool operator==(const Counter<T>& obj) const
	{
		return *this == obj;
	}

	static Counter<T> Zero()
	{
		return Counter<T>(0);
	}
private:
	//Counter(int _count):instanceCount(_count){}
	int const instanceCount;
	static int totalCount;
};

template<class T>
int Counter<T>::totalCount = 1;