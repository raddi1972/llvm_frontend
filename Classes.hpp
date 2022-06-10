#ifndef CLASSES
#define CLASSES

#include <llvm-14/llvm/ADT/APFloat.h>
#include <llvm-14/llvm/ADT/STLExtras.h>
#include <llvm-14/llvm/IR/BasicBlock.h>
#include <llvm-14/llvm/IR/Constants.h>
#include <llvm-14/llvm/IR/DerivedTypes.h>
#include <llvm-14/llvm/IR/Function.h>
#include <llvm-14/llvm/IR/IRBuilder.h>
#include <llvm-14/llvm/IR/LLVMContext.h>
#include <llvm-14/llvm/IR/Module.h>
#include <llvm-14/llvm/IR/Type.h>
#include <llvm-14/llvm/IR/Verifier.h>
#include <string>
#include <memory>
#include <vector>
#include <map>

using namespace llvm;

class ExprAST
{
public:
    virtual ~ExprAST() {}
    virtual Value *codegen() = 0;
};

class NumberExprAST : public ExprAST
{
    double Val;

public:
    NumberExprAST(double Val) : Val(Val) {}
    Value *codegen() override;
};

class VariableExprAST : public ExprAST
{
    std::string Name;

public:
    VariableExprAST(const std::string &Name) : Name(Name) {}
    Value *codegen() override;
};

class BinaryExprAST : public ExprAST
{
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;

public:
    Value *codegen() override;
    BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS)
        : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS))
    {
    }
};

class CallExprAST : public ExprAST
{
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;

public:
    Value *codegen() override;
    CallExprAST(const std::string &Callee, std::vector<std::unique_ptr<ExprAST>> Args)
        : Callee(Callee), Args(std::move(Args))
    {
    }
};

class PrototypeAST
{
    std::string Name;
    std::vector<std::string> Args;

public:
    Function *codegen();
    PrototypeAST(const std::string &name, std::vector<std::string> Args)
        : Name(name), Args(std::move(Args))
    {
    }

    const std::string &getName() const { return Name; }
};

class FunctionAST
{
public:
    Function *codegen();

    FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                std::unique_ptr<ExprAST> Body)
        : Proto(std::move(Proto)), Body(std::move(Body)) {}

private:
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;
};

#endif