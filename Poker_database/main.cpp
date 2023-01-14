#include <iostream>

#include "Hand.h"
#include "Game.h"
#include "Replay.h"
#include "../../VS_CODE/Poker_VS/Poker_VS/Query.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <unordered_set>
#include <random>
#include <stdio.h>
#include <sqlite3.h>


static int createDB(const char* s);
static int createTable(const char* s);
static int insertData(const char* s);
static int selectData(const char* s);
static int callback(void* NotUsed, int argc, char** argv, char** azColName);
static std::vector<int> request_cards(int num_cards, Game g);
static void display();

//static int test_column(const char *s);

using namespace std;

int counter = 0;
static const int options = 5;

int main() {

    Query q{};
    string input{};
    int check{};
    Game g{};
    Replay r{};
    vector<int> cards{};
    cout << "Welcome to the poker database, along with storing hands and allowing for complex searches and replays we also have " << endl << "many other " <<
        "features you may find interesting" << endl;
    while (true) {
        display();
        cin >> input;
        if (input == "Q" || input == "q") {
            cout << "Leaving the poker database" << endl;
            break;
        }
        check = g.check_input_int(input); 
        if (check<=0 || check >options) {
            cout << "invalid input" << endl;
            continue;
        }
        switch (check) {
            case 1://search db
                q.request_info();
                break;
            case 2://enter hands in db
                cout << "Enter the POV of the hand you are inputing: " << endl;
                cin >> input;
                g.play_hands(input);
                break;
            case 3:
                cards = request_cards(2, g);
                if (cards.size() == 0) {
                    break;
                }
                while (true) {
                    cout << endl << "How many players (up to 8): ";
                    cin >> input;
                    check = g.check_input_int(input);
                    if (check <= 0 || check > 8) {
                        cout << "invalid input" << endl;
                        continue;
                    }
                    break;
                }
                g.num_players = check;
                cout << "pre flop odds of winning are " <<  g.read_pre_flop_db(cards.at(0), cards.at(1), "C:\\Users\\jeeva\\SQLite\\Poker.db") << endl;
                break;
            case 4:
                cards = request_cards(4, g);
                cout << "head to head odds are: " << r.hand.pre_flop_vs(cards, "C:\\Users\\jeeva\\SQLite\\Poker.db", 100000) << endl;
                break;
            case 5:
                cout << "Would you like flop, turn or river?" << endl;
                while (true) {
                    cout << "Enter 1 for flop, 2 for turn, 3 for river" << endl;
                    cin >> input;
                    check = g.check_input_int(input);
                    if (check <= 0 || check > 3) {
                        cout << "invalid input" << endl;
                        continue;
                    }
                    break;
                }
                switch (check) {
                case 1:
                    cards = request_cards(5, g);
                    break;
                case 2:
                    cards = request_cards(6, g);
                    break;
                case 3:
                    cards = request_cards(7, g);
                    break;
                default:
                    cout << "PROBLEM" << endl;
                    return -1;
                }
                if (cards.size() == 0) {
                    break;
                }
                while (true) {
                    cout << "how many players (up to 9)" << endl;
                    cin >> input;
                    check = g.check_input_int(input);
                    if (check <= 0 || check > 9) {
                        cout << "invalid input" << endl;
                        continue;
                    }
                    break;
                }
                cout << "odds for the hand are: " << r.calc_odds(cards, 100000, "C:\\Users\\jeeva\\SQLite\\Poker.db", check) << endl;
                break;
            default:
                cout << "PROBLEM" << endl;
                return -1;
        }
    }

    return 0;
}

