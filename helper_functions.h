using namespace std;
bool is_number(string s)
{
    std::size_t char_pos(0);
    char_pos = s.find_first_not_of(' ');
    if (char_pos == s.size()) return false;
    if (s[char_pos] == '+' || s[char_pos] == '-') ++char_pos;
    int n_nm, n_pt;
    for (n_nm = 0, n_pt = 0; std::isdigit(s[char_pos]) || s[char_pos] == '.'; ++char_pos) {
        s[char_pos] == '.' ? ++n_pt : ++n_nm;
    }
    if (n_pt>1 || n_nm<1)
        return false;
    while (s[char_pos] == ' ') {
        ++ char_pos;
    }
    return char_pos == s.size();
}