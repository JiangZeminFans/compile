#include "IR.h"

std::map<Instruction::OpID, std::string> instr_id2string_ = {
    {Instruction::Ret,"ret"},{Instruction::Br,"br"},{Instruction::FNeg,"fneg"},
    {Instruction::Add,"add"},{Instruction::Sub,"sub"},{Instruction::Mul,"mul"},
    {Instruction::SDiv,"sdiv"},{Instruction::SRem,"srem"},{Instruction::UDiv,"udiv"},{Instruction::URem,"urem"},
    {Instruction::FAdd,"fadd"},{Instruction::FSub,"fsub"},{Instruction::FMul,"fmul"},{Instruction::FDiv,"fdiv"},
    {Instruction::Shl,"shl"},{Instruction::LShr,"lshr"},{Instruction::AShr,"ashr"},{Instruction::And,"and"},
    {Instruction::Or,"or"},{Instruction::Xor,"xor"},{Instruction::Alloca,"alloca"},{Instruction::Load,"load"},
    {Instruction::Store,"store"},{Instruction::GetElementPtr,"getelementptr"},{Instruction::ZExt,"zext"},
    {Instruction::FPtoSI,"fptosi"},{Instruction::SItoFP,"sitofp"}, {Instruction::BitCast, "bitcast"},
    {Instruction::ICmp,"icmp"},{Instruction::FCmp,"fcmp"},{Instruction::PHI,"phi"},{Instruction::Call,"call"}};
    // Instruction from OPID to string

const std::map<ICmp_Instruction::ICmp_Op, std::string> ICmp_Instruction::ICmp_Op_Name = {
    {ICmp_Instruction::ICmp_Op::ICMP_EQ, "eq"}, {ICmp_Instruction::ICmp_Op::ICMP_NE, "ne"}, 
    {ICmp_Instruction::ICmp_Op::ICMP_UGT, "hi"}, {ICmp_Instruction::ICmp_Op::ICMP_UGE, "cs"}, 
    {ICmp_Instruction::ICmp_Op::ICMP_ULT, "cc"}, {ICmp_Instruction::ICmp_Op::ICMP_ULE, "ls"}, 
    {ICmp_Instruction::ICmp_Op::ICMP_SGT, "gt"}, {ICmp_Instruction::ICmp_Op::ICMP_SGE, "ge"}, 
    {ICmp_Instruction::ICmp_Op::ICMP_SLT, "lt"}, {ICmp_Instruction::ICmp_Op::ICMP_SLE, "le"}};

const std::map<FCmp_Instruction::FCmp_Op, std::string> FCmp_Instruction::FCmp_Op_Name = {
    {FCmp_Instruction::FCmp_Op::FCMP_FALSE, "nv"}, {FCmp_Instruction::FCmp_Op::FCMP_OEQ, "eq"}, 
    {FCmp_Instruction::FCmp_Op::FCMP_OGT, "gt"}, {FCmp_Instruction::FCmp_Op::FCMP_OGE, "ge"}, 
    {FCmp_Instruction::FCmp_Op::FCMP_OLT, "cc"}, {FCmp_Instruction::FCmp_Op::FCMP_OLE, "ls"}, 
    {FCmp_Instruction::FCmp_Op::FCMP_ONE, "ne"}, {FCmp_Instruction::FCmp_Op::FCMP_ORD, "vc"}, 
    {FCmp_Instruction::FCmp_Op::FCMP_UNO, "vs"}, {FCmp_Instruction::FCmp_Op::FCMP_UEQ, "eq"}, 
    {FCmp_Instruction::FCmp_Op::FCMP_UGT, "hi"}, {FCmp_Instruction::FCmp_Op::FCMP_UGE, "cs"}, 
    {FCmp_Instruction::FCmp_Op::FCMP_ULT, "lt"}, {FCmp_Instruction::FCmp_Op::FCMP_ULE, "le"}, 
    {FCmp_Instruction::FCmp_Op::FCMP_UNE, "ne"}, {FCmp_Instruction::FCmp_Op::FCMP_TRUE, "al"}};


std::string print_as_op( Value *v, bool print_ty );
std::string print_cmp_type( ICmp_Instruction::ICmp_Op op);
std::string print_fcmp_type( FCmp_Instruction::FCmp_Op op);

