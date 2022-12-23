//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_SENTENCES_H
#define COMPILER_SENTENCES_H


#include "../AST.h"
#include "SentenceAST.h"

class Sentences : public AST{
public:
    vector<unique_ptr<SentenceAST>> sentences;
    void add(SentenceAST* sentence);
};


#endif //COMPILER_SENTENCES_H
