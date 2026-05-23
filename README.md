# Restaurant Order System

A console-based Restaurant Order System developed in C++ using Object-Oriented Programming and File Handling concepts as part of the NexGen Skills Hub C++ Internship (Week 3).

---

## Overview

This project simulates a real-world restaurant ordering workflow. Customers can place food orders, view itemized bills, and all order records are saved and retrieved using file handling.

---

## Features

- View full food menu with prices
- Place new orders with customer name, items, and quantities
- Auto-generate itemized bill with grand total
- Save all orders to a file and view them anytime
- Search any order by its Order ID
- Delete all saved records
- Input validation and error handling throughout

---

## OOP Classes

| Class | Responsibility |
|---|---|
| `MenuItem` | Stores food item ID, name, and price |
| `OrderItem` | Stores selected item with quantity and unit price |
| `Order` | Holds complete order details, calculates grand total, prints bill |
| `FileManager` | Handles all file read, write, and delete operations |
| `Restaurant` | Main controller — runs the menu-driven interface |

---

## File Handling

Orders are saved to `orders.txt` using `fstream`:

- `ofstream` with `ios::app` — appends each new order to the file
- `ifstream` — reads all saved orders for display and search
- `ios::trunc` — clears the file when deleting all records

Each order is stored in a structured format using `ORDER_START` / `ORDER_END` markers with pipe-separated item fields.

---

## How To Run

**Dev-C++**
Open `restaurant_order_system.cpp` and press `F11` to compile and run.

---

## Sample Output

```
  ============================================
       WELCOME TO BITES & DELIGHTS
         Restaurant Order System
  ============================================

  ========== MAIN MENU ==========
  [1] View Menu
  [2] Place New Order
  [3] View All Orders
  [4] Search Order by ID
  [5] Delete All Records
  [0] Exit
  ================================
  Enter your choice: 2

  === PLACE NEW ORDER ===
  Enter customer name: Minahil Zahra

  ============================================
  Order ID   : 1
  Customer   : Minahil Zahra
  Date       : 2026-05-23 03:07
  --------------------------------------------
  Item                Qty   Price     Total
  --------------------------------------------
  Chicken Burger      2     Rs.350.00  Rs.700.00
  Soft Drink          1     Rs.100.00  Rs.100.00
  --------------------------------------------
  Grand Total : Rs. 800.00
  ============================================
```

---

## Concepts Covered

- Classes and Objects
- File Handling (fstream, ifstream, ofstream)
- Vectors
- String manipulation using istringstream
- Menu-driven interface
- Input validation and error handling
- Modular code design

---

## Author

**Syeda Minahil Zahra**  
C++ Development Intern — NexGen Skills Hub