//-----------------------------------------------Type-----------------------------------------------
std::string Type::print()
{
    std::string type_ir;
    switch (this->tid_)
    {
        case VoidTyID:
            type_ir += "void";
            break;

        case LabelTyID:
            type_ir += "label";
            break;

        case IntegerTyID:
            type_ir += "i";
            type_ir += std::to_string( static_cast<Integer_Type *>(this)->num_bits_);
            break;

        case FloatTyID:
            type_ir += "float";
            break;

        case FunctionTyID:
            type_ir += static_cast<Function_Type *>(this)->result_->print();
            type_ir += " (";
            for (size_t i = 0 ; i < static_cast<Function_Type *>(this)->args_.size() ;i ++ )
            {
                if (i) type_ir += ", ";
                type_ir += static_cast<Function_Type *>(this)->args_[i]->print();
            }
            type_ir += ")";
            break;

        case PointerTyID:
            type_ir += static_cast<Pointer_Type *>(this)->contained_->print();
            type_ir += "*";
            break;

        case ArrayTyID:
            type_ir += "[";
            type_ir += std::to_string( static_cast<Array_Type *>(this)->num_elements_);
            type_ir += " x ";
            type_ir += static_cast<Array_Type *>(this)->contained_->print();
            type_ir += "]";
            break;

        default:
            break;

    }
    return type_ir;
}


//-----------------------------------------------Value-----------------------------------------------
void Value::replace_all_use_with(Value *new_val)
{
    for (auto use : use_list_)
    {
        auto val = dynamic_cast<Instruction *>(use.val);

    #ifdef DEBUG
        assert(val && "new_val is not a user");
    #endif

        val->set_operand(use.num, new_val);
    }
}
bool Value::remove_used(Instruction* user,unsigned int i)
{
    if(this!=user->operands_[i])
        return false;
    
    auto pos = user->use_pos_[i];

    use_list_.erase( pos);
    user->operands_[i] = nullptr; //??????user->use_pos_[i]??????????????????set_operand???????????????
    return true;
}


//-----------------------------------------------Constant-----------------------------------------------
std::string Constant_Int::print()
{
    std::string const_ir;
    if (this->type_->tid_ == Type::IntegerTyID && static_cast<Integer_Type *>(this->type_)->num_bits_ == 1 )
    {
        //int1
        const_ir += (this->value_ == 0) ? "0" : "1";
    }
    else //int32
        const_ir += std::to_string(this->value_);
    return const_ir;
}

std::string Constant_Float::print()
{
    std::stringstream fp_ir_ss;
    std::string fp_ir;
    double val = this->value_;
    fp_ir_ss << "0x"<< std::hex << *(uint64_t *)&val << std::endl;
    fp_ir_ss >> fp_ir;
    return fp_ir;
}

std::string Constant_Array::print()
{
    std::string const_ir;
    const_ir += "[";
    const_ir += static_cast<Array_Type *>(this->type_)->contained_->print();
    const_ir += " ";
    const_ir += const_array[0]->print();
    for (size_t i = 1 ; i < this->const_array.size() ; i++ )
    {
        const_ir += ", ";
        const_ir += static_cast<Array_Type *>(this->type_)->contained_->print();
        const_ir += " ";
        const_ir += const_array[i]->print();
    }
    const_ir += "]";
    return const_ir;
}

std::string Constant_Zero::print()
{
    return "zeroinitializer";
}

//-----------------------------------------------Module-----------------------------------------------
std::string Module::print()
{
    std::string module_ir;
    for ( auto global_val : this->global_list_)
    {
        module_ir += global_val->print();
        module_ir += "\n";
    }

    for ( auto func : this->function_list_)
    {
        module_ir += func->print();
        module_ir += "\n";
    }

    return module_ir;
}

Function *Module::getMainFunc()
{
    for (auto f : function_list_)
    {
        if (f->name_ == "main")
            return f;
        
    }
    return nullptr;
}

