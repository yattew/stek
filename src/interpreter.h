using namespace std;

//********* definations*****************************
#define FLOAT_THRESHOLD 0.0001
//***************************************************

//**************forward declarations*****************
bool is_number(string s);
//***************************************************
//todo add error handling
struct Interpreter
{
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
    stack<Object> main_stack;
    stack<pair<STATE, bool>> control_flow_stack;
    stack<pair<vector<Object>, vector<Object>>> while_stack;
    unordered_map<string, vector<Object>> function_dict;
    unordered_map<string, Object> var_dict;
    string current_function_name;
    int tokens_count;
    Interpreter() : tokens_count(0)
    {
        control_flow_stack.push(make_pair(NORMAL, true));
    }
    void interpret(const vector<Object> objects);
    void add();
    void subtract();
    void multiply();
    void divide();
    void remainder();
    void dup();
    void _if();
    void _do();
    void _else();
    void _end();
    void _while();
    void execute_while();
    void top();
    void pop();
    void show();
    void _or();
    void _and();
    void _not();
    bool is_truthy(Object obj);
    void less_than();
    void greater_than();
    void equal_to();
    void clear();
    bool inside_compile_condn();
    bool inside_compile_block();
    void var();
    void declare_var(string var);
    void set();
    void val();
    void print();
    void function();
    bool inside_function_block();
    void exec_function(string name);
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
                tokens_count++;
                continue;
            }
            else if (object.data == "do")
            {
                _do();
                idx++;
                tokens_count++;
                continue;
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
            if (object.data == "if")
                _if();
            else if (object.data == "else")
                _else();
            else if (object.data == "while")
                _while();
            else if (object.data == "function")
            {
                function();
            }
            else
            {
                if (control_flow_stack.top().second == true)
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
                    {
                        var();
                    }
                    else if (object.data == "set")
                        set();
                    else if (object.data == "val")
                        val();
                    else if (object.data == "print")
                    {
                        print();
                    }
                    else if (object.data == "\\n")
                    {
                        cout << endl;
                    }
                    else
                    {
                        auto it = var_dict.find(object.data);
                        if (it != var_dict.end())
                        {
                            main_stack.push(object);
                        }
                        else
                        {
                            cout << "undefined word"
                                 << "<" << object.data << ">" << endl;
                        }
                    }
                }
            }
        }
        else
        {
            if (control_flow_stack.top().first == FUNCTION)
            {
                current_function_name = object.data;
                tokens_count++;
                idx++;
                continue;
            }
            if (inside_compile_condn())
            {
                while_stack.top().first.push_back(object);
                tokens_count++;
                idx++;
                continue;
            }

            else if (inside_compile_block())
            {
                while_stack.top().second.push_back(object);
                tokens_count++;
                idx++;
                continue;
            }
            else if (inside_function_block())
            {
                function_dict[current_function_name].push_back(object);
                tokens_count++;
                idx++;
                continue;
            }
            else if (function_dict.find(object.data) != function_dict.end())
            {
                exec_function(object.data);
                tokens_count++;
                idx++;
                continue;
            }
            // else{
            //     cout<<"here"<<endl;
            // }
            if (control_flow_stack.top().second == true)
            {
                main_stack.push(object);
                if (control_flow_stack.top().first == VAR)
                {
                    declare_var(object.data);
                }
            }
        }
        tokens_count++;
        idx++;
    }
}
void Interpreter::function()
{
    control_flow_stack.push(make_pair(FUNCTION, false));
}
void Interpreter::exec_function(string name)
{
    vector<Object> function_body = function_dict[name];
    // for(auto x: function_body) cout<<x.data<<" ";
    // cout<<endl;
    interpret(function_body);
    // cout << "function called with name = " << name << endl;
}
void Interpreter::print()
{
    Object obj = main_stack.top();
    main_stack.pop();
    if (obj.type == NUMBER)
    {
        cout << stof(obj.data);
    }
    else
    {
        cout << obj.data;
    }
}
void Interpreter::val()
{
    Object var = main_stack.top();
    main_stack.pop();
    Object data = var_dict[var.data];
    main_stack.push(data);
}
void Interpreter::set()
{
    Object var = main_stack.top();
    main_stack.pop();
    Object val = main_stack.top();
    main_stack.pop();
    var_dict[var.data] = val;
}
void Interpreter::declare_var(string var)
{

    var_dict[var] = Object();
    main_stack.pop();
    control_flow_stack.pop();
}
void Interpreter::var()
{
    control_flow_stack.push(make_pair(VAR, true));
}
bool Interpreter::inside_compile_condn()
{
    stack<pair<STATE, bool>> st_cp = control_flow_stack;
    while (!st_cp.empty())
    {
        if (st_cp.top().first == STATE::COMPILE_CONDN)
        {
            return true;
        }
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
        {
            return true;
        }
        st_cp.pop();
    }
    return false;
}
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
void Interpreter::_if()
{
    bool this_block = true;
    if (control_flow_stack.top().second == false)
    {
        this_block = false;
    }
    control_flow_stack.push(make_pair(IF, this_block));
}
bool Interpreter::inside_function_block()
{
    stack<pair<STATE, bool>> st_cp = control_flow_stack;
    while (!st_cp.empty())
    {
        if (st_cp.top().first == STATE::COMPILE_FUNCTION)
        {
            return true;
        }
        st_cp.pop();
    }
    return false;
}
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
        {
            curr_state.second = true;
        }
        else
        {
            curr_state.second = false;
        }
    }
}
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
void Interpreter::_end()
{
    if (control_flow_stack.top().first == COMPILE_FUNCTION)
    {
        control_flow_stack.pop();
        current_function_name.clear();
    }
    else if(inside_function_block())
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
void Interpreter::add()
{
    float top = stof(main_stack.top().data);
    main_stack.pop();
    float top1 = stof(main_stack.top().data);
    main_stack.pop();
    Object obj;
    obj.type = NUMBER;
    obj.data = to_string(top + top1);
    main_stack.push(obj);
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
    float top = stof(main_stack.top().data);
    main_stack.pop();
    float top1 = stof(main_stack.top().data);
    main_stack.pop();
    Object obj;
    obj.type = BOOL;
    if (top1 == top)
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
    else if (obj.data.length() > 0)
        flag = true;
    return flag;
}
void Interpreter::clear()
{
    while (!main_stack.empty())
        main_stack.pop();
}