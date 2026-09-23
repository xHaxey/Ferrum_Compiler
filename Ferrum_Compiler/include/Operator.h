#pragma once
#include "OperatorHelpers.h"
#include <string>
#include <vector>

enum class OperatorType
{
	POST_INC,
	POST_DEC,
	PRE_INC,
	PRE_DEC,
	POS,
	NEG,
	LOG_NOT,
	MULTI,
	DIV,
	MOD,
	ADD,
	SUB,
	GR,
	GRE,
	LS,
	LSE,
	EQ,
	NEQ,
	LOG_AND,
	LOG_OR,
	ASSIGN,
	ADD_ASSIGN,
	SUB_ASSIGN,
	MULTI_ASSIGN,
	DIV_ASSIGN,
	MOD_ASSIGN,
	CUSTOM,
	INVALID
};

class OperatorData
{
public:
	OperatorData(std::string representation, size_t leftBp, size_t rightBp) : representation(representation), leftBp(leftBp), rightBp(rightBp) {}

	std::string representation;

	size_t leftBp;
	size_t rightBp;
};

class Operator
{
public:
	Operator(OperatorType type, OperatorData data) : type(type), data(data) {}
	Operator(OperatorType type, OperatorData data, std::vector<OperatorRule*> rules) : type(type), data(data), rules(rules) {}

	OperatorType type;
	OperatorData data;

	std::vector<OperatorRule*> rules;
};

static Operator post_inc =
{
	OperatorType::POST_INC,
	OperatorData("++", 110, 0),
	{
		&incInt,
		&incFloat
	}
};

static Operator post_dec =
{
	OperatorType::POST_DEC,
	OperatorData("--", 110, 0),
	{
		&decInt,
		&decFloat
	}
};

static Operator pre_inc =
{
	OperatorType::PRE_INC,
	OperatorData("++", 0, 100),
	{
		&incInt,
		&incFloat
	}
};

static Operator pre_dec =
{
	OperatorType::PRE_DEC,
	OperatorData("--", 0, 100),
	{
		&decInt,
		&decFloat
	}
};

static Operator pos =
{
	OperatorType::POS,
	OperatorData("+", 0, 90),
	{
		&posInt,
		&posFloat
	}
};

static Operator neg =
{
	OperatorType::NEG,
	OperatorData("-", 0, 90),
	{
		&negInt,
		&negFloat
	}
};

static Operator log_not =
{
	OperatorType::LOG_NOT,
	OperatorData("!", 0, 80),
	{
		&notBool
	}
};

static Operator multi =
{
	OperatorType::MULTI,
	OperatorData("*", 70, 71),
	{
		&mulIntInt,
		&mulIntFloat,
		&mulFloatInt,
		&mulFloatFloat
	}
};

static Operator division =
{
	OperatorType::DIV,
	OperatorData("/", 70, 71),
	{
		&divIntInt,
		&divIntFloat,
		&divFloatInt,
		&divFloatFloat
	}
};

static Operator mod =
{
	OperatorType::MOD,
	OperatorData("%", 70, 71),
	{
		&modIntInt,
		&modIntFloat,
		&modFloatInt,
		&modFloatFloat
	}
};

static Operator add =
{
	OperatorType::ADD,
	OperatorData("+", 60, 61),
	{
		&addIntInt,
		&addIntFloat,
		&addFloatInt,
		&addFloatFloat
	}
};

static Operator sub =
{
	OperatorType::SUB,
	OperatorData("-", 60, 61),
	{
		&subIntInt,
		&subIntFloat,
		&subFloatInt,
		&subFloatFloat
	}
};

static Operator gr =
{
	OperatorType::GR,
	OperatorData(">", 50, 50),
	{
		&grIntInt,
		&grIntFloat,
		&grFloatInt,
		&grFloatFloat
	}
};

static Operator gre =
{
	OperatorType::GRE,
	OperatorData(">=", 50, 50),
	{
		&greIntInt,
		&greIntFloat,
		&greFloatInt,
		&greFloatFloat
	}
};

static Operator ls =
{
	OperatorType::LS,
	OperatorData("<", 50, 50),
	{
		&lsIntInt,
		&lsIntFloat,
		&lsFloatInt,
		&lsFloatFloat
	}
};

