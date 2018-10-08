#pragma once
//������Ķ�����

//��ṹ
template <typename T>
struct Point
{
	T x;
	T y;
	T z;
	Point() {}//Ҫ��������չ��캯���������������������
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

//��ṹ������������
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