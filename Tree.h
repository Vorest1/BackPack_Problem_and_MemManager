#pragma once
#include<utility>
#include<string>

template<typename Key, typename Value, class Comporator = std::less<Key>>
class AVLtree {
public:
	AVLtree(const Comporator& comp = Comporator());
	AVLtree(const AVLtree& other);

	void Insert(const Key& key, const Value& value);
	void Erase(const Key& key);
	bool Find(const Key& key);

	size_t Size() const;

	Value& operator[](const Key& key);
	AVLtree SubTree(const Key& key) const;

	void Stitch(int type); // 0 = NEXT 1 = PREV 
	std::string GoThrough(const std::string& format) const;

	~AVLtree();
private:
	struct _Node {
		const Key key;
		Value value;
		_Node* left;
		_Node* right;
		_Node* parent;
		size_t height;
		_Node* next;
	};
	enum class Side {
		NEXT,
		PREV,
		NONE
	};
private:
	size_t _size;
	_Node* _root;
	Comporator _comporator;
	Side _side;
private:
	static size_t GetHeight(const _Node* node);
	static void FixHeight(_Node* node);
	static int BalanceValue(const _Node* node);
	static void Connect(_Node* child, _Node* parent);
	static _Node* EraseMin(_Node* node, _Node*& min);

	static _Node* LeftTurn(_Node* node);
	static _Node* RightTurn(_Node* node);

	static _Node* Balance(_Node* node);

	static _Node* Copy(const _Node* node, size_t& size);
	static void Delete(_Node* node);
	static std::string GoThrough(_Node* node, const std::string& format);

	_Node* Find(_Node* node, const Key& key) const;
	_Node* Insert(_Node* node, const Key& key, const Value& value);
	_Node* Erase(_Node* node, const Key& key);

	static _Node* Next(_Node* node);
	static _Node* Prev(_Node* node);

public:
	class Iterator {
	private:
		_Node* _ptr;
	private:
		friend class AVLtree;
		Iterator(_Node* ptr);
	public:
		std::pair<const Key&, Value&> operator*() const;
		Iterator& operator++();
		bool operator==(Iterator other) const;
		bool operator!=(Iterator other) const;
	};
};

template<typename Key, typename Value, class Comporator>
AVLtree<Key, Value, Comporator>::Iterator::Iterator(_Node* ptr) : _ptr(ptr) {}

template<typename Key, typename Value, class Comporator>
std::pair<const Key&, Value&> AVLtree<Key, Value, Comporator>::Iterator::operator*() const {
	return { sptr->key , ptr->value };
};

template<typename Key, typename Value, class Comporator>
typename AVLtree<Key, Value, Comporator>::Iterator&   AVLtree<Key, Value, Comporator>::Iterator::operator++() {
	_ptr = Next(_ptr);
	return *this;
}

template<typename Key, typename Value, class Comporator>
bool AVLtree<Key, Value, Comporator>::Iterator::operator==(Iterator other) const{
	return _ptr == other;
}

template<typename Key, typename Value, class Comporator>
bool AVLtree<Key, Value, Comporator>::Iterator::operator!=(Iterator other) const{
	return _ptr != other;
}

template<typename Key, typename Value, class Comporator>
size_t AVLtree<Key, Value, Comporator>::GetHeight(const _Node* node) {
	if (node == nullptr) {
		return 0;
	}
	return node->height;
}

template<typename Key, typename Value, class Comporator>
void AVLtree<Key, Value, Comporator>::FixHeight(_Node* node) {
	node->height = std::max(GetHeight(node->left), GetHeight(node->right)) + 1;
}

template<typename Key, typename Value, class Comporator>
int AVLtree<Key, Value, Comporator>::BalanceValue(const _Node* node) {
	return static_cast<int>(GetHeight(node->left)) - static_cast<int>(GetHeight(node->right));
}

template<typename Key, typename Value, class Comporator>
void AVLtree<Key, Value, Comporator>::Connect(_Node* child, _Node* parent) {
	if (child != nullptr) {
		child->parent = parent;
	}
}

template<typename Key, typename Value, class Comporator>
typename AVLtree<Key, Value, Comporator>::_Node* AVLtree<Key, Value, Comporator>::LeftTurn(_Node* node) {
	_Node* right = node->right;
	node->right = right->left;
	right->left = node;
	Connect(node->right, node);
	Connect(right->left, right);
	FixHeight(node);
	FixHeight(right);
	return right;
}

template<typename Key, typename Value, class Comporator>
typename AVLtree<Key, Value, Comporator>::_Node* AVLtree<Key, Value, Comporator>::RightTurn(_Node* node) {
	_Node* left = node->left;
	node->left = left->right;
	left->right = node;
	Connect(node->left, node);
	Connect(left->right, left);
	FixHeight(node);
	FixHeight(left);
	return left;
}

