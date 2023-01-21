#include <iostream>
#include "ReadWriteTable.h"
#include <map>
#include <set>

using namespace std;

struct State
{
    string name;
    vector<string> transitions;
    vector<string> outputSimvols;
    string firstName;
    vector<string> firstTransitions;
};

bool FirstProcessMealy(vector<vector<string>>& inputAutomaton, vector<State>& inputStates)
{
    if (inputAutomaton.size() < 2)
    {
        return false;
    }
    if (inputAutomaton[0].size() < 2)
    {
        return false;
    }
    set<string> reachableStates;
    for (auto i = 1; i < inputAutomaton[0].size(); i++)
    {
        State state;
        state.name = inputAutomaton[0][i];
        for (auto j = 1; j < inputAutomaton.size(); j++)
        {
            size_t index = inputAutomaton[j][i].find("/");
            string newState = inputAutomaton[j][i].substr(0, index);
            string outputSimvol = inputAutomaton[j][i].substr(index + 1, inputAutomaton[j][i].size() - 1 - index);
            state.outputSimvols.push_back(outputSimvol);
            state.transitions.push_back(newState);
            if (newState != state.name)
            {
                reachableStates.insert(newState);
            }
        }
        state.firstName = state.name;
        state.firstTransitions = state.transitions;
        inputStates.push_back(state);
    }
    for (auto i = 0; i < inputStates.size(); i++)
    {
        if (reachableStates.find(inputStates[i].name) == reachableStates.end())
        {
            inputStates.erase(inputStates.begin() + i);
            i -= 1;
        }
    }
    return true;
}

bool FirstProcessMoore(vector<vector<string>>& inputAutomaton, vector<State>& inputStates)
{
    if (inputAutomaton.size() < 3)
    {
        return false;
    }
    if (inputAutomaton[0].size() < 2)
    {
        return false;
    }
    set<string> reachableStates;
    for (auto i = 1; i < inputAutomaton[0].size(); i++)
    {
        State state;
        state.name = inputAutomaton[1][i];
        for (auto j = 2; j < inputAutomaton.size(); j++)
        {
            string newState = inputAutomaton[j][i];
            string outputSimvol = inputAutomaton[0][i];
            state.outputSimvols.push_back(outputSimvol);
            state.transitions.push_back(newState);
            if (newState != state.name)
            {
                reachableStates.insert(newState);
            }
        }
        state.firstName = state.name;
        state.firstTransitions = state.transitions;
        inputStates.push_back(state);
    }
    for (auto i = 0; i < inputStates.size(); i++)
    {
        if (reachableStates.find(inputStates[i].name) == reachableStates.end())
        {
            inputStates.erase(inputStates.begin() + i);
            i -= 1;
        }
    }
    return true;
}

bool ProcessAutomatonFirst(vector<State>& processStates, vector<State>& outputStates, size_t& countOfStates)
{
    vector<vector<State>> newStates;
    bool notFound = true;
    map<string, string> dict;
    for (auto i = 0; i < processStates.size(); i++)
    {
        notFound = true;
        for (auto j = 0; j < newStates.size(); j++)
        {
            if (processStates[i].outputSimvols == newStates[j][0].outputSimvols)
            {
                notFound = false;
                newStates[j].push_back(processStates[i]);
                string newName = "S" + to_string(j);
                dict.insert(make_pair(processStates[i].firstName, newName));
            }
        }
        if (notFound)
        {
            vector<State> vr;
            vr.push_back(processStates[i]);
            newStates.push_back(vr);
            string newName = "S" + to_string(newStates.size()-1);
            dict.insert(make_pair(processStates[i].firstName, newName));
            notFound = false;
            continue;
        }
    }

    if (countOfStates == newStates.size())
    {
        return true;
    }

    for (auto i = 0; i < processStates.size(); i++)
    {
        State newState;
        newState.name = dict[processStates[i].firstName];
        newState.outputSimvols = processStates[i].outputSimvols;
        newState.firstName = processStates[i].firstName;
        newState.firstTransitions = processStates[i].firstTransitions;
        outputStates.push_back(newState);
        for (auto j = 0; j < outputStates[i].outputSimvols.size(); j++)
        {
            outputStates[i].transitions.push_back(dict[processStates[i].firstTransitions[j]]);
        }
    }

    countOfStates = newStates.size();
    return false;
}

