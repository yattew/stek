using namespace std;
 
//********* definations*****************************
#define FLOAT_THRESHOLD 0.0001
//***************************************************

//**************forward declarations*****************
bool is_number(string s);
//***************************************************

//todo add error handling
class Interpreter
{
public:
    stack<string> main_stack;
    stack<pair<string, int>> control_flow_stack;
    unordered_map<string, string> user_words_dictionary;
    unordered_map<string, string> user_variables_dictionary;
    pair<
        string,
        pair<string, int>>
        skip_to;
    bool ignore_tokens;
    int extra_skips;
    int tokens_count;
    Interpreter() : extra_skips(0), ignore_tokens(false), tokens_count(0)
    {
    }
    void interpret(const string &s);
    vector<string> tokenize(const string &s);
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
};
void Interpreter::interpret(const string &s)
{
    vector<string> tokens = tokenize(s);
    int idx = 0;
    while (idx != tokens.size())
    {
        if (tokens[idx] == "if")
        {
            _if();
        }
        else if (tokens[idx] == "do")
        {
            _do();
        }
        else if (tokens[idx] == "else")
        {
            _else();
        }
        else if (tokens[idx] == "end")
        {
            _end();
        }
        else
        {
            if (!ignore_tokens)
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
                else if(tokens[idx] == "clear")
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

void Interpreter::_if()
{
    control_flow_stack.push(make_pair("if", tokens_count));
}
void Interpreter::_do()
{
    if (skip_to.first.length() != 0)
        return;
    string top = main_stack.top();
    main_stack.pop();
    if (!is_truthy(top))
    {
        skip_to = make_pair("else", control_flow_stack.top());
        ignore_tokens = true;
    }
}
void Interpreter::_else()
{
    if (skip_to.first.length() != 0)
    {
        if (control_flow_stack.top().second != skip_to.second.second)
        {
            return;
        }
        ignore_tokens = false;
        skip_to.first = "";
    }
    else
    {
        skip_to = make_pair("end", control_flow_stack.top());
        ignore_tokens = true;
    }
}
void Interpreter::_end()
{
    if (control_flow_stack.top().second != skip_to.second.second)
    {
        control_flow_stack.pop();
        return;
    }
    control_flow_stack.pop();
    ignore_tokens = false;
    skip_to.first = "";
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
vector<string> Interpreter::tokenize(const string &s)
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
    cout << main_stack.top() << endl;
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
        cout << helper.top() << " ";
        helper.pop();
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
void Interpreter::clear(){
    while(!main_stack.empty())
    {
        main_stack.pop();
    }
}