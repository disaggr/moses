#pragma once

#include "place.h"
#include <vector>

namespace moses {

class PlaceGuardStack {
	public:
		PlaceGuardStack();
		void Push(Place *place);
		void Pop();
		Place* Top();
	private:
		std::vector<Place*> *places;
};

class PlaceGuard {
	public:
		PlaceGuard(Place *place);
		~PlaceGuard();

	private:
		void Initialize();
};

extern thread_local PlaceGuardStack *_pg_stack;

}
