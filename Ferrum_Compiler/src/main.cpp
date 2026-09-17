#include "Lexer.h"
#include "ExpressionParser.h"
#include "SourceManager.h"
#include "ExprPrinter.h"
//#include "Program.h"
#include <iostream>
#include <filesystem>

std::filesystem::path root;

int main(int argc, char* argv[])
{
    root = argv[0];

    SourceManager sourceManager(argc, argv);

    if (!sourceManager.error_queue.Empty())
    {
        sourceManager.error_queue.Print();
        return 0;
    }

    bool HasErrors = false;
    //Program program;

    for (auto& file : sourceManager.Files())
    {
        Lexer lexer(file.get());

        auto tokens = lexer.TokenizeSource();

#ifndef NODEBUG
        for (auto& token : tokens) 
        {
            PrintToken(token.get());
        }
#endif // NDEBUG

        if (!lexer.error_queue.Empty())
        {
            lexer.error_queue.Print();
            HasErrors = true;
            continue;
        }

        ExprParser exprParser(tokens);

        auto ast = exprParser.ParseFile();

#ifndef NODEBUG
        ExprPrinter exprPrinter;

        for (auto& expr : ast)
        {
            exprPrinter.Print(*expr.get());
        }
#endif // !NDEBUG

        if (!exprParser.error_queue.Empty())
        {
            exprParser.error_queue.Print();
            HasErrors = true;
            continue;
        }

        //program.AddAST(std::move(ast));

    }

    if (HasErrors)
    {
        return 0;
    }

    /*program.AnalyzeSemantics();

    if (!program.semantic_analyzer.error_queue.Empty())
    {
        program.semantic_analyzer.error_queue.Print();
        return 0;
    }

    program.CheckTypes();

    if (!program.type_checker.error_queue.Empty())
    {
        program.type_checker.error_queue.Print();
        return 0;
    }

    program.Build();

    auto result = sourceManager.GetOutputDir();

    if (result)
    {
        program.PackToExe(result.value(),sourceManager.GetProjectDir().filename().generic_string(), root.parent_path());
        return 0;
    }
    else
    {
        std::cout << "Couldn't create output directory called 'build' in provided directory!" << std::endl;
    }*/
    
    return 0;
}
