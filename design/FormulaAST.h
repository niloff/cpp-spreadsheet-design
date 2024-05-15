#pragma once

#include "FormulaLexer.h"
#include "common.h"

#include <forward_list>
#include <functional>
#include <stdexcept>

namespace ASTImpl {
class Expr;
}

class ParsingError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

using SheetFunctor = std::function<double(Position)>;

explicit FormulaAST(std::unique_ptr<ASTImpl::Expr> root_expr,
                    std::forward_list<Position> cells);
    FormulaAST(FormulaAST&&) = default;
    FormulaAST& operator=(FormulaAST&&) = default;
    ~FormulaAST();

    double Execute(const SheetFunctor& functor) const;
    
    void Print(std::ostream& out) const;
    void PrintFormula(std::ostream& out) const;
    void PrintCells(std::ostream& out) const;

    std::forward_list<Position>& GetCells() {
        return cells_;
    }

    const std::forward_list<Position>& GetCells() const {
        return cells_;
    }

private:
    std::unique_ptr<ASTImpl::Expr> root_expr_;

    // физически сохраняет ячейки, чтобы их можно было
    // смотреть без обхода всего AST
    std::forward_list<Position> cells_;
};

FormulaAST ParseFormulaAST(std::istream& in);
FormulaAST ParseFormulaAST(const std::string& in_str);