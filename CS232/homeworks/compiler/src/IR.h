#ifndef _IR_H_
#define _IR_H_

// 库引用
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
#include <set>

#include <cassert>
#include <list>
#include <sstream>
#include <string>
#include <utility>

//类声明
class Type;             //语义类
class Integer_Type;     //整数
class Array_Type;       //数组
class Pointer_Type;     //指针
class Function_Type;    //函数类型

class Value;            //属性值

class Constant;         //常量
class Constant_Int;     //常整数
class Constant_Float;   //常浮点数
class Constant_Array;   //常量数组
class Constant_Zero;    //常量零

class Module;           //全局表
class Global_Variable;  //全局变量

class Function;         
class Basic_Block;
class Argument;

class Instruction;
class Binary_Instruction;
class Unary_Instruction;
class ICmp_Instruction;
class FCmp_Instruction;
class Call_Instruction;
class Branch_Instruction;
class Return_Instruction;
class GetElementPtr_Instruction;
class Store_Instruction;
class Load_Instruction;
class Alloca_Instruction;

struct Use
{
    Value* val;
    unsigned int num;  // 操作数的序号，如func(a,b)中a的序号为0，b的序号为1
    Use(Value* v, unsigned int n) : val(v), num(n) {}
};

//-----------------------------------------------Type-----------------------------------------------
class Type
{
public:
    enum TypeID
    {
        VoidTyID,      // Void
        LabelTyID,     // Labels, e.g., Basic_Block
        IntegerTyID,   // Integers, include 32 bits and 1 bit
        FloatTyID,     // Floats, only 32 bits
        FunctionTyID,  // Functions
        ArrayTyID,     // Arrays
        PointerTyID,   // Pointer
    };
    explicit Type(TypeID tid) : tid_(tid) {}
    ~Type() = default;
    virtual std::string print();
    TypeID tid_;
};

class Integer_Type : public Type
{
public:
    explicit Integer_Type(unsigned num_bits) : Type(Type::IntegerTyID), num_bits_(num_bits) {}
    unsigned num_bits_;
};

//[2 x [3 x i32]]: num_elements_ = 2, contained_ = [3 x i32]
class Array_Type : public Type
{
public:
    Array_Type(Type* contained, unsigned num_elements) : Type(Type::ArrayTyID), num_elements_(num_elements), contained_(contained) {}
    Type* contained_;        // The element type of the array.
    unsigned num_elements_;  // Number of elements in the array.
};

//[2 x [3 x i32]]*
class Pointer_Type : public Type
{
public:
    Pointer_Type(Type* contained) : Type(Type::PointerTyID), contained_(contained) {}
    Type* contained_;  // The element type of the ptr.
};

// declare i32 @putarray(i32, i32*)
class Function_Type : public Type
{
public:
    Function_Type(Type* result, std::vector<Type*> params) : Type(Type::FunctionTyID)
    {
        result_ = result;
        for (Type* p : params)
            args_.push_back(p);
    }
    Type* result_;
    std::vector<Type*> args_;
};


//-----------------------------------------------Value-----------------------------------------------
class Value
{
public:
    explicit Value(Type* ty, const std::string& name = "") : type_(ty), name_(name) {}
    ~Value() = default;
    virtual std::string print() = 0;
    
    void remove_use(Value *val)
    {
        auto is_val = [val] (const Use &use) { return use.val == val; };
        use_list_.remove_if(is_val);
    }

    //******************************************************************
    std::list<Use>::iterator add_use(Value* val, unsigned arg_no)
    { 
        use_list_.emplace_back(Use(val, arg_no)); 
        std::list<Use>::iterator re = use_list_.end();
        return --re;
    }
    //删除迭代器指出的use
    void remove_use(std::list<Use>::iterator it)
    {
        use_list_.erase(it);
    }
    //user的第i个操作数准备不再使用this，因此删除this与user相关的use联系
    bool remove_used(Instruction* user,unsigned int i) ;


    //******************************************************************

    void replace_all_use_with(Value *new_val);
    Type* type_;
    std::string name_;
    std::list<Use> use_list_;  //所有引用该Value的Instruction的集合，以及该Value在该Instruction的第几个操作数位置被引用
};


//-----------------------------------------------Constant-----------------------------------------------
//常量都是无名的(name=="")
class Constant : public Value
{
public:
    Constant(Type* ty, const std::string& name = "") : Value(ty, name) {}
    ~Constant() = default;
};


// i32 -23
class Constant_Int : public Constant
{
public:
    Constant_Int(Type* ty, int val) : Constant(ty, ""), value_(val) {}
    virtual std::string print() override;
    int value_;
};

