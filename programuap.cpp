#include <ncurses/ncurses.h>
#include <unistd.h> // Untuk usleep()
#include <string.h> // Untuk strlen()
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h> // Untuk warna dan delay
#include <thread>    // Untuk animasi teks
#include <chrono>    // Untuk delay animasi
#include <cstdlib>   // Untuk random number
#include <ctime>     // Untuk seed random number

using namespace std;

#define DELAY 200000 // Delay dalam mikrodetik

// Fungsi untuk mencetak teks dengan warna tertentu
void print_text(const char* text, int y, int x, int color_pair) {
    attron(COLOR_PAIR(color_pair));
    mvprintw(y, x, "%s", text);
    attroff(COLOR_PAIR(color_pair));
}

// Fungsi untuk menampilkan progress bar dengan animasi rotasi
void progress_bar(int y, int x, int length, int duration) {
    char spinner[] = {'|', '/', '-', '\\'}; // Karakter rotasi
    for (int i = 0; i < length; i++) {
        // Cetak spinner
        print_text(">", y, x + i, 1);
        mvprintw(y, x + length + 2, "[%c]", spinner[i % 4]);

        // Refresh layar
        refresh();
        usleep(duration);
    }
}

// Function prototypes
void registration();
void login();
void viewHighScore(); // Function to view high score
void startGame(const string &username); // Game function
int main();

// Function to set text color
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Function for animated text
void animatedText(const string &text, int speed = 50) {
    for (char c : text) {
        cout << c;
        Sleep(speed); // Fungsi Sleep menggunakan waktu dalam milidetik
    }
    cout << endl;
}

// Permainan Tebak Angka
void startGame(const string &username) {
    system("cls");
    setColor(14); // Yellow
    animatedText("-------------> GAME STARTED <--------------");
    setColor(15); // White

    // Seed random number generator
    srand(static_cast<unsigned>(time(0)));

    int randomNumber = rand() % 100 + 1; // Angka acak 1-100
    int guess, attempts = 0;
    int maxAttempts = 10;

    setColor(11); // Cyan
    cout << "Guess the number (1-100)! You have " << maxAttempts << " attempts.\n\n";
    setColor(15); // White

    while (attempts < maxAttempts) {
        cout << "Attempt " << (attempts + 1) << ": ";
        cin >> guess;
        attempts++;

        if (guess == randomNumber) {
            setColor(10); // Green
            cout << "\nCongratulations! You've guessed the number correctly in " << attempts << " attempts.\n";
            setColor(15); // White
            break;
        } else if (guess < randomNumber) {
            setColor(14); // Yellow
            cout << "Too low! Try again.\n";
        } else {
            setColor(12); // Red
            cout << "Too high! Try again.\n";
        }
        setColor(15); // White
    }

    if (guess != randomNumber) {
        setColor(12); // Red
        cout << "\nGame Over! The correct number was: " << randomNumber << endl;
        setColor(15); // White
    }

    // Calculate score based on attempts
    int score = (maxAttempts - attempts + 1) * 10; // Semakin sedikit percobaan, semakin tinggi skor
    if (score < 0) score = 0; // Skor minimum 0

    // Update user's top score
    ifstream scoreCheck(username + ".txt");
    int currentScore;
    scoreCheck >> currentScore;
    scoreCheck.close();

    if (score > currentScore) {
        ofstream scoreFile(username + ".txt");
        scoreFile << score;
        scoreFile.close();
        setColor(11); // Cyan
        cout << "\nNew High Score: " << score << "!" << endl;
    } else {
        setColor(12); // Red
        cout << "\nYour High Score remains: " << currentScore << "." << endl;
    }
    setColor(15); // Reset to white
    system("pause");
    main(); // Return to main menu
}

