#include "visitshell.h"
#include "shellParser.h"
#include <any>
#include <llvm/ADT/APFloat.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <string>

std::any visitshell::visitProg(shellParser::ProgContext *ctx)
{
	std::cout << "visitProg\n";
	// Llama a visitChildren(ctx), que visitará todos los hijos del contexto actual
	return visitChildren(ctx);
}

std::any visitshell::visitPrintExpr(shellParser::PrintExprContext *ctx)
{
	std::cout << "visitPrintExpr\n";
	// Crea una función LLVM temporal con un bloque básico que contiene la expresión a imprimir
	std::vector<double> Args;
	std::vector<llvm::Type *> Doubles(Args.size(),
									  llvm::Type::getDoubleTy(*context));
	llvm::FunctionType *FT = llvm::FunctionType::get(
		llvm::Type::getDoubleTy(*context), Doubles, false);

	F = llvm::Function::Create(
		FT, llvm::Function::ExternalLinkage, "_anon_", module.get());

	llvm::BasicBlock *BB = llvm::BasicBlock::Create(*context, "entry", F);
	builder->SetInsertPoint(BB);
	// La función retorna un valor de la expresión visitando el contexto expr()
	// y luego crea una instrucción ret con ese valor
	llvm::Value *val = std::any_cast<llvm::Value *>(visit(ctx->expr()));
	builder->CreateRet(val);

	return std::any();
	// return visitChildren(ctx);
}

std::any visitshell::visitAssign(shellParser::AssignContext *ctx)
{
	std::cout << "visitAssign\n";
	return visitChildren(ctx);
}

std::any visitshell::visitIfStatement(shellParser::IfStatementContext *ctx)
{
    std::cout << "visitIfStatement\n";

    // Obtener la condición de la comparación
    llvm::Value *condition = std::any_cast<llvm::Value *>(visit(ctx->comp()));

    // Crear los bloques para el if y el else
    llvm::Function *function = builder->GetInsertBlock()->getParent();
    llvm::BasicBlock *ifBlock = llvm::BasicBlock::Create(*context, "if", function);
    llvm::BasicBlock *elseBlock = llvm::BasicBlock::Create(*context, "else");
    llvm::BasicBlock *mergeBlock = llvm::BasicBlock::Create(*context, "ifcont");

    // Insertar la condición y el salto condicional al bloque if o else
    builder->CreateCondBr(condition, ifBlock, elseBlock);
    builder->SetInsertPoint(ifBlock);

    // Generar el código para el bloque del if
    visit(ctx->stat(0)); // Visitar el bloque del if

    // Agregar salto para continuar después del bloque del if
    builder->CreateBr(mergeBlock);
    function->getBasicBlockList().push_back(elseBlock);
    builder->SetInsertPoint(elseBlock);

    // Generar el código para el bloque del else si existe
    if (ctx->ELSE()) {
        visit(ctx->stat(1)); // Visitar el bloque del else
    }

    // Agregar salto para continuar después del bloque del else o el if
    builder->CreateBr(mergeBlock);
    function->getBasicBlockList().push_back(mergeBlock);
    builder->SetInsertPoint(mergeBlock);

    return std::any();
}

std::any visitshell::visitBlank(shellParser::BlankContext *ctx)
{
	std::cout << "visitBlank\n";
	return visitChildren(ctx);
}

std::any visitshell::visitNumber(shellParser::NumberContext *ctx)
{
	std::cout << "visitNumber\n";
	// Obtiene el valor numérico del número y crea un objeto llvm::ConstantFP con ese valor
	auto numVal = std::stod(ctx->NUMBER()->getText());
	llvm::Value *val = llvm::ConstantFP::get(*context, llvm::APFloat(numVal));
	return std::any(val);
	// return visitChildren(ctx);
}
std::any visitshell::visitAdd(shellParser::AddContext *ctx)
{
	std::cout << "visitAdd\n";
	// Obtiene los valores de las expresiones (ONE) y (TWO)
	llvm::Value *ONE = std::any_cast<llvm::Value *>(visit(ctx->expr(1)));
	llvm::Value *TWO = std::any_cast<llvm::Value *>(visit(ctx->expr(2)));

	return std::any(builder->CreateFAdd(ONE, TWO, "addTemp"));
	// return visitChildren(ctx);
}
std::any visitshell::visitSub(shellParser::SubContext *ctx)
{
	std::cout << "visitAdd\n";
	// Obtiene los valores de las expresiones (ONE) y (TWO)
	llvm::Value *ONE = std::any_cast<llvm::Value *>(visit(ctx->expr(1)));
	llvm::Value *TWO = std::any_cast<llvm::Value *>(visit(ctx->expr(2)));

	return std::any(builder->CreateFAdd(ONE, TWO, "addTemp"));
	// return visitChildren(ctx);
}

std::any visitshell::visitMul(shellParser::MulContext *ctx)
{
	std::cout << "visitMul\n";
	// Obtiene los valores de las expresiones (ONE) y (TWO)
	llvm::Value *ONE = std::any_cast<llvm::Value *>(visit(ctx->expr(1)));
	llvm::Value *TWO = std::any_cast<llvm::Value *>(visit(ctx->expr(2)));

	return std::any(builder->CreateFMul(ONE, TWO, "mulTemp"));
	// return visitChildren(ctx);
}

std::any visitshell::visitDiv(shellParser::DivContext *ctx)
{
	std::cout << "visitAdd\n";
	// Obtiene los valores de las expresiones (ONE) y (TWO)
	llvm::Value *ONE = std::any_cast<llvm::Value *>(visit(ctx->expr(1)));
	llvm::Value *TWO = std::any_cast<llvm::Value *>(visit(ctx->expr(2)));

	return std::any(builder->CreateFDiv(ONE, TWO, "divTemp"));
	// return visitChildren(ctx);
}
std::any visitshell::visitComparison(shellParser::ComparisonContext *ctx)
{
	std::cout << "visitComparison\n";

	llvm::Value *ONE = std::any_cast<llvm::Value *>(visit(ctx->expr(1)));
	llvm::Value *TWO = std::any_cast<llvm::Value *>(visit(ctx->expr(2)));

	// Realiza la operación de comparación según el operador
	std::string comp = ctx->op->getText();
	llvm::Value *result;

	if (comp == "<")
	{
		// Realiza la operación de comparación 'menor que'
		result = builder->CreateICmpSLT(ONE, TWO, "ltTemp");
	}
	else if (comp == ">")
	{
		// Realiza la operación de comparación 'mayor que'
		result = builder->CreateICmpSGT(ONE, TWO, "gtTemp");
	}
	else if (comp == "==")
	{
		// Realiza la operación de comparación 'igual a'
		result = builder->CreateICmpEQ(ONE, TWO, "eqTemp");
	}
	else
	{
		// Realiza la operación de comparación 'diferente de'
		result = builder->CreateICmpNE(ONE, TWO, "neqTemp");
	}

	return std::any(result);
}

std::any visitshell::visitId(shellParser::IdContext *ctx)
{
	std::cout << "visitId\n";

	// Obtiene el nombre del identificador (idName)
	std::string idName = ctx->ID()->getText();
	// crea una instrucción AllocaInst para reservar memoria
	llvm::AllocaInst *Alloca = CreateEntryBlockAlloca(idName);
	// lo almacena en el mapa memory
	memory[idName] = Alloca;
	return std::any(Alloca);
	// return visitChildren(ctx);
}
