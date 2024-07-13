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
    void Insert(Place& place);
    bool Get(std::string path, std::string name, Place* place);
private:
    Node* root;
};
}