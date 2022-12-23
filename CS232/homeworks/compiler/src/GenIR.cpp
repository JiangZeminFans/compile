#include "GenIR.h"
#define CONST_INT(num) new Constant_Int(module->int32_ty_, num)
#define CONST_FLOAT(num) new Constant_Float(module->float32_ty_, num)
#define VOID_T (module->void_ty_)
#define INT1_T (module->int1_ty_)
#define INT32_T  (module->int32_ty_)
#define FLOAT_Ts  (module->float32_ty_)
#define INT32PTR_T (module->get_pointer_type(module->int32_ty_))
#define FLOATPTR_T (module->get_pointer_type(module->float32_ty_))

// store temporary value
Type* cur_Type;                         //��ǰdecl����
bool is_Const;                          //��ǰdecl�Ƿ���const
bool use_Const = false;                 //�����Ƿ�ʹ�ó���
std::vector<Type*> params;              //�����β����ͱ�
std::vector<std::string> paramNames;    //�����β�����
Value* retAlloca = nullptr;             //����ֵ
BasicBlock* retBB = nullptr;            //��������
bool isNewFunc = false;                 //�ж��Ƿ�Ϊ�º�������������������������
bool reqLV = false;                     //����LVal�ڵ㲻��Ҫ����loadָ��
Function* curFun = nullptr;             //��ǰ����
Value* recVal = nullptr;                //����ı��ʽ��value
BasicBlock* whileCondBB = nullptr;      //while���cond��֧
BasicBlock* trueBB = nullptr;           //ͨ��true��֧����while��ifΪ��ʱ����ת�Ļ�����
BasicBlock* falseBB = nullptr;          //ͨ��false��֧����while��ifΪ��ʱ����ת�Ļ�����
BasicBlock* whileFalseBB;               //while���false��֧������break��ת
int id = 1;                             //recent���
bool has_br = false;                    //һ��BB���Ƿ��Ѿ�������br
bool is_single_exp = false;             //��Ϊ������exp�����֣����� "exp;"


//�жϸ�ֵ�����������Ƿ�һ�£�����ת��
void GenIR::checkInitType() const 
{
    if (cur_Type == INT32_T) 
    {
        if (dynamic_cast<Constant_Float*>(recVal)) 
        {
            auto temp = dynamic_cast<Constant_Float*>(recVal);
            recVal = CONST_INT((int)temp->value_);
        }
        else if (recVal->type_->tid_ == Type::FloatTyID) 
        {
            recVal = builder->create_fptosi(recVal, INT32_T);
        }
    }
    else if (cur_Type == FLOAT_Ts) 
    {
        if (dynamic_cast<Constant_Int*>(recVal)) 
        {
            auto temp = dynamic_cast<Constant_Int*>(recVal);
            recVal = CONST_FLOAT(temp->value_);
        }
        else if (recVal->type_->tid_ == Type::IntegerTyID) 
        {
            recVal = builder->create_sitofp(recVal, FLOAT_Ts);
        }
    }
}

void GenIR::visit(CompUnitAST& ast) 
{
    for (const auto& def : ast.decDefList) 
    {
        def->accept(*this);
    }
}


void GenIR::visit(DecDefAST& ast) {
    if (ast.Decl != nullptr) {
        ast.Decl->accept(*this);
    }
    else 
    {
        ast.funcDef->accept(*this);
    }
}

void GenIR::visit(DecAST& ast) 
{
    is_Const = ast.isConst;
    cur_Type = ast.bType == INT_T ? INT32_T : FLOAT_Ts;
    for (auto& def : ast.defList) {
        def->accept(*this);
    }
}

