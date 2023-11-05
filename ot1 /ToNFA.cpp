
#include<iostream>
#include<CString>
#include<stack>
#include<set>
#include<vector>
using namespace std;

struct NFA
{
    set<int> Q;                  //状态集合
    set<char> sigma;             //输入字符集合

    vector<int> delta[128][128]; //状态转化函数,一个状态对应的一个输入可能有两个以上的转移，使用数组不行！！！
    int q0;                      //开始状态
    set<int> F;                  //接收状态的集合
    NFA()                        //构造函数
    {
        //memset(delta, -1, sizeof(delta));//-1表示为此状态没有此输入。
    }
} NFAinstance;

int priority_level(char a) //返回运算符的优先级，优先级定义：闭包（*）>连接符（.）>或运算（|）>括号
{
    if (a == '(' || a == ')')
        return 1;
    else if (a == '|')
        return 2;
    else if (a == '.')
        return 3;
    else if (a == '*')
        return 4;
    else
        return 5;
}

bool is_operand(char a) //判断是否操作数，这里吧数字和字母当做操作数
{
    if (a >= '0' && a <= '9')
        return true;
    if (a >= 'a' && a <= 'z')
        return true;
    if (a >= 'A' && a <= 'Z')
        return true;
    return false;
}

stack<int> st_begin; //开始状态
stack<int> st_end;   //结束状态
/*
struct Node
{
    Node(int id, char input, int nextId) //构造函数
    {
        this->id = id;
        this->input = input;
        this->nextId = nextId;
    }
    int id;
    char input;
    int nextId;
};*/



//vector<Node*> graph;//存放NFA里的每条路径



void strToNFA(string& postfix_expression)
{
    int statusid = 0;

    for (int i = 0; i < postfix_expression.size(); i++)
    {
        if (is_operand(postfix_expression[i]))
        {
            NFAinstance.sigma.insert(postfix_expression[i]); //如果是操作数，加入输入字符集合中
            int pre = statusid;
            NFAinstance.Q.insert(statusid); //加入状态集合
            st_begin.push(statusid++);
            int nxt = statusid;
            NFAinstance.Q.insert(statusid);
            st_end.push(statusid++);
            NFAinstance.delta[pre][postfix_expression[i]].push_back(nxt); //加入状态转移
            //graph.push_back(new Node(pre, postfix_expression[i], nxt));
        }
        else if (postfix_expression[i] == '*')
        {
            int pre = st_end.top();
            //cout<<pre;
            st_end.pop();
            int nxt = st_begin.top();
            st_begin.pop();
            //cout<<nxt;
            //graph.push_back(new Node(pre, '$', nxt));//符号$表示空
            NFAinstance.delta[pre]['$'].push_back(nxt);
            NFAinstance.Q.insert(statusid);
            int pre1 = statusid++;
            NFAinstance.Q.insert(statusid);
            int nxt1 = statusid++;
            NFAinstance.delta[pre1]['$'].push_back(nxt);
            NFAinstance.delta[pre]['$'].push_back(nxt1);
            NFAinstance.delta[pre1]['$'].push_back(nxt1);
            /*graph.push_back(new Node(pre1, '$', nxt));
            graph.push_back(new Node(pre, '$', nxt1));
            graph.push_back(new Node(pre1, '$', nxt1));*/
            st_begin.push(pre1);
            st_end.push(nxt1);
        }
        else if (postfix_expression[i] == '.')
        {
            int pre1 = st_begin.top();
            st_begin.pop();
            int pre2 = st_begin.top();
            st_begin.pop();
            int nxt1 = st_end.top();
            st_end.pop();
            int nxt2 = st_end.top();
            st_end.pop();
            NFAinstance.delta[nxt2]['$'].push_back(pre1);
            //graph.push_back(new Node(nxt2, '$', pre1));
            st_begin.push(pre2);
            st_end.push(nxt1);
        }
        else if (postfix_expression[i] == '|')
        {
            NFAinstance.Q.insert(statusid);
            int pre = statusid++;
            NFAinstance.Q.insert(statusid);
            int nxt = statusid++;
            int pre1 = st_begin.top();
            st_begin.pop();
            int pre2 = st_begin.top();
            st_begin.pop();
            int nxt1 = st_end.top();
            st_end.pop();
            int nxt2 = st_end.top();
            st_end.pop();
            NFAinstance.delta[pre]['$'].push_back(pre1);
            NFAinstance.delta[pre]['$'].push_back(pre2);
            NFAinstance.delta[nxt1]['$'].push_back(nxt);
            NFAinstance.delta[nxt2]['$'].push_back(nxt);
            /*graph.push_back(new Node(pre, '$', pre1));
            graph.push_back(new Node(pre, '$', pre2));
            graph.push_back(new Node(nxt1, '$', nxt));
            graph.push_back(new Node(nxt2, '$', nxt));*/
            st_begin.push(pre);
            st_end.push(nxt);
        }
    }
    NFAinstance.q0 = st_begin.top();    //初始状态
    NFAinstance.F.insert(st_end.top()); //接收状态
    NFAinstance.sigma.insert('$');      //输入为空的转化
} //运行结束之后，两个栈中只存有整体的开始和结束状态

