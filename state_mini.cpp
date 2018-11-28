#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <set>
#include "util.hpp"

using namespace std;

// turn input file to a table
void turn2tab(vector<vector<string> >& state_table, vector<string> state, vector<vector<string> > next_state, vector<vector<string> > output)
{
    int a = 0;
    for (a = 0; a < state.size(); a++)
    {
        vector<string> vec;
        int b = 0;
        vec.push_back(state[a]);
        
        vector<string> temp = getNextState(state[a], state, next_state);
        for (int temp1 = 0; temp1 < temp.size(); temp1++)
        {
            vec.push_back(temp[temp1]);
        }

        temp = getOutput(state[a], state, output);
        for (int temp1 = 0; temp1 < temp.size(); temp1++)
        {
            vec.push_back(temp[temp1]);
        }
        
        state_table.push_back(vec);
    }
}
// classify state by output
void classByOutput(vector<set<string> >& first_class, int o, vector<vector<string> > state_table, vector<string> state)
{
    vector<vector<int> > vec_vec;
    // initialize by the .o variable
    for (int temp = 0; temp < 2 * pow(2, o); temp++)
    {
        vector<int> vec;
        vec_vec.push_back(vec);
    }
    int size = state_table[0].size();
    // traverse all states
    for (int temp = 0; temp < state_table.size(); temp++)
    {
        string str = state_table[temp][size - 2] + state_table[temp][size - 1];
        int bin = string_to_binary(str);

        // store the index
        vec_vec[bin].push_back(temp);
    }

    // index turn to the relative states
    for (int i = 0; i < vec_vec.size(); i++)
    {
        if (vec_vec[i].size() != 0)
        {
            set<string> temp_set;
            for (int j = 0; j < vec_vec[i].size(); j++)
            {
                temp_set.insert(getState(vec_vec[i][j], state) );
            }
            first_class.push_back(temp_set);
        }
    }
}
void partitioning(vector<set<string> >& second_class, vector<set<string> > first_class, vector<vector<string> > state_table, vector<string> state, vector<vector<string> > next_state)
{   
    // To get the next_state from state_table, we get to check the end of next states bound
    int next_state_end = getNextStateBound(state_table);

    // To check whether the state in the same set should be divided
    int base = 0;
    while (base < first_class.size())
    {
        vector<int> score;
        vector<string> relative_state;
        for (set<string>::iterator it = first_class[base].begin(); it != first_class[base].end(); it++)
        {
            int num = 0;
            for (int k = 1; k < next_state_end; k++)
                num += is_in(state_table[position_in_state_table(*it, state_table)][k], first_class);
            score.push_back(num);
            relative_state.push_back(*it);
        }
        int how_many_score = getDiffScore(score);

        if (how_many_score == 1)
            base += 1;
        else
        {
            int offset = min_val(score);

            vector<set<string> > vec_set;
            for (int j = 0; j < how_many_score; j++)
            {
                set<string> temp_set;
                vec_set.push_back(temp_set);
            }

            for (int j = 0; j < relative_state.size(); j++)
            {
                vec_set[score[j] - offset].insert(relative_state[j]);
            }
            for (int j = 0; j < vec_set.size(); j++)
                first_class.push_back(vec_set[j]);
            
            // should delete the "base" element from first_class since it has been divided
            vector<set<string> >::iterator it = first_class.begin() + base;
            first_class.erase(it);
        }
    }

    second_class = first_class;
}

// main func
int main(int argc, char** argv)
{
    fstream in, out, graph;
    // record the state names
    vector<string> state;
    // vecs for next states regarding with inputs
    vector<vector<string> > next_state;
    // record the output
    vector<vector<string> > output;
    // the .s variable standing for how many variables like S1...S7, a...f
    int s = 0;
    // the .i variable standing for how many input bits like 0 and 1, 00, 01, 10, and 11
    int i = 0;
    // the .o variable standing for how many output bits like 0 and 1, 00, 01, 10, and 11
    int o = 0;

    open(in, out, graph, argv);

    int count = 0;
    string str = "";
    string str1 = "";
    string str2 = "";
    string str3 = "";
    while (getline(in, str, '\n'))
    {
        istringstream iss(str);

        if (count == 1)
        {
            iss >> str; iss >> i;
        }
        else if (count == 2)
        {
            iss >> str; iss >> o;
        }
        else if (count == 4)
        {
            iss >> str; iss >> s;
            
            int temp = 0;
            while (temp < s)
            {
                vector<string> vec(pow(2, i), "");
                next_state.push_back(vec);

                vector<string> vec1(pow(2, i), "");
                output.push_back(vec1);
                temp++;
            }
        }
        else if (str.find(".") == string::npos)
        {
            iss >> str; iss >> str1; iss >> str2; iss >> str3;
            if (!isRepeat(state, str1))
                state.push_back(str1);

            int position = position_of_state(state, str1);
            int bin = string_to_binary(str);
            next_state[position][bin] = str2;
            output[position][bin] = str3;
        }

        count++;
    }
    vector<vector<string> > state_table;
    // turn into a table
    turn2tab(state_table, state, next_state, output);

    print_out("state_table:", state_table);

    // first classification by output
    vector<set<string> > first_class;
    classByOutput(first_class, o, state_table, state);

    print_out("First Divided:", first_class);

    // successive partitioning by next state transition
    vector<set<string> > second_class;
    partitioning(second_class, first_class, state_table, state, next_state);

    print_out("Second Partitioning:", second_class);

    // the rest of the states
    vector<string> rest;
    vec_set2vec(second_class, rest);

    print_out(rest);

    // start to deal with kiss format output
    

    close(in, out, graph);
    return 0;
}