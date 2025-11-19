#pragma once

#include "ASTNode.h"
#include <memory>
#include <utility>
#include <vector>
#include <string>

#include "Infra/EnableShared.h"

namespace CC {

    // 表达式节点类型
    enum class ExpressionType {
        ASSIGNMENT_EXPR,
        BINARY_EXPR,
        UNARY_EXPR,
        CALL_EXPR,
        ARRAY_SUBSCRIPT_EXPR,
        IDENTIFIER_EXPR,
        INTEGER_LITERAL_EXPR,
        STRING_LITERAL_EXPR,
        CHAR_LITERAL_EXPR,
        BOOL_LITERAL_EXPR,
        FLOAT_LITERAL_EXPR,
    };

    class Expression : public INFRA::EnableShared<Expression> {
    public:
        ExpressionType type;
    };

    // 表达式基类
    template <typename Derived>
    class ExpressionNode : public ASTNode<Derived>, public Expression {
    public:
        explicit ExpressionNode(ExpressionType type) : ASTNode<Derived>() {
            this->type = type;
        }

        static bool classof(const Expression* node) {
            return node->type == Derived::type;
        }
    };

    // 二元表达式
    class BinaryExpr : public ExpressionNode<BinaryExpr> {
    public:
        std::shared_ptr<Expression> left;
        std::shared_ptr<Expression> right;
        TokenType op;

        BinaryExpr(std::shared_ptr<Expression> left,
                  std::shared_ptr<Expression> right,
                  TokenType op)
            : ExpressionNode<BinaryExpr>(ExpressionType::BINARY_EXPR),
              left(std::move(left)),
              right(std::move(right)),
              op(op) {}

        static bool classof(const Expression* node) {
            return node->type == ExpressionType::BINARY_EXPR;
        }
    };

    // 一元表达式
    class UnaryExpr : public ExpressionNode<UnaryExpr> {
    public:
        std::shared_ptr<Expression> operand;
        TokenType op;
        bool prefix;

        UnaryExpr(std::shared_ptr<Expression> operand,
                 TokenType op,
                 bool prefix)
            : ExpressionNode<UnaryExpr>(ExpressionType::UNARY_EXPR),
              operand(std::move(operand)),
              op(op),
              prefix(prefix) {}

        static bool classof(const Expression* node) {
            return node->type == ExpressionType::UNARY_EXPR;
        }
    };

    // 赋值表达式
    class AssignmentExpr : public ExpressionNode<AssignmentExpr> {
    public:
        std::shared_ptr<Expression> left;
        std::shared_ptr<Expression> right;
        TokenType op;

        AssignmentExpr(std::shared_ptr<Expression> left,
                      std::shared_ptr<Expression> right,
                      TokenType op)
            : ExpressionNode<AssignmentExpr>(ExpressionType::ASSIGNMENT_EXPR),
              left(std::move(left)),
              right(std::move(right)),
              op(op) {}

        static bool classof(const Expression* node) {
            return node->type == ExpressionType::ASSIGNMENT_EXPR;
        }
    };

    // 数组下标表达式
    class ArraySubscriptExpr : public ExpressionNode<ArraySubscriptExpr> {
    public:
        std::shared_ptr<Expression> base;
        std::shared_ptr<Expression> index;

        ArraySubscriptExpr(std::shared_ptr<Expression> base,
                          std::shared_ptr<Expression> index)
            : ExpressionNode<ArraySubscriptExpr>(ExpressionType::ARRAY_SUBSCRIPT_EXPR),
              base(std::move(base)),
              index(std::move(index)) {}

        static bool classof(const Expression* node) {
            return node->type == ExpressionType::ARRAY_SUBSCRIPT_EXPR;
        }
    };

    // 函数调用表达式
    class CallExpr : public ExpressionNode<CallExpr> {
    public:
        std::shared_ptr<Expression> callee;
        std::vector<std::shared_ptr<Expression>> arguments;

        CallExpr(std::shared_ptr<Expression> callee,
                std::vector<std::shared_ptr<Expression>> arguments)
            : ExpressionNode<CallExpr>(ExpressionType::CALL_EXPR),
              callee(std::move(callee)),
              arguments(std::move(arguments)) {}

        static bool classof(const Expression* node) {
            return node->type == ExpressionType::CALL_EXPR;
        }
    };

    // 标识符表达式
    class IdentifierExpr : public ExpressionNode<IdentifierExpr> {
    public:
        std::string name;
        
        IdentifierExpr(const std::string& name)
            : ExpressionNode<IdentifierExpr>(ExpressionType::IDENTIFIER_EXPR),
              name(name) {}
              
        static bool classof(const Expression* node) {
            return node->type == ExpressionType::IDENTIFIER_EXPR;
        }
    };

    // 整数字面量表达式
    class IntegerLiteralExpr : public ExpressionNode<IntegerLiteralExpr> {
    public:
        int value;
        
        IntegerLiteralExpr(int value) 
            : ExpressionNode<IntegerLiteralExpr>(ExpressionType::INTEGER_LITERAL_EXPR),
              value(value) {}
              
        static bool classof(const Expression* node) {
            return node->type == ExpressionType::INTEGER_LITERAL_EXPR;
        }
    };

    // 字符串字面量表达式
    class StringLiteralExpr : public ExpressionNode<StringLiteralExpr> {
    public:
        std::string value;
        
        StringLiteralExpr(const std::string& value) 
            : ExpressionNode<StringLiteralExpr>(ExpressionType::STRING_LITERAL_EXPR),
              value(value) {}
              
        static bool classof(const Expression* node) {
            return node->type == ExpressionType::STRING_LITERAL_EXPR;
        }
    };

    // 字符字面量表达式
    class CharLiteralExpr : public ExpressionNode<CharLiteralExpr> {
    public:
        char value;
        
        CharLiteralExpr(char value) 
            : ExpressionNode<CharLiteralExpr>(ExpressionType::CHAR_LITERAL_EXPR),
              value(value) {}
              
        static bool classof(const Expression* node) {
            return node->type == ExpressionType::CHAR_LITERAL_EXPR;
        }
    };

    // 布尔字面量表达式
    class BoolLiteralExpr : public ExpressionNode<BoolLiteralExpr> {
    public:
        bool value;
        
        BoolLiteralExpr(bool value) 
            : ExpressionNode<BoolLiteralExpr>(ExpressionType::BOOL_LITERAL_EXPR),
              value(value) {}
              
        static bool classof(const Expression* node) {
            return node->type == ExpressionType::BOOL_LITERAL_EXPR;
        }
    };

    // 浮点字面量表达式
    class FloatLiteralExpr : public ExpressionNode<FloatLiteralExpr> {
    public:
        double value;
        
        FloatLiteralExpr(double value) 
            : ExpressionNode<FloatLiteralExpr>(ExpressionType::FLOAT_LITERAL_EXPR),
              value(value) {}
              
        static bool classof(const Expression* node) {
            return node->type == ExpressionType::FLOAT_LITERAL_EXPR;
        }
    };
}
