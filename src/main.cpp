#include <iostream>
#include <occi.h>
#include <iomanip>
#include <fstream>
#include <thread>

#include "user_session.h"
#include "sql_setup.h"
#include "database_procedures.h"

// Oracle DB credentials
const std::string user = "dbs311_251nkk18";
const std::string pass = "30195197";
const std::string constr = "myoracle12c.senecacollege.ca:1521/oracle12c";

int main(int argc, char** argv) {
    std::vector<std::thread> threads;

    std::cout << "Command Line:\n";
    std::cout << "------------------------------------------------------\n";
    for (int i = 0; i < argc; ++i)
        std::cout << std::setw(3) << i + 1 << ": " << argv[i] << '\n';
    std::cout << "------------------------------------------------------\n\n";

    // Load shopping cart data from input file
    std::vector<ShoppingCart> carts;
    std::ifstream file(argv[1]);
    std::string line;

    if (file) {
        while (std::getline(file, line)) {
            if (!line.empty() && line[0] != 'c') {
                carts.emplace_back(line);
            }
        }
    }

    // Create an Oracle DB environment with thread safety enabled
    oracle::occi::Environment* env = oracle::occi::Environment::createEnvironment(oracle::occi::Environment::THREADED_MUTEXED);
    
    // Set up configuration to pass to each session
    UserSessionConfig config = { env, user, pass, constr };

    // Launch a thread per shopping cart to simulate multiple users concurrently
    for (auto& cart : carts) {
        threads.emplace_back(simulateUser, config, std::ref(cart));
    }

    for (auto& t : threads) {
        t.join();
    }

    // Clean up the Oracle environment (terminate DB session)
    oracle::occi::Environment::terminateEnvironment(env);
    return 0;
}
