#pragma once
//BSP��
//����AABB��Χ�зָ��������з��ᣬȻ��ѳ�����Ƭ�ݹ�ػ��ֵ������ӽڵ㣬һ��ʼ�ͻ�����ϣ���Ҫ����Ϊ��̬��������
#include<vector>
#include"face.h"

enum Axis
{
	AXIS_X,
	AXIS_Y,
	AXIS_Z
};

template <typename T>
struct BspTreeNode
{
	std::vector<Face<T>> faces;
	Axis axis;
	int depth;
	T splitter;//ĳ������ķָ�ֵ
	Point<T> minPoint;//�ڵ�İ�Χ�ж���
	Point<T> maxPoint;
	BspTreeNode *lChild;
	BspTreeNode *rChild;
	BspTreeNode() :lChild(nullptr), rChild(nullptr) {}

};

template <typename T>
class BspTree
{
public:
	BspTree();
	~BspTree();
public:
	//���������Լ�����AABB��Χ�У���min��max������������
	void InitBspTree(std::vector<Face<T>> &_faces, Point<T>  min_point, Point<T> max_point, int max_depth);
	void DeleteBspTree();
	void TraverseBspTree() const;
private:
	void cutFace(const Face<T> &face, Axis axis, const T &splitter, int &leftCount, int &rightCount, int &bothCount);
	void splitSpace(BspTreeNode<T> *node, Axis axis, int depth);
	void traverse(BspTreeNode<T> *node)const;
	void deleteTree(BspTreeNode<T> *root);
private:
	BspTreeNode<T> *root;//���ڵ�
	int maxDepth;//���ݹ���ȣ���ֹ��ջ���
};

template <typename T>
BspTree<T>::BspTree()
{

}
template <typename T>
BspTree<T>::~BspTree()
{

}

template <typename T>
void BspTree<T>::InitBspTree(std::vector<Face<T>>& _faces, Point<T> min_point, Point<T> max_point, int max_depth)
{
	maxDepth = max_depth;
	root = new BspTreeNode<T>;
	root->depth = 1;
	root->faces = _faces;
	root->lChild = nullptr;
	root->rChild = nullptr;

	root->maxPoint = max_point;
	root->minPoint = min_point;
	//��ʼ����Ϳ�ʼ�ָ����г���
	splitSpace(root, AXIS_X, 1);
}

template <typename T>
void BspTree<T>::deleteTree(BspTreeNode<T>* root)
{
	if (root)
	{
		deleteTree(root->lChild);
		deleteTree(root->rChild);
		delete root;
		root = nullptr;
	}
}

template <typename T>
void BspTree<T>::DeleteBspTree()
{
	deleteTree(root);
}

template <typename T>
void BspTree<T>::cutFace(const Face<T>& face, Axis axis, const T& splitter, int& leftCount, int& rightCount, int& bothCount)
{
	T p[3];//��¼ĳ������ĵ����
	switch (axis)
	{
	case AXIS_X:
		p[0] = face.point[0].x;
		p[1] = face.point[1].x;
		p[2] = face.point[2].x;
		break;
	case AXIS_Y:
		p[0] = face.point[0].y;
		p[1] = face.point[1].y;
		p[2] = face.point[2].y;
		break;
	case AXIS_Z:
		p[0] = face.point[0].z;
		p[1] = face.point[1].z;
		p[2] = face.point[2].z;
		break;
	}
	//�Էָ������ߵĶ������	
	for (int i = 0; i < 3; ++i)
	{
		if (p[i] < splitter)
			leftCount++;
		else if (p[i] > splitter)
			rightCount++;
		else if (fabs(p[i] - splitter) < 1e-6)				
			bothCount++;
	}
}

template <typename T>void BspTree<T>::splitSpace(BspTreeNode<T> *node, Axis axis, int depth) {	//�ڵ�ǰ��ڵ����ָ��Ϊ�²��ӽڵ���������� 
	if(!node) 
		return;	
	node->axis=axis;
	node->depth=depth;
	if(depth==maxDepth)	
		return;	
	if(node->faces.size()<2) //�ϲ�ڵ�ӵ�е���Ƭ��̫�پͲ��ٻ���		
		return; 	
	node->lChild=new BspTreeNode<T>;
	node->rChild=new BspTreeNode<T>;	
	//�ȸ��ӽڵ��Χ�и�ֵ	
	node->lChild->maxPoint=node->maxPoint;
	node->lChild->minPoint=node->minPoint;
	node->rChild->maxPoint=node->maxPoint;	
	node->rChild->minPoint=node->minPoint; 
	T xLen=node->maxPoint.x-node->minPoint.x;
	T yLen=node->maxPoint.y-node->minPoint.y;	
	T zLen=node->maxPoint.z-node->minPoint.z;	
	//���øýڵ�Ļ��ֵ�
	Axis mAxis=AXIS_X;	
	if(yLen>xLen&&yLen>zLen)	
		mAxis=AXIS_Y;
	if(zLen>xLen&&zLen>yLen)	
		mAxis=AXIS_Z;	
	switch(mAxis)	
	{
		case AXIS_X:	
		node->splitter=(node->maxPoint.x+node->minPoint.x)/2; 
		//ȡ�м�ֵ	
		//�޸��ӽڵ�İ�Χ��ֵ	
		node->lChild->maxPoint.x=node->splitter;	
		node->rChild->minPoint.x=node->splitter;	
		break;	
		case AXIS_Y:	
		node->splitter=(node->maxPoint.y+node->minPoint.y)/2;
		node->lChild->maxPoint.y=node->splitter;	
		node->rChild->minPoint.y=node->splitter;	
		break;	
		case AXIS_Z:	
		node->splitter=(node->maxPoint.z+node->minPoint.z)/2;	
		node->lChild->maxPoint.z=node->splitter;	
		node->rChild->minPoint.z=node->splitter;	
		break;
	} 	

	for(int i=0;i<node->faces.size();++i)	
	{		
		int leftCount = 0;
		int rightCount = 0;
		int bothCount = 0;
		//��ÿ�������зֵ��ӽڵ�	
		cutFace(node->faces[i],mAxis,node->splitter,leftCount,rightCount,bothCount);	
		if(leftCount||bothCount)	
			node->lChild->faces.push_back(node->faces[i]);	
		if(rightCount||bothCount)	
			node->rChild->faces.push_back(node->faces[i]);	
	} 	
	//�ؼ���һ�����ָ�����֮�󣬰ѵ�ǰ��ڵ����Ƭ���	
	node->faces.clear();  	
	//�ݹ�
	splitSpace(node->lChild,mAxis,depth+1);	splitSpace(node->rChild,mAxis,depth+1);	
}

template <typename T>
void BspTree<T>::traverse(BspTreeNode<T> *node) const 
{
	if (!node)
		return;	
	traverse(node->lChild);
	//�������	
	std::cout<<"�ڵ���� "<<node->depth<<" ��Ƭ�� "<<node->faces.size()<<std::endl;	
	switch(node->axis)
	{
		case AXIS_X:	
		std::cout<<"��X��ָ� "<<"�ָ��x "<<node->splitter<<std::endl;	
		break;
		case AXIS_Y:	
		std::cout<<"��Y��ָ� "<<"�ָ��y "<<node->splitter<<std::endl;	
		break;
		case AXIS_Z:	
		std::cout<<"��Z��ָ� "<<"�ָ��z "<<node->splitter<<std::endl;	
		break;
	}	
	traverse(node->rChild);
}


template <typename T>
void BspTree<T>::TraverseBspTree() const
{
	//��װһ���ٱ���
	traverse(root);
}




