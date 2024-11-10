#pragma once
#include "Tree.h"
#include <exception>

class MemManag {
private:
	size_t _size;
	AVLtree<size_t, size_t> _chunks;
	uint8_t* _byte;
public:
	MemManag(size_t size);
	~MemManag();
	void Deallocate(void* ptr);
	void* Allocate(size_t size);

public:
	class DeallocationError : public std::exception {};
};