static Operator lse =
{
	OperatorType::LSE,
	OperatorData("<=", 50, 50),
	{
		&lseIntInt,
		&lseIntFloat,
		&lseFloatInt,
		&lseFloatFloat
	}
};

static Operator eq =
{
	OperatorType::EQ,
	OperatorData("==", 40, 40),
	{
		&eqIntInt,
		&eqIntFloat,
		&eqFloatInt,
		&eqFloatFloat
	}
};

static Operator neq =
{
	OperatorType::NEQ,
	OperatorData("!=", 40, 40),
	{
		&neqIntInt,
		&neqIntFloat,
		&neqFloatInt,
		&neqFloatFloat
	}
};

static Operator log_and =
{
	OperatorType::LOG_AND,
	OperatorData("&&", 30, 30),
	{
		&andBoolBool
	}
};

static Operator log_or =
{
	OperatorType::LOG_OR,
	OperatorData("||", 20, 20),
	{
		&orBoolBool
	}
};

static Operator assign =
{
	OperatorType::ASSIGN,
	OperatorData("=", 10, 10)
};

static Operator add_assign =
{
	OperatorType::ADD_ASSIGN,
	OperatorData("+=", 10, 10),
	{
		&addIntInt,
		&addIntFloat,
		&addFloatInt,
		&addFloatFloat
	}
};

static Operator sub_assign =
{
	OperatorType::SUB_ASSIGN,
	OperatorData("-=", 10, 10),
	{
		&subIntInt,
		&subIntFloat,
		&subFloatInt,
		&subFloatFloat
	}
};

static Operator multi_assign =
{
	OperatorType::MULTI_ASSIGN,
	OperatorData("*=", 10, 10),
	{
		&mulIntInt,
		&mulIntFloat,
		&mulFloatInt,
		&mulFloatFloat
	}
};

static Operator div_assign =
{
	OperatorType::DIV_ASSIGN,
	OperatorData("/=", 10, 10),
	{
		&divIntInt,
		&divIntFloat,
		&divFloatInt,
		&divFloatFloat
	}
};

static Operator mod_assign =
{
	OperatorType::MOD_ASSIGN,
	OperatorData("%=", 10, 10),
	{
		&modIntInt,
		&modIntFloat,
		&modFloatInt,
		&modFloatFloat
	}
};

static std::vector<Operator*> operators = 
{
	&post_inc, 
	&post_dec,
	&pre_inc,
	&pre_dec,
	&pos,
	&neg,
	&log_not,
	&multi,
	&division,
	&mod,
	&add,
	&sub,
	&gr,
	&gre,
	&ls,
	&lse,
	&eq,
	&neq,
	&log_and,
	&log_or,
	&assign,
	&add_assign,
	&sub_assign,
	&multi_assign,
	&div_assign,
	&mod_assign
};

static bool IsPrefix(std::string representation)
{
	for (auto& op : operators)
	{
		if (op->data.leftBp == 0)
		{
			if (op->data.representation == representation)
			{
				return true;
			}
		}
	}

	return false;
}

static bool IsInfix(std::string representation)
{
	for (auto& op : operators)
	{
		if (op->data.leftBp != 0 && op->data.rightBp != 0)
		{
			if (op->data.representation == representation)
			{
				return true;
			}
		}
	}

	return false;
}

static bool IsPostfix(std::string representation)
{
	for (auto& op : operators)
	{
		if (op->data.rightBp == 0)
		{
			if (op->data.representation == representation)
			{
				return true;
			}
		}
	}

	return false;
}

static Operator* ToPrefix(std::string representation)
{
	for (auto& op : operators)
	{
		if (op->data.leftBp == 0)
		{
			if (op->data.representation == representation)
			{
				return op;
			}
		}
	}

	return nullptr;
}

static Operator* ToInfix(std::string representation)
{
	for (auto& op : operators)
	{
		if (op->data.leftBp != 0 && op->data.rightBp != 0)
		{
			if (op->data.representation == representation)
			{
				return op;
			}
		}
	}

	return nullptr;
}

static Operator* ToPostfix(std::string representation)
{
	for (auto& op : operators)
	{
		if (op->data.rightBp == 0)
		{
			if (op->data.representation == representation)
			{
				return op;
			}
		}
	}

	return nullptr;
}