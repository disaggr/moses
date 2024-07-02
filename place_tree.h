#pragma once

#include "place.h"
#include <vector>
#include <string>

namespace moses {
struct Node {
    std::vector<Place*> places;
    std::vector<Node*> children;
    struct Node* parent;
    std::string name;
};

class PlaceTree {
public:
    PlaceTree();
    void Insert(const Place& place);
    bool Get(const Place& place);
private:
    Node* root;
};
}