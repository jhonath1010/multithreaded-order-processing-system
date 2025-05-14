#ifndef DATABASE_PROCEDURES_H
#define DATABASE_PROCEDURES_H

#include <occi.h>
#include "cart.h"

// Verifies if a customer exists using the stored procedure 'find_customer'
int customerLogin(oracle::occi::Statement* stmt, int customerId);

// Retrieves the product price from the database using 'find_product'
double findProduct(oracle::occi::Statement* stmt, int product_id);

// Executes the order and inserts items using 'add_order' and 'add_order_item'
bool performCheckout(oracle::occi::Statement* stmt, const ShoppingCart& cart);

#endif 