enum TYPE
{
    BOOL,
    NUMBER,
    STRING,
    ARRAY,
    _NULL,
    KEYWORD,
};
struct Object
{

    TYPE type;
    string data;
    Object()
    {
        type = _NULL;
        data = "";
    }
};