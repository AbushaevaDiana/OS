#include "consts.h"

using namespace std;

bool MakeToken(string& word, int indexStr, deque<string>& vectorOfStringAndChar, ofstream& outputFile)
{
    if (word == "")
    {
        return false;
    }

    for (auto& item : separatorsDouble)
    {
        size_t index = word.find(item.first);
        if (index != string::npos)
        {
            word.erase(index, item.first.size());
            string secondStr = word.substr(index, word.size() - index);
            word = word.substr(0, index);
            MakeToken(word, indexStr, vectorOfStringAndChar, outputFile);
            outputFile << item.first << " --- " << item.second << " --- line " << indexStr << "\n";
            MakeToken(secondStr, indexStr, vectorOfStringAndChar, outputFile);
            return true;
        }
    }

    for (auto& item: separators)
    {
        size_t index = word.find(item.first);
        if (index != string::npos)
        {
            word.erase(index, item.first.size());
            string secondStr = word.substr(index, word.size() - index);
            word = word.substr(0, index);
            MakeToken(word, indexStr, vectorOfStringAndChar, outputFile);
            outputFile << item.first << " --- " << item.second << " --- line " << indexStr << "\n";
            MakeToken(secondStr, indexStr, vectorOfStringAndChar, outputFile);
            return true;
        }
    }
    //numbers
    if (numbers.find(word[0]) != string::npos)
    {
        bool isPoint = false;
        bool isSecondPoint = false;
        for (auto i = 1; i < word.size(); i++)
        {
            if (isPoint && word[i] == '.')
            {
                isSecondPoint = true;
            }
            if (word[i] == '.')
            {
                isPoint = true;
            }
            if ((numbers.find(word[i]) == string::npos) || word[0] == '.' || word[word.size() - 1] == '.' || isSecondPoint)
            {
                outputFile << word << errorOutputStr << "line " << indexStr << "\n";
                return true;
            }
        }
        outputFile << word << " --- Number --- line " << indexStr << "\n";
        return true;
    }

    string output;
    if (keywords.find(word) != keywords.end())
    {
        output = keywords[word];
        outputFile << word << " --- " << output << " --- line " << indexStr << "\n";
        return true;
    }


    if (separators.find(word) != separators.end())
    {
        output = separators[word];
        outputFile << word << " --- " << output << " --- line " << indexStr << "\n";
        return true;
    }

    if (word == "\"" && !vectorOfStringAndChar.empty())
    {
        word = vectorOfStringAndChar[0];
        vectorOfStringAndChar.pop_front();
        size_t i = word.find("\"") + 1;
        if (i < word.size() && word.find("\"", i) != string::npos)
        {
            outputFile << word << " --- String --- line " << indexStr << "\n";
        }
        else
        {
            outputFile << word << errorOutputStr << indexStr << "\n";
        }
        return true;
    }

    if (word == "\'" && !vectorOfStringAndChar.empty())
    {
        word = vectorOfStringAndChar[0];
        vectorOfStringAndChar.pop_front();
        size_t i = word.find("\'") + 1;
        if (i < word.size() && word.find("\'", i) != string::npos && word.size() == 3)
        {
            outputFile << word << " --- Char --- line " << indexStr << "\n";
        }
        else
        {
            outputFile << word << errorOutputStr << indexStr << "\n";
        }
        return true;
    }

    if (isalpha(word[0]) || word[0] == '_')
    {
        for (auto i = 1; i < word.size(); i++)
        {
            if (!isalpha(word[i]) && word[i] != '_' && (word[i] < '0' || word[i] > '9'))
            {
                outputFile << word << errorOutputStr << indexStr << "\n";
                return true;
            }
        }
        outputFile << word << " --- Identifier --- line " << indexStr << "\n";
        return true;
    }

    outputFile << word << errorOutputStr << indexStr << "\n";
}

void processStringAndChar(deque<string>& vectorOfStringAndChar, string& inputStr, size_t indexStart)
{
    if (inputStr.find("\"", indexStart) != string::npos)
    {
        string str;
        size_t start = inputStr.find("\"", indexStart);
        size_t end = inputStr.size() - 1;
        if (start + 1 < inputStr.size() && inputStr.find("\"", start + 1) != string::npos)
        {
            end = inputStr.find("\"", start + 1);
        }
        str = inputStr.substr(start, end - start + 1);
        vectorOfStringAndChar.push_back(str);
        inputStr.erase(start + 1, end - start);

        processStringAndChar(vectorOfStringAndChar, inputStr, start + 1);
    }
    if (inputStr.find("\'", indexStart) != string::npos)
    {
        string str;
        size_t start = inputStr.find("\'", indexStart);
        size_t end = inputStr.size() - 1;
        if (start + 1 < inputStr.size() && inputStr.find("\'", start + 1) != string::npos)
        {
            end = inputStr.find("\'", start + 1);
        }
        str = inputStr.substr(start, end - start + 1);
        vectorOfStringAndChar.push_back(str);
        inputStr.erase(start + 1, end - start);

        processStringAndChar(vectorOfStringAndChar, inputStr, start + 1);
    }
}

void ProcessStr(string& inputStr, int indexStr, ofstream& outputFile)
{
    int commentsIndex = 0;
    if ((commentsIndex = inputStr.find("//", commentsIndex)) != string::npos)
    {
        string comment = inputStr.substr(commentsIndex, inputStr.size() - commentsIndex);
        inputStr = inputStr.substr(0, commentsIndex);
        outputFile << comment << " --- Comment --- line " << indexStr << "\n";
    }
    deque<string> vectorOfStringAndChar;
    processStringAndChar(vectorOfStringAndChar, inputStr, 0);
    if (!inputStr.empty())
    {
        vector<string> words;
        string item;
        istringstream strStream(inputStr);
        while (getline(strStream, item, ' '))
        {
            words.push_back(item);
        }
        for (auto i = 0; i < words.size(); i++)
        {
            MakeToken(words[i], indexStr, vectorOfStringAndChar, outputFile);
        }
    }
}

void Process(ifstream& input, ofstream& output)
{
    string inputStr;
    int indexStr = 0;
    while (getline(input, inputStr))
    {
        indexStr++;
        if (inputStr.empty())
        {
            continue;
        }
        ProcessStr(inputStr, indexStr, output);
    }
}

int main(int args, char* arsv[])
{
    ifstream inputFile(arsv[1]);
    ofstream outputFile(arsv[2]);
    Process(inputFile, outputFile);
}
