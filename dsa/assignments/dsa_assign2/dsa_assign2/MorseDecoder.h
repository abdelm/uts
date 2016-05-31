//
//  MorseDecoder.h
//  dsa_assign2
//
//  Created by Abdelrahman Ahmed on 29/05/2016.
//  Copyright © 2016 Abdelrahman Ahmed. All rights reserved.
//

#ifndef MorseDecoder_h
#define MorseDecoder_h
#define morseCodeSource "/Users/Abdel/Projects/uts/dsa/assignments/dsa_assign2/dsa_assign2/morsecodes.txt"

#include <string>
#include <cctype>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "bintree.h"

using namespace std;

class MorseDecoder
{
   private:
      struct morseToEnglish
      {
         string morse;
         char letter;
         
         bool operator ==(const morseToEnglish& l) const
         {
            return morse == l.morse;
         }
         
         bool operator <(const morseToEnglish& l) const
         {
            return morse < l.morse;
         }
      };
      
      // Binary tree for morse codes
      treespc::bintree<morseToEnglish> bintreeMorse;
      
      void loadSourceIntoBinaryTree()
      {
         string line;
         ifstream source;
         
         // Open stream to file
         source.open(morseCodeSource);
         
         // Check if file can be read
         if (!source.is_open())
         {
            throw runtime_error("ERROR unable to read file");
         }
         
         // Read line by line
         while (getline(source, line))
         {
            char letter;
            string morse;
            stringstream linestream(line);
            
            // Split line into letter and associated morse code
            linestream >> letter >> morse;
            
            // Init struct values
            morseToEnglish data;
            data.morse = morse;
            data.letter = letter;
            
            // Add to binary tree
            bintreeMorse.insert(data);
         }
         
         source.close();
      }
   
   public:
      MorseDecoder()
      {
      }
   
      // Handles the decoding process by using loadSource to load
      // the morse codes into a binary tree and balacnes the tree.
      // Loads the input & output files and using the morse:letter map, it goes through the
      // input file line by line first then checks character by character on that line.
      // Constructs a morse string from the characters, then outputs the associated letter.
      void decode(ifstream &in, ofstream &out)
      {
         // Check if file can be read
         if (!in.is_open() || !out.is_open())
         {
            throw runtime_error("ERROR unable to read file");
         }
         
         char c;
         string line;
         string output;
         
         // Load morse code source into binary tree
         loadSourceIntoBinaryTree();
         
         // Rebalance tree
         bintreeMorse.rebalance();
         
         // Read line by line
         while (getline(in, line))
         {
            istringstream buffer(line);
            
            // Get each character in a line
            while (buffer.get(c))
            {
               // Add morse only chars to output
               if (c == '*' || c == '-')
               {
                  output += c;
               }
               else if (c != ' ' && output == "")
               {
                  // Retain regular characters
                  out << c;
                  //output = "";
               }
               
               // If not in morse format, or approaching end of line
               if ((c != '*' && c != '-') || buffer.peek() == -1)
               {
                  // Print a space
                  if (output == "" && c == ' ')
                  {
                     out << " ";
                  }
                  
                  // Get letter from morse
                  try {
                     treespc::const_iterator<morseToEnglish> iter;
                     morseToEnglish findMorse;
                     findMorse.morse = output;
                     
                     iter = bintreeMorse.find(findMorse);
                     out << iter->letter;
                     
                     output = "";
                  }
                  catch (exception &e)
                  {
                     output = "";
                     //continue;
                  }
               }
            }
            
            out << endl;
         }
      }
};

#endif /* MorseDecoder_h */
