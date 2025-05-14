#ifndef USER_SESSION_H
#define USER_SESSION_H

#include <occi.h>
#include <string>
#include "database_procedures.h"
#include "cart.h"

// Holds Oracle session configuration for user sessions
struct UserSessionConfig {
    oracle::occi::Environment* env;
    std::string user;
    std::string pass;
    std::string constr;
};

// Simulates one user session: login, cart handling, and checkout
void simulateUser(UserSessionConfig config, ShoppingCart& customer);

// Fills the cart from product info in the database
void fillCartFromDB(ShoppingCart& cart, oracle::occi::Statement* stmt);

// Prompts user to checkout and processes the order
bool checkoutPrompt(oracle::occi::Statement* stmt, const ShoppingCart& cart);

#endif 