//-----------------------------------------------GlobalVariable-----------------------------------------------
std::string Global_Variable::print()
{
    std::string global_val_ir;

    global_val_ir += print_as_op(this, false);
    global_val_ir += " = ";
    global_val_ir += (this->is_const_ ? "constant " : "global ");
    global_val_ir += static_cast<Pointer_Type *>(this->type_)->contained_->print();
    global_val_ir += " ";
    global_val_ir += this->init_val_->print();

    return global_val_ir;
}

//-----------------------------------------------Function-----------------------------------------------
std::string Function::print()
{
    if (this->name_ == "llvm.memset.p0.i32")
    {
        std::string func_ir = "declare void @llvm.memset.p0.i32(i32*, i8, i32, i1)";
        return func_ir;
    }

    set_instr_name();
    std::string func_ir;

    if ( this->is_declaration() )
        func_ir += "declare ";
    
    else
        func_ir += "define ";

    func_ir += this->get_return_type()->print();
    func_ir += " ";
    func_ir += print_as_op(this, false);
    func_ir += "(";

    //print arg
    if (this->is_declaration())
    {
        for (size_t i = 0 ; i < this->arguments_.size() ; i ++ )
        {
            if(i)
                func_ir += ", ";
            func_ir += static_cast<Function_Type *>(this->type_)->args_[i]->print();
        }
    }
    else
    {
        for (auto arg = this->arguments_.begin(); arg != arguments_.end() ; arg++ )
        {
            if( arg != this->arguments_.begin())
                func_ir += ", ";
            
            func_ir += static_cast<Argument *>(*arg)->print();
        }
    }
    func_ir += ")";

    //print bb
    if(!this->is_declaration())
    {
        func_ir += " {";
        func_ir += "\n";
        for ( auto bb : this->basic_blocks_)
            func_ir += bb->print();
        
        func_ir += "}";
    }

    return func_ir;
}

std::string Argument::print()
{
    std::string arg_ir;
    arg_ir += this->type_->print();
    arg_ir += " %";
    arg_ir += this->name_;
    return arg_ir;
}

void Function::remove_bb(Basic_Block* bb)
{
    //basic_blocks_.remove(bb); 
    basic_blocks_.erase(std::remove(basic_blocks_.begin(), basic_blocks_.end(), bb) , basic_blocks_.end());

    for (auto pre : bb->pre_bbs_)
        pre->remove_succ_basic_block(bb);
    
    for (auto succ : bb->succ_bbs_)
        succ->remove_pre_basic_block(bb);
    
}

Basic_Block *Function::getRetBB()
{
    for (auto bb : basic_blocks_)
    {
        if (bb->get_terminator()->is_ret())
            return bb;
        
    }
    return nullptr;
}


//-----------------------------------------------BasicBlock-----------------------------------------------
std::string Basic_Block::print()
{
    std::string bb_ir;
    bb_ir += this->name_;
    bb_ir += ":";

    // print prebb
    if(!this->pre_bbs_.empty())
        bb_ir += "                                                ; preds = ";

    for (auto bb : this->pre_bbs_)
    {
        if( bb != *this->pre_bbs_.begin() )
            bb_ir += ", ";
        bb_ir += print_as_op(bb, false);
    }

    // print prebb
    if ( !this->parent_)
    {
        bb_ir += "\n";
        bb_ir += "; Error: Block without parent!";
    }
    bb_ir += "\n";
    for (auto instr : this->instr_list_)
    {
        bb_ir += "  ";
        bb_ir += instr->print();
        bb_ir += "\n";
    }

    return bb_ir;
}

Instruction *Basic_Block::get_terminator()
{
    if (instr_list_.empty())
        return nullptr;
    switch (instr_list_.back()->op_id_)
    {
        case Instruction::Ret:

        case Instruction::Br:
            return instr_list_.back();
        
        default:
            return nullptr;
    }
}

bool Basic_Block::delete_instr(Instruction *instr)
{
    //******************--------instvec2list-----qwc20220814
    //instr_list_.remove(instr);
    if((!instr)||instr->pos_in_bb.size()!=1||instr->parent_!= this)
        return false;
    
    this->instr_list_.erase(instr->pos_in_bb.back());
    //instr_list_.erase(std::remove(instr_list_.begin(), instr_list_.end(), instr) , instr_list_.end());

    instr->remove_use_of_ops();
    instr->pos_in_bb.clear();//?????????????????????
    instr->parent_ = nullptr;

    return true;
}

