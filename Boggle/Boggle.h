/*
 * Authors: Philip Clark and Susannah Meyer
 * Date: 10/30/15
 * Program Description: Bonus version of Boggle.h modified to include functions for Q bonus.
 */

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
#include "hashset.h"
#include "set.h"
#include "vector.h"

using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col) const;
    bool checkWord(string word) const;
    bool humanWordSearch(string word);
    Set<string> computerWordSearch();
    int getScoreHuman() const;
    int getScoreComputer() const;

    // TODO: add any other member functions/variables necessary
    friend ostream& operator<<(ostream& out, Boggle& boggle);

private:
    int computePosition(int row, int col) const;
    bool humanWordSearchHelper(string word, string formedWord, int row, int col, int pos, HashSet<int>& visited);
    void computerWordSearchHelper(int row, int col, string foundWord, Set<string>& result, HashSet<int>& visited);
    void BoggleToString();
    void makeQUseful();
    void swap(int pos1, int pos2);
    void convertOut(int pos, int& row, int&col);
    Set<int> neighbors(int row, int col);
    Set<int> findAllLetter(char c);
    bool contains(char c);

    Grid<char> board;
    Lexicon boggleDictionary;
    Lexicon humanWordsPlayed;
    Lexicon computerWordsPlayed;
    string boggleString;

    int humanScore;
    int compScore;
};

#endif // _boggle_h
