#pragma once
#include "Type.h"
#include "OperatorFunctions.h"

using OperatorCodegenFunc = llvm::Value* (*)(llvm::IRBuilder<>&, std::vector<llvm::Value*>);

class OperatorRule
{
public:
	OperatorRule(
		std::vector<Type*> operands, 
		Type* result, 
		OperatorCodegenFunc codegen, 
		bool modifiesOperand
	) : 
		operands(operands), 
		result(result), 
		codegen(codegen), 
		modifiesOperand(modifiesOperand) 
	{}

	std::vector<Type*> operands;

	Type* result;

	OperatorCodegenFunc codegen;

	bool modifiesOperand;

	bool Valid() const
	{
		return result != nullptr;
	}
};

static OperatorRule addIntInt =			{ {&intType, &intType},			&intType,		&AddInt,		false };
static OperatorRule addIntFloat =		{ {&intType, &floatType},		&floatType,		&AddFloat,		false };
static OperatorRule addFloatInt =		{ {&floatType, &intType},		&floatType,		&AddFloat,		false };
static OperatorRule addFloatFloat =		{ {&floatType, &floatType},		&floatType,		&AddFloat,		false };

static OperatorRule subIntInt =			{ {&intType, &intType},			&intType,		&SubInt,		false };
static OperatorRule subIntFloat =		{ {&intType, &floatType},		&floatType,		&SubFloat,		false };
static OperatorRule subFloatInt =		{ {&floatType, &intType},		&floatType,		&SubFloat,		false };
static OperatorRule subFloatFloat =		{ {&floatType, &floatType},		&floatType,		&SubFloat,		false };

static OperatorRule mulIntInt =			{ {&intType, &intType},			&intType,		&MulInt,		false };
static OperatorRule mulIntFloat =		{ {&intType, &floatType},		&floatType,		&MulFloat,		false };
static OperatorRule mulFloatInt =		{ {&floatType, &intType},		&floatType,		&MulFloat,		false };
static OperatorRule mulFloatFloat =		{ {&floatType, &floatType},		&floatType,		&MulFloat,		false };

static OperatorRule divIntInt =			{ {&intType, &intType},			&intType,		&DivInt,		false };
static OperatorRule divIntFloat =		{ {&intType, &floatType},		&floatType,		&DivFloat,		false };
static OperatorRule divFloatInt =		{ {&floatType, &intType},		&floatType,		&DivFloat,		false };
static OperatorRule divFloatFloat =		{ {&floatType, &floatType},		&floatType,		&DivFloat,		false };

static OperatorRule modIntInt =			{ {&intType, &intType},			&intType,		&ModInt,		false };
static OperatorRule modIntFloat =		{ {&intType, &floatType},		&floatType,		&ModFloat,		false };
static OperatorRule modFloatInt =		{ {&floatType, &intType},		&floatType,		&ModFloat,		false };
static OperatorRule modFloatFloat =		{ {&floatType, &floatType},		&floatType,		&ModFloat,		false };

static OperatorRule grIntInt =			{ {&intType, &intType},			&intType,		&GreaterInt,	false };
static OperatorRule grIntFloat =		{ {&intType, &floatType},		&floatType,		&GreaterFloat,	false };
static OperatorRule grFloatInt =		{ {&floatType, &intType},		&floatType,		&GreaterFloat,	false };
static OperatorRule grFloatFloat =		{ {&floatType, &floatType},		&floatType,		&GreaterFloat,	false };

static OperatorRule greIntInt =			{ {&intType, &intType},			&intType,		&GreaterEqualInt,	false };
static OperatorRule greIntFloat =		{ {&intType, &floatType},		&floatType,		&GreaterEqualFloat,	false };
static OperatorRule greFloatInt =		{ {&floatType, &intType},		&floatType,		&GreaterEqualFloat,	false };
static OperatorRule greFloatFloat =		{ {&floatType, &floatType},		&floatType,		&GreaterEqualFloat,	false };

static OperatorRule lsIntInt =			{ {&intType, &intType},			&intType,		&LesserInt,		false };
static OperatorRule lsIntFloat =		{ {&intType, &floatType},		&floatType,		&LesserFloat,	false };
static OperatorRule lsFloatInt =		{ {&floatType, &intType},		&floatType,		&LesserFloat,	false };
static OperatorRule lsFloatFloat =		{ {&floatType, &floatType},		&floatType,		&LesserFloat,	false };

static OperatorRule lseIntInt =			{ {&intType, &intType},			&intType,		&LesserEqualInt,	false };
static OperatorRule lseIntFloat =		{ {&intType, &floatType},		&floatType,		&LesserEqualFloat,	false };
static OperatorRule lseFloatInt =		{ {&floatType, &intType},		&floatType,		&LesserEqualFloat,	false };
static OperatorRule lseFloatFloat =		{ {&floatType, &floatType},		&floatType,		&LesserEqualFloat,	false };

static OperatorRule eqIntInt =			{ {&intType, &intType},			&intType,		&EqualInt,		false };
static OperatorRule eqIntFloat =		{ {&intType, &floatType},		&floatType,		&EqualFloat,	false };
static OperatorRule eqFloatInt =		{ {&floatType, &intType},		&floatType,		&EqualFloat,	false };
static OperatorRule eqFloatFloat =		{ {&floatType, &floatType},		&floatType,		&EqualFloat,	false };

static OperatorRule neqIntInt =			{ {&intType, &intType},			&intType,		&NotEqualInt,	false };
static OperatorRule neqIntFloat =		{ {&intType, &floatType},		&floatType,		&NotEqualFloat,	false };
static OperatorRule neqFloatInt =		{ {&floatType, &intType},		&floatType,		&NotEqualFloat,	false };
static OperatorRule neqFloatFloat =		{ {&floatType, &floatType},		&floatType,		&NotEqualFloat,	false };

static OperatorRule andBoolBool =		{ {&boolType, &boolType},		&boolType,		&LogicalAnd,	false };
static OperatorRule orBoolBool =		{ {&boolType, &boolType},		&boolType,		&LogicalOr,		false };

static OperatorRule incInt =			{ {&intType},					&intType,		&IncInt,		true };
static OperatorRule incFloat =			{ {&floatType},					&floatType,		&IncFloat,		true };

static OperatorRule decInt =			{ {&intType},					&intType,		&DecInt,		true };
static OperatorRule decFloat =			{ {&floatType},					&floatType,		&DecFloat,		true };

static OperatorRule posInt =			{ {&intType},					&intType,		&PosInt,		true };
static OperatorRule posFloat =			{ {&floatType},					&floatType,		&PosFloat,		true };

static OperatorRule negInt =			{ {&intType},					&intType,		&NegInt,		true };
static OperatorRule negFloat =			{ {&floatType},					&floatType,		&NegFloat,		true };

static OperatorRule notBool =			{ {&boolType},					&boolType,		&NotBool,		false };