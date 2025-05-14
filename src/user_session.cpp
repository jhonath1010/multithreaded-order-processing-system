
#include <iostream>
#include <fstream>
#include <occi.h>
#include "cart.h"
#include "user_session.h"
#include "database_procedures.h"
#include "sql_setup.h"
#include "utils.h"
#include "console_color.h"



// Fills the cart with product prices by validating each product in the database.
// If a product doesn't exist, prompts the user to try again.
// Once all products are validated, it sets the correct prices in the cart.
void fillCartFromDB(ShoppingCart& cart, oracle::occi::Statement* stmt) {
    size_t i = 0;
    size_t total = cart.size();

    while (i < total) {
        int productId = cart.getItem(i).product_id;

        // Repeat until a valid product is found
        do {
            std::cout << "Enter the product ID: " << productId << std::endl;

            if (findProduct(stmt, productId) == 0) {
                std::cout << "The product does not exist. Try again..." << std::endl;
            }
        } while (findProduct(stmt, productId) == 0);

        double price = findProduct(stmt, productId);
        std::cout << "Product Price: " << price << std::endl;
        std::cout << "Enter the product Quantity: " << cart.getItem(i).quantity << std::endl;

        cart.setItemPrice(i, price);
        ++i;
        
        // Stop adding if max cart size is reached
        if (cart.size() == 5) 
            break;

        // If there are no more items, disable further additions
        if (i + 1 > total)
            cart.setMoreItems(false);

        std::cout << "Enter 1 to add more products or 0 to checkout: "
            << (cart.hasMoreItems() ? 1 : 0) << std::endl;

        if (!cart.hasMoreItems())
            break;
    }
}

// Prompts the user to confirm whether they want to checkout.
// If yes, it calls the performCheckout function and sends a receipt.
bool checkoutPrompt(oracle::occi::Statement* stmt, const ShoppingCart& cart) {
    char choice = cart.isCheckoutEnabled() ? 'Y' : 'N';

    do {
        std::cout << "Would you like to checkout? (Y/y or N/n) : " << choice << std::endl;

        // std::cin >> choice;
        // char newline = std::cin.get();

        if (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n') {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Wrong input. Try again..." << std::endl;
        }
    } while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n');

    if (choice == 'Y' || choice == 'y') {
        if (performCheckout(stmt, cart)) {
            printGreen("The order is successfully completed.");
            return true;
        }
    }
    else {
        printRed("The order is cancelled.");
    }

    return false;
}

// Simulates a full user session from login to cart handling and checkout.
// If the customer exists, it loads their cart, displays it, and processes checkout.
// Upon success, it saves a receipt to file and triggers email sending.
void simulateUser(UserSessionConfig config, ShoppingCart& cart) {
    try {
        oracle::occi::Connection* conn = config.env->createConnection(config.user, config.pass, config.constr);
        oracle::occi::Statement* stmt = conn->createStatement();
        std::cout << "Customer connection is successful" << std::endl;

        int option = 0, id = 0, exists = 0;
        do {
            option = menu();
            switch (option) {
            case 1:
                std::cout << "Enter the customer ID: " << cart.getId() << std::endl;
                exists = customerLogin(stmt, cart.getId());

                if (exists) {
                    fillCartFromDB(cart, stmt);
                    std::cout << cart;

                    if (checkoutPrompt(stmt, cart)) {
                        std::string filename = "receipt" + std::to_string(getRandomNumber()) + "00.txt";
                        std::ofstream out(filename);
                        out << cart;
                        out.close();
                        pythonMailSender(filename); // Send receipt by email
                    }

                    option = 0;
                }
                else {
                    std::cout << "The customer does not exist." << std::endl;
                }
                break;

            case 0:
                std::cout << "Good bye!..." << std::endl;
                break;
            }
        } while (option != 0);

        conn->terminateStatement(stmt);
        config.env->terminateConnection(conn);  
    }
    catch (oracle::occi::SQLException& e) {
        std::cout << e.getErrorCode() << ": " << e.getMessage() << std::endl;
    }
}

