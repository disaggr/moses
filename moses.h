#pragma once

#include <map>
#include <string>
#include "common.h"
#include "placeguard.h"
#include "place.h"
#include "place_tree.h"

namespace moses {

//TODO explicit_place
enum class Policy_Priority {
	Critical,
	Normal,
	May_Spill
};

enum class Policy_Frequency {
	High,
	Medium,
	Low
};

enum class Policy_Lifespan {
	Temporary,
	Long
};

enum class Policy_Granularity {
	Small,
	Large,
};

enum class Policy_Visibility {
	Single_Thread,
	Parallel
};

enum class Policy_Access {
	Read_Only,
	Write_Only,
	Read_Heavy,
	Write_Heavy,
	Mixed
};

enum class Policy_Pattern {
	Pointer_Chase,
	Random_Access,
	Linear_Stream,
	Mixed
};

class Moses {
	public:
		static void Initialize(std::map<std::string, Place> *initial_config);
		static void AddPlace(Place p);
		//static Arena GetArena(Place p);
		//static void CreateArena(Place *place);
	private:
		Moses();
		static PlaceTree _place_tree;
		static std::map<Arena*, Place*> _arena_place_mapping;
};
}