bool Basic_Block::add_instruction(Instruction *instr)
{
//******************--------instvec2list-----qwc20220814

    if(instr->pos_in_bb.size()!=0) //????????????????????????????????????
        return false;
    else
    {
        instr_list_.push_back(instr);

        std::list<Instruction*>::iterator tail = instr_list_.end();

        instr->pos_in_bb.emplace_back(--tail);
        instr->parent_ = this;

        return true;
    }

}

bool Basic_Block::add_instruction_front(Instruction *instr)
{
//******************--------instvec2list-----qwc20220814
    if(instr->pos_in_bb.size()!=0) //????????????????????????????????????
        return false;
    else
    {
        instr_list_.push_front(instr);

        std::list<Instruction*>::iterator head = instr_list_.begin();

        instr->pos_in_bb.emplace_back(head);
        instr->parent_ = this;

        return true;
    }
}



//????????????????????????
bool Basic_Block::add_instruction_before_terminator(Instruction *instr)
{
    if(instr->pos_in_bb.size()!=0)  //????????????????????????????????????
        return false;
    else if(instr_list_.empty())    //??????????????????1??????????????????????????????
        return false;
    else
    {
        auto it = std::end(instr_list_);//??????????????????????????????

        instr_list_.emplace(--it, instr);//?????????????????????????????????--it?????????????????????it?????????????????????????????????????????????

        instr->pos_in_bb.emplace_back(--it);//?????????????????????
        instr->parent_ = this;

        return true;
    }
}

bool Basic_Block::add_instruction_before_inst(Instruction* new_instr,Instruction* instr)
{
    if((!instr)||instr->pos_in_bb.size()!=1||instr->parent_!= this)
        return false;

    if(new_instr->pos_in_bb.size()!=0) //????????????????????????????????????
        return false;

    else if(instr_list_.empty()) //bb????????????????????????instr??????bb???????????????instr->parent_== this
        return false;

    else
    {
        auto it = instr->pos_in_bb.back();

        instr_list_.emplace( it, new_instr);//?????????????????????????????????--it?????????????????????it?????????????????????????????????????????????

        new_instr->pos_in_bb.emplace_back(--it);//?????????????????????
        new_instr->parent_ = this;

        return true;
    }
}

//???bb??????????????????????????????????????????use?????????????????????????????????bb
bool Basic_Block::remove_instr(Instruction *instr)
{
    //instr_list_.remove(instr);
    if((!instr)||instr->pos_in_bb.size()!=1||instr->parent_!= this)
        return false;
    
    this->instr_list_.erase(instr->pos_in_bb.back());

//    instr->remove_use_of_ops();
    instr->pos_in_bb.clear(); //?????????????????????
    instr->parent_ = nullptr;

    return true;
}


//-----------------------------------------------Instruction-----------------------------------------------
std::string Binary_Instruction::print()
{
    std::string instr_ir;

    instr_ir += "%";
    instr_ir += this->name_;
    instr_ir += " = ";
    instr_ir += instr_id2string_[this->op_id_];
    instr_ir += " ";
    instr_ir += this->operands_[0]->type_->print();
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), false);
    instr_ir += ", ";

    assert(this->get_operand(0)->type_->tid_ == this->get_operand(1)->type_->tid_);

    instr_ir += print_as_op(this->get_operand(1), false);
    //instr_ir += print_as_op(this->get_operand(1), true);

    return instr_ir;
}

std::string Unary_Instruction::print()
{
    std::string instr_ir;

    instr_ir += "%";
    instr_ir += this->name_;
    instr_ir += " = ";
    instr_ir += instr_id2string_[this->op_id_];
    instr_ir += " ";
    instr_ir += this->operands_[0]->type_->print();
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), false);

    switch (this->op_id_)
    {
        case Instruction::ZExt:
            assert(this->type_->tid_ == Type::IntegerTyID);
            instr_ir += " to i32";
            break;

        case Instruction::FPtoSI:
            assert(this->type_->tid_ == Type::IntegerTyID);
            instr_ir += " to i32";
            break;

        case Instruction::SItoFP:
            assert(this->type_->tid_ == Type::FloatTyID);
            instr_ir += " to float";
            break;

        default:
            assert(0 && "UnaryInst opID invalid!");
            break;
        
    }
    return instr_ir;
}