//  0((0))-->|$| 1((1))
/*void printNFA()
{
    cout<<"graph LR"<<'\n';
    for(auto node: graph)
    {
        if(node->id == st_begin.top())
            cout<<node->id<<"((BEG))"<<"-->|"<<node->input<<"| "<<node->nextId<<"(("<<node->nextId<<"))"<<'\n';
        else if(node->nextId == st_end.top())
            cout<<node->id<<"(("<<node->id<<"))"<<"-->|"<<node->input<<"| "<<node->nextId<<"((END))"<<'\n';
        else
            cout<<node->id<<"(("<<node->id<<"))"<<"-->|"<<node->input<<"| "<<node->nextId<<"(("<<node->nextId<<"))"<<'\n';
    }
}*/

void printNFA()
{
    cout << "graph LR" << '\n';
    for (auto it = NFAinstance.Q.begin(); it != NFAinstance.Q.end(); it++) //状态
    {
        for (auto it1 = NFAinstance.sigma.begin(); it1 != NFAinstance.sigma.end(); it1++)
        {
            if (!NFAinstance.delta[*it][*it1].empty())
            {
                for (auto it2 = NFAinstance.delta[*it][*it1].begin(); it2 != NFAinstance.delta[*it][*it1].end(); it2++)
                {
                    if (NFAinstance.q0 == *it) //开始状态
                    {
                        cout << *it << "((BEG))"
                            << "-->|" << *it1 << "| " << *it2 << "((" << *it2 << "))" << '\n';
                    }
                    else if (NFAinstance.F.find(*it2) != NFAinstance.F.end()) //结束状态
                    {
                        cout << *it << "((" << *it << "))"
                            << "-->|" << *it1 << "| " << *it2 << "((END))" << '\n';
                    }
                    else
                    {
                        cout << *it << "((" << *it << "))"
                            << "-->|" << *it1 << "| " << *it2 << "((" << *it2 << "))" << '\n';
                    }
                }
            }
        }
    }
}


/*使用a、b、c代表运算数，则下面的情况之间会省略连接符：
*a *( ab a( )(
*/

string infixTopostfix(string& infix_expression) //把中缀转化为后缀，并且补上在中缀中省略的连接符
{
    char temp[255] = {};
    int j = 0;
    stack<char> st;
    for (int i = 0; i < infix_expression.size(); i++)
    {
        if (is_operand(infix_expression[i]))
        {
            temp[j++] = infix_expression[i];
        }
        else
        {
            if (st.empty())
            {
                st.push(infix_expression[i]);
            }
            else
            {
                char a = st.top();
                if (priority_level(infix_expression[i]) <= priority_level(a))
                {
                    while (priority_level(a) >= priority_level(infix_expression[i]))
                    {
                        if (a != '(' && a != ')')
                            temp[j++] = a;
                        st.pop();
                        if (st.empty())
                            break;
                        else
                            a = st.top();
                    }
                    st.push(infix_expression[i]);
                }
                else
                {
                    st.push(infix_expression[i]);
                }
            }
        }
        if (i < infix_expression.size() - 1) //判断下一个符号是不是连接符
            if ((is_operand(infix_expression[i]) && infix_expression[i + 1] == '(') ||
                (is_operand(infix_expression[i]) && is_operand(infix_expression[i + 1])) ||
                (infix_expression[i] == '*' && is_operand(infix_expression[i + 1])) ||
                (infix_expression[i] == '*' && infix_expression[i + 1] == '(') ||
                (infix_expression[i] == ')' && infix_expression[i + 1] == '('))
            {
                if (st.empty())
                {
                    st.push('.');
                }
                else
                {
                    char a = st.top();
                    if (priority_level('.') <= priority_level(a))
                    {
                        while (priority_level(a) >= priority_level('.'))
                        {
                            if (a != '(' && a != ')')
                                temp[j++] = a;
                            st.pop();
                            if (st.empty())
                                break;
                            else
                                a = st.top();
                        }
                        st.push('.');
                    }
                    else
                    {
                        st.push('.');
                    }
                }
            }
    }
    while (!st.empty())
    {
        if (st.top() != '(' && st.top() != ')')
            temp[j++] = st.top();
        st.pop();
    }
    string temp0(temp);
    return temp0;
}

int main()
{
    string infix_expression;
    cin >> infix_expression; //输入中缀表达式
    string postfix_expression;
    postfix_expression = infixTopostfix(infix_expression);
    strToNFA(postfix_expression);
    printNFA();
   

    system("pause");
}