// float 0x4057C21FC0000000
// float -3.300000e+04
class Constant_Float : public Constant
{
public:
    Constant_Float(Type* ty, float val) : Constant(ty, ""), value_(val) {}
    virtual std::string print() override;
    float value_;
};

//[3 x i32] [i32 42, i32 11, i32 74]
class Constant_Array : public Constant
{
public:
    Constant_Array(Array_Type* ty, const std::vector<Constant*>& val) : Constant(ty, "") { this->const_array.assign(val.begin(), val.end()); }
    ~Constant_Array() = default;
    virtual std::string print() override;
    std::vector<Constant*> const_array;
};

// i32 zeroinitializer
//[2 x [100 x float]] zeroinitializer
//注意zeroinitializer是有类型的！
class Constant_Zero : public Constant
{
public:
    Constant_Zero(Type* ty) : Constant(ty, "") {}
    virtual std::string print() override;
};

//-----------------------------------------------Module-----------------------------------------------
class Module
{
public:
    explicit Module() {
        void_ty_ = new Type(Type::VoidTyID);
        label_ty_ = new Type(Type::LabelTyID);
        int1_ty_ = new Integer_Type(1);
        int32_ty_ = new Integer_Type(32);
        float32_ty_ = new Type(Type::FloatTyID);
    }
    ~Module() {
        delete void_ty_;
        delete label_ty_;
        delete int1_ty_;
        delete int32_ty_;
        delete float32_ty_;
    }
    virtual std::string print();
    void add_global_variable(Global_Variable* g) { global_list_.push_back(g); }
    void add_function(Function* f) { function_list_.push_back(f); }
    Pointer_Type* get_pointer_type(Type* contained)
    {
        if (!pointer_map_.count(contained)) {
            pointer_map_[contained] = new Pointer_Type(contained);
        }
        return pointer_map_[contained];
    }
    Array_Type* get_array_type(Type* contained, unsigned num_elements)
    {
        if (!array_map_.count({contained, num_elements}))
            array_map_[{contained, num_elements}] = new Array_Type(contained, num_elements);
        
        return array_map_[{contained, num_elements}];
    }

    Function *getMainFunc();

    std::vector<Global_Variable*> global_list_;
    std::vector<Function*> function_list_;

    Integer_Type* int1_ty_;
    Integer_Type* int32_ty_;
    Type* float32_ty_;
    Type* label_ty_;
    Type* void_ty_;
    std::map<Type*, Pointer_Type*> pointer_map_;
    std::map<std::pair<Type*, int>, Array_Type*> array_map_;
};


//-----------------------------------------------Global_Variable-----------------------------------------------
//@c = global [4 x i32] [i32 6, i32 7, i32 8, i32 9]
//@a = constant [5 x i32] [i32 0, i32 1, i32 2, i32 3, i32 4]
class Global_Variable : public Value
{
public:
    Global_Variable(std::string name, Module* m, Type* ty, bool is_const, Constant* init = nullptr) : Value(m->get_pointer_type(ty), name), is_const_(is_const), init_val_(init) { m->add_global_variable(this); }
    virtual std::string print() override;
    bool is_const_;
    Constant* init_val_;
};

//-----------------------------------------------Function-----------------------------------------------

//注：Argument的构造函数只由Function的构造函数调用，不单独调用！！
class Argument : public Value
{
public:
    explicit Argument(Type* ty, const std::string& name = "", Function* f = nullptr, unsigned arg_no = 0) : Value(ty, name), parent_(f), arg_no_(arg_no) {}
    ~Argument() {}
    virtual std::string print() override;
    Function* parent_;
    unsigned arg_no_;  // argument No.
};

class Function : public Value
{
public:
    Function(Function_Type* ty, const std::string& name, Module* parent) : Value(ty, name), parent_(parent), seq_cnt_(0)
    {
        parent->add_function(this);
        size_t num_args = ty->args_.size();
        use_ret_cnt = 0;

        for (size_t i = 0; i < num_args; i++)
            arguments_.push_back(new Argument(ty->args_[i], "", this, i));
        
    }
    ~Function();

    virtual std::string print() override;

    void add_basic_block(Basic_Block* bb) { basic_blocks_.push_back(bb); }

    Type* get_return_type() const { return static_cast<Function_Type*>(type_)->result_; }

    bool is_declaration() { return basic_blocks_.empty(); }

    void set_instr_name();

    void remove_bb(Basic_Block* bb);

