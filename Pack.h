#pragma once
#include <algorithm>
#include "DynamicArray.h"

struct Stone {
	size_t weight;
	size_t price;
};

DynamicArray<size_t> Backpack(const DynamicArray<Stone> &stones, size_t W) {
	DynamicArray<DynamicArray<size_t>> maxes(stones.GetSize(), DynamicArray<size_t>(W+1));
	for (size_t i = 0; i <= W; ++i) {
		maxes[0][i] = 0;
	}
	for (size_t j = 1; j <= stones.GetSize(); ++j) {
		for (size_t i = 0; i < std::min(W + 1, stones[j - 1].weight); ++i) {
			maxes[j][i] = maxes[j - 1][i];
		}
		for (size_t i = stones[j - 1].weight; i < W+1; ++i) {
			maxes[j][i] = std::max(maxes[j - 1][i], stones[j - 1].price + maxes[j - 1][i - stones[j - 1].weight]);
		}
	}
	DynamicArray<size_t> result;
	size_t i = W;
	size_t j = stones.GetSize();
	while (i > 0 && j > 0) {
		if (maxes[j][i] == maxes[j - 1][i]) {
			--j;
			continue;
		}
		result.PushBack(j-1);
		--j;
		i -= stones[j - 1].weight;
	}
	return result;
}