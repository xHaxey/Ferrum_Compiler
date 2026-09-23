#pragma once
#include "LLVM.h"

inline llvm::Value* AddInt(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateAdd(values[0], values[1]);
}

inline llvm::Value* SubInt(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateSub(values[0], values[1]);
}

inline llvm::Value* MulInt(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateMul(values[0], values[1]);
}

inline llvm::Value* DivInt(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateSDiv(values[0], values[1]);
}

inline llvm::Value* ModInt(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateSRem(values[0], values[1]);
}

inline llvm::Value* AddFloat(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateFAdd(values[0], values[1]);
}

inline llvm::Value* SubFloat(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateFSub(values[0], values[1]);
}

inline llvm::Value* MulFloat(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateFMul(values[0], values[1]);
}

inline llvm::Value* DivFloat(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateFDiv(values[0], values[1]);
}

inline llvm::Value* ModFloat(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateFRem(values[0], values[1]);
}

inline llvm::Value* GreaterInt(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateICmpSGT(values[0], values[1]);
}

inline llvm::Value* GreaterEqualInt(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateICmpSGE(values[0], values[1]);
}

inline llvm::Value* LesserInt(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateICmpSLT(values[0], values[1]);
}

inline llvm::Value* LesserEqualInt(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateICmpSLE(values[0], values[1]);
}

inline llvm::Value* EqualInt(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateICmpEQ(values[0], values[1]);
}

inline llvm::Value* NotEqualInt(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateICmpNE(values[0], values[1]);
}

inline llvm::Value* GreaterFloat(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateFCmpOGT(values[0], values[1]);
}

inline llvm::Value* GreaterEqualFloat(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateFCmpOGE(values[0], values[1]);
}

inline llvm::Value* LesserFloat(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateFCmpOLT(values[0], values[1]);
}

inline llvm::Value* LesserEqualFloat(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateFCmpOLE(values[0], values[1]);
}

inline llvm::Value* EqualFloat(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateFCmpOEQ(values[0], values[1]);
}

inline llvm::Value* NotEqualFloat(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateFCmpONE(values[0], values[1]);
}

inline llvm::Value* LogicalAnd(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateAnd(values[0], values[1]);
}

inline llvm::Value* LogicalOr(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateOr(values[0], values[1]);
}

inline llvm::Value* IncInt(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateAdd(values[0], llvm::ConstantInt::get(llvm::Type::getInt32Ty(builder.getContext()), 1));
}

inline llvm::Value* DecInt(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateSub(values[0], llvm::ConstantInt::get(llvm::Type::getInt32Ty(builder.getContext()), 1));
}

inline llvm::Value* IncFloat(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateFAdd(values[0], llvm::ConstantFP::get(llvm::Type::getFloatTy(builder.getContext()), 1.0));
}

inline llvm::Value* DecFloat(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateFSub(values[0], llvm::ConstantFP::get(llvm::Type::getFloatTy(builder.getContext()), 1.0));
}
inline llvm::Value* PosInt(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return values[0];
}

inline llvm::Value* PosFloat(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return values[0];
}

inline llvm::Value* NegInt(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateNeg(values[0]);
}

inline llvm::Value* NegFloat(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateFNeg(values[0]);
}

inline llvm::Value* NotBool(
    llvm::IRBuilder<>& builder,
    std::vector<llvm::Value*> values)
{
    return builder.CreateNot(values[0]);
}