// Fungsi untuk melihat skor tertinggi
void viewHighScore() {
    ifstream accList("ListAccount.txt");
    string username;
    bool found = false;

    system("cls");
    setColor(14); // Yellow
    animatedText("-------------> VIEW HIGH SCORES <--------------");
    setColor(15); // White

    cout << "Enter your username to check your high score: ";
    cin >> username;

    ifstream scoreFile(username + ".txt");
    if (scoreFile.is_open()) {
        int highScore;
        scoreFile >> highScore;
        scoreFile.close();

        setColor(10); // Green
        cout << "\nHigh Score for " << username << ": " << highScore << endl;
        setColor(15); // Reset to white
    } else {
        setColor(12); // Red
        cout << "\nError: Username not found or no high score recorded!" << endl;
        setColor(15); // Reset to white
    }

    system("pause");
    main(); // Return to main menu
}

// Fungsi pendaftaran akun
void registration() {
    ofstream myFile, accList;
    string userReg;

    system("cls");
    setColor(10); // Green
    animatedText("------------->REGISTER<--------------");
    setColor(15); // White
    cout << "Enter username: ";
    cin >> userReg;

    myFile.open(userReg + ".txt");
    myFile << 0; // Initialize user score to 0
    myFile.close();

    accList.open("ListAccount.txt", ios::app);
    accList << userReg << endl;
    accList.close();

    system("cls");
    setColor(11); // Cyan
    animatedText("Registration successful!");
    setColor(15); // Reset to white
    main(); // Return to main menu
}

// Fungsi login pengguna
void login() {
    ifstream myFile;
    string userLog;
    string userData;
    bool exist = false;

    system("cls");
    setColor(14); // Yellow
    animatedText("------------->LOGIN<--------------");
    setColor(15); // White
    cout << "Enter username: ";
    cin >> userLog;

    myFile.open("ListAccount.txt");
    while (myFile >> userData) {
        if (userData == userLog) {
            exist = true;
            break;
        }
    }
    myFile.close();

    if (exist) {
        system("cls");
        setColor(10); // Green
        animatedText("Login successful!");
        setColor(15); // White
        startGame(userLog); // Mulai permainan setelah login
    } else {
        system("cls");
        setColor(12); // Red
        animatedText("---------------------------------------");
        animatedText("               LOGIN ERROR             ");
        animatedText("Please check your username and try again");
        animatedText("---------------------------------------");
        setColor(15); // Reset to white
        main(); // Return to main menu
    }
}

int main() {
    // Bagian ncurses
    initscr();
    noecho();
    curs_set(0);
    start_color();

    // Definisi warna
    init_pair(1, COLOR_GREEN, COLOR_BLACK);  // Warna progress bar
    init_pair(2, COLOR_RED, COLOR_BLACK);    // Warna teks "Loading..."
    init_pair(3, COLOR_YELLOW, COLOR_BLACK); // Warna teks tambahan

    // Bersihkan layar dan cetak teks "Loading"
    clear();
    print_text("Loading...", 10, 30, 2);
    print_text("--------------------", 11, 30, 3);
    print_text("|                  |", 12, 30, 3);
    print_text("--------------------", 13, 30, 3);

    // Tampilkan progress bar
    progress_bar(12, 31, 18, DELAY);

    // Cetak pesan selesai
    clear();
    print_text("Done!", 10, 35, 1);
    refresh();

    usleep(2000000); // Tunggu sebelum keluar
    endwin();

    // Menu utama
    int choice;
    setColor(13); // Purple
    animatedText("=====================================");
    animatedText("          WELCOME TO THE GAME       ");
    animatedText("=====================================");
    setColor(15); // White
    cout << "1. Registration" << endl;
    cout << "2. Login" << endl;
    cout << "3. View High Score" << endl; // Opsi untuk melihat high score
    cout << "Input your choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            registration();
            break;
        case 2:
            login();
            break;
        case 3:
            viewHighScore(); // Panggil fungsi untuk melihat high score
         break;
}
	return 0; // Akhir dari fungsi main
}
