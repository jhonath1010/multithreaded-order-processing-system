#include <iostream>
#include <chrono>       
#include <ctime>      
#include <iomanip> 
#include <sstream>    
#include <string>     
#include <random>  


int menu() {
    int choice = 1;
    std::cout << "******************** Main Menu ********************" << std::endl;
    std::cout << "1) Login" << std::endl;
    std::cout << "0) Exit" << std::endl;

    // NOTE: Input handling is disabled — choice is hardcoded to 1 for now
    // Uncomment below if interactive menu is needed
    /*
    std::cout << "Enter an option (0-1): ";
    std::cin >> choice;
    if (std::cin.fail() || (choice != 0 && choice != 1)) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "You entered a wrong value. Try again..." << std::endl;
    }
    */
    return choice;
}

// Returns the current local date as a string in "YYYY-MM-DD" format
std::string getTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::tm local_tm;
    localtime_s(&local_tm, &now_c);  // Use localtime_r on Linux

    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%Y-%m-%d");  

    return oss.str();
}

// Runs a Python script and passes the given filename as an argument
void pythonMailSender(const std::string filename)
{
    std::string command = "python \"C:\\Users\\fortn\\source\\repos\\ConsoleApplicationOracle\\PythonScript\\test.py\" " + filename;
    int result = system(command.c_str());

    if (result == 0) {
        std::cout << "Python script ran successfully.\n";
    }
    else {
        std::cerr << "Failed to run Python script.\n";
    }
}
// Generates and returns a random number between 1000 and 9999

int getRandomNumber()
{
    static int min = 1000;
    static int max = 9999;
    static std::random_device rd;        
    static std::mt19937 gen(rd());       
    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}