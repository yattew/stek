using namespace std;

//********* definations*****************************
#define FLOAT_THRESHOLD 0.0001
//***************************************************

//**************forward declarations*****************
bool is_number(string s);
//***************************************************
//***************STATES******************************
enum STATE
{
    NORMAL,
    IF,
    ELSE,
    WHILE,
    COMPILE_CONDN,
    COMPILE_BLOCK,
};
//**************************************************
//todo add error handling
class Interpreter
{
public:
    stack<string> main_stack;
    stack<pair<STATE, bool>> control_flow_stack;
    stack<pair<string, string>> while_stack;
    unordered_map<string, string> user_words_dictionary;
    unordered_map<string, string> user_variables_dictionary;
    ;
    int tokens_count;
    Interpreter() : tokens_count(0)
    {
        control_flow_stack.push(make_pair(NORMAL, true));
    }
    void interpret(const string s);
    vector<string> tokenize(const string s);
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
    void empty();
    void _or();
    void _and();
    void _not();
    bool is_truthy(string token);
    void compile_word();
    void run_word(string word_string);
    void less_than();
    void greater_than();
    void equal_to();
    void clear();
    bool inside_compile_condn();
    bool inside_compile_block();
};
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
void Interpreter::interpret(const string s)
{
    vector<string> tokens = tokenize(s);
    int idx = 0;
    while (idx != tokens.size())
    {
        if (tokens[idx] == "end")
        {
            _end();
            idx++;
            tokens_count++;
            continue;
        }
        else if (tokens[idx] == "do")
        {
            _do();
            idx++;
            tokens_count++;
            continue;
        }
        else if (inside_compile_condn())
        {
            while_stack.top().first += tokens[idx] + " ";
        }
        else if (inside_compile_block())
        {
            while_stack.top().second += tokens[idx] + " ";
        }
        if (tokens[idx] == "if")
        {
            _if();
        }

        else if (tokens[idx] == "else")
        {
            _else();
        }

        else if (tokens[idx] == "while")
        {
            _while();
        }
        else
        {
            if (control_flow_stack.top().second == true)
            {
                if (is_number(tokens[idx]))
                    main_stack.push(tokens[idx]);
                else if (tokens[idx] == "+")
                    add();
                else if (tokens[idx] == "-")
                    subtract();
                else if (tokens[idx] == "*")
                    multiply();
                else if (tokens[idx] == "/")
                    divide();
                else if (tokens[idx] == "%")
                    remainder();
                else if (tokens[idx] == "dup")
                    dup();
                else if (tokens[idx] == "<")
                    less_than();
                else if (tokens[idx] == ">")
                    greater_than();
                else if (tokens[idx] == "==")
                    equal_to();
                else if (tokens[idx] == "and")
                    _and();
                else if (tokens[idx] == "or")
                    _or();
                else if (tokens[idx] == "not")
                    _not();
                else if (tokens[idx] == "makeword")
                    ;
                else if (tokens[idx] == "begin")
                    ;
                else if (tokens[idx] == "variable")
                    ;
                else if (tokens[idx] == "empty")
                    empty();
                else if (tokens[idx] == "pop")
                    pop();
                else if (tokens[idx] == "show")
                    show();
                else if (tokens[idx] == "top")
                    top();
                else if (tokens[idx] == "clear")
                    clear();
                else
                {
                    unordered_map<string, string>::iterator it = user_words_dictionary.find(tokens[idx]);
                    if (it == user_words_dictionary.end())
                        cout << "the word <" << tokens[idx] << " " << idx << "> is not defined" << endl;
                    else
                    {
                        string word_string = it->second;
                        run_word(word_string);
                    }
                }
            }
        }
        tokens_count++;
        idx++;
    }
}

