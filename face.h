#pragma once
//被管理的对象类

//点结构
template <typename T>
struct Point
{
	T x;
	T y;
	T z;
	Point() {}//要声明这个空构造函数，否则下面的类编译出错
	Point(T _x,T _y,T _z):x(_x),y(_y),z(_z){}
	Point(const Point<T> &point)
	{
		x = point.x;
		y = point.y;
		z = point.z;
	}
	Point &operator+=(const T &n)
	{
		x += n;
		y += n;
		z += n;
		return *this;
	}
};

//面结构，带三个顶点
template <typename T>
struct Face
{
	Point<T> point[3];
	void operator+=(const T &offset)
	{
		for (int i = 0; i < 3; ++i)
			point[i] += offset;
	}
};