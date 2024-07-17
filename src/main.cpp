#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <iomanip>

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

void randomWord(std::string& word){
	std::ifstream file("words.txt");
    
    srand((unsigned) time(NULL));
    int i = 1, rand_num = 1 + rand()%10;

    while(std::getline(file, word) && i != rand_num){
        i++;
    }
}


int letterFill(char& guess, std::string& secretword, std::string& guessword){
    int matches=0;
    for (int i = 0; i < secretword.length(); i++)
    {
        // Did we already match this letter in a previous guess?
        if (guess == guessword[i]) return 0;

        // Is the guess in the secret word?
        if (guess == secretword[i]  || (guess - (97-65)) == secretword[i]){
            guessword[i] = secretword[i];
            matches++;
        }
    }
    return matches;
}

int main(){
	int tries = 6;
	
	std::string word;
	randomWord(word);
	std::string unknown(word.length(), '_');
	std::cout << "Welcome to Hangman!" << std::endl;
	std::cout << "Random word has been generated! Word is " << word.length() << " letters long!" << std::endl;
	while (tries > 0){
		
		std::cout << "Guess a letter, you have " << tries << " tries left." << std::endl;
		std::cout << unknown << std::endl;
       		char letter; 
		std::cin >> letter;
		Timer T1;

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
