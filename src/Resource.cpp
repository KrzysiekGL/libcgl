#include "Resource.h"

namespace CGL {

/* Ctor & Dtor */
Resource::~Resource() {
}
/* Ctor & Dtor */
/* Public Methods */
Type Resource::GetType() {
	return mType;
}

std::string Resource::GetName() {
	return mName;
}
/* Public Methods */
/* Protected Methods */
void Resource::setType(Type type) {
	mType = type;
}

void Resource::setName(std::string name) {
	mName = name;
}
/* Protected Methods */

} /* namespace CGL */