void Interpreter::_while()
{
    if (control_flow_stack.top().second == true)
    {
        control_flow_stack.push(make_pair(WHILE, true));
        control_flow_stack.push(make_pair(COMPILE_CONDN, false));
        string condn;
        string block;
        while_stack.push(make_pair(condn, block));
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

void Interpreter::_do()
{
    pair<STATE, bool> &curr_state = control_flow_stack.top();
    if (curr_state.first == COMPILE_CONDN)
    {
        control_flow_stack.pop();
        control_flow_stack.push(make_pair(COMPILE_BLOCK, false));

        return;
    }
    if (inside_compile_block() &&
        control_flow_stack.top().first != STATE::COMPILE_BLOCK)
    {
        while_stack.top().second += "do ";
        return;
    }
    if (curr_state.second)
    {
        string token = main_stack.top();
        main_stack.pop();
        if (is_truthy(token))
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
    if (control_flow_stack.top().first == STATE::COMPILE_BLOCK)
    {
        control_flow_stack.pop();
    }
    if (control_flow_stack.top().first == STATE::WHILE)
    {
        execute_while();
    }
    if (inside_compile_block() &&
        control_flow_stack.top().first != STATE::COMPILE_BLOCK)
    {
        while_stack.top().second += "end ";
    }
    control_flow_stack.pop();
}

void Interpreter::_and()
{
    string top = main_stack.top();
    main_stack.pop();
    string top2 = main_stack.top();
    main_stack.pop();
    if (is_truthy(top) && is_truthy(top2))
        main_stack.push("1");
    else
        main_stack.push("0");
}
void Interpreter::_or()
{
    string top = main_stack.top();
    main_stack.pop();
    string top2 = main_stack.top();
    main_stack.pop();
    if (is_truthy(top) || is_truthy(top2))
        main_stack.push("1");
    else
        main_stack.push("0");
}
void Interpreter::_not()
{
    string top = main_stack.top();
    main_stack.pop();
    if (is_truthy(top))
        main_stack.push("0");
    else
        main_stack.push("1");
}
void Interpreter::compile_word()
{
    //todo compile words
}
void Interpreter::run_word(string word_string)
{
    //todo run the compiles words
}
vector<string> Interpreter::tokenize(const string s)
{
    vector<string> tokens;
    string temp;
    for (auto x : s)
    {
        if (x == ' ' || x == '\t' || x == '\n')
        {
            if (temp.size() > 0)
            {
                tokens.push_back(temp);
                temp.clear();
            }
        }
        else
            temp.push_back(x);
    }
    string temp1;
    for (auto x : temp)
        if (x != ' ')
            temp1.push_back(x);
    if (temp1.size() > 0)
        tokens.push_back(temp1);
    return tokens;
}
void Interpreter::add()
{
    float top = stof(main_stack.top());
    main_stack.pop();
    float top1 = stof(main_stack.top());
    main_stack.pop();
    main_stack.push(to_string(top + top1));
}
void Interpreter::subtract()
{
    float top = stof(main_stack.top());
    main_stack.pop();
    float top1 = stof(main_stack.top());
    main_stack.pop();
    main_stack.push(to_string(top1 - top));
}
void Interpreter::multiply()
{
    float top = stof(main_stack.top());
    main_stack.pop();
    float top1 = stof(main_stack.top());
    main_stack.pop();
    main_stack.push(to_string(top1 * top));
}
void Interpreter::divide()
{
    float top = stof(main_stack.top());
    main_stack.pop();
    float top1 = stof(main_stack.top());
    main_stack.pop();
    main_stack.push(to_string(top1 / top));
}
void Interpreter::remainder()
{
    int top = stoi(main_stack.top());
    main_stack.pop();
    int top1 = stoi(main_stack.top());
    main_stack.pop();
    main_stack.push(to_string(top1 % top));
}
void Interpreter::dup()
{
    main_stack.push(main_stack.top());
}
void Interpreter::top()
{
    if (is_number(main_stack.top()))
    {
        cout << stof(main_stack.top()) << endl;
    }
    else
    {
        cout << main_stack.top() << endl;
    }
}
void Interpreter::pop()
{
    main_stack.pop();
}
void Interpreter::show()
{
    stack<string> temp = main_stack;
    stack<string> helper;
    while (!temp.empty())
    {
        helper.push(temp.top());
        temp.pop();
    }
    cout << "<" << helper.size() << "> ";
    while (!helper.empty())
    {
        string tkn = helper.top();
        helper.pop();
        if (is_number(tkn))
        {
            cout << stof(tkn) << " ";
        }
        else
        {
            cout << tkn << " ";
        }
    }
    cout << endl;
}
void Interpreter::empty()
{
    while (!main_stack.empty())
    {
        main_stack.pop();
    }
}
void Interpreter::less_than()
{
    float top = stof(main_stack.top());
    main_stack.pop();
    float top1 = stof(main_stack.top());
    main_stack.pop();
    if (top1 < top)
        main_stack.push("1");
    else
        main_stack.push("0");
}
void Interpreter::greater_than()
{
    float top = stof(main_stack.top());
    main_stack.pop();
    float top1 = stof(main_stack.top());
    main_stack.pop();
    if (top1 > top)
        main_stack.push("1");
    else
        main_stack.push("0");
}
void Interpreter::equal_to()
{
    float top = stof(main_stack.top());
    main_stack.pop();
    float top1 = stof(main_stack.top());
    main_stack.pop();
    if (abs(top1 - top) < FLOAT_THRESHOLD)
        main_stack.push("1");
    else
        main_stack.push("0");
}
bool Interpreter::is_truthy(string token)
{
    bool flag = false;
    if (is_number)
    {
        float num = stof(token);
        if (num != 0)
            flag = true;
    }
    else if (token.length() > 0)
        flag = true;
    return flag;
}
void Interpreter::clear()
{
    while (!main_stack.empty())
    {
        main_stack.pop();
    }
}