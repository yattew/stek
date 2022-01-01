enum TYPE
{
    BOOL,
    NUMBER,
    STRING,
    ARRAY,
    _NULL,
    KEYWORD,
    T_VAR,
};
struct Object
{

    TYPE type;
    string data;
    Object()
    {
        type = _NULL;
        data = "null";
    }
};