#include "MemManager.h"

MemManag::MemManag(size_t size) : _size(size), _byte(new uint8_t[size]) {};

MemManag::~MemManag() {
	delete[] _byte;
};

void MemManag::Deallocate(void* ptr) {
	uint8_t *byte_ptr = reinterpret_cast<uint8_t>(ptr); //побайтовый перевод 
	if (byte_ptr < _byte) {
		throw DeallocationError{};
	}
	size_t pos = byte_ptr - _byte;
	if (_chunks.Find(pos)) {
		_chunks.Erase(pos);
	}
	else {
		throw DeallocationError{};
	}
}

void* MemManag::Allocate(size_t size) {
	if (_chunks.Empty()) {
		if (size > _size) {
			return nullptr;
		}
		_chunks.Insert(0, size);

		return reinterpret_cast<void*>(_byte);
	}

	size_t start = 0;
	auto current = _chunks.begin();
	auto next = current;
	++next;
	while (next != _chunks.end()) {
		if (start + size <= current->first) {
			_chunks.Insert(start, size);
			return reinterpret_cast<void*>(_byte + start);
		}
		start = current->first + current->second;
	}

	if (start + size <= current->first) {
		_chunks.Insert(start, size);
		return reinterpret_cast<void*>(_byte + start);
	}

	start = current->first + current->second;
	if (start + size > _size) {
		return nullptr;
	}
	_chunks.Insert(start, size);
	return reinterpret_cast<void*>(_byte + start);
}