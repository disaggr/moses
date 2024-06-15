#pragma once

#include <vector>
#include "place.h"


namespace moses {

class PlaceGuardStack {
	//TODO Try the 'tcache.flush' API before setting the custom hooks.
	//See https://github.com/jemalloc/jemalloc/issues/1016
	public:
		PlaceGuardStack();
		void Push(Place *place);
		void Pop();
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

}