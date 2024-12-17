#pragma once

template <typename T>
class QuadTree {
public:
	QuadTree(int x, int y, int width, int height, int capacity);
	~QuadTree();

private:
	int x;
	int y;
	int width;
	int height;
	int capacity;


};

