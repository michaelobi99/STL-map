#include <iostream>
#include <string>
#include <tuple>
#include "Map.h"

class student {
public:
    student() = default;
    student(int age, std::string name) : age{ age }, name{ std::move(name) }{}
    std::string print() const {
        return name + " " + std::to_string(age);
    }
    ~student() {
        //std::cout << "deleting " << name << "\n";
    }
private:
    int age{};
    std::string name{};
};

template <typename T>
struct CustomComparator {
    bool operator()(const T& a, const T& b) const {
        return a < b;
    }
};

void printElements(stl::Map<int, student, CustomComparator<int>> students) {
    students.printInOrder();
}

int main() {
    stl::Map<int, student, CustomComparator<int>> s;
    s = stl::Map<int, student, CustomComparator<int>>{ { 1, { 22, "omaka" } }, { 2, {22, "mike"} } };
    printElements(s);
    printf("size = %d", s.size());
    printf("\n");
    printf("isEmpty = %d", s.empty());

    //printElements(stl::Map<int, student, CustomComparator<int>>{ {1, { 22, "omaka" }, { 2, {22, "mike"} }});


    
}