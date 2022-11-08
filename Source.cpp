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
    bool operator==(student const& other) const {
        return ((name == other.name) && (age == other.age));
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
    stl::Map<int, student, CustomComparator<int>> students;
    students.insert(std::make_pair<int, student>(2, { 22, "michael" }));
    students.insert(std::make_pair<int, student>(1, { 22, "maria" }));
    students.insert(std::make_pair<int, student>(4, { 22, "omaka" }));
    students.insert(std::make_pair<int, student>(3, { 22, "henry" }));
    students.insert(std::make_pair<int, student>(5, {22, "obinna"}));

    students.printInOrder();

    students.invert();
    std::cout << "\n";

    students.printInOrder();


    
    printf("size = %d\n", students.size());
}