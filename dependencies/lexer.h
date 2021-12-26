using namespace std;
struct object;
struct Lexer
{
    unsigned int token_count;
    Lexer()
    {
        token_count = 0;
    }
    Object make_object(string word)
    {
        Object object;
        if (is_number(word))
        {
            object.type = NUMBER;
            object.data = word;
        }
        else if (is_bool(word))
        {
            object.type = BOOL;
            object.data = word;
        }
        else if (is_keyword(word))
        {
            object.type = KEYWORD;
            object.data = word;
        }
        else if (is_string(word))
        {
            object.type = STRING;
            string s = string(++word.begin(), --word.end());
            object.data = s;
        }
        else
        {
            object.type = _NULL;
            object.data = word;
        }
        return object;
    }
    vector<Object> tokenize(string text)
    {
        vector<Object> objects;
        string temp;
        bool is_string = false;
        for (auto it = text.begin(); it < text.end(); it++)
        {
            if (*it == '"')
            {
                temp.clear();
                temp.push_back(*it);
                it++;
                while (*it != '"')
                {
                    temp.push_back(*it);
                    it++;
                }
                temp.push_back(*it);
                it++;
                Object object = make_object(temp);
                objects.push_back(object);
                temp.clear();
                continue;
            }
            if (*it == ' ' || *it == '\t' || *it == '\n' || *it == '\0')
            {
                if (temp.size() > 0)
                {
                    Object object = make_object(temp);
                    objects.push_back(object);
                    temp.clear();
                }
            }
            else
            {
                temp.push_back(*it);
            }
        }
        string temp1;
        for (auto x : temp)
        {
            if (x != ' ' || x != '\t' || x != '\n' || x != '\0')
            {
                temp1.push_back(x);
            }
        }
        if (temp1.size() > 0)
        {
            Object object = make_object(temp);
            objects.push_back(object);
        }
        return objects;
    }
};