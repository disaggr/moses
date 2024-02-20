#pragma once

#include <map>
#include <string>
#include "common.h"
#include "placeguard.h"
#include "extent_hook.h"

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

class Place {
	public:
		Place(std::string name);
		std::string GetName();
	private:
		std::string _name;
};


class Moses {
	public:
		static void Initialize(std::map<std::string, Place> *initial_config);
		static void AddPlace(Place p);
		static Arena GetArena(Place p);
	private:
		Moses();
		static void CreateArena(Place *place);
		static std::map<Place, std::vector<Arena>> _place_arena_mapping;
};
}