std::string ICmp_Instruction::print()
{
    std::string instr_ir;

    instr_ir += "%";
    instr_ir += this->name_;
    instr_ir += " = ";
    instr_ir += instr_id2string_[this->op_id_];
    instr_ir += " ";
    instr_ir += print_cmp_type(this->icmp_op_);
    instr_ir += " ";
    instr_ir += this->get_operand(0)->type_->print();
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), false);
    instr_ir += ", ";

    if (this->get_operand(0)->type_->tid_ == this->get_operand(1)->type_->tid_)
        instr_ir += print_as_op(this->get_operand(1), false);
    else
        instr_ir += print_as_op(this->get_operand(1), true);
    
    return instr_ir;
}

std::string FCmp_Instruction::print()
{
    std::string instr_ir;

    instr_ir += "%";
    instr_ir += this->name_;
    instr_ir += " = ";
    instr_ir += instr_id2string_[this->op_id_];
    instr_ir += " ";
    instr_ir += print_fcmp_type(this->fcmp_op_);
    instr_ir += " ";
    instr_ir += this->get_operand(0)->type_->print();
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), false);
    instr_ir += ", ";

    if (this->get_operand(0)->type_->tid_ == this->get_operand(1)->type_->tid_)
        instr_ir += print_as_op(this->get_operand(1), false);
    else
        instr_ir += print_as_op(this->get_operand(1), true);
    
    return instr_ir;

}

std::string Call_Instruction::print()
{
    std::string instr_ir;

    if(!(this->type_->tid_ == Type::VoidTyID))
    {
        instr_ir += "%";
        instr_ir += this->name_;
        instr_ir += " = ";
    }

    instr_ir += instr_id2string_[this->op_id_];
    instr_ir += " ";

    unsigned int numops = this->num_ops_;

    instr_ir += static_cast<Function_Type *>(this->get_operand(numops-1)->type_)->result_->print();

    instr_ir += " ";

    assert(dynamic_cast<Function *>(this->get_operand(numops - 1)) && "Wrong call operand function");
    //__aeabi_memclr4 -> llvm_memset

    if (dynamic_cast<Function *>(this->get_operand(numops - 1))->name_ == "__aeabi_memclr4")
    {
        instr_ir += "@llvm.memset.p0.i32(";

        //i32* ??????????????????
        instr_ir += this->get_operand(0)->type_->print();
        instr_ir += " ";
        instr_ir += print_as_op(this->get_operand(0), false);

        //i8 0
        instr_ir += ", i8 0, ";

        //i32 ??????????????????
        instr_ir += this->get_operand(1)->type_->print();
        instr_ir += " ";
        instr_ir += print_as_op(this->get_operand(1), false);

        //i1 false
        instr_ir += ", i1 false)";
        
        return instr_ir;
    }

    instr_ir += print_as_op(this->get_operand(numops - 1), false);
    instr_ir += "(";

    for (unsigned int i = 0; i < numops - 1; i++)
    {
        if( i > 0 )
            instr_ir += ", ";
        instr_ir += this->get_operand(i)->type_->print();
        instr_ir += " ";
        instr_ir += print_as_op(this->get_operand(i), false);
    }

    instr_ir += ")";

    return instr_ir;
}

std::string Branch_Instruction::print()
{
    std::string instr_ir;

    instr_ir += instr_id2string_[this->op_id_];
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), true);

    if( this->num_ops_ == 3 )
    {
        instr_ir += ", ";
        instr_ir += print_as_op(this->get_operand(1), true);
        instr_ir += ", ";
        instr_ir += print_as_op(this->get_operand(2), true);
    }

    return instr_ir;
}

std::string Return_Instruction::print()
{
    std::string instr_ir;

    instr_ir += instr_id2string_[this->op_id_];
    instr_ir += " ";

    if ( this->num_ops_ != 0 )
    {
        instr_ir += this->get_operand(0)->type_->print();
        instr_ir += " ";
        instr_ir += print_as_op(this->get_operand(0), false);
    }
    else
        instr_ir += "void";
    

    return instr_ir;
}

