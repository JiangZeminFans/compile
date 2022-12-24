#include "RDTree.h"

void RDTree::execute()
{
    for (auto f : module->function_list_) 
    {
        if (f->basic_blocks_.empty()) 
        {
            continue;
        }
        for (auto bb : f->basic_blocks_) 
        {
            bb->rdoms_.clear();
            bb->rdom_frontier_.clear();
        }
        getBasicBIrdom(f);
        getBasicBRdomFront(f);
        getBasicBRdom(f);
    }
}

void RDTree::getPostOrder(Basic_Block* bb, std::set<Basic_Block*>& visited) 
{
    visited.insert(bb);
    for (auto parent : bb->pre_bbs_) 
    {
        auto is_visited = visited.find(parent);
        if (is_visited == visited.end()) 
        {
            getPostOrder(parent, visited);
        }
    }
    BB2int[bb] = RePostOrder.size();
    RePostOrder.push_back(bb);
}

void RDTree::getRePostOrder(Function* f) 
{
    rdoms.clear();
    RePostOrder.clear();
    BB2int.clear();
    for (auto bb : f->basic_blocks_) 
    {
        auto terminate_instr = bb->get_terminator();
        if (terminate_instr->op_id_ == Instruction::Ret) 
        {
            exitBlock = bb;
            break;
        }
    }
    if (!exitBlock) 
    {
        std::cerr << "exit block is null, function must have only one exit block with a ret instr\n";
        std::cerr << "err function:\n" << f->print() << std::endl;
        exit(1);
    }
    std::set<Basic_Block*> visited = {};
    getPostOrder(exitBlock, visited);
    RePostOrder.reverse();
}

void RDTree::getBasicBIrdom(Function* f) 
{
    getRePostOrder(f);
    auto root = exitBlock;
    auto root_id = BB2int[root];
    for (int i = 0; i < root_id; i++) 
    {
        rdoms.push_back(nullptr);
    }
    rdoms.push_back(root);
    bool changed = true;
    while (changed) 
    {
        changed = false;
        for (auto bb : RePostOrder) 
        {
            if (bb == root) 
            {
                continue;
            }
            Basic_Block* new_irdom = nullptr;
            for (auto rpred_bb : bb->succ_bbs_) 
            {
                if (rdoms[BB2int[rpred_bb]] != nullptr) 
                {
                    new_irdom = rpred_bb;
                    break;
                }
            }
            for (auto rpred_bb : bb->succ_bbs_) 
            {
                if (rdoms[BB2int[rpred_bb]] != nullptr) 
                {
                    new_irdom = Intersect(rpred_bb, new_irdom);
                }
            }
            if (rdoms[BB2int[bb]] != new_irdom) 
            {
                rdoms[BB2int[bb]] = new_irdom;
                changed = true;
            }
        }
    }
}

void RDTree::getBasicBRdom(Function* f) 
{
    for (auto bb : f->basic_blocks_) 
    {
        if (bb == exitBlock) 
        {
            continue;
        }
        auto current = bb;
        while (current != exitBlock) 
        {
            bb->rdoms_.insert(current);
            current = rdoms[BB2int[current]];
        }
    }
}

void RDTree::getBasicBRdomFront(Function* f) 
{
    for (auto bb_iter = f->basic_blocks_.rbegin(); bb_iter != f->basic_blocks_.rend(); bb_iter++) 
    {
        //reverse bb order;
        auto bb = *bb_iter;
        if (bb->succ_bbs_.size() >= 2) 
        {
            for (auto rpred : bb->succ_bbs_) 
            {
                auto runner = rpred;
                while (runner != rdoms[BB2int[bb]]) 
                {
                    runner->rdom_frontier_.insert(bb);
                    runner = rdoms[BB2int[runner]];
                }
            }
        }
    }
}

Basic_Block* RDTree::Intersect(Basic_Block* b1, Basic_Block* b2) 
{
    auto finger1 = b1;
    auto finger2 = b2;
    while (finger1 != finger2) 
    {
        while (BB2int[finger1] < BB2int[finger2]) 
        {
            finger1 = rdoms[BB2int[finger1]];
        }
        while (BB2int[finger2] < BB2int[finger1]) 
        {
            finger2 = rdoms[BB2int[finger2]];
        }
    }
    return finger1;
}
