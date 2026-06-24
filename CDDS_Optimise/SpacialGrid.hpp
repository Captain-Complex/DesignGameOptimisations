#pragma once
#include "DoubleLinkedList.hpp"

#include <algorithm>
#include <cstddef>
#include <utility>

template<typename T>
struct SpacialGrid
{
	std::size_t rows, cols;
	float cellWidth, cellHeight;

	DLinkList<T>* cells;

	SpacialGrid(std::size_t r, std::size_t c, float w, float h):
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


	void Insert(float x, float y, float w, float h, const T& c)
	{
		int minCol = static_cast<int>(x);
		int maxCol = static_cast<int>(x + w);
		maxCol = std::min(maxCol, cols - 1);
		
		int minRow = static_cast<int>(y);
		int maxRow = static_cast<int>(y + h);
		maxRow = std::min(maxRow, rows - 1);

		for (int row = minRow; row <= maxRow; row++)
		{
			for (int col = minCol; col <= maxRow; col++)
			{
				cells = cells->PushBack(c);
			}
		}
	}
	void Clear()
	{
		delete[] cells;
	}
	std::size_t Size() const
	{
		return rows * cols;
	}
};