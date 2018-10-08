#pragma once
//BSP树
//按照AABB包围盒分割，按照最长边切分轴，然后把场景面片递归地划分到两个子节点，一开始就划分完毕，主要是作为静态场景管理。
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
	T splitter;//某个方向的分隔值
	Point<T> minPoint;//节点的包围盒顶点
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
	//构造树根以及整体AABB包围盒，由min和max两个点来设置
	void InitBspTree(std::vector<Face<T>> &_faces, Point<T>  min_point, Point<T> max_point, int max_depth);
	void DeleteBspTree();
	void TraverseBspTree() const;
private:
	void cutFace(const Face<T> &face, Axis axis, const T &splitter, int &leftCount, int &rightCount, int &bothCount);
	void splitSpace(BspTreeNode<T> *node, Axis axis, int depth);
	void traverse(BspTreeNode<T> *node)const;
	void deleteTree(BspTreeNode<T> *root);
private:
	BspTreeNode<T> *root;//根节点
	int maxDepth;//最大递归深度，防止堆栈溢出
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
	//初始化后就开始分割所有场景
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
	T p[3];//记录某个方向的点分量
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
	//对分隔到两边的顶点计数	
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

template <typename T>void BspTree<T>::splitSpace(BspTreeNode<T> *node, Axis axis, int depth) {	//在当前层节点做分割处理，为下层子节点数据做填充 
	if(!node) 
		return;	
	node->axis=axis;
	node->depth=depth;
	if(depth==maxDepth)	
		return;	
	if(node->faces.size()<2) //上层节点拥有的面片数太少就不再划分		
		return; 	
	node->lChild=new BspTreeNode<T>;
	node->rChild=new BspTreeNode<T>;	
	//先给子节点包围盒赋值	
	node->lChild->maxPoint=node->maxPoint;
	node->lChild->minPoint=node->minPoint;
	node->rChild->maxPoint=node->maxPoint;	
	node->rChild->minPoint=node->minPoint; 
	T xLen=node->maxPoint.x-node->minPoint.x;
	T yLen=node->maxPoint.y-node->minPoint.y;	
	T zLen=node->maxPoint.z-node->minPoint.z;	
	//设置该节点的划分点
	Axis mAxis=AXIS_X;	
	if(yLen>xLen&&yLen>zLen)	
		mAxis=AXIS_Y;
	if(zLen>xLen&&zLen>yLen)	
		mAxis=AXIS_Z;	
	switch(mAxis)	
	{
		case AXIS_X:	
		node->splitter=(node->maxPoint.x+node->minPoint.x)/2; 
		//取中间值	
		//修改子节点的包围盒值	
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
		//对每个面做切分到子节点	
		cutFace(node->faces[i],mAxis,node->splitter,leftCount,rightCount,bothCount);	
		if(leftCount||bothCount)	
			node->lChild->faces.push_back(node->faces[i]);	
		if(rightCount||bothCount)	
			node->rChild->faces.push_back(node->faces[i]);	
	} 	
	//关键的一步，分割完了之后，把当前层节点的面片清空	
	node->faces.clear();  	
	//递归
	splitSpace(node->lChild,mAxis,depth+1);	splitSpace(node->rChild,mAxis,depth+1);	
}

template <typename T>
void BspTree<T>::traverse(BspTreeNode<T> *node) const 
{
	if (!node)
		return;	
	traverse(node->lChild);
	//中序遍历	
	std::cout<<"节点深度 "<<node->depth<<" 面片数 "<<node->faces.size()<<std::endl;	
	switch(node->axis)
	{
		case AXIS_X:	
		std::cout<<"沿X轴分割 "<<"分割点x "<<node->splitter<<std::endl;	
		break;
		case AXIS_Y:	
		std::cout<<"沿Y轴分割 "<<"分割点y "<<node->splitter<<std::endl;	
		break;
		case AXIS_Z:	
		std::cout<<"沿Z轴分割 "<<"分割点z "<<node->splitter<<std::endl;	
		break;
	}	
	traverse(node->rChild);
}


template <typename T>
void BspTree<T>::TraverseBspTree() const
{
	//封装一层再遍历
	traverse(root);
}




