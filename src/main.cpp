/*
This code contains the optimized version of a Hangman Game by Hotsotse. 





The optimization process
mainly involved the following:
	1. Removal of the random number generator function "random()". This previously involved the
 	   creation of variables a and b, creation of uniform_int_distribution object and the returning
     	   of produced random variable to randomWord function. All these were removed.
 	2. Optimizing randomWord() function. The new randomWord() function combines both random number 
  	   generation and the word selection in a single function. The previous implementation where a 
      	   word was selected and returned to main function to be copied was replaced with a string reference
	   being directly passed to the randomWord function, removing the need to create new strings and perform
    	   a string copy. Further, instead of loading 10 words to words[] as strings, a random word was directly
	   loaded from the "words.txt" file to the needed string. This dramatically reduced the amount of memory
    	   used for string storage and management. 
	3. Optimizing letterFill() function and its arguments. All arguments passed to letterFill() are now either 
 	   const reference (when the data should be read-only) or reference (when the data needs to be operated on).
     	   Two unessential len variables storing the string length were also removed. 


The following were also tested to see whether optimization was possible. 
	1. The randomWord() function was run in a separate thread (using std::thread) while remainder of the operations
 	   were carried out up till the while loop. The overhead for thread management proved to cause far more inefficiencies
     	   compared to the original code. The simple timing comparison revealed that the multi-threading was slower.
	2. The for loop in letterFill() was attempted to be run parallelly (as a multithread with std::thread). However, as 
 	   experienced previously, the thread overhead outweighed the performance gains and the program was made slower. Hence,
     	   the regular for loop was kept.

The "words.txt" file used alongside this program simply contains the following words arranged line by line:
Indonesia
Pakistan
India
America
Europe
Jamaica
Barbados
England
Scotland
Argentina

*/

#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <iomanip>

/*The following Timer class is not part of the main code and is only used for 
benchmarking, where the time duration of a certain process needs to be measured */
class Timer{
  public:
    Timer(){
        start_timepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer(){
        auto end_timepoint = std::chrono::high_resolution_clock::now();
        auto start_time = std::chrono::time_point_cast<std::chrono::microseconds>(start_timepoint).time_since_epoch().count();
        auto end_time = std::chrono::time_point_cast<std::chrono::microseconds>(end_timepoint).time_since_epoch().count();

        auto time_duration = end_time - start_time;
        std::cout << "Time duration: "<<time_duration << std::endl;
    }

  private:
    	std::chrono::time_point<std::chrono::high_resolution_clock> start_timepoint; 
};


void randomWord(std::string& word){				// "word" initialized in main() passed by reference
	std::ifstream file("words.txt");			// The words stored in a txt file rather and loaded entirely to memory
    
    	srand((unsigned) time(NULL));				// Uses current time as a seed value for random numbers
    	int i = 1, rand_num = 1 + rand()%10;			// Generates random number

    	while(std::getline(file, word) && i != rand_num){	// Reads line by line from file and stores a random word in "word"
        	i++;
    	}
}


int letterFill(const char& guess, const std::string& secretword, std::string& guessword){	// guess and secretword are read only 
    int matches=0;										// while guessword needs to be operated
    for (int i = 0; i < secretword.length(); i++)						
    {
        // Did we already match this letter in a previous guess?
        if (guess == guessword[i]) return 0;

        // Is the guess in the secret word?
        if (guess == secretword[i]  || (guess - (97-65)) == secretword[i]){			// (97-65) accounts for Uppercase
            guessword[i] = secretword[i];							// Reveals letter if guess is correct
            matches++;
        }
    }
    return matches;
}

int main(){
	int tries = 6;
	std::string word;					// Stores the random word to be used
	randomWord(word);
	std::string unknown(word.length(), '_');		// Stores players progress in guessing the correct word
	std::cout << "Welcome to Hangman!" << std::endl;
	std::cout << "Random word has been generated! Word is " << word.length() << " letters long!" << std::endl;
	while (tries > 0){
		std::cout << "Guess a letter, you have " << tries << " tries left." << std::endl;
		std::cout << unknown << std::endl;
       		char letter; 
		std::cin >> letter;				// Takes user's guess
		Timer T1;					// Starts timer for loop

		if (letterFill(letter, word, unknown) == 0){
			std::cout << "Could not find " << letter << " in the word." << std::endl;
			tries--;
		}
		else {
			std::cout << "Found letter! " << std::endl;
		}


		if (word == unknown)
		{
			std::cout << word << std::endl;
			std::cout << "Yeah! You got it!";
            		return 0;
		}
		
	}
	std::cout << "Sorry, you lose...you've been hanged." << std::endl;
	std::cout << "The word was : " << word << std::endl;

	return 0;
}