void GenIR::visit(IdDefAST& ast) {
    string varName = *ast.id;
    //ȫ�ֱ�������
    if (scope.in_global()) {
        if (ast.dimensions.empty()) 
        {   
            //�������飬��ȫ����
            if (ast.initVal == nullptr) 
            { 
                //�޳�ʼ��
                if (is_Const) cout << "no initVal when define const!" << endl; //�޳�ʼ��ȫ�ֳ�������
                //�޳�ʼ��ȫ����һ���Ǳ���
                Global_Variable* var;
                if (cur_Type == INT32_T)
                    var = new Global_Variable(varName, module.get(), cur_Type, false, CONST_INT(0));
                else var = new Global_Variable(varName, module.get(), cur_Type, false, CONST_FLOAT(0));
                scope.push(varName, var);
            }
            else 
            { 
                //�г�ʼ��
                use_Const = true;
                ast.initVal->accept(*this);
                use_Const = false;
                checkInitType();
                if (is_Const) 
                {
                    scope.push(varName, recVal);   //�����������岻��new GlobalVariable
                }
                else 
                { 
                    //ȫ�ֱ���
                    auto initializer = static_cast<Constant*>(recVal);
                    Global_Variable* var;
                    var = new Global_Variable(varName, module.get(), cur_Type, false, initializer);
                    scope.push(varName, var);
                }
            }
        }
        else {        
            //�����飬��ȫ��������
            vector<int> dimensions;  
            //�����ά��; [2][3][4]��Ӧ
            use_Const = true;
            //��ȡ�����ά��
            for (auto& exp : ast.dimensions) 
            {
                exp->accept(*this);
                int dimension = dynamic_cast<Constant_Int*>(recVal)->value_;
                dimensions.push_back(dimension);
            }
            use_Const = false;
            vector<Array_Type*> arrayTys(dimensions.size()); //��������, {[2 x [3 x [4 x i32]]], [3 x [4 x i32]], [4 x i32]}
            for (int i = dimensions.size() - 1; i >= 0; i--) 
            {
                if (i == dimensions.size() - 1) arrayTys[i] = module->get_array_type(cur_Type, dimensions[i]);
                else arrayTys[i] = module->get_array_type(arrayTys[i + 1], dimensions[i]);
            }
            //�޳�ʼ�����߳�ʼ����Ϊ������
            if (ast.initVal == nullptr || ast.initVal->initValList.empty()) 
            {
                auto init = new Constant_Zero(arrayTys[0]);
                auto var = new Global_Variable(varName, module.get(), arrayTys[0], is_Const, init);
                scope.push(varName, var);
            }
            else 
            {
                use_Const = true; //ȫ���������ĳ�ʼֵ��Ϊ����
                auto init = globalInit(dimensions, arrayTys, 0, ast.initVal->initValList);
                use_Const = false;
                auto var = new Global_Variable(varName, module.get(), arrayTys[0], is_Const, init);
                scope.push(varName, var);
            }
        }
        return;
    }


    //�ֲ���������
    if (ast.dimensions.empty()) 
    {   
        //�������飬����ͨ�ֲ���
        if (ast.initVal == nullptr) {   
            //�޳�ʼ��
            if (is_Const) cout << "no initVal when define const!" << endl;   //�޳�ʼ���ֲ���������
            else 
            { 
                //�޳�ʼ������
                Alloca_Instruction* varAlloca;
                varAlloca = builder->create_alloca(cur_Type);
                scope.push(varName, varAlloca);
            }
        }
        else 
        { 
            //�г�ʼ��
            ast.initVal->accept(*this);
            checkInitType();
            if (is_Const) 
            {
                scope.push(varName, recVal);  //�����������岻��create_alloca
            }
            else 
            {
                Alloca_Instruction* varAlloca;
                varAlloca = builder->create_alloca(cur_Type);
                scope.push(varName, varAlloca);
                builder->create_store(recVal, varAlloca);
            }
        }
    }
    else 
    {    
        //�ֲ�������
        vector<int> dimensions(ast.dimensions.size()), dimensionsCnt((ast.dimensions.size()));  //�����ά��, [2][3][4]��Ӧ; ��ά������Ԫ�ظ���, [24][12][4]
        int totalByte = 1; //�洢�ܹ����ֽ���
        use_Const = true;
        //��ȡ�����ά��
        for (int i = dimensions.size() - 1; i >= 0; i--) 
        {
            ast.dimensions[i]->accept(*this);
            int dimension = dynamic_cast<Constant_Int*>(recVal)->value_;
            totalByte *= dimension;
            dimensions[i] = dimension;
            dimensionsCnt[i] = totalByte;
        }
        totalByte *= 4; //�����ֽ���
        use_Const = false;
        Array_Type* arrayTy; //��������
        for (int i = dimensions.size() - 1; i >= 0; i--) 
        {
            if (i == dimensions.size() - 1) arrayTy = module->get_array_type(cur_Type, dimensions[i]);
            else arrayTy = module->get_array_type(arrayTy, dimensions[i]);
        }
        auto arrayAlloc = builder->create_alloca(arrayTy);
        scope.push(varName, arrayAlloc);
        if (ast.initVal == nullptr) 
        { 
            //�޳�ʼ��
            if (is_Const) cout << "no initVal when define const!" << endl;   //�޳�ʼ���ֲ���������
            return; //�޳�ʼ����������������������
        }
        Value* i32P = builder->create_bitcast(arrayAlloc, INT32PTR_T);
        auto memclr = scope.find("memclr");
        builder->create_call(memclr, { i32P, CONST_INT(totalByte) }); //ȫ�����㣬��float����������
        //�����ʼ��ʱ����Աexpһ���ǿգ���initValListҲ�ǿգ����Ǵ����ţ��Ѿ�������ֱ�ӷ���
        if (ast.initVal->initValList.empty()) return;
        vector<Value*> idxs(dimensions.size() + 1);
        for (int i = 0; i < dimensions.size() + 1; i++) 
        {
            idxs[i] = CONST_INT(0);
        }
        Value* ptr = builder->create_gep(arrayAlloc, idxs); //��ȡ���鿪ͷ��ַ
        localInit(ptr, ast.initVal->initValList, dimensionsCnt, 1);
    }
}

//Ƕ�״����������ά�ȣ�����������0�ĵ�һ���� ��[0,1,0,0]������2��[0,0,0,1]������4��
//��ȫ��0��[0,0,0,0],����1
int GenIR::getNextD(vector<int>& elementsCnts, int up) 
{
    for (int i = elementsCnts.size() - 1; i > up; i--) 
    {
        if (elementsCnts[i] != 0) return i + 1;
    }
    return up + 1;
}

