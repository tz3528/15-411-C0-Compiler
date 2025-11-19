//
// Created by 陶子杨 on 25-10-27.
//

#pragma once

#include "ASTNode.h"
#include "DeclarationNode.h"
#include <vector>
#include <memory>

namespace CC {

    // 基础节点类型
    enum UnitType {
        TRANSLATION_UNIT,
    };

    class Unit : public INFRA::EnableShared<Unit> {
    public:
        UnitType type;
    };

    // 翻译单元
    class TranslationUnit : public ASTNode<TranslationUnit>, public Unit{
    public:
        std::vector<std::shared_ptr<Declaration>> declarations;
        
        explicit TranslationUnit(std::vector<std::shared_ptr<Declaration>> declarations)
            : ASTNode<TranslationUnit>(),declarations(std::move(declarations)) {
            this->type = UnitType::TRANSLATION_UNIT;
        }
              
        static bool classof(const Unit* node) {
            return node->type == UnitType::TRANSLATION_UNIT;
        }
    };
}