std::string GetElementPtr_Instruction::print()
{
    std::string instr_ir;

    instr_ir += "%";
    instr_ir += this->name_;
    instr_ir += " = ";
    instr_ir += instr_id2string_[this->op_id_];
    instr_ir += " ";

    assert(this->get_operand(0)->type_->tid_==Type::PointerTyID);

    instr_ir += static_cast<Pointer_Type *>(this->get_operand(0)->type_)->contained_->print();
    instr_ir += ", ";

    for (unsigned int i = 0; i < this->num_ops_; i++)
    {
        if( i > 0 )
            instr_ir += ", ";
        
        instr_ir += this->get_operand(i)->type_->print();
        instr_ir += " ";
        instr_ir += print_as_op(this->get_operand(i), false);
    }
    return instr_ir;
}

std::string Store_Instruction::print()
{
    std::string instr_ir;

    instr_ir += instr_id2string_[this->op_id_];
    instr_ir += " ";
    instr_ir += this->get_operand(0)->type_->print();
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), false);
    instr_ir += ", ";
    instr_ir += print_as_op(this->get_operand(1), true);

    return instr_ir;
}

std::string Load_Instruction::print()
{
    std::string instr_ir;

    instr_ir += "%";
    instr_ir += this->name_;
    instr_ir += " = ";
    instr_ir += instr_id2string_[this->op_id_];
    instr_ir += " ";

    assert(this->get_operand(0)->type_->tid_ == Type::PointerTyID);

    instr_ir += static_cast<Pointer_Type *>(this->get_operand(0)->type_)->contained_->print();
    instr_ir += ",";
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), true);

    return instr_ir;
}

std::string Alloca_Instruction::print()
{
    std::string instr_ir;

    instr_ir += "%";
    instr_ir += this->name_;
    instr_ir += " = ";
    instr_ir += instr_id2string_[this->op_id_];
    instr_ir += " ";
    instr_ir += alloca_ty_->print();

    return instr_ir;
}

std::string Zext_Instruction::print()
{
    std::string instr_ir;

    instr_ir += "%";
    instr_ir += this->name_;

    instr_ir += " = ";
    instr_ir += instr_id2string_[this->op_id_];

    instr_ir += " ";
    instr_ir += this->get_operand(0)->type_->print();

    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), false);

    instr_ir += " to ";
    instr_ir += this->dest_ty_->print();

    return instr_ir;
}

std::string FpToSi_Instruction::print()
{
    std::string instr_ir;

    instr_ir += "%";
    instr_ir += this->name_;

    instr_ir += " = ";
    instr_ir += instr_id2string_[this->op_id_];

    instr_ir += " ";
    instr_ir += this->get_operand(0)->type_->print();

    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), false);

    instr_ir += " to ";
    instr_ir += this->dest_ty_->print();

    return instr_ir;
}

std::string SiToFp_Instruction::print()
{
    std::string instr_ir;

    instr_ir += "%";
    instr_ir += this->name_;

    instr_ir += " = ";
    instr_ir += instr_id2string_[this->op_id_];

    instr_ir += " ";
    instr_ir += this->get_operand(0)->type_->print();

    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), false);

    instr_ir += " to ";
    instr_ir += this->dest_ty_->print();

    return instr_ir;
}

std::string Bitcast::print()
{
    std::string instr_ir;
    instr_ir += "%";

    instr_ir += this->name_;
    instr_ir += " = ";

    instr_ir += instr_id2string_[this->op_id_];
    instr_ir += " ";

    instr_ir += this->get_operand(0)->type_->print();
    instr_ir += " ";

    instr_ir += print_as_op(this->get_operand(0), false);
    instr_ir += " to ";

    instr_ir += this->dest_ty_->print();

    return instr_ir;
}