    Basic_Block *getRetBB();
    std::vector<Basic_Block*> basic_blocks_;  // basic blocks
    std::vector<Argument*> arguments_;       // argument
    Module* parent_;
    unsigned seq_cnt_;
    std::vector<std::set<Value *>> vreg_set_;
    int use_ret_cnt;        //程序中真正使用返回值的次数
};
//-----------------------------------------------Basic_Block-----------------------------------------------
//注：Basic_Block一定是LabelTyID
class Basic_Block : public Value
{
public:
    explicit Basic_Block(Module* m, const std::string& name, Function* parent) : Value(m->label_ty_, name), parent_(parent) { parent_->add_basic_block(this); }
    
    bool add_instruction(Instruction* instr) ; //尾部插入指令，返回成功与否
    
    bool add_instruction_front(Instruction* instr) ;//头部插入指令，返回成功与否
    
    bool add_instruction_before_terminator(Instruction* instr) ;//插入到BB倒数第二条指令，即br前
    
    bool add_instruction_before_inst(Instruction* new_inst,Instruction* inst) ;//将新指令插入到原来指令前，返回成功与否，需要保证原指令在bb内
    
    void add_pre_basic_block(Basic_Block* bb) { pre_bbs_.push_back(bb); }
    
    void add_succ_basic_block(Basic_Block* bb) { succ_bbs_.push_back(bb); }
    
    void remove_pre_basic_block(Basic_Block *bb)
    { 
        //pre_bbs_.remove(bb); 
        pre_bbs_.erase(std::remove(pre_bbs_.begin(), pre_bbs_.end(), bb) , pre_bbs_.end());
    }

    void remove_succ_basic_block(Basic_Block *bb)
    { 
        //succ_bbs_.remove(bb); 
        succ_bbs_.erase(std::remove(succ_bbs_.begin(), succ_bbs_.end(), bb) , succ_bbs_.end());
    }

    int isDominate( Basic_Block* bb2)
    { //返回1表示支配bb2，返回0表示不支配，返回-1输入的块出错
        if (!bb2 || this->parent_ != bb2->parent_)
            return -1;
        while(bb2->name_ != "label_entry") {
            if(bb2->idom_ == this)
                return 1;
            bb2 = bb2->idom_;
        }
        return 0;
    }    
    // Returns the terminator instruction if the block is well formed or null
    // if the block is not well formed.

    Instruction* get_terminator();

    bool delete_instr(Instruction *instr); //返回false则说明指令不能重复删除或者不属于这个bb，

    bool remove_instr(Instruction *instr); //从bb移出一个指令，但是不删指令的use关系，因为还要插入其他bb

    virtual std::string print() override;


    //********************使用list替换vector---------qwc202208141511
    std::list<Instruction*> instr_list_;
    //********************使用list替换vector---------qwc202208141511


    Function* parent_;
    /****************api about cfg****************/
    std::vector<Basic_Block*> pre_bbs_;
    std::vector<Basic_Block*> succ_bbs_;
    /****************api about dominate tree****************/
    std::set<Basic_Block*> dom_frontier_;
    std::set<Basic_Block*> rdom_frontier_;
    std::set<Basic_Block*> rdoms_;
    Basic_Block* idom_;
    std::set<Value*> live_in;
    std::set<Value*> live_out;
};


//-----------------------------------------------Instruction-----------------------------------------------
class Instruction : public Value
{
public:
    enum OpID
    {
        // Terminator Instructions
        Ret = 11,
        Br,
        // Standard unary operators
        FNeg,
        // Standard binary operators
        Add,
        Sub,
        Mul,
        SDiv,
        SRem,
        UDiv,
        URem,
        // Float binary operators
        FAdd,
        FSub,
        FMul,
        FDiv,
        // Logical operators
        Shl,
        LShr,
        AShr,
        And,
        Or,
        Xor,
        // Memory operators
        Alloca,
        Load,
        Store,
        GetElementPtr,
        // Cast operators
        ZExt,
        FPtoSI,
        SItoFP,
        BitCast,
        // Other operators
        ICmp,
        FCmp,
        PHI,
        Call,
    };
    // 创建指令并插入基本块（ty是指令返回值类型）
    Instruction(Type* ty, OpID id, unsigned num_ops, Basic_Block* parent) : Value(ty, ""), op_id_(id), num_ops_(num_ops), parent_(parent)
    {
        operands_.resize(num_ops_, nullptr);  //此句不能删去！否则operands_为空时无法用set_operand设置操作数，而只能用push_back设置操作数！
        use_pos_.resize(num_ops_);
        parent_->add_instruction(this);
    }
    // 仅创建指令，不插入基本块（ty是指令返回值类型）
    Instruction(Type* ty, OpID id, unsigned num_ops) : Value(ty, ""), op_id_(id), num_ops_(num_ops), parent_(nullptr)
    {
        operands_.resize(num_ops_, nullptr);
        use_pos_.resize(num_ops_);
    }
    Value* get_operand(unsigned i) const { return operands_[i]; }


