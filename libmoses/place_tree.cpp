#include "place_tree.h"

#include "place.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

namespace moses {

PlaceTree::PlaceTree() {
    root = new Node();
    root->parent = root;
}

void PlaceTree::Insert(Place& place) {
    std::string path = place.Path;
    std::vector<std::string> pathParts;
    std::stringstream ss(path); // Remove the template argument
    std::string part;
    Node *current_node = root;
    while (std::getline(ss, part, '/')) {
        if(current_node->name == part) {
            continue;
        }
        else {
            Node *new_node = new Node();
            new_node->name = part;
            new_node->parent = current_node;
            current_node->children.push_back(new_node);
            current_node = new_node;
        }
    }
    current_node->places.push_back(new Place(place));
}


bool PlaceTree::Get(std::string path, std::string name, Place* place) {
    std::vector<std::string> pathParts;
    std::stringstream ss(path);
    std::string part;
    Node *current_node = root;
    bool found = false;
    while (std::getline(ss, part, '/')) {
        for (Node* child : current_node->children) {
            if (child->name == part) {
                current_node = child;
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
        else {
            found = false;
        }
    }
    for (Place* p : current_node->places) {
        if (p->Name == name) {
            place = p;
            return true;
        }
    }
    return false;
}

}