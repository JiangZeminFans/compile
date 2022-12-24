#ifndef TEST_IR_H
#define TEST_IR_H
#pragma once
#include "IR.h"
#include "all_ast.h"
#include <map>

// 作用域
class Scope 
{
private:
    std::vector<std::map<std::string, Value*>> symbol;

    
public:
    //创建新作用域
    void enter() 
    {
        symbol.push_back({});
    }

    // 终结对象
    void exit() 
    {
        symbol.pop_back();
    }

    // 判断全局对象
    bool in_global() 
    {
        return symbol.size() == 1;
    }

    // 判断能否添加新对象 可以添加返回1 不可以添加返回0（已经存在） 允许函数与变量重名
    bool push(std::string name, Value* val) 
    {
        bool result;
        result = (symbol[symbol.size() - 1].insert({ name, val })).second;
        return result;
    }

    // 寻找对象
    Value* find(std::string name) {
        for (auto s = symbol.rbegin(); s != symbol.rend(); s++) {
            auto iter = s->find(name);
            if (iter != s->end()) {
                return iter->second;
            }
        }
        return nullptr;
    }

};

class GenIR : public Visitor {
public:
    // 重写visit方法 每一个对应一个AST
    void visit(CompUnitAST& ast);
    void visit(DecDefAST& ast);
    void visit(DecAST& ast);
    void visit(IdDefAST& ast);
    void visit(InitValAST& ast);
    void visit(FuncDefAST& ast);
    void visit(FuncParamAST& ast);
    void visit(BlockAST& ast);
    void visit(SentenceAST& ast);
    void visit(StmtAST& ast);
    void visit(ReturnStmtAST& ast);
    void visit(SelectStmtAST& ast);
    void visit(IterationStmtAST& ast);
    void visit(LeftValAST& ast);
    void visit(PrimaryExpAST& ast);
    void visit(RelExpAST& ast);
    void visit(EqExpAST& ast);
    void visit(CallAST& ast);
    void visit(HighAST& ast);
    void visit(LowAST& ast);
    void visit(NumberAST& ast);
    void visit(UnaryAST& ast);
    void visit(AndAST& ast);
    void visit(OrAST& ast);


    IRStmtBuilder* builder;
    Scope scope;
    std::unique_ptr<Module> module;

