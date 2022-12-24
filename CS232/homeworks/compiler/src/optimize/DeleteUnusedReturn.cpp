#include "DeleteUnusedReturn.h"

#define CONST_INT(num) new Constant_Int(module->int32_ty_, num)
#define CONST_FLOAT(num) new Constant_Float(module->float32_ty_, num)

void DeleteUnusedReturn::execute() {
    std::vector<Instruction*> dlt_vec;
    for (auto fun : module->function_list_) {
        if (fun->basic_blocks_.empty() || fun->name_ == "main")
            continue; // 主函数肯定有用，不用管
        dlt_vec.clear();
        // 返回值未被使用过
        if (!fun->use_ret_cnt && dynamic_cast<Function_Type*>(fun->type_)->result_->tid_ != Type::VoidTyID) {
            // std::cout << fun->name_ << std::endl;
            auto retBB = fun->basic_blocks_[1];
            //删除return块中指令
            for (auto instr : retBB->instr_list_) {
                dlt_vec.push_back(instr);  // 从指令列表中收集所有指令
            }
            for (auto instr : dlt_vec) {
                retBB->delete_instr(instr);
            }
            //新增ret 0为返回指令(返回0)
            if (dynamic_cast<Function_Type*>(fun->type_)->result_->tid_ == Type::IntegerTyID) {
                auto ret0 = new Return_Instruction(CONST_INT(0), retBB, true);
                retBB->add_instruction(ret0);
            }
            else {
                auto ret0 = new Return_Instruction(CONST_FLOAT(0), retBB, true);
                retBB->add_instruction(ret0);
            }
        }
    }
}