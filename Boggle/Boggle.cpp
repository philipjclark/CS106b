/*
 * Authors: Philip Clark and Susannah Meyer
 * Date: 10/30/15
 * Program Description: Bonus version of the program, modified to make Q
   a useful letter and to set the Boggle board to a size of 5x5 instead of 4x4.
 */

#include "Boggle.h"
#include "grid.h"
#include "shuffle.h"
#include "random.h"
#include "map.h"
#include "hashset.h"
#include "set.h"
#include "bogglegui.h"
#include "string.h"
#include "random.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
//static string BIG_BOGGLE_CUBES[25] = {
//    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
//    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
//    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
//    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
//    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
//};

//modified to test Q bonus
static string BIG_BOGGLE_CUBES[25] = {
    "QQQQQQ", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "QQQQQQ", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};


/*
 * The boggle constructor intializes an object, sets several of the boggle classes fields, runs the BoggleToString method (described below)
 * and either creates a random board or takes the user's input string as the board text.
 */
Boggle::Boggle(Lexicon& dictionary, string boardText) {
    humanScore = 0;
    compScore = 0;
    humanWordsPlayed.clear();
    computerWordsPlayed.clear();

    boggleDictionary = dictionary;

    board.resize(5, 5);

    int count = 0;
    if(boardText.length()==0) { //user has chosen random board
        count = 0;
        for (int i = 0; i < board.numRows(); i++) {
            for (int j = 0; j < board.numCols(); ++j) {
                int rand = randomInteger(0, 5);
                board[i][j] = BIG_BOGGLE_CUBES[count][rand];
                count++;
            }
        }
        shuffle(board);
        makeQUseful(); //bonus

    } else { //take user's string
        for (int i = 0; i < board.numRows(); i++) {
            for (int j = 0; j < board.numCols(); ++j) {
                board[i][j] = boardText[count];
                count++;
            }
        }
    }

    BoggleToString(); //creates a board string that can be easily printed
}

/*
 * Bonus function which draws on a number of helper functions (convertOut, contains, findAllLetter, neighbors) to make the q useful on the board.
 * If there is a Q, but no U on the board, the function adds a U to the board to make the rest of the function make sense. Once there is at least one Q
 * and one U on the board, the function creates n pairs of QU for n = min(number of Qs, number of Us).
 */
void Boggle:: makeQUseful() {

    Set<int> qPos = findAllLetter('Q'); //finds the positions of all occurences of Q on the board
    Set<int> availableU = findAllLetter('U'); //finds all Us
    int size = qPos.size(); //makes sure that the loop doesn't go out of bounds

    for (int i = 0; i < size; ++i) { //runs through appropriate pair possibilities
        int row = 0;
        int col = 0;
        convertOut(qPos.first(), row, col);
        Set<int> qNeighbors = neighbors(row, col);
        if((qNeighbors*availableU).isEmpty()) { //sees if the neighbors of the Q being examined contain a U
            int rand = randomInteger(0, qNeighbors.size()-1);
            for (int r = 0; r < rand; ++r) { //walks through a random amount of neighbors to swap from
                qNeighbors.remove(qNeighbors.first());
            }
            if(availableU.size()>0) //if there's a u to swap
            {
                swap(qNeighbors.first(), availableU.first()); //swaps
                availableU.remove(availableU.first());
            } else { //othereise make a U
                convertOut(qNeighbors.first(), row, col);
                board[row][col] = 'U';
            }
        }
        qPos.remove(qPos.first());
    }
}

/*
 * This function simply loops through the board, returning true if the char is there and false if it is not.
 * @c: letter being check if the board contains it
 */
bool Boggle:: contains(char c) {
    for (int i = 0; i < board.numRows(); ++i) {
        for (int j = 0; j < board.numCols(); ++j) {
            if(board[i][j] == c) return true;
        }
    }
    return false;
}

/*
 * Swaps two letters on the board by converting their board position (0-15) into row, col format.
 * @pos1: one of the positions being swapped
 * @pos2: the other position
 */