//����Ԫ�غ󣬺ϲ������ܺϲ�������Ԫ�أ�������
void GenIR::mergeElements(vector<int>& dimensions, vector<Array_Type*>& arrayTys, int up, int dimAdd, vector<Constant*>& elements, vector<int>& elementsCnts) 
{
    for (int i = dimAdd; i > up; i--) 
    {
        if (elementsCnts[i] % dimensions[i] == 0) 
        {
            vector<Constant*> temp;
            temp.assign(elements.end() - dimensions[i], elements.end());
            elements.erase(elements.end() - dimensions[i], elements.end());
            elements.push_back(new Constant_Array(arrayTys[i], temp));
            elementsCnts[i] = 0;
            elementsCnts[i - 1]++;
        }
        else 
            break;
    }
}

//���ϲ�����Ԫ�أ�����ϲ�����0Ԫ�أ�ʹ��elementsֻʣ��һ��arrayTys[up]���͵���������
void GenIR::mergeFinal(vector<int>& dimensions, vector<Array_Type*>& arrayTys, int up, vector<Constant*>& elements, vector<int>& elementsCnts) const 
{
    for (int i = dimensions.size() - 1; i >= up; i--) 
    {
        while (elementsCnts[i] % dimensions[i] != 0) 
        {
            //���䵱ǰ������������0Ԫ��
            if (i == dimensions.size() - 1) 
            {
                if (cur_Type == INT32_T) 
                {
                    elements.push_back(CONST_INT(0));
                }
                else 
                {
                    elements.push_back(CONST_FLOAT(0));
                }
            }
            else 
            {
                elements.push_back(new Constant_Zero(arrayTys[i + 1]));
            }
            elementsCnts[i]++;
        }
        if (elementsCnts[i] != 0) 
        {
            vector<Constant*> temp;
            temp.assign(elements.end() - dimensions[i], elements.end());
            elements.erase(elements.end() - dimensions[i], elements.end());
            elements.push_back(new Constant_Array(arrayTys[i], temp));
            elementsCnts[i] = 0;
            if (i != up) elementsCnts[i - 1]++;
        }
    }
}

//���ɱ�������ĳ�ʼ��
Constant_Array* GenIR::globalInit(vector<int>& dimensions, vector<Array_Type*>& arrayTys, int up, vector<unique_ptr<InitValAST>>& list) 
{
    vector<int> elementsCnts(dimensions.size()); //��Ӧ����ά�ȵ��������Ԫ�ظ���
    vector<Constant*> elements; //����Ԫ��
    int dimAdd;
    for (auto& val : list) 
    {
        if (val->exp != nullptr) 
        {
            dimAdd = dimensions.size() - 1;
            val->exp->accept(*this);
            checkInitType();
            elements.push_back((Constant_Int*)recVal);
        }
        else 
        {
            auto nextUp = getNextD(elementsCnts, up); //��Ƕ�������ά��
            dimAdd = nextUp - 1; //������һά�ȵ�������Ҫ���һ��Ԫ��
            if (nextUp == dimensions.size()) 
                cout << "initial invalid" << endl;//û������0��û���룬���Ϸ�
            if (val->initValList.empty()) 
            {
                elements.push_back(new Constant_Zero(arrayTys[nextUp]));
            }
            else 
            {
                auto temp = globalInit(dimensions, arrayTys, nextUp, val->initValList);
                elements.push_back(temp);
            }
        }
        elementsCnts[dimAdd]++;
        mergeElements(dimensions, arrayTys, up, dimAdd, elements, elementsCnts);
    }
    mergeFinal(dimensions, arrayTys, up, elements, elementsCnts);
    return static_cast<Constant_Array*>(elements[0]);
}


//���ݳ�ʼ����������Ƕ�������ά��
int GenIR::getNextD(vector<int>& dimensionsCnt, int up, int cnt) 
{
    for (int i = up; i < dimensionsCnt.size(); i++) 
    {
        if (cnt % dimensionsCnt[i] == 0) 
            return i;
    }
    return 0;
}

//������ָ��ݹ��ʼ������,up��ʾ���������߶���λ�ã�����[4][2][4]����������߶���[2][4],upΪ1
void GenIR::localInit(Value* ptr, vector<unique_ptr<InitValAST>>& list, vector<int>& dimensionsCnt, int up) 
{
    int cnt = 0;
    Value* tempPtr = ptr;
    for (auto& initVal : list) 
    {
        if (initVal->exp) 
        {
            if (cnt == 0) 
                cnt++; //��һ�θ�ֵʱ������һ��create_gep
            else 
                tempPtr = builder->create_gep(ptr, { CONST_INT(cnt++) });
            initVal->exp->accept(*this);
            checkInitType();
            builder->create_store(recVal, tempPtr);
        }
        else 
        {
            auto nextUp = getNextD(dimensionsCnt, up, cnt);
            if (nextUp == 0) 
                cout << "initial invalid!" << endl;
            if (!initVal->initValList.empty()) 
            {
                if (cnt != 0) 
                    tempPtr = builder->create_gep(ptr, { CONST_INT(cnt) }); //û��ֵ������tempPtrʵ�ʾ���ptr
                localInit(tempPtr, initVal->initValList, dimensionsCnt, nextUp);
            }
            cnt += dimensionsCnt[nextUp]; //�����ʼ����һ��������ô��
        }
    }
}

