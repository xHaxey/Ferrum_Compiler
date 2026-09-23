#pragma once
#include "Type.h"
#include "OperatorFunctions.h"

using OperatorCodegenFunc = llvm::Value* (*)(llvm::IRBuilder<>&, std::vector<llvm::Value*>);

class OperatorRule
{
public:
	OperatorRule(
		std::vector<Type> operands, 
		Type result, 
		OperatorCodegenFunc codegen, 
		bool modifiesOperand
	) : 
		operands(operands), 
		result(result), 
		codegen(codegen), 
		modifiesOperand(modifiesOperand) 
	{}

	std::vector<Type> operands;

	Type result;

	OperatorCodegenFunc codegen;

	bool modifiesOperand;

	bool Valid() const
	{
		return result != Type::INVALID;
	}
};

static OperatorRule addIntInt =			{ {Type::INT, Type::INT},		Type::INT,		&AddInt,		false };
static OperatorRule addIntFloat =		{ {Type::INT, Type::FLOAT},		Type::FLOAT,	&AddFloat,		false };
static OperatorRule addFloatInt =		{ {Type::FLOAT, Type::INT},		Type::FLOAT,	&AddFloat,		false };
static OperatorRule addFloatFloat =		{ {Type::FLOAT, Type::FLOAT},	Type::FLOAT,	&AddFloat,		false };

static OperatorRule subIntInt =			{ {Type::INT, Type::INT},		Type::INT,		&SubInt,		false };
static OperatorRule subIntFloat =		{ {Type::INT, Type::FLOAT},		Type::FLOAT,	&SubFloat,		false };
static OperatorRule subFloatInt =		{ {Type::FLOAT, Type::INT},		Type::FLOAT,	&SubFloat,		false };
static OperatorRule subFloatFloat =		{ {Type::FLOAT, Type::FLOAT},	Type::FLOAT,	&SubFloat,		false };

static OperatorRule mulIntInt =			{ {Type::INT, Type::INT},		Type::INT,		&MulInt,		false };
static OperatorRule mulIntFloat =		{ {Type::INT, Type::FLOAT},		Type::FLOAT,	&MulFloat,		false };
static OperatorRule mulFloatInt =		{ {Type::FLOAT, Type::INT},		Type::FLOAT,	&MulFloat,		false };
static OperatorRule mulFloatFloat =		{ {Type::FLOAT, Type::FLOAT},	Type::FLOAT,	&MulFloat,		false };

static OperatorRule divIntInt =			{ {Type::INT, Type::INT},		Type::INT,		&DivInt,		false };
static OperatorRule divIntFloat =		{ {Type::INT, Type::FLOAT},		Type::FLOAT,	&DivFloat,		false };
static OperatorRule divFloatInt =		{ {Type::FLOAT, Type::INT},		Type::FLOAT,	&DivFloat,		false };
static OperatorRule divFloatFloat =		{ {Type::FLOAT, Type::FLOAT},	Type::FLOAT,	&DivFloat,		false };

static OperatorRule modIntInt =			{ {Type::INT, Type::INT},		Type::INT,		&ModInt,		false };
static OperatorRule modIntFloat =		{ {Type::INT, Type::FLOAT},		Type::FLOAT,	&ModFloat,		false };
static OperatorRule modFloatInt =		{ {Type::FLOAT, Type::INT},		Type::FLOAT,	&ModFloat,		false };
static OperatorRule modFloatFloat =		{ {Type::FLOAT, Type::FLOAT},	Type::FLOAT,	&ModFloat,		false };

static OperatorRule grIntInt =			{ {Type::INT, Type::INT},		Type::INT,		&GreaterInt,	false };
static OperatorRule grIntFloat =		{ {Type::INT, Type::FLOAT},		Type::FLOAT,	&GreaterFloat,	false };
static OperatorRule grFloatInt =		{ {Type::FLOAT, Type::INT},		Type::FLOAT,	&GreaterFloat,	false };
static OperatorRule grFloatFloat =		{ {Type::FLOAT, Type::FLOAT},	Type::FLOAT,	&GreaterFloat,	false };

