#pragma once
#include <vector>
#include <format>
#include <iterator>
#include <map>
#include <stack>


namespace stl {
	template <typename KeyType, typename ValueType,
		typename compare = std::less<KeyType>,
		typename Allocator = std::allocator<std::pair<const KeyType, ValueType>>
	>
		class Map {
		public:
			//type aliases........................................
			using value_type = std::pair<KeyType, ValueType>;
			using reference = value_type&;
			using const_reference = const value_type&;
			using size_type = size_t;
			using difference_type = ptrdiff_t;
			//friend class BidirectionalIterator<KeyType, ValueType>;

			//....................................................


			Map() = default;

			//copy constructor
			Map(Map const& other) {
				copyTree(rootNodePtr, other.rootNodePtr);
				mapSize = other.mapSize;
			}

			//move constructor
			Map(Map&& other) noexcept {
				rootNodePtr = std::exchange(other.rootNodePtr, nullptr);
				mapSize = std::exchange(other.mapSize, 0);
			}


			//constructor that takes an initializer list
			Map(std::initializer_list<value_type>const& initList) {
				for (auto elem : initList) {
					insert(elem);
				}
			}

			//copy assignment operator
			const Map& operator= (Map const& other) {
				if (*this != other) {
					if (rootNodePtr != nullptr)
						clearTree(rootNodePtr);
					copyTree(rootNodePtr, other.rootNodePtr);
					mapSize = other.mapSize;
				}
				return *this;
			}

			//move assignment operator
			const Map& operator= (Map&& other) noexcept {
				if (rootNodePtr != nullptr)
					clearTree(rootNodePtr);
				rootNodePtr = std::exchange(other.rootNodePtr, nullptr);
				mapSize = std::exchange(other.mapSize, 0);
				return *this;
			}

			//destructor
			~Map() {
				clear();
			}

			bool operator== (Map const& other) const {
				if (mapSize != other.mapSize) return false;
				return checkEquality(rootNodePtr, other.rootNodePtr);
			}

			bool operator!= (Map const& other) const {
				return !(*this == other);
			}

			bool insert(const_reference value) {
				Node* newNode = new Node();
				auto& [k, v] = value;
				newNode->key = k;
				newNode->value = v;
				newNode->leftChild = newNode->rightChild = nullptr;
				mapSize++;
				auto result = insertNode(newNode, rootNodePtr);
				return result;
			}

			ValueType& operator[] (KeyType key) {
				Node* newNode = new Node();
				newNode->key = key;
				newNode->leftChild = newNode->rightChild = nullptr;
				mapSize++;
				auto result = insertAndReturnValueReference(newNode, rootNodePtr);
				return result;
			}

			void printInOrder() const {
				printInOrder(rootNodePtr);
			}

			[[nodiscard]] size_type size() const {
				return mapSize;
			}

			[[nodiscard]] bool empty() const {
				if (rootNodePtr) return false;
				return true;
			}

			//removes an element witk keyvalue key, and returns the number of element removed (0 or 1)
			size_type erase(const KeyType& key) {
				if (!rootNodePtr) return 0;
				size_t result = erase(key, rootNodePtr);
				return result;
			}

			void clear() {
				clearTree(rootNodePtr);
				mapSize = 0;
			}

			size_type height() {
				int length{ 0 };
				length = treeHeight(rootNodePtr);
				return length;
			}
			void invert() {
				invert(rootNodePtr);
			}


		private:
			compare func;
			size_type mapSize{ 0 };
			struct Node {
				KeyType key;
				ValueType value;
				Node* leftChild;
				Node* rightChild;
				bool operator==(Node other) {
					return ((key == other.key) && (value == other.value));
				}
			};
			Node* rootNodePtr{ nullptr };

			bool insertNode(Node*& newNode, Node*& rootNodePtr) {
				if (rootNodePtr == nullptr) {
					rootNodePtr = newNode;
					return 1;
				}
				else if (func(newNode->key, rootNodePtr->key))
					return insertNode(newNode, rootNodePtr->leftChild);
				else {
					if (newNode->key == rootNodePtr->key) {
						mapSize--;
						delete newNode;
						return 0;
					}
					return insertNode(newNode, rootNodePtr->rightChild);
				}
			}

