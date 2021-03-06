using namespace std;
//**************forward declarations*****************
bool is_number(string s);
//***************************************************
enum STATE
{
    NORMAL,
    IF,
    ELSE,
    WHILE,
    COMPILE_CONDN,
    COMPILE_BLOCK,
    VAR,
    FUNCTION,
    COMPILE_FUNCTION,
};

struct Interpreter
{

    stack<Object> main_stack;
    stack<pair<STATE, bool>> control_flow_stack;
    stack<pair<vector<Object>, vector<Object>>> while_stack;
    unordered_map<string, vector<Object>> function_dict;
    unordered_map<string, Object> var_dict;
    string current_function_name;
    Interpreter()
    {
        control_flow_stack.push(make_pair(NORMAL, true));
    }
    void interpret(const vector<Object> objects);
    void add();
    void subtract();
    void multiply();
    void divide();
    void remainder();
    bool is_truthy(Object obj);
    void less_than();
    void greater_than();
    void equal_to();
    void dup();
    void top();
    void pop();
    void swap();
    void show();
    void clear();
    void _or();
    void _and();
    void _not();
    void _if();
    void _else();
    void _while();
    bool inside_compile_condn();
    bool inside_compile_block();
    void execute_while();
    void function();
    bool inside_function_block();
    void exec_function(string name);
    void _do();
    void _end();
    void var();
    void declare_var(string var);
    void set();
    void val();
    void print_control_flow_stack();
    void include();
    void _typeof();
    void _strlen();
    void at();
};

void Interpreter::interpret(const vector<Object> objects)
{
    int idx = 0;
    while (idx != objects.size())
    {
        Object object = objects[idx];
        if (object.type == KEYWORD)
        {
            if (object.data == "end")
            {
                _end();
                idx++;
                continue;
            }
            else if (object.data == "do")
            {
                _do();
                idx++;
                continue;
            }
            else if (inside_compile_condn())
                while_stack.top().first.push_back(object);
            else if (inside_compile_block())
                while_stack.top().second.push_back(object);
            else if (inside_function_block())
                function_dict[current_function_name].push_back(object);
            if (object.data == "if")
                _if();
            else if (object.data == "else")
                _else();
            else if (object.data == "while")
                _while();
            else if (object.data == "function")
                function();
            else if (object.data == "include")
                include();
            else if (control_flow_stack.top().second == true)
            {
                if (object.data == "+")
                    add();
                else if (object.data == "-")
                    subtract();
                else if (object.data == "*")
                    multiply();
                else if (object.data == "/")
                    divide();
                else if (object.data == "%")
                    remainder();
                else if (object.data == "dup")
                    dup();
                else if (object.data == "<")
                    less_than();
                else if (object.data == ">")
                    greater_than();
                else if (object.data == "==")
                    equal_to();
                else if (object.data == "and")
                    _and();
                else if (object.data == "or")
                    _or();
                else if (object.data == "not")
                    _not();
                else if (object.data == "pop")
                    pop();
                else if (object.data == "show")
                    show();
                else if (object.data == "top")
                    top();
                else if (object.data == "clear")
                    clear();
                else if (object.data == "var")
                    var();
                else if (object.data == "set")
                    set();
                else if (object.data == "val")
                    val();
                else if (object.data == "\\n")
                    cout << endl;
                else if (object.data == "swap")
                    swap();
                else if (object.data == "typeof")
                    _typeof();
                else if (object.data == "strlen")
                    _strlen();
                else if (object.data == "at")
                    at();
            }
        }
        else
        {
            if (control_flow_stack.top().first == FUNCTION)
            {
                current_function_name = object.data;
            }
            else if (inside_compile_condn())
            {
                while_stack.top().first.push_back(object);
            }
            else if (inside_compile_block())
            {
                while_stack.top().second.push_back(object);
            }
            else if (inside_function_block())
            {
                function_dict[current_function_name].push_back(object);
            }
            else if (control_flow_stack.top().second == true && function_dict.find(object.data) != function_dict.end())
            {
                exec_function(object.data);
            }
            else if (control_flow_stack.top().second == true)
            {
                if(var_dict.find(object.data) != var_dict.end())
                {
                    object.type = TYPE::T_VAR;
                }
                main_stack.push(object);
                if (control_flow_stack.top().first == VAR)
                {
                    declare_var(object.data);
                }
            }
        }
        idx++;
    }
}

