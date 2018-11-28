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
        int bin = string_to_decimal(str);

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
void newStateTable(vector<string> rest, vector<vector<string> > state_table, vector<vector<string> >& new_state_table)
{
    for (int i = 0; i < rest.size(); i++)
    {
        int position = position_in_state_table(rest[i], state_table);
        new_state_table.push_back(state_table[position]);
    }
}
void next_state_deal(vector<set<string> > second_class, vector<vector<string> >& new_state_table)
{
    for (int i = 0; i < new_state_table.size(); i++)
    {
        int bound = getNextStateBound(new_state_table);
        for (int j = 1; j < bound; j++)
        {
            int position = is_in(new_state_table[i][j], second_class);
            set<string>::iterator it = second_class[position].begin();
            new_state_table[i][j] = *it;
        }
    }
}
void kissOutput(fstream& out, int i, int o, vector<string> rest, vector<vector<string> > state_table)
{
    out << ".start_kiss\n";
    out << ".i " << i << "\n";
    out << ".o " << o << "\n";
    out << ".p " << rest.size() * pow(2, i) << "\n";
    out << ".s " << rest.size() << "\n";
    out << ".r " << rest[0] << "\n";
    // state transition part
    int next_state_end = getNextStateBound(state_table);

    for (int j = 0; j < rest.size(); j++)
    {
        int position = position_in_state_table(rest[j], state_table);

        for (int k = 1; k < next_state_end; k++)
        {
            out << dec2str(k - 1, i) << " ";
            out << state_table[position][0] << " ";
            if (!is_in(state_table[position][k], rest) )
                out << state_table[position][0];
            else
                out << state_table[position][k];
            out << " ";
            out << state_table[position][k - 1 + next_state_end] << "\n";
        }
    }

    out << ".end_kiss\n";
}
void dotOutput(fstream& out, int i, vector<vector<string> > state_table)
{
    out << "digraph STG {\n";
    out << "\trankdir=LR;\n";
    out << "\t\n";
    out << "\tINIT [shape=point];\n";
    for (int a = 0; a < state_table.size(); a++)
    {
        out << "\t" << state_table[a][0] << " [label=\"" << state_table[a][0] << "\"];\n";
    }
    out << "\t\n";
    out << "\tINIT -> " << state_table[0][0] << ";\n";

    vector<vector<string> > trans;
    int bound = getNextStateBound(state_table);
    for (int a = 0; a < state_table.size(); a++)
    {
        for (int j = 1; j < bound; j++)
        {
            vector<string> vec;
            vec.push_back(state_table[a][0] );
            vec.push_back(dec2str(j - 1, i) ); 
            vec.push_back(state_table[a][j] );
            vec.push_back(state_table[a][j - 1 + bound] );
            trans.push_back(vec);
        }
    }
    // print_out("Transition:", trans);
    
    for (int a = 0; a < trans.size(); a++)
    {
        for (int j = a + 1; j < trans.size(); j++)
        {
            if (trans[a][0] == trans[j][0] && trans[a][2] == trans[j][2])
            {
                trans[a].push_back(trans[j][1] );
                trans[a].push_back(trans[j][3] );
                vector<vector<string> >::iterator it = trans.begin() + j;
                trans.erase(it);
                j -= 1;
            }
        }
    }
    // print_out("Transition:", trans);

    for (int a = 0; a < trans.size(); a++)
    {
        out << "\t" << trans[a][0] << " -> " << trans[a][2]
            << " [label=\"" << trans[a][1] << "/" << trans[a][3];
        int j = 4;
        while (j < trans[a].size())
        {
            out << ",";
            out << trans[a][j] << "/" << trans[a][j + 1];
            j += 2;
        }
        out << "\"];\n";
    }

    // end of file
    out << "}\n";
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
            int bin = string_to_decimal(str);
            next_state[position][bin] = str2;
            output[position][bin] = str3;
        }

        count++;
    }
    vector<vector<string> > state_table;
    // turn into a table
    turn2tab(state_table, state, next_state, output);

    print_out("State Table:", state_table);

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
    // print_out(rest);

    // deal with the next_state problem since the next_state will changed
    vector<vector<string> > new_state_table;
    newStateTable(rest, state_table, new_state_table);
    // print_out("new_state_table:", new_state_table);
    next_state_deal(second_class, new_state_table);
    print_out("The Rest of States:", new_state_table);

    // start to deal with kiss-format output
    kissOutput(out, i, o, rest, new_state_table);

    // start to deal with dot-format output
    dotOutput(graph, i, new_state_table);

    close(in, out, graph);
    return 0;
}