template<typename Key, typename Value, class Comporator>
typename AVLtree<Key, Value, Comporator>::_Node* AVLtree<Key, Value, Comporator>::Balance(_Node* node) {
	FixHeight(node);
	int balance_value = BalanceValue(node);
	if (balance_value == -2) {
		if (BalanceValue(node->right) > 0) {
			node->right = RightTurn(node->right);
			FixHeight(node);
			Connect(node->right, node);
		}
		return LeftTurn(node);
	}
	if (balance_value == 2) {
		if (BalanceValue(node->left) < 0) {
			node->left = LeftTurn(node->left);
			FixHeight(node);
			Connect(node->left, node);
		}
		return RightTurn(node);
	}
	return node;
}

template<typename Key, typename Value, class Comporator>
typename AVLtree<Key, Value, Comporator>::_Node* AVLtree<Key, Value, Comporator>::Find(_Node* node, const Key& key) const {
	if (node == nullptr) {
		return nullptr;
	}
	if (_comporator(node->key, key)) {
		return Find(node->right, key);
	}
	if (_comporator(key, node->key)) {
		return Find(node->left, key);
	}
	return node;
}

template<typename Key, typename Value, class Comporator>
typename AVLtree<Key, Value, Comporator>::_Node* AVLtree<Key, Value, Comporator>::Insert(_Node* node, const Key& key, const Value& value) {
	if (node == nullptr) {
		++_size;
		return new _Node{ key, value, nullptr, nullptr, nullptr, 1, nullptr };
	}
	if (_comporator(node->key, key)) {
		node->right = Insert(node->right, key, value);
		Connect(node->right, node);
		return Balance(node);
	}
	if (_comporator(key, node->key)) {
		node->left = Insert(node->left, key, value);
		Connect(node->left, node);
		return Balance(node);
	}
	node->value = value;
	return node;
}

template<typename Key, typename Value, class Comporator>
typename AVLtree<Key, Value, Comporator>::_Node* AVLtree<Key, Value, Comporator>::EraseMin(_Node* node, _Node*& min) {
	if (node->left == nullptr) {
		min = node;
		_Node* right = min->right;
		min->right = nullptr;
		return right;
	}
	node->left = EraseMin(node->left, min);
	Connect(node->left, node);
	return Balance(node);
}

template<typename Key, typename Value, class Comporator>
typename AVLtree<Key, Value, Comporator>::_Node* AVLtree<Key, Value, Comporator>::Erase(_Node* node, const Key& key) {
	if (node == nullptr) {
		return nullptr;
	}
	if (_comporator(node->key, key)) {
		node->right = Erase(node->right, key);
		Connect(node->right, node);
		return Balance(node);
	}
	if (_comporator(key, node->key)) {
		node->left = Erase(node->left, key);
		Connect(node->left, node);
		return Balance(node);
	}
	--_size;
	if (node->left == nullptr && node->right == nullptr) {
		delete node;
		return nullptr;
	}
	if (node->left == nullptr) {
		_Node* right = node->right;
		delete node;
		return right;
	}
	if (node->right == nullptr) {
		_Node* left = node->left;
		delete node;
		return left;
	}
	if (node->right->left == nullptr) {
		_Node* right = node->right;
		right->left = node->left;
		Connect(right->left, right);
		delete node;
		return Balance(right);
	}
	_Node* min;
	node->right = EraseMin(node->right, min);
	min->left = node->left;
	min->right = node->right;
	Connect(min->left, min);
	Connect(min->right, min);
	delete node;
	return Balance(min);
}


template<typename Key, typename Value, class Comporator>
AVLtree<Key, Value, Comporator>::AVLtree(const Comporator& comp) : _size(0), _root(nullptr), _comporator(comp), _side(Side::NONE) {}

template<typename Key, typename Value, class Comporator>
typename AVLtree<Key, Value, Comporator>::_Node* AVLtree<Key, Value, Comporator>::Copy(const _Node* node, size_t& size) {
	if (node == nullptr) {
		return nullptr;
	}
	_Node* new_node = new _Node{ node->key, node->value, nullptr, nullptr, nullptr, node->height, nullptr };
	new_node->left = Copy(node->left, size);
	new_node->right = Copy(node->right, size);
	Connect(new_node->left, new_node);
	Connect(new_node->right, new_node);
	++size;
	return new_node;
}

template<typename Key, typename Value, class Comporator>
void AVLtree<Key, Value, Comporator>::Delete(_Node* node) {
	if (node == nullptr) {
		return;
	}
	Delete(node->left);
	Delete(node->right);
	delete node;
}

template<typename Key, typename Value, class Comporator>
AVLtree<Key, Value, Comporator>::AVLtree(const AVLtree& other) : _size(0), _root(Copy(other._root, _size)), _comporator(other._comporator), _side(Side::NONE) {}


template<typename Key, typename Value, class Comporator>
AVLtree<Key, Value, Comporator>::~AVLtree() {
	Delete(_root);
}

