#include <iostream>
#include <iomanip>
#include <string>
#include <list>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include "maxent.h"

using namespace std;

void split(string& str, vector<string>& tokens)
{
    istringstream in(str);
    char c;

    while (in){
        string token;
        token = "";
        while (in.get(c) && (c != '\t')) token.push_back(c);
        tokens.push_back(token);
    }
}

/* TODO: Repeating the usage string */
/* TODO: Incorporate the post-processing from BioNLP 2011? */
int main(int argc, char* argv[])
{
    int ch, quiet;

    /* Initialise arguments */
    quiet = 0;

    string progName = string(argv[0]);

    /* Parse cmd-line arguments */
    while((ch = getopt(argc, argv, "qh")) != -1) {
        switch(ch) {
            case 'q':
                quiet = 1;
                break;
            case 'h':
                cerr << "Usage: " << progName << " input output [path-to-ruby]" << endl;
                exit(0);
            case '?':
            default:
                cerr << "Usage: " << progName << " input output [path-to-ruby]" << endl;
                exit(-1);
        }
    }
    argc -= optind;
    argv += optind;

    if (argc < 2 || argc > 3) {
        cerr << "Usage: " << progName << " input output [path-to-ruby]" << endl;
        exit(-1);
    }

    ME_Model model;

    string inFile = argv[0];
    string outFile = argv[1];
    //string modelFile = argv[2];
    string modelFile = "model1-1.0";
	string rubyCommand = (argc == 3) ? argv[2] : "ruby";

    /* XXX: Should use temporary files if any! */
	string eventFile = inFile + ".event";
	string resultFile = inFile + ".result";

    if (!quiet) {
        cerr << "Extracting events.";
    }

	string extractionCommand = 
    	rubyCommand + " EventExtracter.rb " + inFile + " " + eventFile;
    system(extractionCommand.c_str());

    if (!quiet) {
        cerr << "roading model file." << endl;
    }
    model.load_from_file(modelFile.c_str());
    //model.load_from_file("model" + setID + "-" + ineq);
    //ifstream fileIn(string("/home/users/y-matsu/private/workspace/eclipse-workspace/GENIASS/" + setID + "/test.txt").c_str());
    //ofstream fileOut(string("/home/users/y-matsu/private/workspace/eclipse-workspace/GENIASS/" + setID + "/test-" + ineq + ".prob").c_str());

    ifstream fileIn(eventFile.c_str());
    ofstream fileOut(resultFile.c_str());

    string line, markedTxt;

    getline(fileIn, markedTxt);
    if (!quiet) {
        cerr << "start classification." << endl;
    }
    while (getline(fileIn, line)){
        vector<string> tokens;
        split(line, tokens);
        ME_Sample s;

        for(vector<string>::const_iterator token = tokens.begin() + 1;
				token != tokens.end(); ++token){
            s.add_feature(*token);
        }

        (void) model.classify(s);
        fileOut << s.label << endl;
    }
    fileOut.close();
    fileIn.close();

    remove(eventFile.c_str());

	string splitCommand =
    	rubyCommand + " Classifying2Splitting.rb "
		+ resultFile + " " + markedTxt + " " + outFile;

    system(splitCommand.c_str());

	return 0;
}

