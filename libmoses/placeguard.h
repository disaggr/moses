#pragma once

#include "place.h"

#include <vector>

namespace moses {

class PlaceGuardStack {
    public:
        PlaceGuardStack() : _places() {}

        void Push(Place *place);
        void Pop();
        Place* Top();

    private:
        void UpdateArena();

        std::vector<Place*> _places;
};

class PlaceGuard {
    public:
        PlaceGuard(Place *place);
        ~PlaceGuard();
};

extern thread_local PlaceGuardStack _pg_stack;

}
