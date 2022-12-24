#ifndef RDTree_H
#define RDTree_H
#include "Pass.h"
#include "../IR/IR.h"

#include <vector>
#include <memory>
#include <set>
#include <map>

class RDTree : public Pass 
{
//Ö§ÅäÊ÷
public:
    explicit RDTree(Module* module) : Pass(module) {}
    void execute()final;
    void getRePostOrder(Function* f);
    void getPostOrder(Basic_Block* bb, std::set<Basic_Block*>& visited);
    void getBasicBIrdom(Function* f);
    void getBasicBRdom(Function* f);
    void getBasicBRdomFront(Function* f);
    const std::string getName() const override { return name; }
    Basic_Block* Intersect(Basic_Block* b1, Basic_Block* b2);
private:
    Basic_Block* exitBlock = nullptr;
    std::list<Basic_Block*> RePostOrder;
    std::map<Basic_Block*, int> BB2int;
    std::vector<Basic_Block*> rdoms;
    const std::string name = "RDTree";
};

#endif //RDTree_H
