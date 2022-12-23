//
// Created by 86180 on 2022/12/22.
//

#include "DecDefAST.h"

void DecDefAST::accept(Visitor &visitor) {

}

void DecDefAST::setDecl(DecAST *decl) {
    this->Decl=unique_ptr<DecAST>(decl);
}

void DecDefAST::setDef(FuncDefAST *def) {
    this->funcDef=unique_ptr<FuncDefAST>(def);
}