void GenIR::visit(InitValAST& ast) 
{
    //������������exp��ֵ���������鲻�����˺���
    if (ast.exp != nullptr) 
    {
        ast.exp->accept(*this);
    }
}

void GenIR::visit(FuncDefAST& ast) 
{
    isNewFunc = true;
    params.clear();
    paramNames.clear();
    Type* retType;
    if (ast.funcType == INT_T) retType = INT32_T;
    else if (ast.funcType == FLOAT_T) retType = FLOAT_Ts;
    else retType = VOID_T;

    //��ȡ�����б�
    for (auto& funcFParam : ast.funcParamList) 
    {
        funcFParam->accept(*this);
    }
    //��ȡ��������
    auto funTy = new Function_Type(retType, params);
    //��Ӻ���
    auto func = new Function(funTy, *ast.id, module.get());
    curFun = func;
    scope.push(*ast.id, func); //�ڽ����µ�������֮ǰ��ӵ����ű���
    //���뺯��(�����µ�������)
    scope.enter();

    std::vector<Value*> args; // ��ȡ�������β�,ͨ��Function�е�iterator
    for (auto arg = func->arguments_.begin(); arg != func->arguments_.end(); arg++)
        args.push_back(*arg);

    auto bb = new BasicBlock(module.get(), "label_entry", func);
    builder->BB_ = bb;
    for (int i = 0; i < (int)(paramNames.size()); i++) 
    {
        auto alloc = builder->create_alloca(params[i]); //�����βοռ�
        builder->create_store(args[i], alloc);          // store �β�
        scope.push(paramNames[i], alloc);         //����������
    }
    //����ͳһreturn��֧
    retBB = new BasicBlock(module.get(), "label_ret", func);
    if (retType == VOID_T) 
    {
        // void�������践��ֵ
        builder->BB_ = retBB;
        builder->create_void_ret();
    }
    else 
    {
        retAlloca = builder->create_alloca(retType); // ���ڴ��з��䷵��ֵ��λ��
        builder->BB_ = retBB;
        auto retLoad = builder->create_load(retAlloca);
        builder->create_ret(retLoad);
    }
    //���»ص�������ʼ
    builder->BB_ = bb;
    has_br = false;
    ast.block->accept(*this);

    //����û��return�Ŀտ�
    if (!builder->BB_->get_terminator())
        builder->create_br(retBB);
}

void GenIR::visit(FuncParamAST& ast) 
{
    //��ȡ��������
    Type* paramType;
    if (ast.bType == INT_T) paramType = INT32_T;
    else paramType = FLOAT_Ts;
    //�Ƿ�Ϊ����
    if (ast.isArray) 
    {
        use_Const = true; //����ά�������ͳ���
        for (int i = ast.arrays.size() - 1; i >= 0; i--) 
        {
            ast.arrays[i]->accept(*this);
            paramType = module->get_array_type(paramType, ((Constant_Int*)recVal)->value_);
        }
        use_Const = false;
        //��int a[][2]�������Ϊ[2 x i32]* ;  int a[],����Ϊi32 *
        paramType = module->get_pointer_type(paramType);
    }
    params.push_back(paramType);
    paramNames.push_back(*ast.id);
}

void GenIR::visit(BlockAST& ast) 
{
    //�����һ���µĺ����������ٽ���һ���µ�������
    if (isNewFunc)
        isNewFunc = false;
    //�����������Ҫ����һ���µ�������
    else 
    {
        scope.enter();
    }
    //����ÿһ������
    for (auto& item : ast.sentences) 
    {
        if (has_br) break;     //��BB�Ѿ�������br������ָ����Ч
        item->accept(*this);
    }

    scope.exit();
}

void GenIR::visit(SentenceAST& ast) 
{
    if (ast.decl != nullptr) 
    {
        ast.decl->accept(*this);
    }
    else 
    {
        ast.stmt->accept(*this);
    }
}

void GenIR::visit(StmtAST& ast) 
{
    switch (ast.stmtType) 
    {
    case SEMI_T:
        break;
    case ASS_T: 
    {
        Value* val[2]; //lVal, rVal
        ast.exp->accept(*this);
        val[1] = recVal;
        reqLV = true; //��ʾ���ص�ַ
        ast.leftVal->accept(*this);
        val[0] = recVal;
        //��������Ƿ�һ��
        if (val[0]->type_ == INT32PTR_T && val[1]->type_ == FLOAT_Ts) {
            val[1] = builder->create_fptosi(val[1], INT32_T);
        }
        if (val[1]->type_ == INT32_T && val[0]->type_ == FLOATPTR_T) {
            val[1] = builder->create_sitofp(val[1], FLOAT_Ts);
        }
        builder->create_store(val[1], val[0]);
        break;
    }
    case EXP_T:
        is_single_exp = true;
        ast.exp->accept(*this);
        is_single_exp = false;
        break;
    case CONT_T:
        builder->create_br(whileCondBB);
        has_br = true;
        break;
    case BRK_T:
        builder->create_br(whileFalseBB);
        has_br = true;
        break;
    case RET_T:
        ast.ret->accept(*this);
        break;
    case BLK_T:
        ast.block->accept(*this);
        break;
    case SEL_T:
        ast.sel->accept(*this);
        break;
    case ITR_T:
        ast.itr->accept(*this);
        break;
    }

}