    //***************************202208121533
    void set_operand(unsigned i, Value* v)
    {
        operands_[i] = v;
        use_pos_[i] = v->add_use(this, i);
    }
    void add_operand(Value *v)
    {    //添加指令操作数，用于phi指令
        operands_.push_back(v);  
        use_pos_.emplace_back(v->add_use(this, num_ops_));
        num_ops_++;
    }
    void remove_use_of_ops()
    { //删除此指令所有操作数的uselist中，与此指令相关的use
        for (int i = 0; i < operands_.size(); i ++ )
            operands_[i]->remove_use(use_pos_[i]);

    }
    // 删除phi指令中的一对操作数
    void remove_operands(int index1, int index2)
    {
        for(int i = index1; i <= index2; i ++ )
            operands_[i]->remove_use(use_pos_[i]);
        
        //后面操作数的位置要做相应修改
        for (int i = index2 + 1; i < operands_.size(); i++)
        {
            for (auto &use : operands_[i]->use_list_)
            {
                if (use.val == this)
                {
                    use.num -= index2 - index1 + 1;
                    break;
                }
            }
        }        
        operands_.erase(operands_.begin() + index1, operands_.begin() + index2 + 1);
        use_pos_.erase(use_pos_.begin() + index1, use_pos_.begin() + index2 + 1);
        // std::cout<<operands_.size()<<std::endl;
        num_ops_ = operands_.size();
    }
    //***************************202208121533

    //↓↓↓↓↓↓↓↓--------增加快速类型判断---------qwc202208132346
    bool is_void() { return ((op_id_ == Ret) || (op_id_ == Br) || (op_id_ == Store) || (op_id_ == Call && this->type_->tid_==Type::VoidTyID)); }

    bool is_phi()  { return op_id_ == PHI; }
    bool is_store() { return op_id_ == Store; }
    bool is_alloca() { return op_id_ == Alloca; }
    bool is_ret() { return op_id_ == Ret; }
    bool is_load() { return op_id_ == Load; }
    bool is_br() { return op_id_ == Br; }

    bool is_add() { return op_id_ == Add; }
    bool is_sub() { return op_id_ == Sub; }
    bool is_mul() { return op_id_ == Mul; }
    bool is_div() { return op_id_ == SDiv; }
    bool is_rem() { return op_id_ == SRem; }

    bool is_fadd() { return op_id_ == FAdd; }
    bool is_fsub() { return op_id_ == FSub; }
    bool is_fmul() { return op_id_ == FMul; }
    bool is_fdiv() { return op_id_ == FDiv; }

    bool is_cmp() { return op_id_ == ICmp; }
    bool is_fcmp() { return op_id_ == FCmp; }

    bool is_call() { return op_id_ == Call; }
    bool is_gep() { return op_id_ == GetElementPtr; }
    bool is_zext() { return op_id_ == ZExt; }
    bool is_fptosi() { return op_id_ == FPtoSI; }
    bool is_sitofp() { return op_id_ == SItoFP; }

    bool is_int_binary()
    {
        return (is_add() || is_sub() || is_mul() || is_div() || is_rem()) && (num_ops_ == 2);
    }

    bool is_float_binary()
    {
        return (is_fadd() || is_fsub() || is_fmul() || is_fdiv()) && (num_ops_ == 2);
    }

    bool is_binary(){ return is_int_binary() || is_float_binary();}

    bool isTerminator() { return is_br() || is_ret(); }

    //↑↑↑↑↑↑↑↑--------增加快速类型判断---------qwc202208132346

    
    virtual std::string print() = 0;
    Basic_Block* parent_;
    OpID op_id_;
    unsigned num_ops_;
    std::vector<Value*> operands_;  // operands of this value
    std::vector<std::list<Use>::iterator> use_pos_; // 与操作数数组一一对应，是对应的操作数的uselist里面，与当前指令相关的use的迭代器
    std::vector<std::list<Instruction*>::iterator> pos_in_bb;// 在bb的指令list的位置迭代器,最多只能有一个
};


//%77 = add i32 %74, %76
//%10 = and i1 %7, %9
//%7 = xor i1 %6, true
//%13 = fmul float %12, 0x400921FB60000000
class Binary_Instruction : public Instruction
{
public:
    Binary_Instruction(Type* ty, OpID op, Value* v1, Value* v2, Basic_Block* bb) : Instruction(ty, op, 2, bb)
    {
        set_operand(0, v1);
        set_operand(1, v2);
    }
    // 只创建，不加入基本块末尾
    Binary_Instruction(Type* ty, OpID op, Value* v1, Value* v2, Basic_Block* bb, bool flag)
     : Instruction(ty, op, 2) {
        set_operand(0, v1);
        set_operand(1, v2);
        this->parent_ = bb;
    }
    virtual std::string print() override;
};


