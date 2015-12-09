/*
 * Authors: Philip Clark and Susannah Meyer
 * Date: 10/9/15
 * Description: This is our bonus version of the N-Gram program. Now, the program generates text in full sentences, always begining with an uppercase
 * and ending with punctuation ("?", "!", "."). The text will contain the number of words specified by the user + however many more required
 * to reach the end of a sentence. Note that the only substantively changed functions from the original program are genNgram and createMap.
 */

#include "filelib.h"
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "stack.h"
#include "queue.h"
#include "random.h"
#include "hashmap.h"
#include "simpio.h"

using namespace std;

void playGame (int n_value, ifstream& in);
void genNgram (int word_num, int n_value, Queue <string>& window, HashMap<Queue<string>,Vector<string> > &word_map, Vector<string>& text, Vector<Queue<string> >& start_sentence);
void createMap (int n_value, HashMap<Queue<string>,Vector<string> >& word_map, ifstream& in, Queue <string> window, Vector<Queue<string> >& start_sentence);

/*
 * The main function simply displays the welcome message, reads in the text file to be used for the random generation,
 * takes the value of n, and runs the playGame function (the operational function for the program).
 */
int main() {
    ifstream in;
    cout << "Welcome to CS 106B Random Writer ('N-Grams'').\n"
    << "This program makes random text based on a document. \n"
    << "Give me an input file and an 'N' value for groups.\n"
    << "of words, and I'll create random text for you." << endl << endl;

    string fileName = promptUserForFile(in, "Input file name? ", "Unable to open that file.  Try again.");

    int n_value = getInteger("Value of N? ");
    cout << endl;

    playGame(n_value, in);

    cout << "Exiting." << endl;
    return 0;
}

/*
 * The playGame function creates a number of data structures that will be used to the create the word map and generate
 * random words as well as runs the createMap function (which creates the map from prefixes to suffixes) and
 * subsequently loops through the genNgram function (which generates random text) until the user types a zero.
 * @n_value: the "window" value used to determine the length of the prefixes in the word map
 * @in: needed to read the text file
 */
void playGame (int n_value, ifstream& in) {
        Queue <string> window;
        HashMap<Queue<string>,Vector<string> > word_map;
        Vector<string> text;
        Vector<Queue<string> > start_sentence;

        createMap(n_value, word_map, in, window, start_sentence);
        while(true)
        {
            int word_num = getInteger("# of random words to generate (0 to quit) ");
            if(word_num == 0) break;
            genNgram(word_num, n_value, window, word_map, text, start_sentence);
        }
}

/*
 * The createMap function does the same thing as in the original program but now also keeps a list of all those prefix keys beginning with a capital letter.
 * @n_value: the "window" value used to determine the length of the prefixes in the word map
 * @word_map: takes in a queue of strings for the prefixes and, by reading through the file, matches them
 * to vectors of strings as suffixes.
 * @in: needed to read the text file
 * @window: the sliding window is a queue which for each new word read in the file, dequeues the earliest word in the window and
 * enqueues the new word.
 * @Vector<Queue<string> >& start_sentence: the list of keys with prefixes whose first word starts with an uppercase letter
 */
void createMap (int n_value, HashMap<Queue<string>, Vector<string> >& word_map, ifstream& in, Queue <string> window, Vector<Queue<string> >& start_sentence)
{
    string new_word; //string to strong the new words of the file as they are read in
    Vector<string> initial_window; //holds the value of the initial window of strings in the file for later use by the wrap around loop
    Vector<string> suf; //stores the suffixes used as values in the map

    for (int i = 0; i < n_value-1; i++) { //initial window adding
        in >> new_word;
        window.enqueue(new_word);
        initial_window.add(new_word);
    }
    if(window.peek()[0] == toupper(window.peek()[0])) start_sentence.add(window); //checks to see if the prefix begins with an uppercase

    while(in >> new_word) //body of file adding
    {
        suf.add(new_word);
        if(word_map.containsKey(window)) { //if the map already has the key add it to the corresponding value
           word_map[window] += new_word;
        } else{ //otherwise make a new key with the corresponding suffix value
            word_map[window] = suf;
           }
        if(window.peek()[0] == toupper(window.peek()[0])) start_sentence.add(window); //checks to see if the prefix begins with an uppercase

        window.dequeue(); //update the window
        window.enqueue(new_word);
        suf.clear(); //clear the suffixes
    }

    for (int i = 0; i < n_value-1; i++) { //wrap around
        suf.add(initial_window[i]);
        if(word_map.containsKey(window)) {
           word_map[window] += initial_window[i];
        } else{
            word_map[window] = suf;
        }

    if(window.peek()[0] == toupper(window.peek()[0])) start_sentence.add(window); //checks to see if the prefix begins with an uppercase

    window.dequeue();
    window.enqueue(initial_window.get(i));
    suf.clear();
    }
}

/*
 * The genNgram function acts the same as on the original N-Gram program, but now picks its random starting place from those
 * prefixes which begin with an uppercase letter and continues generating text until it satisfies the user's specified
 * text length and reaches a suffix with punctuation "?", ".", or "!".
 * @word_num: the number of words in the random text
 * @n_value: the "window" value used to determine the length of the prefixes in the word map
 * @window: the sliding window is a queue which for each new word read in the file, dequeues the earliest word in the window and
 * enqueues the new word.
 * @word_map: takes in a queue of strings for the prefixes and, by reading through the file, matches them
 * to vectors of strings as suffixes.
 * @text: used to store the randomly generated text
 * @Vector<Queue<string> >& start_sentence: the list of keys with prefixes whose first word starts with an uppercase letter
 */
void genNgram (int word_num, int n_value, Queue <string>& window, HashMap<Queue<string>,Vector<string> > &word_map, Vector<string>& text, Vector<Queue<string> >& start_sentence)
{
    text.clear();
    int start = randomInteger(0, start_sentence.size()); //picks random starting place from the prefixes that could begin a sentence

    Queue<string> random_key = start_sentence.get(start); //finds the key at the starting place
    window = random_key;

    for (int i = 0; i < n_value-1; i++) {
    text.add(random_key.dequeue()); //adds initial prefix starting place to the text
    }

    Vector <string> poss_words;

    bool finish = false;
    int count = 0;
    while(!finish) //continues until both the length of the text is sufficiently long and an ending word with punctuation of the specified type is reached.
    {
        poss_words.clear();
        poss_words = word_map[window];
        int next = randomInteger(0, word_map[window].size()-1); //picks random suffix index
        window.dequeue(); //the next two lines update the sliding window
        window.enqueue(poss_words.get(next));
        text.add(poss_words.get(next) + " ");
        char final_char = poss_words.get(next)[poss_words.get(next).length()-1];
        if((final_char == '.' || final_char == '?' || final_char == '!') && count > word_num-(n_value-1)) finish = true; //checks if the conditions for the while loop are met
        count++;
    }

    for (string s : text) {
        cout << s << " ";
    }
    cout << endl << endl;
}
