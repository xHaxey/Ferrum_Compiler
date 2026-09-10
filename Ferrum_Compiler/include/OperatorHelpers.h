#pragma once
#include "Type.h"
#include "Operator.h"
#include "OperatorFunctions.h"

using BinaryCodegenFunc = llvm::Value* (*)(llvm::IRBuilder<>&, llvm::Value*, llvm::Value*);
using UnaryCodegenFunc = llvm::Value* (*)(llvm::IRBuilder<>&, llvm::Value*);

struct BinaryOperatorRule
{
	Type result;

	BinaryCodegenFunc codegen;

	bool Valid() const
	{
		return result != Type::INVALID;
	}
};

struct UnaryOperatorRule
{
	Type result;

	UnaryCodegenFunc codegen;

	bool modifesOperand;

	bool Valid() const
	{
		return result != Type::INVALID;
	}
};

constexpr size_t TYPES = static_cast<size_t>(Type::INVALID);

static constexpr BinaryOperatorRule InvalidBinaryRule
{
	Type::INVALID
};

static constexpr UnaryOperatorRule InvalidUnaryRule
{
	Type::INVALID
};

// +
static constexpr BinaryOperatorRule AddMatrix[TYPES][TYPES] =
{	//			INT							FLOAT						CHAR						STRING						BOOL						VOID
	/*INT*/		{{Type::INT, &AddInt},		{Type::FLOAT, &AddFloat},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*FLOAT*/	{{Type::FLOAT, &AddFloat},	{Type::FLOAT, &AddFloat},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*CHAR*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*STRING*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*BOOL*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*VOID*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}}
};

// -
static constexpr BinaryOperatorRule SubMatrix[TYPES][TYPES] =
{	//			INT							FLOAT						CHAR						STRING						BOOL						VOID
	/*INT*/		{{Type::INT, &SubInt},		{Type::FLOAT, &SubFloat},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*FLOAT*/	{{Type::FLOAT, &SubFloat},	{Type::FLOAT, &SubFloat},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*CHAR*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*STRING*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*BOOL*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*VOID*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}}
};

