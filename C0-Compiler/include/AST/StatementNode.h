#pragma once

#include "ASTNode.h"
#include "ExprisionNode.h"
#include "DeclarationNode.h"

#include <memory>
#include <vector>

namespace CC {
    class Declaration;

    // 语句节点类型
    enum class StatementType {
        COMPOUND_STMT,
        EXPR_STMT,
        IF_STMT,
        WHILE_STMT,
        FOR_STMT,
        DO_WHILE_STMT,
        RETURN_STMT,
        BREAK_STMT,
        CONTINUE_STMT,
        DECL_STMT,
        NULL_STMT,
    };

    class Statement : public INFRA::EnableShared<Statement> {
    public:
        StatementType type;
    };

    // 语句基类
    template <typename Derived>
    class StatementNode : public ASTNode<Derived>, public Statement {
    public:
        explicit StatementNode(StatementType type) : ASTNode<Derived>() {
            this->type = type;
        }
        
        static bool classof(const Statement* node) {
            return node->type == Derived::type;
        }
    };

    // 表达式语句
    class ExpressionStmt : public StatementNode<ExpressionStmt> {
    public:
        std::shared_ptr<Expression> expression;
        
        explicit ExpressionStmt(std::shared_ptr<Expression> expression)
            : StatementNode<ExpressionStmt>(StatementType::EXPR_STMT),
              expression(std::move(expression)) {}
              
        static bool classof(const Statement* node) {
            return node->type == StatementType::EXPR_STMT;
        }
    };

    // 复合语句（代码块）
    class CompoundStmt : public StatementNode<CompoundStmt> {
    public:
        std::vector<std::shared_ptr<StatementNode>> statements;
        
        explicit CompoundStmt(std::vector<std::shared_ptr<StatementNode>> statements)
            : StatementNode<CompoundStmt>(StatementType::COMPOUND_STMT),
              statements(std::move(statements)) {}
              
        static bool classof(const Statement* node) {
            return node->type == StatementType::COMPOUND_STMT;
        }
    };

    // If语句
    class IfStmt : public StatementNode<IfStmt> {
    public:
        std::shared_ptr<Expression> condition;
        std::shared_ptr<Statement> thenStmt;
        std::shared_ptr<Statement> elseStmt; // 可能为空
        
        IfStmt(std::shared_ptr<Expression> condition,
               std::shared_ptr<Statement> thenStmt,
               std::shared_ptr<Statement> elseStmt)
            : StatementNode<IfStmt>(StatementType::IF_STMT),
              condition(std::move(condition)),
              thenStmt(std::move(thenStmt)),
              elseStmt(std::move(elseStmt)) {}
              
        static bool classof(const Statement* node) {
            return node->type == StatementType::IF_STMT;
        }
    };

    // While语句
    class WhileStmt : public StatementNode<WhileStmt> {
    public:
        std::shared_ptr<Expression> condition;
        std::shared_ptr<Statement> body;
        
        WhileStmt(std::shared_ptr<Expression> condition,
                  std::shared_ptr<Statement> body)
            : StatementNode<WhileStmt>(StatementType::WHILE_STMT),
              condition(std::move(condition)),
              body(std::move(body)) {}
              
        static bool classof(const Statement* node) {
            return node->type == StatementType::WHILE_STMT;
        }
    };

    // Return语句
    class ReturnStmt : public StatementNode<ReturnStmt> {
    public:
        std::shared_ptr<Expression> expression; // 可能为空
        
        explicit ReturnStmt(std::shared_ptr<Expression> expression)
            : StatementNode<ReturnStmt>(StatementType::RETURN_STMT),
              expression(std::move(expression)) {}
              
        static bool classof(const Statement* node) {
            return node->type == StatementType::RETURN_STMT;
        }
    };

    // For语句
    class ForStmt : public StatementNode<ForStmt> {
    public:
        std::shared_ptr<Statement> init;
        std::shared_ptr<Expression> condition;
        std::shared_ptr<Expression> increment;
        std::shared_ptr<Statement> body;
        
        ForStmt(std::shared_ptr<Statement> init,
                std::shared_ptr<Expression> condition,
                std::shared_ptr<Expression> increment,
                std::shared_ptr<Statement> body)
            : StatementNode<ForStmt>(StatementType::FOR_STMT),
              init(std::move(init)),
              condition(std::move(condition)),
              increment(std::move(increment)),
              body(std::move(body)) {}
              
        static bool classof(const Statement* node) {
            return node->type == StatementType::FOR_STMT;
        }
    };

    // Do-While语句
    class DoWhileStmt : public StatementNode<DoWhileStmt> {
    public:
        std::shared_ptr<Statement> body;
        std::shared_ptr<Expression> condition;
        
        DoWhileStmt(std::shared_ptr<Statement> body,
                    std::shared_ptr<Expression> condition)
            : StatementNode<DoWhileStmt>(StatementType::DO_WHILE_STMT),
              body(std::move(body)),
              condition(std::move(condition)) {}
              
        static bool classof(const Statement* node) {
            return node->type == StatementType::DO_WHILE_STMT;
        }
    };

    // Break语句
    class BreakStmt : public StatementNode<BreakStmt> {
    public:
        BreakStmt() : StatementNode<BreakStmt>(StatementType::BREAK_STMT) {}
        
        static bool classof(const Statement* node) {
            return node->type == StatementType::BREAK_STMT;
        }
    };

    // Continue语句
    class ContinueStmt : public StatementNode<ContinueStmt> {
    public:
        ContinueStmt() : StatementNode<ContinueStmt>(StatementType::CONTINUE_STMT) {}
        
        static bool classof(const Statement* node) {
            return node->type == StatementType::CONTINUE_STMT;
        }
    };

    // 声明语句
    class DeclStmt : public StatementNode<DeclStmt> {
    public:
        std::shared_ptr<Declaration> declaration;
        
        explicit DeclStmt(std::shared_ptr<Declaration> declaration)
            : StatementNode<DeclStmt>(StatementType::DECL_STMT),
              declaration(std::move(declaration)) {}
              
        static bool classof(const Statement* node) {
            return node->type == StatementType::DECL_STMT;
        }
    };

    // 空语句
    class NullStmt : public StatementNode<NullStmt> {
    public:
        NullStmt() : StatementNode<NullStmt>(StatementType::NULL_STMT) {}
        
        static bool classof(const Statement* node) {
            return node->type == StatementType::NULL_STMT;
        }
    };
}