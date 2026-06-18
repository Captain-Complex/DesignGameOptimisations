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

	}
	~SpacialGrid()
	{
		delete cells;
	}
	SpacialGrid(const SpacialGrid&) = delete;
	SpacialGrid& operator=(const SpacialGrid&) = delete;


	void Insert(float x, float y, float w, float h, const T& c)
	{

	}
	void Clear()
	{

	}
	std::size_t Size() const
	{
		return rows * cols;
	}
};