//
// Created by 86180 on 2022/12/22.
//

#include "BlockAST.h"

void BlockAST::accept(Visitor &visitor) {

}

void BlockAST::setSentences(Sentences *sentences) {
    this->sentences.swap(sentences->sentences);
}