template<typename Key, typename Value, class Comporator>
void AVLtree<Key, Value, Comporator>::Insert(const Key& key, const Value& value) {
	_root = Insert(_root, key, value);
}

template<typename Key, typename Value, class Comporator>
void AVLtree<Key, Value, Comporator>::Erase(const Key& key) {
	_root = Erase(_root, key);
}

template<typename Key, typename Value, class Comporator>
bool AVLtree<Key, Value, Comporator>::Find(const Key& key) {
	return Find(_root, key) != nullptr;
}

template<typename Key, typename Value, class Comporator>
typename AVLtree<Key, Value, Comporator>::_Node* AVLtree<Key, Value, Comporator>::Next(_Node* node) {
	if (_side == Side::NEXT) {
		return node->next;
	}
	if (node->right != nullptr) {
		node = node->right;
		while (node->left != nullptr) {
			node = node->left;
		}
		return node;
	}
	while (node->parent != nullptr) {
		if (node->parent->left == node) {
			return node->parent;
		}
		node = node->parent;
	}
	return nullptr;
}

template<typename Key, typename Value, class Comporator>
typename AVLtree<Key, Value, Comporator>::_Node* AVLtree<Key, Value, Comporator>::Prev(_Node* node) {
	if (_side == Side::PREV) {
		return node->next;
	}
	if (node->left != nullptr) {
		node = node->left;
		while (node->right != nullptr) {
			node = node->right;
		}
		return node;
	}
	while (node->parent != nullptr) {
		if (node->parent->right == node) {
			return node->parent;
		}
		node = node->parent;
	}
	return nullptr;
}

template<typename Key, typename Value, class Comporator>
void  AVLtree<Key, Value, Comporator>::Stitch(int type) {
	if (_root == nullptr) {
		return;
	}
	if (type == 0) {
		if (_side == Side::NEXT) {
			return;
		}
		_Node* cur = _root;
		while (cur->left != nullptr) {
			cur = cur->left;
		}
		_Node* next;
		while ((next = Next(cur)) != nullptr) {
			cur->next = next;
			cur = next;
		}
		cur->next = nullptr;
		_side == Side::NEXT;
		return;
	}
	if (type == 1) {
		if (_side == Side::PREV) {
			return;
		}
		_Node* cur = _root;
		while (cur->right != nullptr) {
			cur = cur->right;
		}
		_Node* prev;
		while ((prev = Prev(cur)) != nullptr) {
			cur->next = prev;
			cur = prev;
		}
		cur->next = nullptr;
		_side == Side::PREV;
		return;
	}
}

template<typename Key, typename Value, class Comporator>
std::string AVLtree<Key, Value, Comporator>::GoThrough(_Node* node, const std::string& format) {
	if (node == nullptr) {
		return "";
	}
	std::string result;
	std::string current = std::to_string(node->key) + " , " + std::to_string(node->value);
	if (format[1] == 'C') {
		result += format[0] + current + format[2];
	}
	if (format[1] == 'L') {
		std::string left = GoThrough(node->left, format);
		if (!left.empty()) {
			result += format[0] + left + format[2];
		}
	}
	if (format[1] == 'R') {
		std::string right = GoThrough(node->right, format);
		if (!right.empty()) {
			result += format[0] + right + format[2];
		}
	}
	if (format[4] == 'C') {
		result += format[3] + current + format[5];
	}
	if (format[4] == 'L') {
		std::string left = GoThrough(node->left, format);
		if (!left.empty()) {
			result += format[3] + left + format[5];
		}
	}
	if (format[4] == 'R') {
		std::string right = GoThrough(node->right, format);
		if (!right.empty()) {
			result += format[3] + right + format[5];
		}
	}
	if (format[7] == 'C') {
		result += format[6] + current + format[8];
	}
	if (format[7] == 'L') {
		std::string left = GoThrough(node->left, format);
		if (!left.empty()) {
			result += format[6] + left + format[8];
		}
	}
	if (format[7] == 'R') {
		std::string right = GoThrough(node->right, format);
		if (!right.empty()) {
			result += format[6] + right + format[8];
		}
	}
	return result;
}

template<typename Key, typename Value, class Comporator>
std::string AVLtree<Key, Value, Comporator>::GoThrough(const std::string& format) const {
	return GoThrough(_root, format);
}

template<typename Key, typename Value, class Comporator>
Value& AVLtree<Key, Value, Comporator>::operator[](const Key& key) {
	_Node* find = Find(_root, key);
	if (find == nullptr) {
		Insert(key, Value());
		find = Find(_root, key);
	}
	return find->value;
}

template<typename Key, typename Value, class Comporator>
size_t AVLtree<Key, Value, Comporator>::Size() const {
	return _size;
}

template<typename Key, typename Value, class Comporator>
AVLtree<Key, Value, Comporator> AVLtree<Key, Value, Comporator>::SubTree(const Key& key) const {
	AVLtree result(_comporator);
	result._root = Copy(Find(_root, key), result._size);
	return result;
}