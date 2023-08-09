#include "Nation.h"

namespace SeaBlue {
	Action::Action(ActionType action_type, function<void(Core::tile tile)> act) {
		type = action_type;
		action = act;
	}

	void Nation::AddTile(vec location) { territory[location] = Core::tile(name, 1); }
	void Nation::RemoveTile(vec location) { territory[location] = Core::tile(); }
}