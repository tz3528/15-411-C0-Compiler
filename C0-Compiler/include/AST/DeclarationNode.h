//
// Created by 陶子杨 on 25-10-27.
//

#pragma once

#include "ASTNode.h"
#include "ExprisionNode.h"
#include "StatementNode.h"
#include <string>
#include <utility>
#include <vector>
#include <memory>

namespace CC {

    class CompoundStmt;

    // 声明节点类型
    enum class DeclarationType {
        FUNCTION_DECL,
        VARIABLE_DECL,
        PARAMETER_DECL
    };

    class Declaration : public INFRA::EnableShared<Declaration>{
    public:
        DeclarationType type;
    };

    // 声明基类
    template <typename Derived>
    class DeclarationNode : public ASTNode<Derived>, public Declaration{
    public:
        explicit DeclarationNode(DeclarationType type) : ASTNode<Derived>() {
            this->type = type;
        }

        static bool classof(const DeclarationNode* decl) {
            return decl->type == Derived::type;
        }
    };

    // 函数声明
    class FunctionDecl : public DeclarationNode<FunctionDecl> {
    public:
        std::string name;
        std::string returnType;
        std::vector<std::shared_ptr<Declaration>> parameters;
        std::shared_ptr<CompoundStmt> body;

        FunctionDecl(std::string  name,
                    std::string  returnType,
                    std::vector<std::shared_ptr<Declaration>> parameters,
                    std::shared_ptr<CompoundStmt> body)
            : DeclarationNode<FunctionDecl>(DeclarationType::FUNCTION_DECL),
              name(std::move(name)),
              returnType(std::move(returnType)),
              parameters(std::move(parameters)),
              body(std::move(body)) {}

        static bool classof(const Declaration* decl) {
            return decl->type == DeclarationType::FUNCTION_DECL;
        }
    };

    // 变量声明
    class VariableDecl : public DeclarationNode<VariableDecl> {
    public:
        std::string name;
        std::string type;
        std::shared_ptr<Expression> initializer;
        
        VariableDecl(std::string  name,
                    std::string  type,
                    std::shared_ptr<Expression> initializer)
            : DeclarationNode<VariableDecl>(DeclarationType::VARIABLE_DECL),
              name(std::move(name)),
              type(std::move(type)),
              initializer(std::move(initializer)) {}
              
        static bool classof(const Declaration* decl) {
            return decl->type == DeclarationType::VARIABLE_DECL;
        }
    };

}