void GenIR::visit(ReturnStmtAST& ast) 
{
    if (ast.exp == nullptr) 
    {
        recVal = builder->create_br(retBB);
    }
    else 
    {
        //�Ȱѷ���ֵstore��retAlloca�У�����ת��ͳһ�ķ������
        ast.exp->accept(*this);
        //����ת��
        if (recVal->type_ == FLOAT_Ts && curFun->get_return_type() == INT32_T) 
        {
            auto temp = builder->create_fptosi(recVal, INT32_T);
            builder->create_store(temp, retAlloca);
        }
        else if (recVal->type_ == INT32_T && curFun->get_return_type() == FLOAT_Ts) 
        {
            auto temp = builder->create_sitofp(recVal, FLOAT_Ts);
            builder->create_store(temp, retAlloca);
        }
        else
            builder->create_store(recVal, retAlloca);
        recVal = builder->create_br(retBB);
    }
    has_br = true;
}

void GenIR::visit(SelectStmtAST& ast) 
{
    //�ȱ���trueBB��falseBB����ֹǶ�׵��·�����һ���ʧ��ĵ�ַ
    auto tempTrue = trueBB;
    auto tempFalse = falseBB;

    trueBB = new BasicBlock(module.get(), to_string(id++), curFun);
    falseBB = new BasicBlock(module.get(), to_string(id++), curFun);
    BasicBlock* nextIf; // if����Ļ�����
    if (ast.elseStmt == nullptr) 
        nextIf = falseBB;
    else 
        nextIf = new BasicBlock(module.get(), to_string(id++), curFun);
    ast.cond->accept(*this);
    //����Ƿ���i1����������бȽ�
    if (recVal->type_ == INT32_T) 
    {
        recVal = builder->create_icmp_ne(recVal, CONST_INT(0));
    }
    else if (recVal->type_ == FLOAT_Ts) 
    {
        recVal = builder->create_fcmp_ne(recVal, CONST_FLOAT(0));
    }
    builder->create_cond_br(recVal, trueBB, falseBB);

    builder->BB_ = trueBB; //��ʼ����trueBB
    has_br = false;
    ast.ifStmt->accept(*this);
    if (!builder->BB_->get_terminator()) 
    {
        builder->create_br(nextIf);
    }

    if (ast.elseStmt != nullptr) 
    { // ��ʼ����falseBB
        builder->BB_ = falseBB;
        has_br = false;
        ast.elseStmt->accept(*this);
        if (!builder->BB_->get_terminator()) 
        {
            builder->create_br(nextIf);
        }
    }

    builder->BB_ = nextIf;
    has_br = false;
    // ��ԭtrueBB��falseBB
    trueBB = tempTrue;
    falseBB = tempFalse;
}

void GenIR::visit(IterationStmtAST& ast) 
{
    //�ȱ���trueBB��falseBB����ֹǶ�׵��·�����һ���ʧ��ĵ�ַ
    auto tempTrue = trueBB;
    auto tempFalse = falseBB; //��while��next block
    auto tempCond = whileCondBB;
    auto tempWhileFalseBB = whileFalseBB; //breakֻ��while��false��������ȫ��false

    whileCondBB = new BasicBlock(module.get(), to_string(id++), curFun);
    trueBB = new BasicBlock(module.get(), to_string(id++), curFun);
    falseBB = new BasicBlock(module.get(), to_string(id++), curFun);
    whileFalseBB = falseBB;

    builder->create_br(whileCondBB);
    builder->BB_ = whileCondBB; //����Ҳ��һ��������
    has_br = false;
    ast.cond->accept(*this);
    if (recVal->type_ == INT32_T) 
    {
        recVal = builder->create_icmp_ne(recVal, CONST_INT(0));
    }
    else if (recVal->type_ == FLOAT_Ts) 
    {
        recVal = builder->create_fcmp_ne(recVal, CONST_FLOAT(0.0));
    }
    builder->create_cond_br(recVal, trueBB, falseBB);

    builder->BB_ = trueBB;
    has_br = false;
    ast.stmt->accept(*this);
    //while�����һ��������cond
    if (!builder->BB_->get_terminator()) 
    {
        builder->create_br(whileCondBB);
    }

    builder->BB_ = falseBB;
    has_br = false;

    //��ԭtrueBB��falseBB��tempCond
    trueBB = tempTrue;
    falseBB = tempFalse;
    whileCondBB = tempCond;
    whileFalseBB = tempWhileFalseBB;
}

//���ݴ����������Constant�����ͣ������Ӧ��ֵ��ֵ��intVal��floatVal�У����ؼ������Ƿ�Ϊint
bool GenIR::checkCType(Value* val[], int intVal[], float floatVal[]) 
{
    bool resultIsInt = false;
    if (dynamic_cast<Constant_Int*>(val[0]) && dynamic_cast<Constant_Int*>(val[1])) 
    {
        resultIsInt = true;
        intVal[0] = dynamic_cast<Constant_Int*>(val[0])->value_;
        intVal[1] = dynamic_cast<Constant_Int*>(val[1])->value_;
    }
    else 
    { //�������һ����float
        if (dynamic_cast<Constant_Int*>(val[0])) floatVal[0] = dynamic_cast<Constant_Int*>(val[0])->value_;
        else floatVal[0] = dynamic_cast<Constant_Float*>(val[0])->value_;
        if (dynamic_cast<Constant_Int*>(val[1])) floatVal[1] = dynamic_cast<Constant_Int*>(val[1])->value_;
        else floatVal[1] = dynamic_cast<Constant_Float*>(val[1])->value_;
    }
    return resultIsInt;
}

