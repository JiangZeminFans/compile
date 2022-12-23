//
// Created by 86180 on 2022/12/22.
//

#include "InitValAST.h"

void InitValAST::accept(Visitor &visitor) {

}

void InitValAST::setExpression(LowAST *expression) {
    this->exp=unique_ptr<LowAST>(expression);
}

void InitValAST::setInitValList(InitValList* list) {
    this->initValList.swap(list->initValList);
}
