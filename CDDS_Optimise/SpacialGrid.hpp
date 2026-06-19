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



		int minRow = static_cast<int>(y);
		int maxRow = static_cast<int>(y + h);



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