void Boggle:: swap(int pos1, int pos2) {
    int row = 0;
    int col = 0;
    convertOut(pos1, row, col);
    char temp = board[row][col];

    int row2 =0;
    int col2 = 0;
    convertOut(pos2, row2, col2);
    board[row][col] = board[row2][col2];
    board[row2][col2] = temp;
}

/*
 * Takes a board position (0-24) and modifies the reference passed row and col parameters to the corresponding value of the board index.
 * @pos: the board position
 * @row: the row of the position
 * @col: the col of the position
 */
void Boggle:: convertOut(int pos, int& row, int&col) {
    col = pos%5;
    row = pos/5;
}

/*
 * Returns a set of all the neighbors of a letter on the board.
 * @row: the letter in question's row
 * @col: the letter's col
 */
Set<int> Boggle:: neighbors(int row, int col) {
    Set<int> neighbors;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if(board.inBounds(row+i, col+j) && !(i==0 && j==0)) { //checks to make sure the potential neighbor is in bounds
                neighbors.add(computePosition(row+i, col+j)); //adds the position (0-15) to the set
            }
        }
    }
    return neighbors;
}

/*
 * Returns a set of all the positions at which a letter appears on the board.
 * @c: the letter being checked
 */
Set<int> Boggle:: findAllLetter(char c) {
    Set<int> occurences;
    for (int i = 0; i < board.numRows(); ++i) {
        for (int j = 0; j < board.numCols(); ++j) {
            if(board[i][j] == c) occurences.add(computePosition(i, j));
        }
    }
    return occurences;
}

/*
 * Converts the boggle board to a string that can be later used for the overloading print function.
 */
void Boggle:: BoggleToString() {
    for (int i = 0; i < board.numRows(); ++i) {
        for (int j = 0; j < board.numCols(); ++j) {
            boggleString += board[i][j];
        }
        boggleString +="\n";
    }
}

/*
 * Takes a row, col pair within the Boggle board and returns the letter at that position
 * @row: row of char
 * @col: col of char
 */
char Boggle::getLetter(int row, int col) const {
    return board[row][col];
}

bool Boggle::checkWord(string word) const {
    if(word.length() >= 4 && boggleDictionary.contains(word) && !(humanWordsPlayed.contains(word))) {
        return true;
    }
    return false;
}

/*
 * Takes a row, col pair and uses it to calculate the numeric position of that it corresponds to on the board (0-24).
 * Useful for storing whether certain board positions have already been used for something.
 * @row: row of letter
 * @col: col of letter
 */
int Boggle:: computePosition(int row, int col) const {
    return 5*row+col;
}

/*
 * The helper function to see if the human word can be formed on the board which relies on recursive backtracking.
 * For every char in the string being searched (already been confirmed to be suitable), the function checks
 * if any neighbors which have not been used in this branch of the function are the next letter in the word.
 * If so, the recursion continues, otherwise, it fails fast. The recursion returns true if the word
 * can be formed from the letters on the board and false if it cannot.
 * @word: the word being checked if it can be formed
 * @formedWord: the word formed so far by the recursive branch
 * @row: the row of the last letter used whose neighbors are being checked
 * @col: the col of the above letter
 * @pos: the index the letter occurs at
 * @visited: the set of all the letters on the board visited (stored by their position)
 */
bool Boggle:: humanWordSearchHelper(string word, string formedWord, int row, int col, int pos, HashSet<int>& visited) {

    for (int i = -1; i <= 1; ++i) { //loops through possible neighbors
        for (int j = -1; j <= 1; ++j) {
            if(board.inBounds(row+i, col+j) && !(i == 0 && j == 0) && !visited.contains(computePosition(row+i, col+j)) && board[row+i][col+j] == word[pos] && formedWord.length() < word.length()) { //if the neighbor is in bounds, not the original letter, not visited and is the next letter in the human word
                visited.add(computePosition(row+i, col+j)); //marks the now suitable letter as visited (choose)
                BoggleGUI::setHighlighted(row+i, col+j, true);
                if(formedWord + board[row+i][col+j] == word) return true;
                else {
                    if(humanWordSearchHelper(word, formedWord + board[row+i][col+j], row+i, col+j, pos+1, visited)) { //returns if the remainder of the word can be formed (explore)
                        return true;
                    }
                    visited.remove(computePosition(row+i, col+j)); //unchoose
                    BoggleGUI::setHighlighted(row+i, col+j, false);
                }
            }
        }
    }
    return false;

}