//%8 = zext i1 %7 to i32
//%51 = fptosi float %50 to i32
//%4 = sitofp i32 %3 to float
//%8 = fneg float %7
//%3 = bitcast [4 x [2 x i32]]* %2 to i32*
class Unary_Instruction : public Instruction
{
public:
    Unary_Instruction(Type* ty, OpID op, Value* val, Basic_Block* bb) : Instruction(ty, op, 1, bb) { set_operand(0, val); }
    virtual std::string print() override;
};


//%18 = icmp ne i32 %12, %17
class ICmp_Instruction : public Instruction
{
public:
    enum ICmp_Op
    {
        ICMP_EQ = 32,   ///< equal
        ICMP_NE = 33,   ///< not equal
        ICMP_UGT = 34,  ///< unsigned greater than
        ICMP_UGE = 35,  ///< unsigned greater or equal
        ICMP_ULT = 36,  ///< unsigned less than
        ICMP_ULE = 37,  ///< unsigned less or equal
        ICMP_SGT = 38,  ///< signed greater than
        ICMP_SGE = 39,  ///< signed greater or equal
        ICMP_SLT = 40,  ///< signed less than
        ICMP_SLE = 41   ///< signed less or equal
    };
    static const std::map<ICmp_Instruction::ICmp_Op, std::string> ICmp_Op_Name;
    ICmp_Instruction(ICmp_Op op, Value* v1, Value* v2, Basic_Block* bb) : Instruction(bb->parent_->parent_->int1_ty_, Instruction::ICmp, 2, bb), icmp_op_(op) {
        set_operand(0, v1);
        set_operand(1, v2);
    }
    virtual std::string print() override;
    ICmp_Op icmp_op_;
};


//%5 = fcmp olt float %4, 0.000000e+00
class FCmp_Instruction : public Instruction
{
public:
    enum FCmp_Op
    {
        FCMP_FALSE = 10,  // Always false (always folded)
        FCMP_OEQ = 11,    // True if ordered and equal
        FCMP_OGT = 12,    // True if ordered and greater than
        FCMP_OGE = 13,    // True if ordered and greater than or equal
        FCMP_OLT = 14,    // True if ordered and less than
        FCMP_OLE = 15,    // True if ordered and less than or equal
        FCMP_ONE = 16,    // True if ordered and operands are unequal
        FCMP_ORD = 17,    // True if ordered (no nans)
        FCMP_UNO = 18,    // True if unordered: isnan(X) | isnan(Y)
        FCMP_UEQ = 19,    // True if unordered or equal
        FCMP_UGT = 20,    // True if unordered or greater than
        FCMP_UGE = 21,    // True if unordered, greater than, or equal
        FCMP_ULT = 22,    // True if unordered or less than
        FCMP_ULE = 23,    // True if unordered, less than, or equal
        FCMP_UNE = 24,    // True if unordered or not equal
        FCMP_TRUE = 25    // Always true (always folded)
    };
    static const std::map<FCmp_Instruction::FCmp_Op, std::string> FCmp_Op_Name;
    FCmp_Instruction(FCmp_Op op, Value* v1, Value* v2, Basic_Block* bb) : Instruction(bb->parent_->parent_->int1_ty_, Instruction::FCmp, 2, bb), fcmp_op_(op) {
        set_operand(0, v1);
        set_operand(1, v2);
    }
    virtual std::string print() override;
    FCmp_Op fcmp_op_;
};


//%111 = call i32 @QuickSort(i32* %108, i32 %109, i32 %110)
class Call_Instruction : public Instruction
{
public:
    Call_Instruction(Function* func, std::vector<Value*> args, Basic_Block* bb) : Instruction(static_cast<Function_Type*>(func->type_)->result_, Instruction::Call, args.size() + 1, bb) {
        int num_ops = args.size() + 1;
        for (int i = 0; i < num_ops - 1; i++) {
            set_operand(i, args[i]);
        }
        set_operand(num_ops - 1, func);
    }
    virtual std::string print() override;
};


//注：br的返回值类型一定是VoidTyID
class Branch_Instruction : public Instruction
{
public:
    // br i1 %7, label %8, label %9
    Branch_Instruction(Value* cond, Basic_Block* if_true, Basic_Block* if_false, Basic_Block* bb) : Instruction(if_true->parent_->parent_->void_ty_, Instruction::Br, 3, bb)
    {
        if_true->add_pre_basic_block(bb);
        if_false->add_pre_basic_block(bb);
        bb->add_succ_basic_block(if_false);
        bb->add_succ_basic_block(if_true);
        set_operand(0, cond);
        set_operand(1, if_true);
        set_operand(2, if_false);
    }