//���ݴ�����������Ĵ����������ͣ�����Ҫת���������ת��ָ��
void GenIR::checkCType(Value* val[]) 
{
    if (val[0]->type_ == INT1_T) 
    {
        val[0] = builder->create_zext(val[0], INT32_T);
    }
    if (val[1]->type_ == INT1_T) 
    {
        val[1] = builder->create_zext(val[1], INT32_T);
    }
    if (val[0]->type_ == INT32_T && val[1]->type_ == FLOAT_Ts) 
    {
        val[0] = builder->create_sitofp(val[0], FLOAT_Ts);
    }
    if (val[1]->type_ == INT32_T && val[0]->type_ == FLOAT_Ts) 
    {
        val[1] = builder->create_sitofp(val[1], FLOAT_Ts);
    }
}

void GenIR::visit(LowAST& ast) 
{
    if (ast.low == nullptr) 
    {
        ast.high->accept(*this);
        return;
    }

    Value* val[2]; //lVal, rVal
    ast.low->accept(*this);
    val[0] = recVal;
    ast.high->accept(*this);
    val[1] = recVal;

    //�����ǳ���
    if (use_Const) 
    {
        int intVal[3]; //lInt, rInt, relInt;
        float floatVal[3]; // lFloat, rFloat, relFloat;
        bool resultIsInt = checkCType(val, intVal, floatVal);
        switch (ast.op) 
        {
        case ADD_OP:
            intVal[2] = intVal[0] + intVal[1];
            floatVal[2] = floatVal[0] + floatVal[1];
            break;
        case MINUS_OP:
            intVal[2] = intVal[0] - intVal[1];
            floatVal[2] = floatVal[0] - floatVal[1];
            break;
        }
        if (resultIsInt) recVal = CONST_INT(intVal[2]);
        else recVal = CONST_FLOAT(floatVal[2]);
        return;
    }

    //�����ǳ��������м��㣬���ָ��
    checkCType(val);
    if (val[0]->type_ == INT32_T) 
    {
        switch (ast.op) 
        {
        case ADD_OP:
            recVal = builder->create_iadd(val[0], val[1]);
            break;
        case MINUS_OP:
            recVal = builder->create_isub(val[0], val[1]);
            break;
        }
    }
    else 
    {
        switch (ast.op) 
        {
        case ADD_OP:
            recVal = builder->create_fadd(val[0], val[1]);
            break;
        case MINUS_OP:
            recVal = builder->create_fsub(val[0], val[1]);
            break;
        }
    }
}

void GenIR::visit(HighAST& ast) 
{
    if (ast.high == nullptr) 
    {
        ast.unary->accept(*this);
        return;
    }

    Value* val[2]; //lVal, rVal
    ast.high->accept(*this);
    val[0] = recVal;
    ast.unary->accept(*this);
    val[1] = recVal;

    //�����ǳ���
    if (use_Const) 
    {
        int intVal[3]; //lInt, rInt, relInt;
        float floatVal[3]; // lFloat, rFloat, relFloat;
        bool resultIsInt = checkCType(val, intVal, floatVal);
        switch (ast.op) 
        {
        case MUL_OP:
            intVal[2] = intVal[0] * intVal[1];
            floatVal[2] = floatVal[0] * floatVal[1];
            break;
        case DIV_OP:
            intVal[2] = intVal[0] / intVal[1];
            floatVal[2] = floatVal[0] / floatVal[1];
            break;
        case MOD_OP:
            intVal[2] = intVal[0] % intVal[1];
            break;
        }
        if (resultIsInt) recVal = CONST_INT(intVal[2]);
        else recVal = CONST_FLOAT(floatVal[2]);
        return;
    }

    //�����ǳ��������м��㣬���ָ��
    checkCType(val);
    if (val[0]->type_ == INT32_T) 
    {
        switch (ast.op) 
        {
        case MUL_OP:
            recVal = builder->create_imul(val[0], val[1]);
            break;
        case DIV_OP:
            recVal = builder->create_isdiv(val[0], val[1]);
            break;
        case MOD_OP:
            recVal = builder->create_isrem(val[0], val[1]);
            break;
        }
    }
    else {
        switch (ast.op) 
        {
        case MUL_OP:
            recVal = builder->create_fmul(val[0], val[1]);
            break;
        case DIV_OP:
            recVal = builder->create_fdiv(val[0], val[1]);
            break;
        case MOD_OP://never occur
            break;
        }
    }
}