bool Boggle::humanWordSearch(string word) {
    HashSet<int> visited;

    if(checkWord(word)) {
        for (int i = 0; i < board.numRows(); ++i) {
            for (int j = 0; j < board.numCols(); ++j) {
                BoggleGUI::setHighlighted(i, j, true);
                if(board[i][j] == word[0]) {
                    string formedWord;
                    formedWord += word[0];
                    int pos = 1;
                    visited.clear();
                    visited.add(computePosition(i, j));
                    if(humanWordSearchHelper(word, formedWord, i, j, pos, visited)) {
                        humanScore+=word.length()-3;
                        humanWordsPlayed.add(word);
                        BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
                        return true;
                    }
                }
                BoggleGUI::setHighlighted(i, j, false);
            }
        }
    }
    return false;
}

/*
 * returns the computer's score so far (can be accessed by the client)
 */
int Boggle::getScoreHuman() const {
    return humanScore;
}

/*
 * The recursive backtracking function for computer word searches functions somewhat analogously to the human word search at this point, now that
 * the main computerWordSearch function. The function will keep branching out to all possible neighbors until the branch is not a prefix for a word
 * in the boggle dictionary.
 * @foundWord: the word formed so far by the recursive branch
 * @row: the row of the last letter used whose neighbors are being checked
 * @col: the col of the above letter
 * @result: the set of all formed words
 * @visited: the set of all letters so far visited in the branch
 */
void Boggle:: computerWordSearchHelper(int row, int col, string foundWord, Set<string>& result, HashSet<int>& visited) {
    for (int i = -1; i <= 1; ++i) { //loops through possible neighbors
        for (int j = -1; j <= 1; ++j) {
            if(board.inBounds(row+i, col+j)) { //checks if neighbor is in bounds
                string newWord = foundWord + board[row+i][col+j]; //adds to new branch (choose)
                if(boggleDictionary.containsPrefix(newWord) && !visited.contains(computePosition(row+i, col+j))) { //makes sure the word is valid and the position unused in the branch
                    if(boggleDictionary.contains(newWord) && !humanWordsPlayed.contains(newWord) && newWord.length() >=4 && !computerWordsPlayed.contains(newWord)) { //sees if the word is suitable
                        result.add(newWord); //adds the new word to the result set
                        compScore+= newWord.length()-3; //computes the change in score
                        computerWordsPlayed.add(newWord);
                        BoggleGUI::recordWord(newWord, BoggleGUI::COMPUTER);
                    }
                    visited.add(computePosition(row+i, col+j));
                    computerWordSearchHelper(row+i, col+j, newWord, result, visited); //recursive call (explore)
                    visited.remove(computePosition(row+i, col+j)); //unchoose
                }
            }
        }
    }
}

/*
 * Used to loop through all board entries and generate recursive backtracking calls on each in order to explore all possible words formed by the board.
 */
Set<string> Boggle::computerWordSearch() {
    Set<string> result;
    HashSet<int> visited; //list of all positions visited
    for (int i = 0; i < board.numRows(); ++i) { //loops through each board letter to use it as a starting point
        for (int j = 0; j < board.numCols(); ++j) {
            string foundWord; //empty string used to store recursive branching
            foundWord += board[i][j]; //adds first letter
            visited.clear(); //wipes previous data
            visited.add(computePosition(i, j));
            computerWordSearchHelper(i, j, foundWord, result, visited); //calls recursive backtracking helper
        }
    }
    return result;
}

/*
 * returns the computer's score so far (can be accessed by the client)
 */
int Boggle::getScoreComputer() const {
    return compScore;
}

/*
 * Overloads the << operator and uses the boggleString to make the boggle object printable
 */
ostream& operator<<(ostream& out, Boggle& boggle) {
    out << boggle.boggleString;
    return out;
}
