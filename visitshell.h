
#ifndef __VISIT_SHELL__
#define __VISIT_SHELL__

// Cabeceras para el parser y visitor de Antlr4
#include "shellBaseVisitor.h"
#include "shellParser.h"

// Cabeceras para generar código LLVM
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"

#include <algorithm>
#include <any>
#include <iostream>
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>
#include <llvm/Support/raw_ostream.h>
#include <map>
#include <memory>
#include <string>
#include <system_error>

// Clase que hereda shellBaseVisitor para sobreescribir sus métodos
class visitshell : shellBaseVisitor
{
public:
	visitshell()
		: // Creación de contexto, módulo y un constructor IR
		  context(std::make_unique<llvm::LLVMContext>()),
		  module(std::make_unique<llvm::Module>("EF", *context)),
		  builder(std::make_unique<llvm::IRBuilder<>>(*context))
	{
	}
	// Sobrescribe los métodos de visita generados por ANTLR4 para cada regla definida en la gramática
	virtual std::any visitProg(shellParser::ProgContext *ctx) override;
	virtual std::any visitPrintExpr(shellParser::PrintExprContext *ctx) override;
	virtual std::any visitAssign(shellParser::AssignContext *ctx) override;
	virtual std::any visitIfStatement(shellParser::IfStatementContext *ctx) override;
	virtual std::any visitBlank(shellParser::BlankContext *ctx) override;
	virtual std::any visitMul(shellParser::MulContext *ctx) override;
	virtual std::any visitDiv(shellParser::DivContext *ctx) override;
	virtual std::any visitAdd(shellParser::AddContext *ctx) override;
	virtual std::any visitSub(shellParser::SubContext *ctx) override;
	virtual std::any visitNumber(shellParser::NumberContext *ctx) override;
	virtual std::any visitComparison(shellParser::ComparisonContext *ctx) override;
	virtual std::any visitId(shellParser::IdContext *ctx) override;
	void test()
	{
		// Generación de un test.ll para guardar le código LLVM generado del modulo
		std::error_code error;
		llvm::raw_fd_stream outLL("shell.ll", error);
		module->print(outLL, nullptr);
	}

private:
	std::map<std::string, std::any> memory;
	std::unique_ptr<llvm::LLVMContext> context;
	std::unique_ptr<llvm::Module> module;
	std::unique_ptr<llvm::IRBuilder<>> builder;

	// aux
	llvm::Function *F;

	// aux methods
	// Método auxiliar que crea una instrucción de asignación de memoria
	// para una variable local en el bloque de entrada de una función LLVM

	// devuelve un puntero a una instrucción AllocaInst de LLVM.
	// llvm::StringRef -> nombre de la variable que se va a asignar en la pila
	llvm::AllocaInst *CreateEntryBlockAlloca(llvm::StringRef varName)
	{
		// Crea un objeto IRBuilder llamado TmpB.
		// construye un instrucción IR dentro de un bloque específico (EntryBlock) de una función LLVM (F)
		// Se coloca al principio del bloque de entrada (begin()).
		llvm::IRBuilder<> TmpB(&F->getEntryBlock(), F->getEntryBlock().begin());
		return TmpB.CreateAlloca(llvm::Type::getDoubleTy(*context), nullptr,
								 varName);
		// Crea una instrucción que reserva espacio en la pila para una variable local de tipo double
		// en el bloque de entrada de una función LLVM
	}
};

#endif