#pragma once
#include <algorithm>
#include <utility>

namespace stl {
	template <typename KeyType, typename ValueType,
		typename compare = std::less<KeyType>,
		typename Allocator = std::allocator<std::pair<const KeyType, ValueType>>
		>
	class Map {
	public:
		Map() = default;

		//copy constructor
		Map(Map const& other) {
			copyTree(rootNodePtr, other.rootNodePtr);
		}

		//move constructor
		Map(Map&& other) noexcept {
			rootNodePtr = std::exchange(other.rootNodePtr, nullptr);
			mapSize = std::exchange(other.mapSize, 0);
		}


		//constructor that takes an initializer list
		Map(std::initializer_list<std::pair<KeyType, ValueType>>const& initList) {
			for (auto elem : initList) {
				insert(elem);
			}
		}

		//copy assignment operator
		const Map& operator= (Map const& other) {
			if (this != &other) {
				if (rootNodePtr != nullptr)
					clearTree(rootNodePtr);
				copyTree(rootNodePtr, other.rootNodePtr);
			}
			return *this;
		}

		//move assignment operator
		const Map& operator= (Map&& other) noexcept{
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

		void insert(std::pair<KeyType, ValueType>const& value) {
			Node* newNode = new Node();
			auto& [k, v] = value;
			newNode->key = k;
			newNode->value = v;
			newNode->leftChild = newNode->rightChild = nullptr;
			insertNode(newNode, rootNodePtr);
			mapSize++;
		}
			
		ValueType& operator[] (KeyType key) {
			Node* newNode = new Node();
			newNode->key = key;
			newNode->leftChild = newNode->rightChild = nullptr;
			mapSize++;
			return insertAndReturnValueReference(newNode, rootNodePtr);
		}

		void printInOrder() const{
			printInOrder(rootNodePtr);
		}

		size_t size() const {
			return mapSize;
		}

		bool empty() const {
			if (rootNodePtr) return false;
			return true;
		}

		//removes an element witk keyvalue key, and returns the number of element removed (0 or 1)
		size_t erase(const KeyType& key) {
			if (!rootNodePtr) return 0;
			size_t result = erase(key, rootNodePtr);
			return result;
		}

		void clear() {
			clearTree(rootNodePtr);
			mapSize = 0;
		}

		size_t height() {
			int length{ 0 };
			length = treeHeight(rootNodePtr);
			return length;
		}


	private:
		compare func;
		size_t mapSize{0};
		struct Node {
			KeyType key;
			ValueType value;
			Node* leftChild;
			Node* rightChild;
		};
		Node* rootNodePtr{ nullptr };

		void insertNode(Node*& newNode, Node*& rootNodePtr) {
			if (rootNodePtr == nullptr)
				rootNodePtr = newNode;
			else if (func(newNode->key, rootNodePtr->key))
				insertNode(newNode, rootNodePtr->leftChild);
			else {
				if (newNode->key == rootNodePtr->key) {
					mapSize--;
					return;
				}
				insertNode(newNode, rootNodePtr->rightChild);
			}
				
		}

		ValueType& insertAndReturnValueReference(Node* &newNode, Node* &rootNodePtr) {
			if (rootNodePtr == nullptr) {
				rootNodePtr = newNode;
				return rootNodePtr->value;
			}	
			else if (func(newNode->key, rootNodePtr->key))
				return insertAndReturnValueReference(newNode, rootNodePtr->leftChild);
			else {
				if (newNode->key == rootNodePtr->key) {
					mapSize--;
					return rootNodePtr->value;
				}
				return insertAndReturnValueReference(newNode, rootNodePtr->rightChild);
			}
		}

		void printInOrder(Node* rootNodePtr) const{
			if (rootNodePtr) {
				printInOrder(rootNodePtr->leftChild);
				std::cout << "{" << rootNodePtr->key << " : " << rootNodePtr->value.print() << "}\n";
				printInOrder(rootNodePtr->rightChild);
			}
		}

		size_t erase(const KeyType& key, Node* &nodePtr) {
			if (key < nodePtr->key)
				return erase(key, nodePtr->leftChild);
			else if (key > nodePtr->key)
				return erase(key, nodePtr->rightChild);
			else
				return deleteNode(nodePtr);
		}
		
		size_t  deleteNode(Node* &node) {
			size_t nodePresent{ 0 };
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

		void clearTree(Node* &root) {
			//clearing a binary tree requires an post-order traversal
			if (root) {
				clearTree(root->leftChild);
				clearTree(root->rightChild);
				delete root;
				root = nullptr;
			}
		}

		size_t treeHeight(Node* cursor) {
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
	};
}