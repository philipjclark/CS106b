/*
 * Authors: Philip Clark and Susannah Meyer
 * Description: This is our Huffman encoding program. The program follows the spec in building a frequency table,
 * an encoding tree, and an encoding map which are used to encode the file/string inputed
 * by the user and decode the resulting encoding. The program also uses the same functions
 * to write the compress and decompression methods by writing and reading a header which
 * states the frequency table to the output/from the input stream in order to deal with completing
 * the encoding/decoding process for files in one fell swoop.
 * Date: 11/18/15
 */

#include "encoding.h"
#include "pqueue.h"
#include "filelib.h"
#include "vector.h"

/*
 * This method reads in the string or file from the input stream and makes a map with the characters
 * as keys and the frequencies/number of occurences of these characters as values.
 * @input: the stream where the file/string is being read from
 */
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    while(true) { //continue until no more input is read
   int c = input.get(); //uses int to prevent bug of char halving (only goes to 128)
   if(input.fail()) break;
   freqTable[c]++; //increment frequency
    }

    freqTable[PSEUDO_EOF] = 1; //add EOF character
    return freqTable;
}

/*
 * This function loops through the intergers (characters) in the frequency table
 * and, for each key, creates a node with that character value and the frequency and
 * enqueues to the priority queue with a priority of the frequency (ie to ensure that
 * characters that don't occur often are in front). Then it combines entries at the front of
 * priority queue into a binary tree.
 * @freqTable: the frequency table made in the above functions which maps the characters from the
 * input to their frequencies
 */
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> p;
    for(int i: freqTable) {
    HuffmanNode* next = new HuffmanNode; //make new node
    next->character = i; //set pointers
    next->count = freqTable[i];
    p.enqueue(next, next->count); //enqueue with priority of frequency of character pointer
    }

    while(p.size()>1) { //continue until there is a root node
    HuffmanNode* first = p.dequeue(); //take first two nodes in priority queue
    HuffmanNode* second = p.dequeue();
    HuffmanNode* comb = new HuffmanNode; //make parent node
    comb->character = NOT_A_CHAR;
    comb->count = first->count + second->count; //make frequency combined frequency (to put farther back on the queue)
    comb->zero = first; //child pointer
    comb->one = second; //child pointer
    p.enqueue(comb, comb->count); //enqueue new parent ndoe
    }

    return p.dequeue();
}

/*
 * The recursive helper function that largely implements the encoding map. The method recursively
 * walks down the tree keeping track of its path (of 1s and 0s) through the s parameter until it hits
 * a leaf.
 */
void buildHelper(HuffmanNode* curr, string s, Map<int, string>& encodingMap) {
    if(curr->isLeaf()) { //base case -- if the recursion has reached a leaf, map the leaf's character pointer to the path string
        encodingMap[curr->character] = s;
        return;
    }
    else if(curr!=NULL) { //otherwise
    buildHelper(curr->zero, s+"0", encodingMap); //recurse to left
    buildHelper(curr->one, s+"1", encodingMap); //recurse to right
    }
}

/*
 * The buildEncodingMap method creates a map from a character in the input (represented as an int)
 * to a string of 1s and 0s that represent the compression of that character.
 * @encodingTree: the root node of the binary tree which organizes the characters in the input
 * and their frequency.
 */
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    buildHelper(encodingTree, "", encodingMap); //calls helper function which does the recursive work
    return encodingMap;
}

/*
 * The encodeData method takes in the file/string input and, character by character, encodes
 * it.
 * @input: what the data to be encoded is being read from
 * @encodingMap: the map which takes characters (as integers) to the encoded string of 1s and 0s
 * @output: what the encoded data is being written to
 */
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    while(true) { //as longas there are characters to get
        int c = input.get();
        if(input.fail()) break;
        for (int i = 0; i < encodingMap[c].length(); ++i) { //loop through encoded series of 1s and 0s and write them to the output
            output.writeBit(encodingMap[c][i]-'0');
        }
    }
    for (int i = 0; i < encodingMap[PSEUDO_EOF].length(); ++i) { //do same thing for EOF character
        output.writeBit(encodingMap[PSEUDO_EOF][i]-'0');
    }
    output.flush(); //flush output
}

/*
 * The helper function for decodeData that recursively moves down the path specified by the input (1 for right, 0 for left)
 * until it hits a leaf which is either a character to be written to the output or the marker for the end of the file.
 * @input: what the encoded data is being read from
 * @curr: the node the recursion is currently on in the binary tree
 * @output: what the decoded data is being written to
 * @encodingTree: the root node of the binary encoding tree
 */
bool decodeDataHelper (ibitstream& input, HuffmanNode* curr, ostream& output, HuffmanNode* encodingTree) {
    if(!input.fail()) { //continue whiel there is input
        if(curr->isLeaf()) { //base case which "fails fast" in order to prevent unnecessary recursive calls
            if(curr->character==PSEUDO_EOF) return false;
            output.put(curr->character);
            return true;
        } else { //recursive case which reads in the next encoding bit and uses it to pick the next step down in the tree path
            int branch = input.readBit();
           if(branch == 0) return decodeDataHelper(input, curr->zero, output, encodingTree);
           else if(branch == 1) return decodeDataHelper(input, curr->one, output, encodingTree);
        }
    }
    return false;
}

/*
 * The decodeData method takes in the encoding tree's root node and the input stream with the
 * encoded data and recursively moves down the encoding tree to decode the data.
 * @input: what the encoded data is being read from
 * @encodingTree: the root node of the binary encoding tree
 * @output: what the decoded data is being written to
 */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    HuffmanNode* curr = encodingTree; //a current node pointer to prevent us from losing the root node's place
    while (decodeDataHelper(input, curr, output, encodingTree)); //continues until a false case is found (ie EOF or no more nodes)
}

/*
 * The compress method runs the above functions (and writes the header frequency table to the output)
 * in order to encode the data in one fell swoop.
 * @input: where the data is being encoded from
 * @output: where the encoded data is being written to
 */
void compress(istream& input, obitstream& output) {
  Map<int, int> freqTable = buildFrequencyTable(input);
  output << freqTable; //writes out header
  HuffmanNode* root = buildEncodingTree(freqTable); //builds encoding map from freq table
  rewindStream(input); //rewinds the stream to allow the useer to read from it in decompress
  Map<int, string> encodingMap = buildEncodingMap(root); //builds encoding map from encoding tree
  encodeData(input, encodingMap, output); //calls encodeData with above tools
  freeTree(root); //free memory
}

/*
 * The decompress method runs a combination of the above methods, after reading the frequency table
 * from the input stream, to decompress the file and free the created encoding tree.
 * @input: where the encoded data is being read from
 * @output: where the decoded data is being written to
 */
void decompress(ibitstream& input, ostream& output) {
Map<int, int> freqTable;
input >> freqTable; //reads frequency map (header)
HuffmanNode* root = buildEncodingTree(freqTable); //creates encoding tree from table
decodeData(input, root, output); //calls decode data with above tools
freeTree(root); //free memory
}

/*
 * The freeTree method recursively moves down the tree to delete all the nodes and prevent a memory leak
 * @node: the root node of the tree
 */
void freeTree(HuffmanNode* node) {
    if(node!=NULL) { //implicit base case to stop when it is null
        freeTree(node->zero);
        freeTree(node->one);
        delete node; //kill current node
    }
}
