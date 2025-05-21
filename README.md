
# 🧵 Multithreaded Order Processing System

A high-performance C++ application designed to process customer orders concurrently using multithreading and Oracle's OCCI interface. It reads orders from a CSV file, processes them using stored procedures, generates text-based receipts, and passes them to a Python script to convert to PDF and send via email.

## 📋 Table of Contents

- [About the Project](#about-the-project)
- [Features](#features)
- [Built With](#built-with)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
  - [Running the Application](#running-the-application)
- [Project Structure](#project-structure)
- [Use Case](#use-case)
- [License](#license)
- [Acknowledgments](#acknowledgments)

## 📖 About the Project

This project demonstrates real-world integration of a multithreaded C++ application with an Oracle database and Python automation. It simulates concurrent users processing online orders, creates receipt files, and automates email dispatch with receipt attachments.

## 🚀 Features

- **Multithreading**: Processes multiple shopping carts in parallel using threads.
- **Oracle OCCI Integration**: Communicates with Oracle DB using stored procedures.
- **Receipt Generation**: Creates `.txt` receipt files from transaction data.
- **Python Email Dispatch**: Converts `.txt` to `.pdf` and emails it via Gmail SMTP.

## 🛠️ Built With

- C++ (Multithreading, Oracle OCCI)
- Oracle 12c Database
- Python 3 (smtplib, email, fpdf)
- Shell scripting (for compilation and environment setup)

## 🏁 Getting Started

### Prerequisites

- C++11 compatible compiler
- Oracle Instant Client (`basic` and `sdk` packages)
- Python 3.x
- Python libraries:
  - `fpdf`
  - (Built-in) `smtplib`, `email`

### 🔧 Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/jhonath1010/multithreaded-order-processing-system.git
   cd multithreaded-order-processing-system
   ```

2. **Set up the C++ OCCI environment**:
   - Download Oracle Instant Client from: https://www.oracle.com/database/technologies/instant-client.html
   - Install both:
     - `instantclient-basic`
     - `instantclient-sdk`
   - Set up environment variables:
     ```bash
     export ORACLE_HOME=/path/to/instantclient_19_8
     export LD_LIBRARY_PATH=$ORACLE_HOME
     export PATH=$ORACLE_HOME:$PATH
     ```

   - Compile with OCCI:
     ```bash
     g++ -std=c++11 -I$ORACLE_HOME/sdk/include -L$ORACLE_HOME -locci -lclntsh -pthread src/*.cpp -o order_processor
     ```

3. **Set up the Python environment**:
   ```bash
   cd PythonScript
   pip install fpdf
   ```

---

### ▶️ Running the Application

1. **Prepare your input file** (e.g., `orders.csv`) with one shopping cart per line.  
   **Format:**
   ```
   customerID,productID1/quantity1|productID2/quantity2|...,Y|N
   ```

   - `customerID`: ID of the customer placing the order  
   - `productID/quantity`: Product ID and quantity in the cart  
   - `|`: Separator between multiple products  
   - `Y` or `N`: Indicates whether the customer wants to proceed to checkout (`Y` = Yes, `N` = No)

   **Example:**
   ```
   1234,11/2|12/3|14/1,Y
   ```

   This means customer `1234` wants to buy:
   - 2 units of product 11  
   - 3 units of product 12  
   - 1 unit of product 14  
   and **does want to proceed with the checkout**.

2. **Run the C++ application**:
   ```bash
   ./order_processor orders.csv
   ```
   This creates a `.txt` receipt file per order using Oracle DB stored procedures.  
   Each line in `orders.csv` is processed by a separate thread.

3. **Send the receipt via email**:
   ```bash
   cd PythonScript
   python test.py ../receipts/receipt_001.txt
   ```
   > The Python script takes the `.txt` file path as an argument, converts it to `.pdf`, and sends it via Gmail SMTP.

---

## 🗂️ Project Structure

```
multithreaded-order-processing-system/
├── include/
├── src/
├── receipts/                    # Generated .txt and .pdf files
├── orders.csv
├── PythonScript/
│   └── test.py                  # Converts and emails receipts
├── ConsoleApplicationOracle.sln
├── README.md
└── LICENSE.txt
```

---

## 🎯 Use Case

A customer with ID `1234` adds 3 products to their cart and checks out.  
The system:

    1. Verifies the customer via stored procedure.
    2. Validates each product and retrieves prices.
    3. Inserts the order and items into the Oracle database.
    4. Generates a receipt `.txt` file.
    5. Calls a Python script to convert the receipt to `.pdf` and email it to the user.

---

## 📄 License

This project is licensed under the MIT License.

---

## 🙏 Acknowledgments

- Inspired by real-world use cases in e-commerce order systems.
- Special thanks to the developers of Oracle Instant Client and FPDF.