void Interpreter::add()
{
    Object top = main_stack.top();
    main_stack.pop();
    Object top1 = main_stack.top();
    main_stack.pop();
    if (top.type == NUMBER && top1.type == NUMBER)
    {
        Object obj;
        obj.type = NUMBER;
        obj.data = to_string(stof(top1.data) + stof(top.data));
        main_stack.push(obj);
        return;
    }
    else if (top.type == STRING && top1.type == STRING)
    {
        Object obj;
        obj.type = STRING;
        obj.data = top1.data + top.data;
        main_stack.push(obj);
        return;
    }
}
void Interpreter::subtract()
{
    float top = stof(main_stack.top().data);
    main_stack.pop();
    float top1 = stof(main_stack.top().data);
    main_stack.pop();
    Object obj;
    obj.type = NUMBER;
    obj.data = to_string(top1 - top);
    main_stack.push(obj);
}
void Interpreter::multiply()
{
    float top = stof(main_stack.top().data);
    main_stack.pop();
    float top1 = stof(main_stack.top().data);
    main_stack.pop();
    Object obj;
    obj.type = NUMBER;
    obj.data = to_string(top * top1);
    main_stack.push(obj);
}
void Interpreter::divide()
{
    float top = stof(main_stack.top().data);
    main_stack.pop();
    float top1 = stof(main_stack.top().data);
    main_stack.pop();
    Object obj;
    obj.type = NUMBER;
    obj.data = to_string(top1 / top);
    main_stack.push(obj);
}
void Interpreter::remainder()
{
    int top = stoi(main_stack.top().data);
    main_stack.pop();
    int top1 = stoi(main_stack.top().data);
    main_stack.pop();
    Object obj;
    obj.type = NUMBER;
    obj.data = to_string(top1 % top);
    main_stack.push(obj);
}
bool Interpreter::is_truthy(Object obj)
{
    bool flag = false;
    if (obj.type == NUMBER)
    {
        float num = stof(obj.data);
        if (num != 0)
            flag = true;
    }
    else if (obj.type == BOOL)
    {
        if (obj.data == "true")
            flag = true;
    }
    else if (obj.type == _NULL)
    {
        flag = false;
    }
    else if (obj.data.length() > 0)
        flag = true;
    return flag;
}
void Interpreter::less_than()
{
    float top = stof(main_stack.top().data);
    main_stack.pop();
    float top1 = stof(main_stack.top().data);
    main_stack.pop();
    Object obj;
    obj.type = BOOL;
    if (top1 < top)
    {
        obj.data = "true";
        main_stack.push(obj);
    }
    else
    {
        obj.data = "false";
        main_stack.push(obj);
    }
}
void Interpreter::greater_than()
{
    float top = stof(main_stack.top().data);
    main_stack.pop();
    float top1 = stof(main_stack.top().data);
    main_stack.pop();
    Object obj;
    obj.type = BOOL;
    if (top1 > top)
    {
        obj.data = "true";
        main_stack.push(obj);
    }
    else
    {
        obj.data = "false";
        main_stack.push(obj);
    }
}
void Interpreter::equal_to()
{
    Object top = main_stack.top();
    main_stack.pop();
    Object top1 = main_stack.top();
    main_stack.pop();
    Object obj;
    obj.type = BOOL;
    if (top1.type == NUMBER)
    {
        if (stof(top1.data) == stof(top.data))
            obj.data = "true";
        else
            obj.data = "false";
    }
    else
    {
        if (top1.data == top.data)
            obj.data = "true";
        else
            obj.data = "false";
    }
    main_stack.push(obj);
}
void Interpreter::dup()
{
    main_stack.push(main_stack.top());
}
void Interpreter::top()
{
    Object obj = main_stack.top();
    if (obj.type == NUMBER)
    {
        cout << stof(obj.data);
    }
    else
    {
        cout << obj.data;
    }
}
void Interpreter::pop()
{
    main_stack.pop();
}
void Interpreter::swap()
{
    Object top1 = main_stack.top();
    main_stack.pop();
    Object top2 = main_stack.top();
    main_stack.pop();
    main_stack.push(top1);
    main_stack.push(top2);
}
void Interpreter::show()
{
    stack<Object> temp = main_stack;
    stack<Object> helper;
    while (!temp.empty())
    {
        helper.push(temp.top());
        temp.pop();
    }
    cout << "<" << helper.size() << "> ";
    while (!helper.empty())
    {
        Object obj = helper.top();
        helper.pop();
        if (obj.type == NUMBER)
        {
            cout << stof(obj.data);
        }
        else if (obj.type == STRING)
        {
            cout << "\"";
            cout << obj.data;
            cout << "\"";
        }
        else
        {
            cout << obj.data;
        }
        cout << " ";
    }
    cout << endl;
}
void Interpreter::clear()
{
    while (!main_stack.empty())
        main_stack.pop();
}
void Interpreter::_or()
{
    Object top = main_stack.top();
    main_stack.pop();
    Object top2 = main_stack.top();
    main_stack.pop();
    Object obj;
    obj.type = BOOL;
    if (is_truthy(top) || is_truthy(top2))
    {
        obj.data = "true";
        main_stack.push(obj);
    }
    else
    {
        obj.data = "false";
        main_stack.push(obj);
    }
}
void Interpreter::_and()
{
    Object top = main_stack.top();
    main_stack.pop();
    Object top2 = main_stack.top();
    main_stack.pop();
    Object obj;
    obj.type = BOOL;
    if (is_truthy(top) && is_truthy(top2))
    {
        obj.data = "true";
        main_stack.push(obj);
    }
    else
    {
        obj.data = "false";
        main_stack.push(obj);
    }
}
void Interpreter::_not()
{
    Object top = main_stack.top();
    main_stack.pop();
    Object obj;
    obj.type = BOOL;
    if (is_truthy(top))
    {
        obj.data = "false";
        main_stack.push(obj);
    }
    else
    {
        obj.data = "true";
        main_stack.push(obj);
    }
}
//**********************if else block***********************************
//check if the if condition can be executed, if yes then execute
void Interpreter::_if()
{
    bool this_block = true;
    if (control_flow_stack.top().second == false)
    {
        this_block = false;
    }
    control_flow_stack.push(make_pair(IF, this_block));
}
//when the else keyword is encountered, if the previous state is true and the current state is false
//i.e the if block did not execute then set the new state aka the state of the else block to be true
void Interpreter::_else()
{
    pair<STATE, bool> curr_state = control_flow_stack.top();
    control_flow_stack.pop();
    pair<STATE, bool> prev_state = control_flow_stack.top();
    pair<STATE, bool> new_state = make_pair(ELSE, false);
    if (prev_state.second)
    {
        if (curr_state.second == false)
        {
            new_state.second = true;
        }
    }
    control_flow_stack.push(new_state);
}
//**********************************************************************