    GenIR() 
    {
        module = std::unique_ptr<Module>(new Module());
        builder = new IRStmtBuilder(nullptr, module.get());
        auto Ty_Void = module->void_ty_;
        auto Ty_Int32 = module->int32_ty_;
        auto Ty_Int32Ptr = module->get_pointer_type(module->int32_ty_);
        auto Ty_Float = module->float32_ty_;
        auto Ty_FloatPtr = module->get_pointer_type(module->float32_ty_);
        auto input_type = new Function_Type(Ty_Int32, {});
        auto get_int = new Function(input_type, "getint", module.get());
        input_type = new Function_Type(Ty_Float, {});
        auto get_float = new Function(input_type, "getfloat", module.get());
        input_type = new Function_Type(Ty_Int32, {});
        auto get_char = new Function(input_type, "getch", module.get());
        std::vector<Type*> input_params;
        std::vector<Type*>().swap(input_params);
        input_params.push_back(Ty_Int32Ptr);
        input_type = new Function_Type(Ty_Int32, input_params);
        auto get_int_array = new Function(input_type, "getarray", module.get());

        std::vector<Type*>().swap(input_params);
        input_params.push_back(Ty_FloatPtr);
        input_type = new Function_Type(Ty_Int32, input_params);
        auto get_float_array = new Function(input_type, "getfarray", module.get());

        std::vector<Type*> output_params;
        std::vector<Type*>().swap(output_params);
        output_params.push_back(Ty_Int32);
        auto output_type = new Function_Type(Ty_Void, output_params);
        auto put_int = new Function(output_type, "putint", module.get());

        std::vector<Type*>().swap(output_params);
        output_params.push_back(Ty_Float);
        output_type = new Function_Type(Ty_Void, output_params);
        auto put_float = new Function(output_type, "putfloat", module.get());

        std::vector<Type*>().swap(output_params);
        output_params.push_back(Ty_Int32);
        output_type = new Function_Type(Ty_Void, output_params);
        auto put_char = new Function(output_type, "putch", module.get());

        std::vector<Type*>().swap(output_params);
        output_params.push_back(Ty_Int32);
        output_params.push_back(Ty_Int32Ptr);
        output_type = new Function_Type(Ty_Void, output_params);
        auto put_int_array = new Function(output_type, "putarray", module.get());

        std::vector<Type*>().swap(output_params);
        output_params.push_back(Ty_Int32);
        output_params.push_back(Ty_FloatPtr);
        output_type = new Function_Type(Ty_Void, output_params);
        auto put_float_array = new Function(output_type, "putfarray", module.get());

        output_params.clear();
        output_params.push_back(Ty_Int32);
        auto time_type = new Function_Type(Ty_Void, output_params);
        auto sysy_start_time = new Function(time_type, "_sysy_starttime", module.get());
        auto sysy_stop_time = new Function(time_type, "_sysy_stoptime", module.get());

        output_params.clear();
        output_params.push_back(Ty_Int32Ptr);
        output_params.push_back(Ty_Int32Ptr);
        output_params.push_back(Ty_Int32);
        output_type = new Function_Type(Ty_Void, output_params);
        auto memcpy = new Function(output_type, "__aeabi_memcpy4", module.get());

        output_params.clear();
        output_params.push_back(Ty_Int32Ptr);
        output_params.push_back(Ty_Int32);
        output_type = new Function_Type(Ty_Void, output_params);
        auto memclr = new Function(output_type, "__aeabi_memclr4", module.get());

        output_params.push_back(Ty_Int32);
        output_type = new Function_Type(Ty_Void, output_params);
        auto memset = new Function(output_type, "__aeabi_memset4", module.get());

        output_params.clear();
        output_type = new Function_Type(Ty_Void, output_params);
        auto llvm_memset = new Function(output_type, "llvm.memset.p0.i32", module.get());

        scope.enter();
        scope.push("getint", get_int);
        scope.push("getfloat", get_float);
        scope.push("getch", get_char);
        scope.push("getarray", get_int_array);
        scope.push("getfarray", get_float_array);
        scope.push("putint", put_int);
        scope.push("putfloat", put_float);
        scope.push("putch", put_char);
        scope.push("putarray", put_int_array);
        scope.push("putfarray", put_float_array);
        scope.push("_sysy_starttime", sysy_start_time);
        scope.push("_sysy_stoptime", sysy_stop_time);
        scope.push("memcpy", memcpy);
        scope.push("memclr", memclr);
        scope.push("memset", memset);
        scope.push("llvm.memset.p0.i32", llvm_memset);
    }
    std::unique_ptr<Module> getMod() 
    {
        return std::move(module);
    }

    void checkInitType() const;
    static int getNextD(vector<int>& dimensionsCnt, int up, int cnt);
    void localInit(Value* ptr, vector<unique_ptr<InitValAST>>& list, vector<int>& dimensionsCnt, int up);
    static int getNextD(vector<int>& elementsCnts, int up);
    Constant_Array* globalInit(vector<int>& dimensions, vector<Array_Type*>& arrayTys, int up, vector<unique_ptr<InitValAST>>& list);
    static void mergeElements(vector<int>& dimensions, vector<Array_Type*>& arrayTys, int up, int dimAdd, vector<Constant*>& elements, vector<int>& elementsCnts);
    void mergeFinal(vector<int>& dimensions, vector<Array_Type*>& arrayTys, int up, vector<Constant*>& elements, vector<int>& elementsCnts) const;
    bool checkCType(Value** val, int* intVal, float* floatVal);
    void checkCType(Value** val);
};
#endif //TEST_IR_H
