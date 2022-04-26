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
    stl::Map<int, student, CustomComparator<int>> students;
    students.insert(std::make_pair<int, student>(3, { 22, "michael" }));
    students.insert(std::make_pair<int, student>(1, { 22, "maria" }));
    students[4] = { 22, "omaka" };
    students[6] = { 22, "a" };
    students[9] = { 22, "q" };
    students[8] = { 22, "w" };
    students[7] = { 22, "e" };
    students[11] = { 22, "o" };
    students[5] = { 22, "t" };
    students[15] = { 22, "y" };
    students[12] = { 22, "u" };
    students[13] = { 22, "i" };
    students[10] = { 22, "r" };
    students[17] = { 22, "p" };
    students[19] = { 22, "l" };
    students[16] = { 22, "d" };
    students[2] = { 22, "paul" };
    auto students2 = students;
    students.insert({ 22, {23, "henry"} });
    students2.insert({ 22, {26, "nancy"} });
    students.printInOrder();
    printf("\n\n");
    students2.printInOrder();
    printf("%d\n", students.size());
    printf("%d\n", students2.size());
    if (students2 == students) {
        printf("equal\n");
    }
    else
        printf("not equal\n");
    
}