//************************ while block *********************************
void Interpreter::_while()
{
    if (control_flow_stack.top().second == true)
    {
        control_flow_stack.push(make_pair(WHILE, true));
        control_flow_stack.push(make_pair(COMPILE_CONDN, false));
        while_stack.push(make_pair(vector<Object>(), vector<Object>()));
    }
    else
    {
        control_flow_stack.push(make_pair(WHILE, false));
    }
}
bool Interpreter::inside_compile_condn()
{
    stack<pair<STATE, bool>> st_cp = control_flow_stack;
    while (!st_cp.empty())
    {
        if (st_cp.top().first == STATE::COMPILE_CONDN)
            return true;
        st_cp.pop();
    }
    return false;
}
bool Interpreter::inside_compile_block()
{
    stack<pair<STATE, bool>> st_cp = control_flow_stack;
    while (!st_cp.empty())
    {
        if (st_cp.top().first == STATE::COMPILE_BLOCK)
            return true;
        st_cp.pop();
    }
    return false;
}
void Interpreter::execute_while()
{
    while (true)
    {
        interpret(while_stack.top().first);
        bool check = is_truthy(main_stack.top());
        main_stack.pop();
        if (check)
        {
            interpret(while_stack.top().second);
        }
        else
        {
            break;
        }
    }
    while_stack.pop();
}
//**********************************************************************

//********************* functions ********************
//when the function keyword is encountered
void Interpreter::function()
{
    control_flow_stack.push(make_pair(FUNCTION, false));
}
//for storing the body of the function
//after the do keyword everything is a part of the function
bool Interpreter::inside_function_block()
{
    stack<pair<STATE, bool>> st_cp = control_flow_stack;
    while (!st_cp.empty())
    {
        if (st_cp.top().first == STATE::COMPILE_FUNCTION)
            return true;
        st_cp.pop();
    }
    return false;
}
//execute the function
void Interpreter::exec_function(string name)
{
    vector<Object> function_body = function_dict[name];
    interpret(function_body);
}
//********************************************************************

