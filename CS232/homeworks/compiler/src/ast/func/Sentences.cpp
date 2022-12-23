//
// Created by 86180 on 2022/12/22.
//

#include "Sentences.h"

void Sentences::add(SentenceAST *sentence) {
    sentences.push_back(unique_ptr<SentenceAST>(sentence));
}