void GenIR::visit(UnaryAST& ast) 
{
    // Ϊ������ʽ
    if (use_Const) 
    {
        if (ast.primaryExp) 
        {
            ast.primaryExp->accept(*this);
        }
        else if (ast.unary) 
        {
            ast.unary->accept(*this);
            if (ast.op == MINUS_OP) 
            {
                //�����ͳ���
                if (dynamic_cast<Constant_Int*>(recVal)) 
                {
                    auto temp = (Constant_Int*)recVal;
                    temp->value_ = -temp->value_;
                    recVal = temp;
                }
                else 
                {
                    auto temp = (Constant_Float*)recVal;
                    temp->value_ = -temp->value_;
                    recVal = temp;
                }
            }
        }
        else 
        {
            cout << "Function call in ConstExp!" << endl;
        }
        return;
    }


    //���ǳ�����ʽ
    if (ast.primaryExp != nullptr) 
    {
        ast.primaryExp->accept(*this);
    }
    else if (ast.call != nullptr) 
    {
        ast.call->accept(*this);
    }
    else {
        ast.unary->accept(*this);
        if (recVal->type_ == VOID_T)
            return;
        else if (recVal->type_ == INT1_T) // INT1-->INT32
            recVal = builder->create_zext(recVal, INT32_T);

        if (recVal->type_ == INT32_T) 
        {
            switch (ast.op) 
            {
            case U_NEG:
                recVal = builder->create_isub(CONST_INT(0), recVal);
                break;
            case U_NOT:
                recVal = builder->create_icmp_eq(recVal, CONST_INT(0));
                break;
            case U_POS:
                break;
            }
        }
        else 
        {
            switch (ast.op) 
            {
            case U_NEG:
                recVal = builder->create_fsub(CONST_FLOAT(0), recVal);
                break;
            case U_NOT:
                recVal = builder->create_fcmp_eq(recVal, CONST_FLOAT(0));
                break;
            case U_POS:
                break;
            }
        }
    }
}

void GenIR::visit(PrimaryExpAST& ast) 
{
    if (ast.exp) 
    {
        ast.exp->accept(*this);
    }
    else if (ast.leftVal) 
    {
        ast.leftVal->accept(*this);
    }
    else if (ast.number) 
    {
        ast.number->accept(*this);
    }
}

void GenIR::visit(LeftValAST& ast) 
{
    bool isTrueLVal = reqLV; //�Ƿ�������Ϊ��ֵ
    reqLV = false;
    auto var = scope.find(*ast.id);
    //ȫ���������ڣ�һ��ʹ�ó�����ȫ���������·��ʵ�LValAST����ôuse_constһ������Ϊ��true
    if (scope.in_global()) 
    {
        //�������飬ֱ�ӷ��ظó���
        if (ast.dimensions.empty()) 
        {
            recVal = var;
            return;
        }
        //�������飬��varһ����ȫ�ֳ�������
        vector<int> index;
        for (auto& exp : ast.dimensions) 
        {
            exp->accept(*this);
            index.push_back(dynamic_cast<Constant_Int*>(recVal)->value_);
        }
        recVal = ((Global_Variable*)var)->init_val_; //ʹ��var�ĳ�ʼ��������ҳ���Ԫ��
        for (int i : index) 
        {
            //ĳ����Ԫ��ΪConstantZero�������һ����0
            if (dynamic_cast<Constant_Zero*>(recVal)) 
            {
                Type* arrayTy = recVal->type_;
                //������Ԫ�ر�ǩ
                while (dynamic_cast<Array_Type*>(arrayTy)) 
                {
                    arrayTy = dynamic_cast<Array_Type*>(arrayTy)->contained_;
                }
                if (arrayTy == INT32_T) recVal = CONST_INT(0);
                else recVal = CONST_FLOAT(0);
                return;
            }
            if (dynamic_cast<Constant_Array*>(recVal)) 
            {
                recVal = ((Constant_Array*)recVal)->const_array[i];
            }
        }
        return;
    }

    //�ֲ�������
    if (var->type_->tid_ == Type::IntegerTyID || var->type_->tid_ == Type::FloatTyID) 
    { //˵��Ϊ�ֲ�����
        recVal = var;
        return;
    }
    // ���ǳ�����ôvarһ����ָ������
    Type* varType = static_cast<Pointer_Type*>(var->type_)->contained_; //��ָ������
    if (!ast.dimensions.empty()) 
    { //˵��������
        vector<Value*> idxs;
        for (auto& exp : ast.dimensions) 
        {
            exp->accept(*this);
            idxs.push_back(recVal);
        }
        // �������������i32 *������������Ϊi32 **�ľֲ��������������
        if (varType->tid_ == Type::PointerTyID) 
        {
            var = builder->create_load(var);
        }
        else if (varType->tid_ == Type::ArrayTyID) 
        {
            idxs.insert(idxs.begin(), CONST_INT(0));
        }
        var = builder->create_gep(var, idxs); //��ȡ��һ����ָ������
        varType = ((Pointer_Type*)var->type_)->contained_;
    }

    //ָ��Ļ�������,��ôһ���Ǵ������,������Ϊx[2], ����Ϊint a[]����Ҫ��i32 *
    if (varType->tid_ == Type::ArrayTyID) 
    {
        recVal = builder->create_gep(var, { CONST_INT(0), CONST_INT(0) });
    }
    else if (!isTrueLVal) 
    { //�������ȡ��ֵ����ôload
        recVal = builder->create_load(var);
    }
    else 
    { //���򷵻ص�ֵַ
        recVal = var;
    }
}

