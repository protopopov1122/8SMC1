#include "ctrl-lib/CtrlCore.h"

namespace CalX {

	std::ostream &operator<<(std::ostream &os, const ErrorCode &errcode)
	{
		os << static_cast<int>(errcode);
		return os;
	}
}