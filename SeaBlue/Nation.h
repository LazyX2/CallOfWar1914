#pragma once

#include "framework.h"

using namespace std;
using namespace SeaBlue;
using namespace SeaBlue::Math;

namespace SeaBlue {
	enum ActionType {
		Diplomatic_Action,
		Diplomatic_Play,
		Diplomacy,

	};
	struct Action {
		ActionType type;
		function<void(Core::tile tile)> action;
		Action(ActionType action_type, function<void(Core::tile tile)> act);
	};

	class Nation {
	public:
		Nation(string name, color c, vec capital_vec, Core::tile capital);
		~Nation();
		void DeclareAction(Action act);
		void AddTile(vec location);
		void RemoveTile(vec location);
	private:
		string name;
		color c;
		unordered_map<vec, Core::tile, vec_hash> territory = {};
	};
}