static vector<int> request_cards(int num_cards, Game g) {
    int value{};
    char suit{};
    vector<int> cards{};
    int check{};
    string input{};
    cout << "please input your cards (or q to quit)" << endl;
    for (int i = 0; i < num_cards; i++) {

        while (true) {
            cout << "card " << (i + 1) << " value (jack = 11, queen = 12, king = 13): ";
            cin >> input;
            if (input == "Q" || input == "q") {
                cout << "quitting" << endl;
                return {};
            }
            check = g.check_input_int(input);
            if (check < 0) {
                cout << "invalid input" << endl;
                continue;
            }
            value = check;
            cout << "suit(H for heart, D for diamond, S for spade, C for club) : ";
            cin >> suit;
            check = g.get_index(value, suit);
            if (check >= 0) {
                break;
            }
            cout << "invalid input" << endl;
        }

        if (find(cards.begin(), cards.end(), check) == cards.end()) {
            cards.push_back(check);
        }
        else {
            cout << "card has already been played" << endl;
            i--;
        }
        //cout << endl;
    }
    cout << endl;
    /*g.read_pre_flop_db(cards.at(0), cards.at(1), "C:\\Users\\jeeva\\SQLite\\Poker.db");*/
    return cards;
}

static void display() {
    cout << "Enter 1 to search the poker hand database " << endl;
    cout << "Enter 2 to enter hands into the poker hand database " << endl;
    cout << "Enter 3 to find pre-flop win percentages based on the number of players" << endl;
    cout << "Enter 4 to find the head to head pre-flop win percentages" << endl;
    cout << "Enter 5 to find baseline flop, turn, or river win percentages" << endl;
    cout << "Press q to quit" << endl;
}

static int createDB(const char* s) {
    sqlite3* db;
    int exit = 0;

    exit = sqlite3_open(s, &db);

    sqlite3_close(db);

    return 0;

}

static int createTable(const char* s) {
    sqlite3* db;

    string sql = "create table if not exists hands("
        "id integer primary key autoincrement, "
        "n_players              int not null, "
        "pre_stack_amounts      text not null, "
        "post_stack_amounts     int not null, "
        "position               int not null, "
        "names                  text not null, "
        "big_blind              int not null, "
        "small_blind            int not null, "
        "bet_amounts            text not null, "
        "pre_flop_pot           int not null, "
        "flop_pot               int not null, "
        "turn_pot               int not null, "
        "river_pot              int not null, "
        "player_card_one        int not null, "
        "player_card_two        int not null, "
        "cards                  text not null, "
        "showdown               int not null, "
        "last_action            int not null, "
        "folded                 text not null );";


    try {
        int exit = 0;
        exit = sqlite3_open(s, &db);

        char* messageerror;
        exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageerror);

        if (exit != SQLITE_OK) {
            cerr << "error creating table" << endl;
            sqlite3_free(messageerror);
        }
        else {
            cout << "table created successfully" << endl;

        }
        sqlite3_close(db);
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
    }

    return 0;
}

static int insertData(const char* s) {
    sqlite3* db;

    char* messageerror;

    int exit = sqlite3_open(s, &db);

    string test = "[[0,0,0],[0.0.1]";

    string sql = "insert into grades (name, lname, age, address, grade) values('alice', 'chapa', 35, '" + test + "', 'a');";
    cout << sql << endl;

    exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageerror);

    if (exit != SQLITE_OK) {
        cerr << "error with insert" << endl;
        sqlite3_free(messageerror);
    }
    else {
        cout << "record inserted successfully" << endl;
    }
    return 0;
}

static int selectData(const char* s) {
    sqlite3* db;

    int exit = sqlite3_open(s, &db);

    string sql = "select * from hands";

    //an open dataabase, sql to be evaluated, callback function, 1st argument to callback, error msg written here

    sqlite3_exec(db, sql.c_str(), callback, NULL, NULL);//callback is called once for each entry we are selecting from the db

    return 0;
}




//retrive contents of database used by selectdata();
//argc holds the number of results, azcolname holds each column return in array, argv holds each value in array

static int callback(void* notused, int argc, char** argv, char** azcolname) {
    for (int i = 0; i < argc; i++) {
        //column name and value
        cout << azcolname[i] << ": " << argv[i] << endl;
    }

    cout << endl;

    counter++;

    return 0;
}

