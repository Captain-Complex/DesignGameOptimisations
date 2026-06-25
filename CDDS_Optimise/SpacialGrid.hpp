#pragma once
#include "DoubleLinkedList.hpp"

#include <algorithm>
#include <cstddef>
#include <utility>

template<typename T>
struct SpacialGrid
{
	int rows, cols;
	float cellWidth, cellHeight;

	DLinkList<T>* cells;

	SpacialGrid(int r, int c, float w, float h):
		rows(r), cols(c), cellWidth(w), cellHeight(h)
	{
		cells = new DLinkList<T>[rows * cols];
	}
	~SpacialGrid()
	{
		delete[] cells;
	}
	SpacialGrid(const SpacialGrid&) = delete;
	SpacialGrid& operator=(const SpacialGrid&) = delete;

	//[] for cells
	void Insert(float x, float y, float w, float h, const T& c)
	{
		int minCol = static_cast<int>(x) / cellWidth;
		int maxCol = static_cast<int>(x + w) / cellWidth;
		maxCol = std::min(maxCol, cols - 1);
		
		int minRow = static_cast<int>(y) / cellHeight;
		int maxRow = static_cast<int>(y + h) / cellHeight;
		maxRow = std::min(maxRow, rows - 1);

		for (int row = minRow; row <= maxRow; row++)
		{
			for (int col = minCol; col <= maxCol; col++)
			{
				cells[row * cols + col].PushBack(c);
			}
		}
	}
	void Clear()
	{
		delete[] cells;
		cells = new DLinkList<T>[rows * cols];
	}
	std::size_t Size() const
	{
		return rows * cols;
	}
};