bool ProcessAutomaton(vector<State>& processStates, vector<State>& outputStates, size_t& countOfStates)
{
    vector<vector<State>> newStates;
    bool notFound = true;
    map<string, string> dict;
    for (auto i = 0; i < processStates.size(); i++)
    {
        notFound = true;
        for (auto j = 0; j < newStates.size(); j++)
        {
            if ((processStates[i].transitions == newStates[j][0].transitions) && (processStates[i].outputSimvols == newStates[j][0].outputSimvols))
            {
                notFound = false;
                newStates[j].push_back(processStates[i]);
                string newName = "S" + to_string(j);
                dict.insert(make_pair(processStates[i].firstName, newName));
            }
        }
        if (notFound)
        {
            vector<State> vr;
            vr.push_back(processStates[i]);
            newStates.push_back(vr);
            string newName = "S" + to_string(newStates.size() - 1);
            dict.insert(make_pair(processStates[i].firstName, newName));
        }
    }

    if (countOfStates == newStates.size())
    {
        return true;
    }

    for (auto i = 0; i < processStates.size(); i++)
    {
        State newState;
        newState.name = dict[processStates[i].firstName];
        newState.outputSimvols = processStates[i].outputSimvols;
        newState.firstName = processStates[i].firstName;
        newState.firstTransitions = processStates[i].firstTransitions;
        outputStates.push_back(newState);
        for (auto j = 0; j < outputStates[i].outputSimvols.size(); j++)
        {
            outputStates[i].transitions.push_back(dict[processStates[i].firstTransitions[j]]);
        }

    }
    countOfStates = newStates.size();
    processStates.clear();

    return false;
}

void MakeOutputMealy(vector<State>& outputStates, vector<vector<string>>& outputAutomaton, vector<vector<string>>& inputAutomaton)
{
    for (auto i = 0; i < inputAutomaton.size(); i++)
    {
        vector<string> vr;
        vr.push_back(inputAutomaton[i][0]);
        outputAutomaton.push_back(vr);
    }
    set<string> thisStateThereis;
    for (auto i = 0; i < outputStates.size(); i++)
    {
        if (thisStateThereis.find(outputStates[i].name) != thisStateThereis.end())
        {
            continue;
        }
        thisStateThereis.insert(outputStates[i].name);
        outputAutomaton[0].push_back(outputStates[i].name);
        for (auto j = 1; j < outputAutomaton.size(); j++)
        {
            string str = outputStates[i].transitions[j-1] + "/" + outputStates[i].outputSimvols[j-1];
            outputAutomaton[j].push_back(str);
        }
    }
}

void MakeOutputMoore(vector<State>& outputStates, vector<vector<string>>& outputAutomaton, vector<vector<string>>& inputAutomaton)
{
    for (auto i = 0; i < inputAutomaton.size(); i++)
    {
        vector<string> vr;
        vr.push_back(inputAutomaton[i][0]);
        outputAutomaton.push_back(vr);
    }
    set<string> thisStateThereis;
    for (auto i = 0; i < outputStates.size(); i++)
    {
        if (thisStateThereis.find(outputStates[i].name) != thisStateThereis.end())
        {
            continue;
        }
        thisStateThereis.insert(outputStates[i].name);
        outputAutomaton[1].push_back(outputStates[i].name);
        outputAutomaton[0].push_back(outputStates[i].outputSimvols[0]);
        for (auto j = 2; j < outputAutomaton.size(); j++)
        {
            outputAutomaton[j].push_back(outputStates[i].transitions[j-2]);
        }
    }
}

void Minimization(vector<State>& inputStates, vector<State>& outputStates)
{
    size_t countOfStates = inputStates.size();
    if (ProcessAutomatonFirst(inputStates, outputStates, countOfStates))
    {
        outputStates = inputStates;
        return;
    }
    vector<State> processStates;
    bool end = false;
   
    while (!end)
    {
        processStates.clear();
        processStates = outputStates;
        outputStates.clear();
        end = ProcessAutomaton(processStates, outputStates, countOfStates);
    }
    
    outputStates = processStates;
   
    //cout << outputStates.size();
    return;
}

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cout << "Invalid input format";
        return 0;
    }

    std::string type = argv[1];
    std::string file = argv[2];
    std::ifstream fileIn;
    fileIn.open(file);
    file = argv[3];
    std::ofstream fileOut;
    fileOut.open(file);

    if (!(fileIn.is_open() && fileOut.is_open()))
    {
        cerr << "Error open files!\n";
        return 1;
    }
    if (type != "mealy" && type != "moore")
    {
        cerr << "You need to enter type of automaton: mealy or moore\n";
        return 1;
    }

    vector<vector<string>> inputAutomaton;
    vector<vector<string>> outputAutomaton;
    ReadTable(fileIn, inputAutomaton);
    vector<State> inputStates;
    vector<State> outputStates;

    bool wasNotError = false;
    if (type == "mealy")
    {
        wasNotError = FirstProcessMealy(inputAutomaton, inputStates);
    }
    if (type == "moore")
    {
        wasNotError = FirstProcessMoore(inputAutomaton, inputStates); 
    }
    if (!wasNotError)
    {
        cerr << "Error automaton!\n";
        return 1;
    }
    Minimization(inputStates, outputStates);
    if (type == "mealy")
    {
        MakeOutputMealy(outputStates, outputAutomaton, inputAutomaton);
    }
    if (type == "moore")
    {
        MakeOutputMoore(outputStates, outputAutomaton, inputAutomaton);
    }
    WriteTable(fileOut, outputAutomaton);

    fileIn.close();
    fileOut.close();
    return 0;
}

