#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "llvm/Support/CommandLine.h"

using namespace clang;
using namespace clang::tooling;
using namespace llvm;

class MyASTVisitor : public RecursiveASTVisitor<MyASTVisitor> {
public:
    bool VisitFunctionDecl(FunctionDecl *func) {
        if (func->hasBody()) {
            llvm::outs() << "Function Name: " << func->getNameInfo().getName().getAsString() << "\n";
        }
        return true;
    }

    bool VisitCXXMethodDecl(CXXMethodDecl *method) {
        if (method->hasBody()) {
            llvm::outs() << "Method Name: " << method->getNameInfo().getName().getAsString() << "\n";
        }
        return true;
    }
};

class MyASTConsumer : public ASTConsumer {
public:
    explicit MyASTConsumer(CompilerInstance *CI)
        : Visitor() {}

    virtual void HandleTranslationUnit(ASTContext &Context) {
        Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }

private:
    MyASTVisitor Visitor;
};

class MyFrontendAction : public ASTFrontendAction {
public:
    virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) {
        return std::make_unique<MyASTConsumer>(&CI);
    }
};

int runClangTool(const std::string& buildDir) {
    // Command to build the Clang tool
    std::string buildCommand = "clang++ -std=c++14 YourClangTool.cpp -o YourClangTool -lclangTooling -lclangASTMatchers -lclangAST -lclangBasic -lclangFrontend -lclangSerialization -lLLVMSupport -lpthread";
    
    int buildResult = std::system(buildCommand.c_str());

    if (buildResult != 0) {
        std::cerr << "Clang tool build failed." << std::endl;
        return buildResult;
    }

    // Command to run the Clang tool
    std::string runCommand = "./YourClangTool -p " + buildDir;
    int runResult = std::system(runCommand.c_str());

    if (runResult != 0) {
        std::cerr << "Clang tool run failed." << std::endl;
        return runResult;
    }

    return 0;
}

