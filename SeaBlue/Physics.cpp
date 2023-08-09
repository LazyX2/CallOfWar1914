#include "Physics.h"
namespace SeaBlue {
	namespace Physics {
		bool rectIntercept(RECT r1, RECT r2) {
			return r1.left >= r2.left &&
				r1.right <= r2.right &&
				r1.bottom >= r2.bottom &&
				r1.top <= r2.top;
		}
	}
}