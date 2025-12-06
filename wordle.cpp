#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void wordleHelper(const std::string& pattern, const std::set<std::string>& dict,
                  std::set<std::string>& results,
                  std::string current, std::string floating,
                  int index);

int countBlanks(const std::string& pattern, int start);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::set<std::string> results;
    wordleHelper(in, dict, results, "", floating, 0);
    return results;
}

// Define any helper functions here

int countBlanks(const std::string& pattern, int start)
{
    int blanks = 0;
    for(int i = start; i <(int)pattern.size(); i++){
        if(pattern[i] == '-'){
            blanks++;
        }
    }
    return blanks;
}

void wordleHelper(const std::string& pattern, const std::set<std::string>& dict,
                  std::set<std::string>& results,
                  std::string current, std::string floating,
                  int index)
{
    int n = (int)pattern.size();

    //all characters are placed
    if(index == n){
        //only valid if all floating letter used
        if(floating.empty() && dict.find(current) != dict.end()){
            results.insert(current);
        }
        return;
    }

    //not enough '-' left to place all floating letters
    int blanks = countBlanks(pattern, index);
    if((int)floating.size() > blanks){
        return;
    }

    //fixed letter at position
    if(pattern[index] != '-'){
        std::string nextCurrent = current;
        nextCurrent.push_back(pattern[index]);
        wordleHelper(pattern, dict, results, nextCurrent, floating, index + 1);
        return;
    }

    //blank position, try all letters a-z
    for(char c = 'a'; c <= 'z'; c++){
        std::string nextCurrent = current;
        nextCurrent.push_back(c);

        std::string nextFloating = floating;
        int pos = -1;
        for(int i = 0; i < (int)nextFloating.size(); i++){
            if(nextFloating[i] == c){
                pos = i;
                break;
            }
        }
        if(pos != -1){
            nextFloating.erase(pos, 1);
        }
        wordleHelper(pattern, dict, results, nextCurrent, nextFloating, index + 1);
    }
}