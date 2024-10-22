# <p>Password🔑Manager</p>

# Key Features:
* __User Authentication__: User's can log in with a master password, which is securely hashed for protection
* __Password Storage__: Users can add, view, and delete passwords for various applications
* __Encryption and Decryption__: Passwords are encrypted using OpenSSL, ensuring that sensitive information remains confidential and protected against unauthorized access
* __User-Friendly Interface__: Designed with simplicity in mind, allowing users to easily and quickly navigate and manage their passwords

# OOP Principles:
* __Encapsulation__: Sensitive data and functionality are encapsulated within classes, ensuring that access is controlled and modifications are managed through designated methods
* __Polymorphism__: The application can handle different password types through a common interface, enabling flexibility in how passwords are managed

# Makefile:
* Implemented a makefile to streamline build process by automating source code compilation, includes rules and dependencies to ensure the app is built efficiently
* Assist in cleaning codespace by removing unwanted files

# Usage:
* __1st log in__: The first time logging in you will be prompted to create a password to access the app, after creating it run the program again to log in and have full access
* __Passwords__: They can be set to anything you think of
* __Resetting__: If you want to delete your passwords or delete your master password to create a new one, delete the masterpasswordhash.txt and storedpasswordhash.txt in order to restart the app with no saved data