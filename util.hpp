/*
    General funcs put in this file
*/

#ifndef LIB_HPP
#define LIB_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <sstream>
using namespace std;

template <typename T>
string to_string(T value)
{
	ostringstream os;
	os << value;
	return os.str();
}

// file I/O open and close
void open(fstream& in, fstream& out, fstream& graph, char** argv)
{
    in.open(argv[1], ios::in);
    out.open(argv[2], ios::out);
    graph.open(argv[3], ios::out);
}
void close(fstream& in, fstream& out, fstream& graph)
{
    in.close();
    out.close();
    graph.close();
}
int position_of_state(vector<string> state, string str)
{
    for (int i = 0; i < state.size(); i++)
    {
        if (state[i] == str) return i;
    }
    return -1;
}
// string to a decimal number
int string_to_decimal(string str)
{
    int num = 0;
    reverse(str.begin(), str.end());
    for (int i = 0, j = 0; i < str.size(); i++, j++)
    {
        num += (int)(str[i] - '0') * pow(2, j);
    }
    return num;
}
// decimal number to a binary number and then turn to a string
string dec2str(int num, int i) // .i var to deal with 0
{
    // decimal to binary
    vector<int> arr(i, 0);
    int j = 0, r;
    
    while (num != 0)
    {
        r = num % 2;
        arr[j++] = r;
        num /= 2;
    }
    reverse(arr.begin(), arr.end());

    string str = "";
    for (j = 0; j < arr.size(); j++)
    {
	str += to_string(arr[j]);
    }
    return str;
}
bool isRepeat(vector<string> vec, string str)
{
    for (int i = 0; i < vec.size(); i++)
        if (vec[i] == str)
            return true;
    return false;
}
// get the current state
string getState(int position, vector<string> state)
{
    return state[position];
}
// get the vector of next states
vector<string> getNextState(string str, vector<string> state, vector<vector<string> > next_state)
{
    int position = position_of_state(state, str);

    return next_state[position];
}
vector<string> getNextState(int position, vector<vector<string> > next_state)
{
    return next_state[position];
}
// get the vector of outputs
vector<string> getOutput(string str, vector<string> state, vector<vector<string> > output)
{
    int position = position_of_state(state, str);

    return output[position];
}
// search the position in the state_table by state_name
int position_in_state_table(string state, vector<vector<string> > state_table)
{
    for (int i = 0; i < state_table.size(); i++)
    {
        if (state_table[i][0] == state)
        {
            return i;
        }
    }
    return -1;
}
// check whether the element is in the set or not and return score
int is_in(string element, vector<set<string> > class_set)
{
    for (int i = 0; i < class_set.size(); i++)
    {
        if (class_set[i].find(element) != class_set[i].end() )
        {
            return i;
        }
    }
    return -1;
}
// check whether the state in rest or not
bool is_in(string str, vector<string> rest)
{
    for (int i = 0; i < rest.size(); i++)
    {
        if (rest[i] == str)
            return true;
    }
    return false;
}
// find min num in an integer vector
int min_val(vector<int> vec)
{
    int min = vec[0];
    for (int i = 1; i < vec.size(); i++)
    {
        if (min > vec[i])
            min = vec[i];
    }
    return min;
}
bool is_digit(string str)
{
    return str.find_first_not_of("0123456789") == string::npos;
}
// get the bound of next states in the state_table
int getNextStateBound(vector<vector<string> > state_table)
{
    int next_state_end = 0;
    for (int i = 1; i < state_table[0].size(); i++)
    {
        if (is_digit(state_table[0][i] ) )
        {
            return i;
        }
    }
    // error return
    return -1;
}
void vec_set2vec(vector<set<string> > vec_set, vector<string>& vec)
{
    // prevent something wrong
    vec.clear();
    // firstly, turn to set since sort can be done
    set<string> temp_set;

    for (int i = 0; i < vec_set.size(); i++)
    {
        set<string>::iterator it = vec_set[i].begin();
        temp_set.insert(*it);
    }
    
    for (set<string>::iterator it = temp_set.begin(); it != temp_set.end(); it++)
    {
        vec.push_back(*it);
    }
}
template <class T>
void print_out(string title, vector<vector<T> > vec)
{
    cout << title << endl;
    // test print out
    for (int temp = 0; temp < vec.size(); temp++)
    {
        for (int temp1 = 0; temp1 < vec[temp].size(); temp1++)
        {
            cout << vec[temp][temp1] << " | ";
        }
        cout << endl;
    }
    cout << endl;
}
void print_out(string title, vector<set<string> > vec)
{
    cout << title << endl;
    // test print out
    for (int temp = 0; temp < vec.size(); temp++)
    {
        for (set<string>::iterator it = vec[temp].begin(); it != vec[temp].end(); it++)
        {
            cout << *it << " | ";
        }
        cout << endl;
    }
    cout << endl;
}
template <class T>
void print_out(vector<T> vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        cout << vec[i] << " | ";
    }
    cout << endl;
}

#endif
