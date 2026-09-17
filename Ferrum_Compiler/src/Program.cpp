#include "Program.h"

void Program::AddAST(std::vector<std::unique_ptr<Expression>>&& AST)
{
	expressions.insert(expressions.end(), std::make_move_iterator(AST.begin()), std::make_move_iterator(AST.end()));
}

const std::vector<std::unique_ptr<Expression>>& Program::Expressions() const noexcept
{
	return expressions;
}

void Program::AnalyzeSemantics()
{
	for (auto& expr : expressions)
	{
        expr->Accept(semantic_analyzer);
	}

	if (!semantic_analyzer.hasMain)
	{
		semantic_analyzer.error_queue.Add("No main function defined!", expressions.end()->get()->range);
	}
}

void Program::CheckTypes()
{
	for (auto& expr : expressions)
	{
		type_checker.Check(*expr.get());
	}
}

void Program::Build()
{
	for (auto& expr : expressions)
	{
		builder.Build(*expr.get());
	}

#ifndef NODEBUG
	builder.m_module.print(llvm::outs(), nullptr);
#endif

	if (llvm::verifyModule(builder.m_module, &llvm::errs()))
	{
		std::cerr << "Invalid LLVM Module!" << std::endl;
	}
}

void Program::PackToExe(std::filesystem::path outputDir, std::string projectName, std::filesystem::path root)
{
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    auto triple = llvm::sys::getDefaultTargetTriple();

    builder.m_module.setTargetTriple(triple);

    std::string error;

    auto target = llvm::TargetRegistry::lookupTarget(triple, error);

    if (!target)
    {
        std::cerr << error << std::endl;
        return;
    }

    auto cpu = "generic";
    auto features = "";

    llvm::TargetOptions options;

    auto relocationModel = std::optional<llvm::Reloc::Model>();

    auto targetMachine = target->createTargetMachine(triple, cpu, features, options, relocationModel);

    builder.m_module.setDataLayout(targetMachine->createDataLayout());

    std::error_code ec;

    auto objPath = outputDir / (projectName + ".obj");

    llvm::raw_fd_ostream dest(objPath.generic_string(), ec, llvm::sys::fs::OF_None);

    if (ec)
    {
        std::cerr
            << "Cannot create output.obj: "
            << ec.message()
            << std::endl;

        return;
    }

    if (llvm::verifyModule(builder.m_module, &llvm::errs()))
    {
        std::cerr << "Invalid LLVM module!" << std::endl;
        return;
    }

    llvm::legacy::PassManager pass;

    if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, llvm::CodeGenFileType::ObjectFile))
    {
        return;
    }

    std::cout << "Successfully create object file." << std::endl << std::endl;

    pass.run(builder.m_module);
    dest.flush();
    dest.close();

    auto clangPath = root;

    auto exePath = outputDir / (projectName + ".exe");

    auto exeCommand = clangPath.generic_string() + "/clang++.exe " + objPath.generic_string() + " -o " + exePath.generic_string();

    system(exeCommand.c_str());

}