// *
static constexpr BinaryOperatorRule MultiMatrix[TYPES][TYPES] =
{	//			INT							FLOAT						CHAR						STRING						BOOL						VOID
		/*INT*/		{{Type::INT, &MulInt},		{Type::FLOAT, &MulFloat},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
		/*FLOAT*/	{{Type::FLOAT, &MulFloat},	{Type::FLOAT, &MulFloat},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
		/*CHAR*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
		/*STRING*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
		/*BOOL*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
		/*VOID*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}}
};

// /
static constexpr BinaryOperatorRule DivMatrix[TYPES][TYPES] =
{	//			INT							FLOAT						CHAR						STRING						BOOL						VOID
	/*INT*/		{{Type::INT, &DivInt},		{Type::FLOAT, &DivFloat},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*FLOAT*/	{{Type::FLOAT, &DivFloat},	{Type::FLOAT, &DivFloat},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*CHAR*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*STRING*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*BOOL*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*VOID*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}}
};

// %
static constexpr BinaryOperatorRule ModMatrix[TYPES][TYPES] =
{	//			INT							FLOAT						CHAR						STRING						BOOL						VOID
	/*INT*/		{{Type::INT, &ModInt},		{Type::FLOAT, &ModFloat},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*FLOAT*/	{{Type::FLOAT, &ModFloat},	{Type::FLOAT, &ModFloat},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*CHAR*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*STRING*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*BOOL*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*VOID*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}}
};

// >
static constexpr BinaryOperatorRule GreaterMatrix[TYPES][TYPES] =
{	//			INT								FLOAT							CHAR						STRING						BOOL						VOID
	/*INT*/		{{Type::BOOL, &GreaterInt},		{Type::BOOL, &GreaterFloat},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*FLOAT*/	{{Type::BOOL, &GreaterFloat},	{Type::BOOL, &GreaterFloat},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*CHAR*/	{{Type::INVALID, nullptr},		{Type::INVALID, nullptr},		{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*STRING*/	{{Type::INVALID, nullptr},		{Type::INVALID, nullptr},		{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*BOOL*/	{{Type::INVALID, nullptr},		{Type::INVALID, nullptr},		{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*VOID*/	{{Type::INVALID, nullptr},		{Type::INVALID, nullptr},		{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}}
};

// >=
static constexpr BinaryOperatorRule GreaterEqualMatrix[TYPES][TYPES] =
{	//			INT									FLOAT								CHAR						STRING						BOOL						VOID
	/*INT*/		{{Type::BOOL, &GreaterEqualInt},	{Type::BOOL, &GreaterEqualFloat},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*FLOAT*/	{{Type::BOOL, &GreaterEqualFloat},	{Type::BOOL, &GreaterEqualFloat},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*CHAR*/	{{Type::INVALID, nullptr},			{Type::INVALID, nullptr},			{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*STRING*/	{{Type::INVALID, nullptr},			{Type::INVALID, nullptr},			{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*BOOL*/	{{Type::INVALID, nullptr},			{Type::INVALID, nullptr},			{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*VOID*/	{{Type::INVALID, nullptr},			{Type::INVALID, nullptr},			{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}}
};

// <
static constexpr BinaryOperatorRule LesserMatrix[TYPES][TYPES] =
{	//			INT								FLOAT						CHAR						STRING						BOOL						VOID
	/*INT*/		{{Type::BOOL, &LesserInt},		{Type::BOOL, &LesserFloat},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*FLOAT*/	{{Type::BOOL, &LesserFloat},	{Type::BOOL, &LesserFloat},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*CHAR*/	{{Type::INVALID, nullptr},		{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*STRING*/	{{Type::INVALID, nullptr},		{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*BOOL*/	{{Type::INVALID, nullptr},		{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*VOID*/	{{Type::INVALID, nullptr},		{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}}
};

// <=
static constexpr BinaryOperatorRule LesserEqualMatrix[TYPES][TYPES] =
{	//			INT									FLOAT								CHAR						STRING						BOOL						VOID
	/*INT*/		{{Type::BOOL, &LesserEqualInt},		{Type::BOOL, &LesserEqualFloat},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*FLOAT*/	{{Type::BOOL, &LesserEqualFloat},	{Type::BOOL, &LesserEqualFloat},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*CHAR*/	{{Type::INVALID, nullptr},			{Type::INVALID, nullptr},			{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*STRING*/	{{Type::INVALID, nullptr},			{Type::INVALID, nullptr},			{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*BOOL*/	{{Type::INVALID, nullptr},			{Type::INVALID, nullptr},			{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*VOID*/	{{Type::INVALID, nullptr},			{Type::INVALID, nullptr},			{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}}
};

// ==
static constexpr BinaryOperatorRule EqualityMatrix[TYPES][TYPES] =
{	//			INT							FLOAT						CHAR						STRING						BOOL						VOID
	/*INT*/		{{Type::BOOL, &EqualInt},	{Type::BOOL, &EqualFloat},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*FLOAT*/	{{Type::BOOL, &EqualFloat},	{Type::BOOL, &EqualFloat},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*CHAR*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*STRING*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*BOOL*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*VOID*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}}
};

// !=
static constexpr BinaryOperatorRule NotEqualityMatrix[TYPES][TYPES] =
{	//			INT								FLOAT							CHAR						STRING						BOOL						VOID
	/*INT*/		{{Type::BOOL, &NotEqualInt},	{Type::BOOL, &NotEqualFloat},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*FLOAT*/	{{Type::BOOL, &NotEqualFloat},	{Type::BOOL, &NotEqualFloat},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*CHAR*/	{{Type::INVALID, nullptr},		{Type::INVALID, nullptr},		{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*STRING*/	{{Type::INVALID, nullptr},		{Type::INVALID, nullptr},		{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*BOOL*/	{{Type::INVALID, nullptr},		{Type::INVALID, nullptr},		{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*VOID*/	{{Type::INVALID, nullptr},		{Type::INVALID, nullptr},		{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}}
};

// &&
static constexpr BinaryOperatorRule LogicalANDMatrix[TYPES][TYPES] =
{	//			INT							FLOAT						CHAR						STRING						BOOL						VOID
	/*INT*/		{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*FLOAT*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*CHAR*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*STRING*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*BOOL*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::BOOL, &LogicalAnd},	{Type::INVALID, nullptr}},
	/*VOID*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}}
};

// ||
static constexpr BinaryOperatorRule LogicalORMatrix[TYPES][TYPES] =
{	//			INT							FLOAT						CHAR						STRING						BOOL						VOID
	/*INT*/		{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*FLOAT*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*CHAR*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*STRING*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}},
	/*BOOL*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::BOOL, &LogicalOr},	{Type::INVALID, nullptr}},
	/*VOID*/	{{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr},	{Type::INVALID, nullptr}}
};

// +
static constexpr UnaryOperatorRule UnaryPOSArithmetic[TYPES]
{
	/*INT*/		{Type::INT, &PosInt, false},
	/*FLOAT*/	{Type::FLOAT, &PosFloat, false},
	/*CHAR*/	{Type::INVALID, nullptr, false},
	/*STRING*/	{Type::INVALID, nullptr, false},
	/*BOOL*/	{Type::INVALID, nullptr, false},
	/*INVALID*/	{Type::INVALID, nullptr, false}
};

// -
static constexpr UnaryOperatorRule UnaryNEGArithmetic[TYPES]
{
	/*INT*/		{Type::INT, &NegInt, false},
	/*FLOAT*/	{Type::FLOAT, &NegFloat, false},
	/*CHAR*/	{Type::INVALID, nullptr, false},
	/*STRING*/	{Type::INVALID, nullptr, false},
	/*BOOL*/	{Type::INVALID, nullptr, false},
	/*INVALID*/	{Type::INVALID, nullptr, false}
};

// !
static constexpr UnaryOperatorRule UnaryNOTLogical[TYPES]
{
	/*INT*/		{Type::INVALID, nullptr, false},
	/*FLOAT*/	{Type::INVALID, nullptr, false},
	/*CHAR*/	{Type::INVALID, nullptr, false},
	/*STRING*/	{Type::INVALID, nullptr, false},
	/*BOOL*/	{Type::BOOL, &NotBool, false},
	/*VOID*/	{Type::INVALID, nullptr, false}
};

// ++
static constexpr UnaryOperatorRule UnaryPreIncrementMatrix[TYPES]
{
	/*INT*/		{Type::INT, &IncInt, true},
	/*FLOAT*/	{Type::FLOAT, &IncFloat, true},
	/*CHAR*/	{Type::INVALID, nullptr, true},
	/*STRING*/	{Type::INVALID, nullptr, true},
	/*BOOL*/	{Type::INVALID, nullptr, true},
	/*VOID*/	{Type::INVALID, nullptr, true}
};

// --
static constexpr UnaryOperatorRule UnaryPreDecrementMatrix[TYPES]
{
	/*INT*/		{Type::INT, &DecInt, true},
	/*FLOAT*/	{Type::FLOAT, &DecFloat, true},
	/*CHAR*/	{Type::INVALID, nullptr, true},
	/*STRING*/	{Type::INVALID, nullptr, true},
	/*BOOL*/	{Type::INVALID, nullptr, true},
	/*VOID*/	{Type::INVALID, nullptr, true}
};

static constexpr UnaryOperatorRule UnaryPostIncrementMatrix[TYPES]
{
	/*INT*/		{Type::INT, &IncInt, true},
	/*FLOAT*/	{Type::FLOAT, &IncFloat, true},
	/*CHAR*/	{Type::INVALID, nullptr, true},
	/*STRING*/	{Type::INVALID, nullptr, true},
	/*BOOL*/	{Type::INVALID, nullptr, true},
	/*VOID*/	{Type::INVALID, nullptr, true}
};

// --
static constexpr UnaryOperatorRule UnaryPostDecrementMatrix[TYPES]
{
	/*INT*/		{Type::INT, &DecInt, true},
	/*FLOAT*/	{Type::FLOAT, &DecFloat, true},
	/*CHAR*/	{Type::INVALID, nullptr, true},
	/*STRING*/	{Type::INVALID, nullptr, true},
	/*BOOL*/	{Type::INVALID, nullptr, true},
	/*VOID*/	{Type::INVALID, nullptr, true}
};

static BinaryOperator AssignToBinary(BinaryOperator op)
{
	switch (op)
	{
	case BinaryOperator::ADD_ASSIGN:
	{
		return BinaryOperator::ADD;
	}
	case BinaryOperator::SUB_ASSIGN:
	{
		return BinaryOperator::SUB;
	}
	case BinaryOperator::MULTI_ASSIGN:
	{
		return BinaryOperator::MULTI;
	}
	case BinaryOperator::DIV_ASSIGN:
	{
		return BinaryOperator::DIV;
	}
	case BinaryOperator::MOD_ASSIGN:
	{
		return BinaryOperator::MOD;
	}
	default:
	{
		return BinaryOperator::INVALID;
	}
	}
}

static const BinaryOperatorRule& GetBinaryRule(BinaryOperator op, Type lhs, Type rhs)
{
	if (lhs == Type::INVALID || rhs == Type::INVALID)
	{
		return InvalidBinaryRule;
	}

	switch (op)
	{
	case BinaryOperator::ADD:
	{
		return AddMatrix[(size_t)lhs][(size_t)rhs];
	}
	case BinaryOperator::SUB:
	{
		return SubMatrix[(size_t)lhs][(size_t)rhs];
	}
	case BinaryOperator::MULTI:
	{
		return MultiMatrix[(size_t)lhs][(size_t)rhs];
	}
	case BinaryOperator::DIV:
	{
		return DivMatrix[(size_t)lhs][(size_t)rhs];
	}
	case BinaryOperator::MOD:
	{
		return ModMatrix[(size_t)lhs][(size_t)rhs];
	}
	case BinaryOperator::GR:
	{
		return GreaterMatrix[(size_t)lhs][(size_t)rhs];
	}
	case BinaryOperator::GRE:
	{
		return GreaterEqualMatrix[(size_t)lhs][(size_t)rhs];
	}
	case BinaryOperator::LS:
	{
		return LesserMatrix[(size_t)lhs][(size_t)rhs];
	}
	case BinaryOperator::LSE:
	{
		return LesserEqualMatrix[(size_t)lhs][(size_t)rhs];
	}
	case BinaryOperator::EQ:
	{
		return EqualityMatrix[(size_t)lhs][(size_t)rhs];
	}
	case BinaryOperator::NEQ:
	{
		return NotEqualityMatrix[(size_t)lhs][(size_t)rhs];
	}
	case BinaryOperator::LOG_AND:
	{
		return LogicalANDMatrix[(size_t)lhs][(size_t)rhs];
	}
	case BinaryOperator::LOG_OR:
	{
		return LogicalORMatrix[(size_t)lhs][(size_t)rhs];
	}
	default:
	{
		return InvalidBinaryRule;
	}
	}
}

static const UnaryOperatorRule& GetUnaryPreRule(PreOperator op, Type operand)
{
	if (operand == Type::INVALID)
	{
		return InvalidUnaryRule;
	}

	switch (op)
	{
	case PreOperator::POS:
	{
		return UnaryPOSArithmetic[(size_t)operand];
	}
	case PreOperator::NEG:
	{
		return UnaryNEGArithmetic[(size_t)operand];
	}
	case PreOperator::LOG_NOT:
	{
		return UnaryNOTLogical[(size_t)operand];
	}
	case PreOperator::PRE_DEC:
	{
		return UnaryPreDecrementMatrix[(size_t)operand];
	}
	case PreOperator::PRE_INC:
	{
		return UnaryPreIncrementMatrix[(size_t)operand];
	}
	default:
	{
		return InvalidUnaryRule;
	}
	}
}

static const UnaryOperatorRule& GetUnaryPostRule(PostOperator op, Type operand)
{
	if (operand == Type::INVALID)
	{
		return InvalidUnaryRule;
	}

	switch (op)
	{
	case PostOperator::POST_DEC:
	{
		return UnaryPostDecrementMatrix[(size_t)operand];
	}
	case PostOperator::POST_INC:
	{
		return UnaryPostIncrementMatrix[(size_t)operand];
	}
	default:
	{
		return InvalidUnaryRule;
	}
	}
}