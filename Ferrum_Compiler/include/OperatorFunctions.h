#pragma once
#include "LLVM.h"

inline llvm::Value* AddInt(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateAdd(lhs, rhs);
}

inline llvm::Value* SubInt(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateSub(lhs, rhs);
}

inline llvm::Value* MulInt(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateMul(lhs, rhs);
}

inline llvm::Value* DivInt(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateSDiv(lhs, rhs);
}

inline llvm::Value* ModInt(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateSRem(lhs, rhs);
}

inline llvm::Value* AddFloat(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateFAdd(lhs, rhs);
}

inline llvm::Value* SubFloat(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateFSub(lhs, rhs);
}

inline llvm::Value* MulFloat(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateFMul(lhs, rhs);
}

inline llvm::Value* DivFloat(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateFDiv(lhs, rhs);
}

inline llvm::Value* ModFloat(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateFRem(lhs, rhs);
}

inline llvm::Value* GreaterInt(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateICmpSGT(lhs,rhs);
}

inline llvm::Value* GreaterEqualInt(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateICmpSGE(lhs, rhs);
}

inline llvm::Value* LesserInt(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateICmpSLT(lhs, rhs);
}

inline llvm::Value* LesserEqualInt(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateICmpSLE(lhs, rhs);
}

inline llvm::Value* EqualInt(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateICmpEQ(lhs, rhs);
}

inline llvm::Value* NotEqualInt(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateICmpNE(lhs, rhs);
}

inline llvm::Value* GreaterFloat(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateFCmpOGT(lhs, rhs);
}

inline llvm::Value* GreaterEqualFloat(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateFCmpOGE(lhs, rhs);
}

inline llvm::Value* LesserFloat(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateFCmpOLT(lhs, rhs);
}

inline llvm::Value* LesserEqualFloat(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateFCmpOLE(lhs, rhs);
}

inline llvm::Value* EqualFloat(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateFCmpOEQ(lhs, rhs);
}

inline llvm::Value* NotEqualFloat(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateFCmpONE(lhs, rhs);
}

inline llvm::Value* LogicalAnd(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateAnd(lhs,rhs);
}

inline llvm::Value* LogicalOr(
    llvm::IRBuilder<>& builder,
    llvm::Value* lhs,
    llvm::Value* rhs)
{
    return builder.CreateOr(lhs, rhs);
}

inline llvm::Value* IncInt(
    llvm::IRBuilder<>& builder,
    llvm::Value* value
)
{
    return builder.CreateAdd(value, llvm::ConstantInt::get(llvm::Type::getInt32Ty(builder.getContext()), 1));
}

inline llvm::Value* DecInt(
    llvm::IRBuilder<>& builder,
    llvm::Value* value
)
{
    return builder.CreateSub(value, llvm::ConstantInt::get(llvm::Type::getInt32Ty(builder.getContext()), 1));
}

inline llvm::Value* IncFloat(
    llvm::IRBuilder<>& builder,
    llvm::Value* value
)
{
    return builder.CreateFAdd(value, llvm::ConstantFP::get(llvm::Type::getFloatTy(builder.getContext()), 1.0));
}

inline llvm::Value* DecFloat(
    llvm::IRBuilder<>& builder,
    llvm::Value* value
)
{
    return builder.CreateFSub(value, llvm::ConstantFP::get(llvm::Type::getFloatTy(builder.getContext()), 1.0));
}
inline llvm::Value* PosInt(
    llvm::IRBuilder<>& builder,
    llvm::Value* value
)
{
    return value;
}

inline llvm::Value* PosFloat(
    llvm::IRBuilder<>& builder,
    llvm::Value* value)
{
    return value;
}

inline llvm::Value* NegInt(
    llvm::IRBuilder<>& builder,
    llvm::Value* value)
{
    return builder.CreateNeg(value);
}

inline llvm::Value* NegFloat(
    llvm::IRBuilder<>& builder,
    llvm::Value* value)
{
    return builder.CreateFNeg(value);
}

inline llvm::Value* NotBool(
    llvm::IRBuilder<>& builder,
    llvm::Value* value)
{
    return builder.CreateNot(value);
}