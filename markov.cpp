#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iterator>
#include <time.h>
using namespace std;

/** printVect prints the given vector, separated by spaces
 * input: string of vectors
 * output: the elements of that vector
 * */
void printVect(vector<string> v){
    for(int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl;
}

/**printMap prints a given map.
 * not used currently, but we wrote it to test if map was built correctly
 * input: map you wish to print in map<string, vector<string> > format
 * output: the elements of that map 
 * */
void printMap(map<string, vector<string> > markov){
    for(map<string, vector<string> >::const_iterator it = markov.begin();
    it != markov.end(); ++it) {
        cout << it->first << " ";
        for(int i = 0; i < it->second.size(); i++) {
            cout << it->second[i] << ", ";
        }
        cout << endl;
    }
}

/** split takes in a string and makes it a vector
 * called in buildMarkovTable()
 * Input: a string
 * Output: the strings but now in a vector.
 * */
vector<string> split(string s) {
    vector<string> words;
    words.push_back("");
    int word_count = 0;
    
    for(int i = 0; i < s.length(); i++) {
        if(s[i] == ' ' || s[i] == '\n') { 
            word_count++; 
            words.push_back("");
            continue; 
        }
        else{
            words.back() += s[i];
        }
    }
    return words;
}

/** separates the input string into a vector
 * separates words by spaces, can be changed
 * input: string input file
 * output: a vector of strings
 * */
string separate(vector<string> v) {
    string s = "";
    for(int i = 0; i < v.size()-1; i++) {
        s += v[i];
        s += " ";
    }
    s += v[v.size()-1];
    return s;
}

/** physically builds a map which is later used to assemble tweet
 * inputs: input file in form of string 
 * output: a map later used to build the tweet itself. 
 * */
map<string, vector<string> > buildMarkovTable(string markovString) {
    map<string, vector<string> > markMap; // your output map
    
    // splits the input string into a vector of individual words
    vector<string> words;
    words = split(markovString);
    
    // hopefully will map the prefixes and suffixes correctly? :)
    for(int i = 1; i < words.size()-1; i++) {
        string s = words[i-1]+" "+words[i];
        map<string, vector<string> >::iterator it = markMap.find(s);
        
        if(markMap.empty() || it == markMap.end()) {
            vector<string> vect;
            vect.push_back(words[i+1]);
            markMap[s] = vect;
        }
        else {
            markMap[s].push_back(words[i+1]);
        }
    }
    return markMap;
}

/** physically generates a tweet below 140 characters in size
 * of a given input file
 * input: generated map of strings (keys) linked to vectors of strings
 * output: an assembled string from the given map
 * */
void generate_markov_tweet(map<string, vector<string> > markMap, int maxChar) {
    srand(time(NULL));
    string tweet= "";
    string suffix;
    
    // this should get a random prefix?
    map<string, vector<string> >::iterator it = markMap.begin();
    std::advance(it, rand() % markMap.size());
    tweet += it->first;
    
    //initialize first prefix
    vector<string> prefix = split(tweet); 
    // append suffix to tweet and add to vector, delete first word until string size <= 140
    while(tweet.size() <= maxChar) {
        // get a random suffix
        vector<string> temp = markMap[separate(prefix)];
        suffix = temp[rand() % temp.size()];
        tweet = tweet + " " + suffix;
        
        //update vector
        prefix.erase(prefix.begin());
        prefix.push_back(suffix);
        
        // make sure the new prefix is in the map
        map<string, vector<string> >::iterator iter = markMap.find(separate(prefix));
        if(iter == markMap.end()) {
            break;
        }
    }
    ofstream myfile;
    myfile.open("output.txt");
    myfile << tweet;
    myfile.close();
}

int main() {
    //changes input to a string
    ifstream ifs("input.txt");
    string line;
    line.assign((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
    
    // create map and table
    map<string, vector<string> > markov;
    markov = buildMarkovTable(line);
    
    // generate the tweet
    generate_markov_tweet(markov, 600);
}
