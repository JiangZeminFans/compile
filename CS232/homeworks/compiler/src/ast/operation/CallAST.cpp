//
// Created by 86180 on 2022/12/22.
//

#include "CallAST.h"

void CallAST::accept(Visitor &visitor) {

}

void CallAST::setId(string *id) {
    this->id=unique_ptr<string>(id);
}
