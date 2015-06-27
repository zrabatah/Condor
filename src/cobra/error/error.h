#ifndef ERROR_H_
#define ERROR_H_

#include <string>
#include "../token/token.h"

namespace Cobra {
	namespace Error {
		enum ERROR {
			INVALID_MODE,
			EXPECTED,
			EXPECTED_MODE,
			MODE_UNKNOWN,
			EXPECTED_VARIABLE_TYPE,
			INVALID_STMT,
			VAR_NOT_ALLOWED_IN_STRICT_MODE,
			EXPECTED_VAR,
			EXPECTED_OPERATOR,
			INVALID_FUNCTION_CALL,
			MISSING_EXPR
		};

		std::string String(ERROR e, Token* tok);
	}
}

#endif // ERROR_H_