    // br label %31
    Branch_Instruction(Basic_Block* if_true, Basic_Block* bb) : Instruction(if_true->parent_->parent_->void_ty_, Instruction::Br, 1, bb)
    {
        if_true->add_pre_basic_block(bb);
        bb->add_succ_basic_block(if_true);
        set_operand(0, if_true);
    }
    virtual std::string print() override;
};

// ret i32 %4
// ret void
//注：ret的返回值类型一定是VoidTyID
class Return_Instruction : public Instruction
{
public:
    Return_Instruction(Value* val, Basic_Block* bb) : Instruction(bb->parent_->parent_->void_ty_, Instruction::Ret, 1, bb) { set_operand(0, val); }
    
    Return_Instruction(Value* val, Basic_Block* bb, bool flag) : Instruction(bb->parent_->parent_->void_ty_, Instruction::Ret, 1)
    { 
        set_operand(0, val);
        this->parent_ = bb;
    }
    Return_Instruction(Basic_Block* bb) : Instruction(bb->parent_->parent_->void_ty_, Instruction::Ret, 0, bb) {}
    virtual std::string print() override;
};


//%1 = getelementptr [5 x [4 x i32]], [5 x [4 x i32]]* @a, i32 0, i32 2, i32 3
class GetElementPtr_Instruction : public Instruction
{
public:
    GetElementPtr_Instruction(Value* ptr, std::vector<Value*> idxs, Basic_Block* bb) :
        Instruction(bb->parent_->parent_->get_pointer_type(get_GEP_return_type(ptr, idxs.size())),
        Instruction::GetElementPtr, idxs.size() + 1, bb)
    {
        set_operand(0, ptr);
        for (size_t i = 0; i < idxs.size(); i++)
            set_operand(i + 1, idxs[i]);
    }
    Type* get_GEP_return_type(Value* ptr, size_t idxs_size)
    {
        Type* ty = static_cast<Pointer_Type*>(ptr->type_)->contained_;  //[5 x [4 x i32]]
        if (ty->tid_ == Type::ArrayTyID)
        {
            Array_Type* arr_ty = static_cast<Array_Type*>(ty);
            for (size_t i = 1; i < idxs_size; i++)
            {
                ty = arr_ty->contained_;  //[4 x i32], i32
                if (ty->tid_ == Type::ArrayTyID)
                    arr_ty = static_cast<Array_Type*>(ty);
                
            }
        }
        return ty;
    }
    virtual std::string print() override;
};

// store <ty> <value>, <ty>* <pointer>
// store i32 %57, i32* %55
//注：store的返回值类型一定是VoidTyID
class Store_Instruction : public Instruction
{
public:
    Store_Instruction(Value* val, Value* ptr, Basic_Block* bb) : Instruction(bb->parent_->parent_->void_ty_, Instruction::Store, 2, bb) {
        assert(val->type_ == static_cast<Pointer_Type*>(ptr->type_)->contained_);
        set_operand(0, val);
        set_operand(1, ptr);
    }

    //创建store指令，不插入到基本块中，但是设定parent
    Store_Instruction(Value* val, Value* ptr, Basic_Block* bb, bool) : Instruction(bb->parent_->parent_->void_ty_, Instruction::Store, 2)
    {
        assert(val->type_ == static_cast<Pointer_Type*>(ptr->type_)->contained_);
        set_operand(0, val);
        set_operand(1, ptr);
        this->parent_ = bb;
    }

    virtual std::string print() override;
};

//<result> = load <ty>, <ty>* <pointer>
//%60 = load i32, i32* %59
class Load_Instruction : public Instruction
{
public:
    Load_Instruction(Value* ptr, Basic_Block* bb) : Instruction(static_cast<Pointer_Type*>(ptr->type_)->contained_, Instruction::Load, 1, bb)
    { set_operand(0, ptr); }

    virtual std::string print() override;
};


//%8 = alloca i32
class Alloca_Instruction : public Instruction
{
public:
    Alloca_Instruction(Type* ty, Basic_Block* bb) : Instruction(bb->parent_->parent_->get_pointer_type(ty), Instruction::Alloca, 0, bb), alloca_ty_(ty) {}

    //创建指令，不插入到最后，但是会设定parent
    Alloca_Instruction(Type* ty, Basic_Block* bb, bool) : Instruction(bb->parent_->parent_->get_pointer_type(ty), Instruction::Alloca, 0), alloca_ty_(ty) {this->parent_ = bb;}

    virtual std::string print() override;
    Type* alloca_ty_;
};


