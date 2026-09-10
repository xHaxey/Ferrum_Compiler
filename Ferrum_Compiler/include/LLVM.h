#pragma once
#define _SILENCE_CXX23_ALIGNED_UNION_DEPRECATION_WARNING

#pragma warning(push)
#pragma warning(disable: 4244)
#pragma warning(disable: 4267)
#pragma warning(disable: 4624)
#pragma warning(disable: 4996)
#pragma warning(disable: 26495)
#pragma warning(disable: 26439)
#pragma warning(disable: 26450)
#pragma warning(disable: 26498)

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/MC/TargetRegistry.h>

#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>

#include <llvm/TargetParser/Host.h>

#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

#include <llvm/IR/LegacyPassManager.h>

#include <llvm/CodeGen/CommandFlags.h>
#include <llvm/Support/CodeGen.h>

#pragma warning(pop)