/*
 * Authors: Philip Clark and Susannah Meyer
 * Program Description: Bonus version modified to adapt to 5x5 board.
 * Date: 10/30/15
 */

#include "lexicon.h"
#include "Boggle.h"
#include "simpio.h"
#include "ostream";
#include "set.h";
#include "console.h"
#include "bogglegui.h"


/*
 * This function takes the user's choice to either force a valid boggle configuration
 * or use random input to great the boggle board by initializing the GUI.
 * @b: the boggle object containing the board cubes
 */
void initializeGUI(Boggle b) {
    for (int i = 0; i < 5; ++i) { //loop over the board
        for (int j = 0; j < 5; ++j) {
            BoggleGUI::labelCube(i, j, b.getLetter(i, j), false); //labels the GUI cube with the corresponding boggle object cube
        }
    }
}

/*
 * This method determines whether the text the user has inputed to force a boggle board configuration
 * is actually valid (25 characters long and only containing alphas)
 */
bool isValidBoardText(string boardText) {
    bool correctSize = true;
    bool validLetters = true;
    if (boardText.size() != 25) { //if not the appropriate length
        correctSize = false;
    }
    for (int i = 0; i < boardText.length(); i++) {
        if (!isalpha(boardText[i])) { //if any non alphabetic characters
            validLetters = false;
        }
    }
    return (correctSize && validLetters);
}

/*
 * setUpGame sets up the game, asks how the user wants to set up the board,
 * and returns the string representative of the board.
 */
string setUpGame() {
    setConsoleClearEnabled(true);
    string cubeLetters;

    if (!getYesOrNo("Do you want to generate a random board? ")) { //if the user wishes to force a board configuration
        while(true) {
            cubeLetters = getLine("Type the 25 letters to appear on the board: ");
            cubeLetters = toUpperCase(cubeLetters);
            if (isValidBoardText(cubeLetters)) {
                break;
            }
            cout << "That is not a valid 25-letter board string. Try again." << endl;
        }
    }
    return cubeLetters;
}

/*
 * processUserInput displays the current state of the user's score and processes the words inputted
 * to see if they are valid answers.
 * @b: Boggle object
 * @humanWords: set of the user's valid word responses
 */
void processUserInput(Boggle& b, Set<string>& humanWords) {
    while (true) {
        cout << b << endl;
        cout << "Your words (" << humanWords.size() << "): " << humanWords << endl
        << "Your score: " << b.getScoreHuman() << endl;
        BoggleGUI::setScore(b.getScoreHuman(), BoggleGUI::HUMAN); //updates human score
        string userWord = getLine("Type a word (or Enter to stop):");
        if (userWord == "") { //user has chosen to end turn
            BoggleGUI::clearHighlighting();
            break;
        }
        userWord = toUpperCase(userWord); //deals with case issues
        clearConsole();
        if (b.checkWord(userWord)) { //checks first if the word is suitable
            BoggleGUI::clearHighlighting();
            if (b.humanWordSearch(userWord)) { //now that the word is suitable, sees whether the board can actually form it (highlights cubes as it searches)
                humanWords.add(userWord);
                string foundWord = "You found a new word! \"" + userWord + "\"";
                cout << foundWord << endl;
                BoggleGUI::setStatusMessage(foundWord);
            } else {
                cout << "That word can't be formed on this board." << endl; //if word can't be formed on the board
                BoggleGUI::setStatusMessage("That word can't be formed on this board.");
            }
        } else {
            cout << "You must enter an unfound 4+ letter word from the dictionary." << endl; //if word is unsuitable
            BoggleGUI::setStatusMessage("You must enter an unfound 4+ letter word from the dictionary.");
        }
    }
}

/*
 * determineWinner determines the winner of the game.
 * @b: Boggle object
 */
void determineWinner(Boggle& b) {
    if (b.getScoreHuman() < b.getScoreComputer()) { //if computer has one
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
        BoggleGUI::setStatusMessage("Ha ha ha, I destroyed you. Better luck next time, puny human!");
    } else if(b.getScoreHuman() >= b.getScoreComputer()) { //if user has tied or won
        cout << "WOW, you defeated me! Congratulations!" << endl;
        BoggleGUI::setStatusMessage("WOW, you defeated me! Congratulations!");
    }
}

/*
 * playOneGame is the function looped in the main that allows an individual boggle game to run.
 * The method is the main function in boggleplay.cpp in that it acts as a road map for the output of
 * the game as well as handles the user interactions.
 * @dictionary: the Boggle dictionary
 */
void playOneGame(Lexicon& dictionary) {
    BoggleGUI::initialize(5, 5);
    BoggleGUI::reset();

    Set<string> humanWords;
    string cubeLetters = setUpGame();
    Boggle b(dictionary, cubeLetters); //runs boggle constructor to create boggle object

    clearConsole();
    initializeGUI(b);
    BoggleGUI::setAnimationDelay(100);
    cout << "It's your turn!" << endl;
    BoggleGUI::setStatusMessage("It's your turn!");

    processUserInput(b, humanWords);

    Set<string> computerWords = b.computerWordSearch(); //creates a set to hold the computer's word results generated via the recursive backtracking algorithim
    cout << endl << "It's my turn!" << endl
    << "My words (" << computerWords.size() << "): " << computerWords << endl
    << "My score: " << b.getScoreComputer() << endl;

    BoggleGUI::setScore(b.getScoreComputer(), BoggleGUI::COMPUTER); //updates computer score
    determineWinner(b);
}
