# 🔐 Simple Brute Force Password Finder (C)

## 📌 Overview

This is a small C program that demonstrates a basic **brute-force attack** on a 3-letter lowercase password.
It systematically tries all possible combinations from `aaa` to `zzz` until it finds the correct password.

## ⚙️ Features

* Tries all 3-letter lowercase combinations (`a-z`)
* Compares each attempt with a target password
* Stops immediately when the password is found
* Displays the number of attempts made
* ⏱️ Measures execution time

## 🧠 How It Works

The program uses three nested loops to generate every possible combination of letters:

* First loop → first character
* Second loop → second character
* Third loop → third character

Each generated string is compared using `strcmp()`.

## 🛠️ Requirements

* C compiler (e.g., `gcc`)
* Works on Windows, Linux, and macOS

## ▶️ How to Compile & Run

```bash
gcc main.c -o bruteforce
./bruteforce
```

## 🧪 Example Output

```
Password found: abc
Attempts: 731
```

## ⚠️ Disclaimer

This project is for **educational purposes only**.
It demonstrates how brute-force attacks work so you can better understand the importance of strong passwords.

## 🚀 Possible Improvements

* Support longer passwords
* Add uppercase letters, numbers, and symbols
* Measure execution time
* Multi-threading for faster performance

## 📄 License

Totally free to use! Play around with it and make it your own 😄

