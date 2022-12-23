//
// Created by 86180 on 2022/12/22.
//

#ifndef COMPILER_BLOCKAST_H
#define COMPILER_BLOCKAST_H


#include "../AST.h"
#include "SentenceAST.h"
#include "Sentences.h"

class BlockAST: public AST{
public:
    vector<unique_ptr<SentenceAST>> sentences;
    void accept(Visitor &visitor) override;
    void setSentences(Sentences* sentences);
};


#endif //COMPILER_BLOCKAST_H