			ValueType& insertAndReturnValueReference(Node*& newNode, Node*& rootNodePtr) {
				if (rootNodePtr == nullptr) {
					rootNodePtr = newNode;
					return rootNodePtr->value;
				}
				else if (func(newNode->key, rootNodePtr->key))
					return insertAndReturnValueReference(newNode, rootNodePtr->leftChild);
				else {
					if (newNode->key == rootNodePtr->key) {
						mapSize--;
						delete newNode;
						return rootNodePtr->value;
					}
					return insertAndReturnValueReference(newNode, rootNodePtr->rightChild);
				}
			}

			void printInOrder(Node* rootNodePtr) const {
				if (rootNodePtr) {
					printInOrder(rootNodePtr->leftChild);
					std::cout << "{" << rootNodePtr->key << " : " << rootNodePtr->value.print() << "}\n";
					printInOrder(rootNodePtr->rightChild);
				}
			}

			size_type erase(const KeyType& key, Node*& nodePtr) {
				if (key < nodePtr->key)
					return erase(key, nodePtr->leftChild);
				else if (key > nodePtr->key)
					return erase(key, nodePtr->rightChild);
				else
					return deleteNode(nodePtr);
			}

			size_type  deleteNode(Node*& node) {
				size_type nodePresent{ 0 };
				auto updateVariables = [this, &nodePresent]() {mapSize--; nodePresent = 1; };
				Node* tempNode = nullptr;
				if (node == nullptr)
					nodePresent = 0;
				else if (node->rightChild == nullptr) {
					tempNode = node;
					node = node->leftChild;
					delete tempNode;
					updateVariables();
				}
				else if (node->leftChild == nullptr) {
					tempNode = node;
					node = node->rightChild;
					delete tempNode;
					updateVariables();
				}
				//has two children nodes
				else {
					tempNode = node->rightChild;
					while (tempNode->leftChild)
						tempNode = tempNode->leftChild;
					tempNode->leftChild = node->leftChild;
					tempNode = node;
					node = node->rightChild;
					delete tempNode;
					updateVariables();
				}
				return nodePresent;
			}

			void clearTree(Node*& root) {
				//clearing a binary tree requires an post-order traversal
				if (root) {
					clearTree(root->leftChild);
					clearTree(root->rightChild);
					delete root;
					root = nullptr;
				}
			}

			size_type treeHeight(Node* cursor) {
				if (!cursor) return 0;
				return 1 + std::max(treeHeight(cursor->leftChild), treeHeight(cursor->rightChild));
			}

			void copyTree(Node*& rootNode, Node* otherRootNode) {
				if (otherRootNode == nullptr)
					rootNode = nullptr;
				else {
					rootNode = new Node;
					rootNode->key = otherRootNode->key;
					rootNode->value = otherRootNode->value;
					copyTree(rootNode->leftChild, otherRootNode->leftChild);
					copyTree(rootNode->rightChild, otherRootNode->rightChild);
				}
			}

			bool checkEquality(Node* thisRoot, Node* otherRoot) const {
				bool isEqual = true;
				std::vector<value_type> thisValues;
				fillThisArray(thisValues, thisRoot);
				for (auto const& [k, v] : thisValues) {
					Node* cursor = otherRoot;
					while (cursor) {
						if (cursor->key == k && cursor->value == v) {
							break;
						}
						else if (k < cursor->key)
							cursor = cursor->leftChild;
						else
							cursor = cursor->rightChild;
					}
					if (!cursor) {//if cursor is a nullptr, meaning that no match was found for that element
						isEqual = false;
						break;
					}
				}
				return isEqual;
			}

			void fillThisArray(std::vector<value_type>& vec, Node* root) const {
				vec.resize(mapSize);
				size_type counter = 0;
				std::stack<Node*> stack;
				Node* current = root;
				while (current || !stack.empty()) {
					if (current) {
						stack.push(current);
						current = current->leftChild;
					}
					else {
						current = stack.top();
						stack.pop();
						vec.at(counter) = value_type(current->key, current->value);
						++counter;
						current = current->rightChild;
					}
				}
			}

			void invert(Node*& node) {
				if (!node)
					return;
				else {
					invert(node->leftChild);
					invert(node->rightChild);
					//swap the pointers in this node
					Node* temp = node->leftChild;
					node->leftChild = node->rightChild;
					node->rightChild = temp;
				}
			}
	};
}