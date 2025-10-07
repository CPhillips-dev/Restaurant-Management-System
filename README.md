# C++ Restaurant Management System

![Status](https://img.shields.io/badge/status-active-success)

This program is a console-based application written in C++ that simulates a simple restaurant order management system. It allows a user to manage multiple tables, place orders for guests, track the status of each order, and process payments, including generating a unique receipt for each transaction.

## Features

* **Table Management:** Manages up to 4 tables, each with a capacity of 4 guests.
* **Order Placement:** Place itemized orders for guests at any table with available seating.
* **Status Tracking:** Track the state of each order through multiple stages: `seated`, `awaiting completion`, `awaiting payment`, and `all done`.
* **Dynamic Menu:** The main menu intelligently displays options based on the current state of orders (e.g., "Complete Order" only appears if an order has been placed).
* **Billing Calculation:** Automatically calculates the subtotal, a 10% tax, a 20% tip, and the final total for each order.
* **Receipt Generation:** Upon successful payment, generates a unique, itemized receipt saved to a `.txt` file (e.g., `Transaction#1234.txt`).
* **Input Validation:** Ensures user input is within a valid range for all menu selections and prompts.

## Getting Started

Follow these instructions to compile and run the program on your local machine.

### Prerequisites

You will need a C++ compiler installed on your system. The following instructions use `g++`, which is part of the GCC (GNU Compiler Collection).

#### Windows: Installing MinGW-w64

For Windows users, you can install the `g++` compiler via MinGW-w64. Here is a summary of the steps from the [official VS Code documentation](https://code.visualstudio.com/docs/cpp/config-mingw):

1.  **Download MSYS2:** Go to the [MSYS2 website](https://www.msys2.org/) and download the latest installer.
2.  **Run the Installer:** Follow the installation prompts. By default, this will install to `C:\msys64`.
3.  **Install the Toolchain:** After installation, an MSYS2 terminal will open. Run the following command to install the MinGW-w64 toolchain:
    ```sh
    pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
    ```
    Accept the default selections to proceed.
4.  **Update Windows PATH:**
    * In the Windows search bar, type "Edit environment variables for your account" and open it.
    * Select the `Path` variable under "User variables" and click "Edit".
    * Click "New" and add the path to your MinGW-w64 bin folder. The default path is `C:\msys64\ucrt64\bin`.
    * Click "OK" to save the changes.
5.  **Verify Installation:** Open a **new** Command Prompt or PowerShell window and run `g++ --version`. You should see output indicating that the compiler has been successfully installed.

### Compilation

1.  Navigate to the directory containing the `main.cpp` file in your terminal.
2.  Run the following command to compile the program:

    ```sh
    g++ main.cpp -o restaurant_manager
    ```

### Execution

Once compiled, run the application from your terminal with the following command:

```sh
# On Windows Command Prompt
restaurant_manager.exe

# On PowerShell, Git Bash, or Linux/macOS
./restaurant_manager

## How to Use

1.  Launch the program to see the main menu.
2.  Select option **1. Enter Order** to begin.
3.  Enter the table number (1-4) and the number of guests you wish to seat.
4.  Select menu items for each guest.
5.  After an order is placed, new options will appear. Select **2. Complete Order** and specify the table number to mark the order as ready for payment.
6.  Select **3. Calculate and Pay Bill**. Enter the table number to view the itemized bill.
7.  Confirm the payment by entering `y`.
8.  A confirmation message will appear, indicating the name of the receipt file (e.g., `Transaction#4815.txt`) that has been saved in the same directory.
9.  Once all orders are completed and paid, option **4. Close the Restaurant** will become available to exit the program.
