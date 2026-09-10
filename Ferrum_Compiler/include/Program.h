#pragma once
#include "Expressions.h"
#include "SemanticAnalyzer.h"
#include "TypeChecker.h"
#include "LLVMBuilder.h"
#include <vector>
#include <memory>

class Program
{
public:
	Program() : table(), semantic_analyzer(&table), builder(&table) {};

	void AddAST(std::vector<std::unique_ptr<Expression>>&& AST);

	const std::vector<std::unique_ptr<Expression>>& Expressions() const noexcept;

	void AnalyzeSemantics();

	void CheckTypes();

	void Build();

	void PackToExe(std::filesystem::path outputDir, std::string projectName, std::filesystem::path root);

public:
	SymbolTable table;

	SemanticAnalyzer semantic_analyzer;
	TypeChecker type_checker;
	LLVMBuilder builder;

private:
	std::vector<std::unique_ptr<Expression>> expressions;

};

