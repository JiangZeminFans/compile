//
// Created by 86180 on 2022/12/22.
//

#include "AST.h"

void AST::accept(Visitor &visitor) {
    visitor.visit(this);
}
