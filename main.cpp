#include<iostream>
#include<vector>
#include<time.h>
#include"bsp_tree.h"
using namespace std;
//�����bsp��ԭ��ο���http://gad.qq.com/article/detail/22133
int main(void)
{
	BspTree<float> bspTree;

	//����һЩ��Ƭ
	vector<Face<float>> face_vec;
	srand(time(0));//srand()�Ǹ������������һ�����ӣ�time(NULL)���ǰѵ�ǰ��ʱ��ֵ��Ϊ���ӡ� ����������ӣ�����������Ŀ��ܾͻ�������
	for(int i = 0;i<16;++i)
	{
		Face<float> face;
		for(int j = 0;j<3;++j)
		{
			face.point[j] = Point<float>(-200 + rand() % 400, -200 + rand() % 400,-200 + rand() % 400);
		}
		face_vec.push_back(face);
	}
	//��ʼ��bsp��������
	bspTree.InitBspTree(face_vec, Point<float>(-200, -200, -200), Point<float>(200, 200, 200), 5);
	bspTree.TraverseBspTree();
	bspTree.DeleteBspTree();
	system("pause");
	return 0;
}