class Zext_Instruction : public Instruction
{
public:
    Zext_Instruction(OpID op, Value* val, Type* ty, Basic_Block* bb) : Instruction(ty, op, 1, bb), dest_ty_(ty) { set_operand(0, val); }
    virtual std::string print() override;
    Type* dest_ty_;
};

class FpToSi_Instruction : public Instruction
{
public:
    FpToSi_Instruction(OpID op, Value* val, Type* ty, Basic_Block* bb) : Instruction(ty, op, 1, bb), dest_ty_(ty) { set_operand(0, val); }
    virtual std::string print() override;
    Type* dest_ty_;
};

class SiToFp_Instruction : public Instruction
{
public:
    SiToFp_Instruction(OpID op, Value* val, Type* ty, Basic_Block* bb) : Instruction(ty, op, 1, bb), dest_ty_(ty) { set_operand(0, val); }
    virtual std::string print() override;
    Type* dest_ty_;
};

//%3 = bitcast [4 x [2 x i32]]* %2 to i32*
class Bitcast : public Instruction
{
public:
    Bitcast(OpID op, Value *val, Type *ty, Basic_Block *bb) : Instruction(ty, op, 1, bb), dest_ty_(ty) {set_operand(0, val);}
    virtual std::string print() override;
    Type *dest_ty_;
};


//%4 = phi i32 [ 1, %2 ], [ %6, %5 ]
class PhiInst : public Instruction
{
public:
    PhiInst(OpID op, std::vector<Value *> vals, std::vector<Basic_Block *> val_bbs, Type *ty, Basic_Block *bb)
    : Instruction(ty, op, 2*vals.size())
    {
        for ( int i = 0; i < vals.size(); i++)
        {
            set_operand(2*i, vals[i]);
            set_operand(2*i+1, val_bbs[i]);
        }
        this->parent_ = bb;
    }
    static PhiInst *create_phi(Type *ty, Basic_Block *bb)
    {
        std::vector<Value *> vals;
        std::vector<Basic_Block *> val_bbs;
        return new PhiInst(Instruction::PHI, vals, val_bbs, ty, bb);
    }
    void add_phi_pair_operand(Value *val, Value *pre_bb)
    {
        this->add_operand(val);
        this->add_operand(pre_bb);
    }
    virtual std::string print() override;

    Value *l_val_;

};

//-----------------------------------------------IRStmtBuilder-----------------------------------------------
class IRStmtBuilder
{
public:
    Basic_Block* BB_;
    Module* m_;

    IRStmtBuilder(Basic_Block* bb, Module* m) : BB_(bb), m_(m){};
    ~IRStmtBuilder() = default;
    Module* get_module() { return m_; }
    Basic_Block* get_insert_block() { return this->BB_; }
    void set_insert_point(Basic_Block* bb) { this->BB_ = bb; }                                                                           //在某个基本块中插入指令
    Binary_Instruction* create_iadd(Value* v1, Value* v2) { return new Binary_Instruction(this->m_->int32_ty_, Instruction::Add, v1, v2, this->BB_); }  //创建加法指令（以及其他算术指令）
    Binary_Instruction* create_isub(Value* v1, Value* v2) { return new Binary_Instruction(this->m_->int32_ty_, Instruction::Sub, v1, v2, this->BB_); }
    Binary_Instruction* create_imul(Value* v1, Value* v2) { return new Binary_Instruction(this->m_->int32_ty_, Instruction::Mul, v1, v2, this->BB_); }
    Binary_Instruction* create_isdiv(Value* v1, Value* v2) { return new Binary_Instruction(this->m_->int32_ty_, Instruction::SDiv, v1, v2, this->BB_); }
    Binary_Instruction* create_isrem(Value* v1, Value* v2) { return new Binary_Instruction(this->m_->int32_ty_, Instruction::SRem, v1, v2, this->BB_); }

    ICmp_Instruction* create_icmp_eq(Value* v1, Value* v2) { return new ICmp_Instruction(ICmp_Instruction::ICMP_EQ, v1, v2, this->BB_); }
    ICmp_Instruction* create_icmp_ne(Value* v1, Value* v2) { return new ICmp_Instruction(ICmp_Instruction::ICMP_NE, v1, v2, this->BB_); }
    ICmp_Instruction* create_icmp_gt(Value* v1, Value* v2) { return new ICmp_Instruction(ICmp_Instruction::ICMP_SGT, v1, v2, this->BB_); }
    ICmp_Instruction* create_icmp_ge(Value* v1, Value* v2) { return new ICmp_Instruction(ICmp_Instruction::ICMP_SGE, v1, v2, this->BB_); }
    ICmp_Instruction* create_icmp_lt(Value* v1, Value* v2) { return new ICmp_Instruction(ICmp_Instruction::ICMP_SLT, v1, v2, this->BB_); }
    ICmp_Instruction* create_icmp_le(Value* v1, Value* v2) { return new ICmp_Instruction(ICmp_Instruction::ICMP_SLE, v1, v2, this->BB_); }