static OperatorRule greIntInt =			{ {Type::INT, Type::INT},		Type::INT,		&GreaterEqualInt,	false };
static OperatorRule greIntFloat =		{ {Type::INT, Type::FLOAT},		Type::FLOAT,	&GreaterEqualFloat,	false };
static OperatorRule greFloatInt =		{ {Type::FLOAT, Type::INT},		Type::FLOAT,	&GreaterEqualFloat,	false };
static OperatorRule greFloatFloat =		{ {Type::FLOAT, Type::FLOAT},	Type::FLOAT,	&GreaterEqualFloat,	false };

static OperatorRule lsIntInt =			{ {Type::INT, Type::INT},		Type::INT,		&LesserInt,		false };
static OperatorRule lsIntFloat =		{ {Type::INT, Type::FLOAT},		Type::FLOAT,	&LesserFloat,	false };
static OperatorRule lsFloatInt =		{ {Type::FLOAT, Type::INT},		Type::FLOAT,	&LesserFloat,	false };
static OperatorRule lsFloatFloat =		{ {Type::FLOAT, Type::FLOAT},	Type::FLOAT,	&LesserFloat,	false };

static OperatorRule lseIntInt =			{ {Type::INT, Type::INT},		Type::INT,		&LesserEqualInt,	false };
static OperatorRule lseIntFloat =		{ {Type::INT, Type::FLOAT},		Type::FLOAT,	&LesserEqualFloat,	false };
static OperatorRule lseFloatInt =		{ {Type::FLOAT, Type::INT},		Type::FLOAT,	&LesserEqualFloat,	false };
static OperatorRule lseFloatFloat =		{ {Type::FLOAT, Type::FLOAT},	Type::FLOAT,	&LesserEqualFloat,	false };

static OperatorRule eqIntInt =			{ {Type::INT, Type::INT},		Type::INT,		&EqualInt,		false };
static OperatorRule eqIntFloat =		{ {Type::INT, Type::FLOAT},		Type::FLOAT,	&EqualFloat,	false };
static OperatorRule eqFloatInt =		{ {Type::FLOAT, Type::INT},		Type::FLOAT,	&EqualFloat,	false };
static OperatorRule eqFloatFloat =		{ {Type::FLOAT, Type::FLOAT},	Type::FLOAT,	&EqualFloat,	false };

static OperatorRule neqIntInt =			{ {Type::INT, Type::INT},		Type::INT,		&NotEqualInt,	false };
static OperatorRule neqIntFloat =		{ {Type::INT, Type::FLOAT},		Type::FLOAT,	&NotEqualFloat,	false };
static OperatorRule neqFloatInt =		{ {Type::FLOAT, Type::INT},		Type::FLOAT,	&NotEqualFloat,	false };
static OperatorRule neqFloatFloat =		{ {Type::FLOAT, Type::FLOAT},	Type::FLOAT,	&NotEqualFloat,	false };

static OperatorRule andBoolBool =		{ {Type::BOOL, Type::BOOL},		Type::BOOL,		&LogicalAnd,	false };
static OperatorRule orBoolBool =		{ {Type::BOOL, Type::BOOL},		Type::BOOL,		&LogicalOr,		false };

static OperatorRule incInt =			{ {Type::INT},					Type::INT,		&IncInt,		true };
static OperatorRule incFloat =			{ {Type::FLOAT},				Type::FLOAT,	&IncFloat,		true };

static OperatorRule decInt =			{ {Type::INT},					Type::INT,		&DecInt,		true };
static OperatorRule decFloat =			{ {Type::FLOAT},				Type::FLOAT,	&DecFloat,		true };

static OperatorRule posInt =			{ {Type::INT},					Type::INT,		&PosInt,		true };
static OperatorRule posFloat =			{ {Type::FLOAT},				Type::FLOAT,	&PosFloat,		true };

static OperatorRule negInt =			{ {Type::INT},					Type::INT,		&NegInt,		true };
static OperatorRule negFloat =			{ {Type::FLOAT},				Type::FLOAT,	&NegFloat,		true };

static OperatorRule notBool =			{ {Type::BOOL},					Type::BOOL,		&NotBool,		false };