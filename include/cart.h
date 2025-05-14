#ifndef CART_H
#define CART_H

#include <vector>
#include <string>
#include <iostream>
#include <occi.h>

// Represents a shopping cart loaded from a file or user input.
// Supports operations like pricing items, printing receipts, and database checkout.
class ShoppingCart {
public:
    // Represents an individual item in the cart
    struct Item {
        int product_id;
        double price;
        int quantity;
    };

private:
    unsigned int m_id;               // Customer ID
    std::vector<Item> m_items;       // List of items in the cart
    static const int MAX_SIZE = 5;   // Maximum number of items allowed
    bool m_more_items;               // Whether more items can be added
    bool m_checkout;                 // Whether the cart should be checked out

public:
    ShoppingCart() = default;

    // Parses a CSV-style string to initialize the cart
    explicit ShoppingCart(const std::string str);

    void addItemPrice(Item& item, double price);
    void fillCart(oracle::occi::Statement* stmt);
    void display() const;
    size_t size() const;

    const Item& getItem(size_t index) const;
    const std::vector<Item>& getItems() const;

    bool isCheckoutEnabled() const;
    bool hasMoreItems() const;

    void setMoreItems(bool more);
    void setItemPrice(size_t index, double price);
    unsigned int getId() const;


    friend std::ostream& operator<<(std::ostream& os, const ShoppingCart& cart);
};

#endif