    Binary_Instruction* create_fadd(Value* v1, Value* v2) { return new Binary_Instruction(this->m_->float32_ty_, Instruction::FAdd, v1, v2, this->BB_); }
    Binary_Instruction* create_fsub(Value* v1, Value* v2) { return new Binary_Instruction(this->m_->float32_ty_, Instruction::FSub, v1, v2, this->BB_); }
    Binary_Instruction* create_fmul(Value* v1, Value* v2) { return new Binary_Instruction(this->m_->float32_ty_, Instruction::FMul, v1, v2, this->BB_); }
    Binary_Instruction* create_fdiv(Value* v1, Value* v2) { return new Binary_Instruction(this->m_->float32_ty_, Instruction::FDiv, v1, v2, this->BB_); }

    FCmp_Instruction* create_fcmp_eq(Value* v1, Value* v2) { return new FCmp_Instruction(FCmp_Instruction::FCMP_UEQ, v1, v2, this->BB_); }
    FCmp_Instruction* create_fcmp_ne(Value* v1, Value* v2) { return new FCmp_Instruction(FCmp_Instruction::FCMP_UNE, v1, v2, this->BB_); }
    FCmp_Instruction* create_fcmp_gt(Value* v1, Value* v2) { return new FCmp_Instruction(FCmp_Instruction::FCMP_UGT, v1, v2, this->BB_); }
    FCmp_Instruction* create_fcmp_ge(Value* v1, Value* v2) { return new FCmp_Instruction(FCmp_Instruction::FCMP_UGE, v1, v2, this->BB_); }
    FCmp_Instruction* create_fcmp_lt(Value* v1, Value* v2) { return new FCmp_Instruction(FCmp_Instruction::FCMP_ULT, v1, v2, this->BB_); }
    FCmp_Instruction* create_fcmp_le(Value* v1, Value* v2) { return new FCmp_Instruction(FCmp_Instruction::FCMP_ULE, v1, v2, this->BB_); }

    Call_Instruction* create_call(Value* func, std::vector<Value*> args)
    {
#ifdef DEBUG
        assert(dynamic_cast<Function*>(func) && "func must be Function * type");
#endif
        return new Call_Instruction(static_cast<Function*>(func), args, this->BB_);
    }

    Branch_Instruction* create_br(Basic_Block* if_true)
    {
        return new Branch_Instruction(if_true, this->BB_);
    }

    Branch_Instruction* create_cond_br(Value* cond, Basic_Block* if_true, Basic_Block* if_false)
    {
        return new Branch_Instruction(cond, if_true, if_false, this->BB_);
    }

    Return_Instruction* create_ret(Value* val)
    {
        return new Return_Instruction(val, this->BB_);
    }

    Return_Instruction* create_void_ret()
    {
        return new Return_Instruction(this->BB_);
    }

    GetElementPtr_Instruction* create_gep(Value* ptr, std::vector<Value*> idxs)
    {
        return new GetElementPtr_Instruction(ptr, idxs, this->BB_);
    }

    Store_Instruction* create_store(Value* val, Value* ptr)
    {
        return new Store_Instruction(val, ptr, this->BB_);
    }

    Load_Instruction* create_load(Type* ty, Value* ptr)
    {
        return new Load_Instruction(ptr, this->BB_);
    }

    Load_Instruction* create_load(Value* ptr)
    {
#ifdef DEBUG
        assert(ptr->get_type()->is_pointer_type() && "ptr must be pointer type");
#endif
        // return Load_Instruction::create_load(ptr->get_type()->get_pointer_element_type(), ptr, this->BB_);
        return new Load_Instruction(ptr, this->BB_);
    }

    Alloca_Instruction* create_alloca(Type* ty)
    {
        return new Alloca_Instruction(ty, this->BB_);
    }

    Zext_Instruction* create_zext(Value* val, Type* ty)
    {
        return new Zext_Instruction(Instruction::ZExt, val, ty, this->BB_);
    }

    FpToSi_Instruction* create_fptosi(Value* val, Type* ty)
    {
        return new FpToSi_Instruction(Instruction::FPtoSI, val, ty, this->BB_);
    }

    SiToFp_Instruction* create_sitofp(Value* val, Type* ty)
    {
        return new SiToFp_Instruction(Instruction::SItoFP, val, ty, this->BB_);
    }

    Bitcast* create_bitcast(Value *val, Type *ty)
    {
        return new Bitcast(Instruction::BitCast, val, ty, this->BB_);
    }

};

#endif  //_IR_H_