void GenIR::visit(NumberAST& ast) 
{
    if (ast.isInt) recVal = CONST_INT(ast.int_t);
    else recVal = CONST_FLOAT(ast.float_t);
}

void GenIR::visit(CallAST& ast) 
{
    auto fun = (Function*)scope.find(*ast.id);
    //���ú�������ֵ
    if (fun->basic_blocks_.size() && !is_single_exp)
        fun->use_ret_cnt++;
    is_single_exp = false;
    vector<Value*> args;
    for (int i = 0; i < ast.funcParamList.size(); i++) 
    {
        ast.funcParamList[i]->accept(*this);
        //��麯���β���ʵ�������Ƿ�ƥ��
        if (recVal->type_ == INT32_T && fun->arguments_[i]->type_ == FLOAT_Ts) 
        {
            recVal = builder->create_sitofp(recVal, FLOAT_Ts);
        }
        else if (recVal->type_ == FLOAT_Ts && fun->arguments_[i]->type_ == INT32_T) 
        {
            recVal = builder->create_fptosi(recVal, INT32_T);
        }
        args.push_back(recVal);
    }
    recVal = builder->create_call(fun, args);
}

void GenIR::visit(RelExpAST& ast) 
{
    if (ast.low == nullptr) 
    {
        ast.high->accept(*this);
        return;
    }
    Value* val[2];
    ast.low->accept(*this);
    val[0] = recVal;
    ast.high->accept(*this);
    val[1] = recVal;
    checkCType(val);
    if (val[0]->type_ == INT32_T) 
    {
        switch (ast.op) 
        {
        case LE_REL:
            recVal = builder->create_icmp_le(val[0], val[1]);
            break;
        case LT_REL:
            recVal = builder->create_icmp_lt(val[0], val[1]);
            break;
        case GT_REL:
            recVal = builder->create_icmp_gt(val[0], val[1]);
            break;
        case GE_REL:
            recVal = builder->create_icmp_ge(val[0], val[1]);
            break;
        }
    }
    else 
    {
        switch (ast.op) 
        {
        case LE_REL:
            recVal = builder->create_fcmp_le(val[0], val[1]);
            break;
        case LT_REL:
            recVal = builder->create_fcmp_lt(val[0], val[1]);
            break;
        case GT_REL:
            recVal = builder->create_fcmp_gt(val[0], val[1]);
            break;
        case GE_REL:
            recVal = builder->create_fcmp_ge(val[0], val[1]);
            break;
        }
    }
}

void GenIR::visit(EqExpAST& ast) 
{
    if (ast.low == nullptr) 
    {
        ast.high->accept(*this);
        return;
    }
    Value* val[2];
    ast.low->accept(*this);
    val[0] = recVal;
    ast.high->accept(*this);
    val[1] = recVal;
    checkCType(val);
    if (val[0]->type_ == INT32_T) 
    {
        switch (ast.op) 
        {
        case EQ_REL:
            recVal = builder->create_icmp_eq(val[0], val[1]);
            break;
        case NE_REL:
            recVal = builder->create_icmp_ne(val[0], val[1]);
            break;
        }
    }
    else 
    {
        switch (ast.op) 
        {
        case EQ_REL:
            recVal = builder->create_fcmp_eq(val[0], val[1]);
            break;
        case NE_REL:
            recVal = builder->create_fcmp_ne(val[0], val[1]);
            break;
        }
    }
}

void GenIR::visit(AndAST& ast) 
{
    if (ast.low == nullptr) 
    {
        ast.high->accept(*this);
        return;
    }
    auto tempTrue = trueBB; //��ֹǶ��and����ԭtrueBB��ʧ���������ɶ�·ģ��
    trueBB = new BasicBlock(module.get(), to_string(id++), curFun);
    ast.low->accept(*this);

    if (recVal->type_ == INT32_T) 
    {
        recVal = builder->create_icmp_ne(recVal, CONST_INT(0));
    }
    else if (recVal->type_ == FLOAT_Ts) 
    {
        recVal = builder->create_fcmp_ne(recVal, CONST_FLOAT(0));
    }
    builder->create_cond_br(recVal, trueBB, falseBB);
    builder->BB_ = trueBB;
    has_br = false;
    trueBB = tempTrue; //��ԭԭ����trueģ��

    ast.high->accept(*this);
}

void GenIR::visit(OrAST& ast) 
{
    if (ast.low == nullptr) 
    {
        ast.high->accept(*this);
        return;
    }
    auto tempFalse = falseBB; //��ֹǶ��and����ԭtrueBB��ʧ���������ɶ�·ģ��
    falseBB = new BasicBlock(module.get(), to_string(id++), curFun);
    ast.low->accept(*this);
    if (recVal->type_ == INT32_T) 
    {
        recVal = builder->create_icmp_ne(recVal, CONST_INT(0));
    }
    else if (recVal->type_ == FLOAT_Ts) 
    {
        recVal = builder->create_fcmp_ne(recVal, CONST_FLOAT(0));
    }
    builder->create_cond_br(recVal, trueBB, falseBB);
    builder->BB_ = falseBB;
    has_br = false;
    falseBB = tempFalse;
    ast.high->accept(*this);
}
