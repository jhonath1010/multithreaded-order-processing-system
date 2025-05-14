#include <iostream>
#include <fstream>
#include "cart.h"
#include "database_procedures.h"
#include "console_color.h"

// Parses a CSV-like string to construct a shopping cart object
// Format: customerID,prod1/qty1|prod2/qty2|...,Y/N
ShoppingCart::ShoppingCart(const std::string str) {
    size_t first_comma = str.find(',');
    size_t last_comma = str.rfind(',');

    if (first_comma == std::string::npos || last_comma == std::string::npos) return;

    // Extract customer ID
    m_id = static_cast<unsigned int>(std::stoul(str.substr(0, first_comma)));

    // Determine checkout flag
    m_checkout = (str.at(last_comma + 1) == 'Y' || str.at(last_comma + 1) == 'y');

    // Parse item list
    std::string items_part = str.substr(first_comma + 1, last_comma - first_comma - 1);
    size_t start = 0, sep;

    while ((sep = items_part.find('|', start)) != std::string::npos) {
        std::string pair = items_part.substr(start, sep - start);
        size_t slash = pair.find('/');
        if (slash != std::string::npos) {
            int id = std::stoi(pair.substr(0, slash));
            int qty = std::stoi(pair.substr(slash + 1));
            m_items.push_back({ id, 0.0, qty });
        }
        start = sep + 1;
    }

    // Parse the last item
    std::string last = items_part.substr(start);
    size_t slash = last.find('/');
    if (slash != std::string::npos) {
        int id = std::stoi(last.substr(0, slash));
        int qty = std::stoi(last.substr(slash + 1));
        m_items.push_back({ id, 0.0, qty });
    }

    m_more_items = m_items.size() > 1;
}

// Sets the price for a single item
void ShoppingCart::addItemPrice(Item& item, double price) {
    item.price = price;
}

// Fills the cart by validating product IDs against the database
// and applying their prices; user interaction code is commented for now
void ShoppingCart::fillCart(oracle::occi::Statement* stmt) {
    int prodID = 0, prodQTY = 0, selection = 0;
    std::size_t i = 0;
    int total = m_items.size();

    while (i < total) {
        // Validate product ID
        do {
            std::cout << "Enter the product ID: ";
            // std::cin >> prodID; // used for real user input
            std::cout << m_items[i].product_id << std::endl;

            if (findProduct(stmt, m_items[i].product_id) == 0) {
                std::cout << "The product does not exist. Try again..." << std::endl;
            }
        } while (findProduct(stmt, m_items[i].product_id) == 0);

        // Set item price from DB
        double price = findProduct(stmt, m_items[i].product_id);
        std::cout << "Product Price: " << price << std::endl;

        std::cout << "Enter the product Quantity: " << m_items[i].quantity << std::endl;
        // std::cin >> prodQTY; // used for real user input

        addItemPrice(m_items[i], price);
        ++i;

        if (m_items.size() == MAX_SIZE) break;

        if (i + 1 > total) {
            m_more_items = false;
        }

        std::cout << "Enter 1 to add more products or 0 to checkout: "
            << m_more_items << std::endl;
        // std::cin >> selection; // used for real user input

        if (!m_more_items) break;
    }
}

// Displays the shopping cart (delegates to operator<<)
void ShoppingCart::display() const {
    std::cout << *this;
}

// Returns the number of items in the cart
std::size_t ShoppingCart::size() const {
    return m_items.size();
}

// Returns a reference to the item at the specified index
const ShoppingCart::Item& ShoppingCart::getItem(std::size_t index) const {
    return m_items.at(index);
}

// Returns a const reference to the entire item vector
const std::vector<ShoppingCart::Item>& ShoppingCart::getItems() const {
    return m_items;
}

// Returns true if the cart is flagged for checkout
bool ShoppingCart::isCheckoutEnabled() const {
    return m_checkout;
}

// Returns true if the user has more items to add
bool ShoppingCart::hasMoreItems() const {
    return m_more_items;
}

// Sets whether the user wants to add more items
void ShoppingCart::setMoreItems(bool more) {
    m_more_items = more;
}

// Updates the price of the item at the given index
void ShoppingCart::setItemPrice(std::size_t index, double price) {
    if (index < m_items.size()) {
        m_items[index].price = price;
    }
}

// Returns the customer ID associated with the cart
unsigned int ShoppingCart::getId() const {
    return m_id;
}

// Prints the contents of the cart and computes the total
std::ostream& operator<<(std::ostream& os, const ShoppingCart& cart) {
    double total = 0.0;
    os << "------- Ordered Products ---------" << std::endl;

    for (std::size_t i = 0; i < cart.m_items.size(); ++i) {
        os << "---Item " << i + 1 << std::endl;
        os << "Product ID: " << cart.m_items[i].product_id << std::endl;
        os << "Price: " << cart.m_items[i].price << std::endl;
        os << "Quantity: " << cart.m_items[i].quantity << std::endl;
        total += cart.m_items[i].price * cart.m_items[i].quantity;
    }

    os << "Total: " << total << std::endl;
    return os;
}