std::string PhiInst::print()
{
    std::string instr_ir;

    instr_ir += "%";
    instr_ir += this->name_;

    instr_ir += " = ";
    instr_ir += instr_id2string_[this->op_id_];

    instr_ir += " ";
    instr_ir += this->get_operand(0)->type_->print();

    instr_ir += " ";

    for (int i = 0; i < this->num_ops_ / 2; i++)
    {
        if( i > 0 )
            instr_ir += ", ";

        instr_ir += "[ ";
        instr_ir += print_as_op(this->get_operand(2*i), false);

        instr_ir += ", ";
        instr_ir += print_as_op(this->get_operand(2*i+1), false);

        instr_ir += " ]";
    }

    if ( this->num_ops_ / 2 < this->parent_->pre_bbs_.size() )
    {
        for ( auto pre_bb : this->parent_->pre_bbs_ )
        {
            if (std::find(this->operands_.begin(), this->operands_.end(), static_cast<Value *>(pre_bb)) == this->operands_.end())
                instr_ir += ", [ undef, " +print_as_op(pre_bb, false)+" ]";// find a pre_bb is not in phi
            
        }
    }

    return instr_ir; 
}

std::string print_as_op( Value *v, bool print_ty )
{
    std::string op_ir;

    if( print_ty )
    {
        op_ir += v->type_->print();
        op_ir += " ";
    }

    if (dynamic_cast<Global_Variable *>(v))
        op_ir += "@" + v->name_;
    else if ( dynamic_cast<Function *>(v) )
        op_ir += "@" + v->name_;
    else if ( dynamic_cast<Constant *>(v))
        op_ir += v->print();
    else
        op_ir += "%" + v->name_;

    return op_ir;
}

std::string print_cmp_type( ICmp_Instruction::ICmp_Op op )
{
    switch (op)
    {
        case ICmp_Instruction::ICMP_SGE:
            return "sge";
            break;

        case ICmp_Instruction::ICMP_SGT:
            return "sgt";
            break;

        case ICmp_Instruction::ICMP_SLE:
            return "sle";
            break;

        case ICmp_Instruction::ICMP_SLT:
            return "slt";
            break;

        case ICmp_Instruction::ICMP_EQ:
            return "eq";
            break;

        case ICmp_Instruction::ICMP_NE:
            return "ne";
            break;

        default:
            break;

    }
    return "wrong cmp_op";
}

std::string print_fcmp_type( FCmp_Instruction::FCmp_Op op )
{
    switch (op)
    {
        case FCmp_Instruction::FCMP_UGE:
            return "uge";
            break;

        case FCmp_Instruction::FCMP_UGT:
            return "ugt";
            break;

        case FCmp_Instruction::FCMP_ULE:
            return "ule";
            break;

        case FCmp_Instruction::FCMP_ULT:
            return "ult";
            break;

        case FCmp_Instruction::FCMP_UEQ:
            return "ueq";
            break;

        case FCmp_Instruction::FCMP_UNE:
            return "une";
            break;

        default:
            break;

    }
    return "wrong FCmp_Op";
}

void Function::set_instr_name()
{
    std::map<Value*, int> seq;

    for (auto arg : this->arguments_)
    {
        if (!seq.count(arg))
        {
            auto seq_num = seq.size() + seq_cnt_;

            if (arg->name_ == "")
            {
                arg->name_ = "arg_" + std::to_string(seq_num);
                seq.insert({ arg, seq_num });
            }
        }
    }

    for (auto bb : basic_blocks_)
    {
        if (!seq.count(bb))
        {
            auto seq_num = seq.size() + seq_cnt_;

            if (bb->name_.length() <= 6 || bb->name_.substr(0, 6) != "label_")
            {
                bb->name_ = "label_" + std::to_string(seq_num);
                seq.insert({ bb, seq_num });
            }
        }
        for (auto instr : bb->instr_list_)
        {
            if (instr->type_->tid_ != Type::VoidTyID && !seq.count(instr))
            {
                auto seq_num = seq.size() + seq_cnt_;

                if (instr->name_ == "")
                {
                    instr->name_ = "v" + std::to_string(seq_num);
                    seq.insert({ instr, seq_num });
                }
            }
        }
    }
    seq_cnt_ += seq.size();
}

bool Instruction::isArithmetic() {
    switch (op_id_) {
        case Add:
        case Sub:
        case Mul:
        case SDiv:
        case SRem:
        case FAdd:
        case FSub:
        case FMul:
        case FDiv:
        case ICmp:
        case FCmp:
            return true;
        default:
            return false;
    }
}
