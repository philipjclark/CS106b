/*
 * Authors: Philip Clark and Susannah Meyer
 * Description: This bonus program is an implementation of the LZW compression algorithim. It takes in a user's string
 * as input, converts it using the compression algorithim into a coded series of numbers (which it prints), and
 * then uses the decompression algorithim to convert it back out into the original input.
 * Date: 11/18/15
 */

#include "LZW.h"
#include "pqueue.h"
#include "filelib.h"
#include "vector.h"
#include "map.h"
#include "console.h"
#include "simpio.h"
using namespace std;

/*
 * The compress function takes a string and compresses in into a coded vector of ints.
 * The algorithim works by creating a baseline dictionary and then expanding it by
 * taking the longest part of the uncompressed string found in the dictionary
 * and using the dictionary value to encode it or creating a new dictionary entry with
 * the corresponding code.
 * @uncompressed: the input being compressed
 */
Vector<int> compress (string uncompressed) {
    Map<string, int> dict; //dictionary map which takes string values to a code
    for (int i = 0; i < 256; ++i) { //creates dictionary of all possible string values
        dict[charToString(char(i))]+=i;
    }
    string w;
    Vector<int> compressed; //the ints which will make up the code

    for (int i = 0; i < uncompressed.length(); ++i) { //for every character in the input string
        string wc = w + uncompressed[i]; //the current string + the new character
        if(dict.containsKey(wc)) { //if it's already in the dictionary make w the exapnded string described above
            w = wc;
        }
        else {
            compressed.add(dict[w]); //otherwise add it to the dictionary with the character code
            dict[wc]+=dict.size(); //make a new dictionary entry with the expanded string
            w = uncompressed[i]; //make w the character (need to start checking a new string)
        }
    }

    if(w != "") { //if there is anything remaining in w, make it the final part of the code
       compressed.add(dict[w]);
    }

    return compressed;
}

/*
 * The decompress method takes an encoded string represented by a vector of integers and
 * uses a reverse dictionary and a roughly analogous process to the compress method to
 * convert this code back into the original string.
 * @compressed: the encoded string
 */
string decompress (Vector<int> compressed) {
    Map<int, string> dict;
    for (int i = 0; i < 256; ++i) { //make reverse dictionary (opposite of first step of compress)
        dict[i]+=charToString(char(i));
    }

    string w = dict[compressed[0]]; //take out the first int of the encoded string
    compressed.remove(0);
    string decompressed = w; //set the first code value to w

    for (int i: compressed) { //for every code value of the encoded string
        string entry;
        if(dict.containsKey(i)) { //if the reverse dictionary has the code, create the corresponding string
            entry = dict[i];
        }
        else if (i == dict.size()) { //otherwise if its the next added value, make it the current w + the first character of w
            entry = w + w[0];
        }
        decompressed+=entry; //add next part of string to decompressed string
        dict[dict.size()]+=w+entry[0]; //add new value to expand the reverse dictionary

        w = entry;
    }

    return decompressed;
}

/*
 * The main method the bonus LZW algorithim allows the user to input a string (or press q
 * for quitting) and then prints output that shows the encoding and decodign process.
 */
int main2() {
    ifstream in;
    while(true) {
        cout << "Welcome to the LZW bonus compression algorithim!" << endl;
        string input = getLine("Please input a string to be compressed. Enter q to quit.");
        if(input == "q") break;
        Vector<int> compressed = compress(input);
        string decompressed = decompress(compressed);
        cout << "\""<< input << "\" gets converted to " << compressed << " and back to \""
             << decompressed << "\"\n" << endl;
    }

    return 0;
}