//************************ other control flow handlers *****************
void Interpreter::_do()
{
    pair<STATE, bool> &curr_state = control_flow_stack.top();
    //for handling the function <name> do portion of a function declaration
    if (curr_state.first == FUNCTION)
    {
        control_flow_stack.push(make_pair(COMPILE_FUNCTION, false));
        function_dict[current_function_name] = vector<Object>();
        return;
    }
    // for handling the case when we encounter a do inside a function declaration
    if (inside_function_block())
    {
        Object object;
        object.type = KEYWORD;
        object.data = "do";
        function_dict[current_function_name].push_back(object);
        return;
    }
    //for handling the while .... do portion of the while loop
    if (curr_state.first == COMPILE_CONDN)
    {
        control_flow_stack.pop();
        control_flow_stack.push(make_pair(COMPILE_BLOCK, false));
        return;
    }
    //for handling the do .... end portion of the while loop when there is a nested
    //if or while block inside a while block durin the compilation of the while block
    if (inside_compile_block() &&
        control_flow_stack.top().first != STATE::COMPILE_BLOCK) //remove the second condition and check

    {
        Object object;
        object.type = KEYWORD;
        object.data = "do";
        while_stack.top().second.push_back(object);
        return;
    }
    //for handling simple if ... do ... else ... end statements
    if (curr_state.second)
    {
        Object object = main_stack.top();
        main_stack.pop();
        if (is_truthy(object))
            curr_state.second = true;
        else
            curr_state.second = false;
    }
}
void Interpreter::_end()
{
    if (control_flow_stack.top().first == COMPILE_FUNCTION)
    {
        control_flow_stack.pop();
        current_function_name.clear();
    }
    else if (inside_function_block())
    {
        Object obj;
        obj.type = KEYWORD;
        obj.data = "end";
        function_dict[current_function_name].push_back(obj);
        control_flow_stack.pop();
        return;
    }
    if (control_flow_stack.top().first == STATE::COMPILE_BLOCK)
    {
        control_flow_stack.pop();
    }
    if (control_flow_stack.top().first == STATE::WHILE && !inside_compile_block())
    {
        execute_while();
    }
    if (inside_compile_block() &&
        control_flow_stack.top().first != STATE::COMPILE_BLOCK)
    {
        Object obj;
        obj.type = KEYWORD;
        obj.data = "end";
        while_stack.top().second.push_back(obj);
    }
    control_flow_stack.pop();
}
//**********************************************************************

//******************************* var **********************************
void Interpreter::var()
{
    control_flow_stack.push(make_pair(VAR, true));
}
void Interpreter::declare_var(string var)
{

    var_dict[var] = Object();
    main_stack.pop();
    control_flow_stack.pop();
}
void Interpreter::set()
{
    Object var = main_stack.top();
    main_stack.pop();
    Object val = main_stack.top();
    main_stack.pop();
    var_dict[var.data] = val;
}
void Interpreter::val()
{
    Object var = main_stack.top();
    main_stack.pop();
    Object data = var_dict[var.data];
    main_stack.push(data);
}
//**********************************************************************

void Interpreter::print_control_flow_stack()
{
    stack<pair<STATE, bool>> cfs_cpy = control_flow_stack;
    stack<pair<STATE, bool>> cfs_cpy1;
    while (!cfs_cpy.empty())
    {
        cfs_cpy1.push(cfs_cpy.top());
        cfs_cpy.pop();
    }
    while (!cfs_cpy1.empty())
    {
        cout << "<"
             << "state :" << cfs_cpy1.top().first << ", "
             << "status: " << cfs_cpy1.top().second
             << ">"
             << " ";
        cfs_cpy1.pop();
    }
    cout << endl;
}

void Interpreter::include()
{
    string name = main_stack.top().data;
    Lexer L;
    main_stack.pop();
    ifstream file(name);
    string temp;
    string all;
    while (getline(file, temp))
    {
        vector<Object> objects = L.tokenize(temp);
        interpret(objects);
    }
}
void Interpreter::_typeof()
{
    Object obj = main_stack.top();
    main_stack.pop();
    Object res;
    res.type = STRING;
    switch (obj.type)
    {
    case BOOL:
        res.data = "bool";
        break;
    case NUMBER:
        res.data = "number";
        break;
    case STRING:
        res.data = "string";
        break;
    case ARRAY:
        res.data = "array";
        break;
    case _NULL:
        res.data = "null";
        break;
    case TYPE::T_VAR:
        res.data = "var";
        break;
    default:
        res.data = "keyword";
        break;
    }
    main_stack.push(res);
}
void Interpreter::_strlen()
{
    Object obj = main_stack.top();
    main_stack.pop();
    Object res;
    res.type = NUMBER;
    res.data = to_string(obj.data.size());
    main_stack.push(res);
}
void Interpreter::at()
{
    Object num = main_stack.top();
    main_stack.pop();
    Object obj = main_stack.top();
    main_stack.pop();
    Object res;
    res.type = STRING;
    res.data = obj.data[stoi(num.data)];
    main_stack.push(res);
}