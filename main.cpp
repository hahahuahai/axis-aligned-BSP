#include<iostream>
#include<vector>
#include<time.h>
#include"bsp_tree.h"
using namespace std;
//轴对齐bsp的原理参考：http://gad.qq.com/article/detail/22133
int main(void)
{
	BspTree<float> bspTree;

	//创造一些面片
	vector<Face<float>> face_vec;
	srand(time(0));//srand()是给随机数产生器一个种子，time(NULL)就是把当前的时间值作为种子。 如果不设种子，随机数产生的可能就会有问题
	for(int i = 0;i<16;++i)
	{
		Face<float> face;
		for(int j = 0;j<3;++j)
		{
			face.point[j] = Point<float>(-200 + rand() % 400, -200 + rand() % 400,-200 + rand() % 400);
		}
		face_vec.push_back(face);
	}
	//初始化bsp树并遍历
	bspTree.InitBspTree(face_vec, Point<float>(-200, -200, -200), Point<float>(200, 200, 200), 5);
	bspTree.TraverseBspTree();
	bspTree.DeleteBspTree();
	system("pause");
	return 0;
}