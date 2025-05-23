#include "sql_setup.h"

void createProcedures(oracle::occi::Statement* stmt) {
    stmt->execute("CREATE OR REPLACE PROCEDURE find_customer (p_customer_id IN NUMBER, found OUT NUMBER) IS"
        " v_custid NUMBER; BEGIN found:=1; SELECT customer_id INTO v_custid FROM customers WHERE customer_id=p_customer_id;"
        " EXCEPTION WHEN NO_DATA_FOUND THEN found:=0; END;");

    stmt->execute("CREATE OR REPLACE PROCEDURE find_product(p_product_id IN NUMBER, price OUT products.list_price%TYPE) IS"
        " BEGIN SELECT list_price INTO price FROM products WHERE product_id=p_product_id;"
        " EXCEPTION WHEN NO_DATA_FOUND THEN price:=0; END;");

    stmt->execute("CREATE OR REPLACE PROCEDURE add_order(p_customer_id IN NUMBER, new_order_id OUT NUMBER) IS"
        " BEGIN SELECT MAX(order_id) INTO new_order_id FROM orders; new_order_id:=new_order_id+1;"
        " INSERT INTO orders VALUES(new_order_id, p_customer_id, 'Shipped', 56, sysdate); END;");

    stmt->execute("CREATE OR REPLACE PROCEDURE add_order_item(orderId IN order_items.order_id%TYPE,"
        " itemId IN order_items.item_id%TYPE, productId IN order_items.product_id%TYPE,"
        " quantity IN order_items.quantity%TYPE, price IN order_items.unit_price%TYPE) IS"
        " BEGIN INSERT INTO order_items VALUES(orderId, itemId, productId, quantity, price); END;");
}
