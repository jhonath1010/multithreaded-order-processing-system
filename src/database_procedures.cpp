#include "database_procedures.h"
#include <iostream>

// Authenticates a customer by calling the stored procedure 'find_customer'
// Returns 1 if customer exists, 0 otherwise
int customerLogin(oracle::occi::Statement* stmt, int customerId) {
    stmt->setSQL("BEGIN find_customer(:1, :2); END;");
    stmt->setNumber(1, customerId);
    stmt->registerOutParam(2, oracle::occi::Type::OCCIINT, sizeof(int));
    stmt->executeUpdate();
    return stmt->getInt(2);
}

// Retrieves the price of a product by calling the stored procedure 'find_product'
// Returns the product price, or 0 if not found
double findProduct(oracle::occi::Statement* stmt, int product_id) {
    stmt->setSQL("BEGIN find_product(:1, :2); END;");
    stmt->setNumber(1, product_id);
    stmt->registerOutParam(2, oracle::occi::Type::OCCIDOUBLE, sizeof(double));
    stmt->executeUpdate();
    return stmt->getDouble(2);
}

// Performs the full checkout process:
// 1. Calls 'add_order' to insert a new order
// 2. Iterates through cart items and calls 'add_order_item' for each
// Returns true on success
bool performCheckout(oracle::occi::Statement* stmt, const ShoppingCart& cart) {
    int newOrderId = 0;

    // Insert new order and retrieve the generated order ID
    stmt->setSQL("BEGIN add_order(:1, :2); END;");
    stmt->setNumber(1, cart.getId());
    stmt->registerOutParam(2, oracle::occi::Type::OCCIINT);
    stmt->executeUpdate();
    newOrderId = stmt->getInt(2);

    // Insert each item into the order_items table
    for (std::size_t i = 0; i < cart.size(); ++i) {
        const auto& item = cart.getItem(i);
        stmt->setSQL("BEGIN add_order_item(:1, :2, :3, :4, :5); END;");
        stmt->setNumber(1, newOrderId);                // Order ID
        stmt->setNumber(2, static_cast<int>(i + 1));   // Item ID (sequence)
        stmt->setNumber(3, item.product_id);           // Product ID
        stmt->setNumber(4, item.quantity);             // Quantity
        stmt->setDouble(5, item.price);                // Unit price
        stmt->executeUpdate();
    }

    return true;
}
