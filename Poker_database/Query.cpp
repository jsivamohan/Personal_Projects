#include "Query.h"
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <sqlite3.h>

using namespace std;

Query::Query() :r{ Replay{} }, g{ Game{} } {}

vector<Replay> Query::interpret_extract(vector<string>& col_vals, int num_entries) {//returns a vector of replay objects of size num_entries created from the extracted info
    //need columns index 1-18 (index 0 is just the id)
    vector<vector<string>> entries{};//vector where each sub-vector is of length 19 and is the parameters for a specific replay object
    vector<Replay> replays{};
    for (int i = 0; i < num_entries; i++) {
        entries.push_back({});
    }
    for (int i = 0; i < num_entries; i++) {
        for (int j = 0; j < num_cols;j++) {
            entries.at(i).push_back(col_vals.at(j * num_entries + i));
        }
    }

    for (int i = 0; i < entries.size(); i++) {
        replays.push_back(create_object(entries.at(i)));
    }

    return replays;

}

Replay Query::create_object(std::vector<std::string>& entry) {

    int n_players = stoi(entry.at(N_PLAYERS));

    istringstream iss{ entry.at(PRE_STACK_AMOUNTS) };
    string output{};
    vector<int> s_amounts{};

    while (getline(iss, output, ',')) {
        s_amounts.push_back(stoi(output));
    }

    int position = stoi(entry.at(POSITION));
    string names_s = entry.at(NAMES).substr(1, entry.at(NAMES).size() - 2);
    istringstream iss_name{ names_s };
    vector<string> name_v{};

    while (getline(iss_name, output, ',')) {
        name_v.push_back(output);
    }

    vector<int> blinds_v{};
    blinds_v.push_back(stoi(entry.at(BIG_BLIND)));
    blinds_v.push_back(stoi(entry.at(SMALL_BLIND)));

    istringstream iss_bet{ entry.at(BET_AMOUNTS) };
    vector<vector<vector<int>>> b_amounts{};
    string output_sit{};
    string output_it{};

    int player_counter = 0;
    int sit_counter = 0;

    while (getline(iss_bet, output, '!')) {
        istringstream iss_sit{ output };
        b_amounts.push_back({});
        while (getline(iss_sit, output_sit, '|')) {
            b_amounts.at(player_counter).push_back({});
            istringstream iss_it{ output_sit };
            while (getline(iss_it, output_it, ',')) {
                b_amounts.at(player_counter).at(sit_counter).push_back(stoi(output_it));
            }
            sit_counter++;
        }
        sit_counter = 0;
        player_counter++;
    }

    vector<int> p_sizes{};
    p_sizes.push_back(stoi(entry.at(PRE_FLOP_POT)));
    p_sizes.push_back(stoi(entry.at(FLOP_POT)));
    p_sizes.push_back(stoi(entry.at(TURN_POT)));
    p_sizes.push_back(stoi(entry.at(RIVER_POT)));



    vector<int> c{};
    istringstream iss_c{ entry.at(CARDS) };
    while (getline(iss_c, output, ',')) {
        c.push_back(stoi(output));
    }

    bool g_showdown{};
    if (stoi(output)) {
        g_showdown = true;
    }
    else {
        g_showdown = false;
    }

    int l_action = stoi(entry.at(LAST_ACTION));

    vector<int> f{};
    string folded_s = entry.at(FOLDED).substr(1, entry.at(FOLDED).size() - 2);
    istringstream iss_f{ folded_s };
    while (getline(iss_f, output, ',')) {
        f.push_back(stoi(output));
    }

    vector<double> ss{};
    ss.push_back(stod(entry.at(FLOP_STRENGTH)));
    ss.push_back(stod(entry.at(TURN_STRENGTH)));
    ss.push_back(stod(entry.at(RIVER_STRENGTH)));

    double p_f_w = stod(entry.at(PRE_FLOP_WIN_PER));

    string winner = entry.at(WINNER);

    Replay r{ n_players, s_amounts, position, name_v, blinds_v, b_amounts, p_sizes, c, g_showdown, l_action, f, ss, p_f_w, winner};

    return r;

}

vector<Replay> Query::extract_info(const char* s, const char* sql) {
    vector<string> col_vals{};
    sqlite3* db;
    int test_val{};
    int exit = sqlite3_open(s, &db);

    if (exit != SQLITE_OK) {
        cout << "error with test column" << endl;
    }

    sqlite3_stmt* ppstmt{};
    int ret_val{};

    sqlite3_stmt* stmt;
    int j{};
    for (int i = 0; i < num_cols; i++) {//path of the iterator goes down the column until it can't anymore and then goes to the next one (size of col_vals = 19 * # of entries)
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
            cout << "error with prepare in test" << endl;
        }
        else {
            for (;;) {
                int rc = sqlite3_step(stmt);
                if (rc == SQLITE_DONE)
                    break;
                if (rc != SQLITE_ROW) {
                    cout << "error with step in test" << endl;
                    break;
                }
                const unsigned char* text_char = sqlite3_column_text(stmt, i);
                string text{};
                j = 0;
                while (text_char[j] != '\0') {
                    text += text_char[j];
                    j++;
                }
                col_vals.push_back(text);
            }
            sqlite3_finalize(stmt);
        }
    }
    sqlite3_close(db);
    return interpret_extract(col_vals, (int)(col_vals.size() / num_cols));

}

void Query::request_info() {
    cout << "By what measure would you like to query the database (keep in mind you can request multiple things, through multiple requests)" << endl;
    string input{};
    int option{};
    string append{};
    int check{};
    vector<string> full_sql{};//vector full of bits of sql that will be combined into a full string for query
    string sql = "SELECT * FROM HANDS ";
    while (true) {
        display();
        cin >> input;
        if (input == "Q" || input == "q") {
            cout << "quitting the query" << endl;
            return;
        }
        else if (input == "f" || input == "F") {
            cout << "done with the query" << endl;
            break;
        }
        option = g.check_input_int(input);
        if (option<0 || option > options) {
            cout << "invalid option, try again" << endl;
            continue;
        }
        full_sql.push_back(resolve_query(option));
    }
    if (full_sql.size() > 0) {
        sql += "WHERE ";
    }
    for (int i = 0; i < full_sql.size(); i++) {
        sql += (full_sql.at(i));
        if (i != full_sql.size() - 1) {
            sql += " AND ";
        }
    }
    sql += ";";
    cout << endl;
    vector<Replay> replays = extract_info("C:\\Users\\jeeva\\SQLite\\Poker.db", sql.c_str());
    cout << "This query collected " << replays.size() << " hands that match your request" << endl;

    while (true) {
        cout << "Enter the index of the hand you want to fully replay (enter q to quit)" << endl;
        cout << "Their summaries are display below" << endl;
        cout << "****************************************" << endl;
        for (int i = 0; i < replays.size(); i++) {
            cout << "Hand " << (i + 1) << endl;
            replays.at(i).summary();
            cout << endl << endl;
        }
        cin >> input;
        if (input == "Q" || input == "q") {
            cout << "quitting the query" << endl;
            break;
        }
        check = g.check_input_int(input);
        if (check <= 0 || check > replays.size()) {
            cout << "invalid index" << endl;
            continue;
        }
        cout << "replaying hand " << check << endl;
        replays.at(check-1).replay_hand();
        cout << endl << "Done with replay" << endl;
    }
}

string Query::resolve_query(int option) {
    string input{};
    int check{};
    string pot_string{};
    int pot_size{};
    int position{};
    int n_players{};
    int value{};
    char suit{};
    int situation{};
    bool above{};
    int strength{};
    int blind_type{};
    int blind_amount{};
    switch (option) {
        case 1:
            //card pre-flop
            cout << "What card would you like to search for that you had pre-flop?" << endl;
            while (true) {
                cout << "Card value: ";
                cin >> input;
                check = g.check_input_int(input);
                if (check < 0) {
                    cout << "invalid input" << endl;
                    continue;
                }
                value = check;
                cout << "Card suit: ";
                cin >> suit;
                check = g.get_index(value, suit);
                if (check < 0) {
                    cout << "invalid input" << endl;
                    continue;
                }
                break;
            }
            return "(PLAYER_CARD_ONE = " + std::to_string(check) + " OR PLAYER_CARD_TWO = " + std::to_string(check) + ")";

            break;
        case 2:
            //strength of hand (flop, turn, river)
            cout << "Do you want to look at the strength of the hand at the flop, the turn, or the river?" << endl;
            while (true) {
                cout << "Enter 0 for flop" << endl;
                cout << "Enter 1 for turn" << endl;
                cout << "Enter 2 for river" << endl;
                cin >> input;
                check = g.check_input_int(input);
                if (check < 0 || check >2) {
                    cout << "invalid option" << endl;
                    continue;
                }
                situation = check;
                break;
            }
            cout << "How strong a hand do you want to filter by?" << endl;
            while (true) {
                cout << "Enter 1 for high card hands" << endl;
                cout << "Enter 2 for one pair hands" << endl;
                cout << "Enter 3 for two pair hands" << endl;
                cout << "Enter 4 for three of a kind hands" << endl;
                cout << "Enter 5 for straight hands" << endl;
                cout << "Enter 6 for flush hands" << endl;
                cout << "Enter 7 for full house hands" << endl;
                cout << "Enter 8 for four of a kind hands" << endl;
                cout << "Enter 9 for straight flush hands" << endl;
                cin >> input;
                check = g.check_input_int(input);
                if (check < 0 || check > 9) {
                    cout << "invalid input" << endl;
                    continue;
                }
                strength = check;
                break;
            }
            while(true){
                cout << "Lastly, would you like to look above or below that hand strength?" << endl;
                cout << "Enter 1 for above and 0 for below: ";
                cin >> input;
                check = g.check_input_int(input);
                if (check < 0 || check >1) {
                    cout << "invalid input" << endl;
                    continue;
                }
                above = check;
                break;
            }
            switch (situation) {
                case 0://flop
                    if (above) {
                        return "(FLOP_STRENGTH >= " + std::to_string(strength-1) + ")";
                    }
                    else {
                        return "(FLOP_STRENGTH <= " + std::to_string(strength) + ")";
                    }
                    break;
                case 1://turn
                    if (above) {
                        return "(TURN_STRENGTH >= " + std::to_string(strength-1) + ")";
                    }
                    else {
                        return "(TURN_STRENGTH <= " + std::to_string(strength) + ")";
                    }
                    break;
                case 2://river
                    if (above) {
                        return "(RIVER_STRENGTH >= " + std::to_string(strength-1) + ")";
                    }
                    else {
                        return "(RIVER_STRENGTH <= " + std::to_string(strength) + ")";
                    }
                    break;
                default:
                    cout << "PROBLEM STRENGTH" << endl;
                    return NULL;
            }
            break;
        case 3:
            //showdown or not
            cout << "would you like to see all the hands that reached showdown or not?" << endl;
            while (true) {
                cout << "Enter 1 for the hands that reached showdown, enter 0 for the ones that did not: ";
                cin >> input;
                check = g.check_input_int(input);
                if (check < 0 || check>1) {
                    cout << "invalid input " << endl;
                    continue;
                }
                break;
            }
            if (check) {
                return "(SHOWDOWN = 1)";
            }
            else {
                return "(SHOWDOWN = 0)";
            }
            break;
        case 4:
            //won or not (need to add into table)
            cout << "would you like to search for games in which the POV player won or lost?" << endl;
            while (true) {
                cout << "Enter 1 for won, 0 for lost" << endl;
                cin >> input;
                check = g.check_input_int(input);
                if (check < 0) {
                    cout << "invalid input" << endl;
                }
                break;
            }
            if (check) {//won
                return "(INSTR(WINNER, PLAYER_NAME) > 0)";
            }
            else {//lost
                return "(INSTR(WINNER, PLAYER_NAME) = 0)";
            }
            return NULL;
            break;
        case 5:
            //blinds
            cout << "Would you like to search by big blind or small blind?" << endl;
            while (true) {
                cout << "Enter 1 to search by big blind, enter 2 to search by small blind: ";
                cin >> input;
                check = g.check_input_int(input);
                if (check <= 0 || check > 2) {
                    cout << "Invalid input" << endl;
                    continue;
                }
                blind_type = check-2;
                break;
            }
            cout << "Now, how much did you want to check?" << endl;
            while (true) {
                cout << "Please input the value you would like to check: ";
                cin >> input;
                check = g.check_input_int(input);
                if (check < 0) {
                    cout << "invalid input" << endl;
                }
                blind_amount = check;
                break;
            }
            cout << "Now would you like to check less than that, more than that, or equal to that amount?" << endl;
            while (true) {
                cout << "Enter 1 for less, 2 for more, and 3 for equal: ";
                cin >> input;
                check = g.check_input_int(input);
                if (check <= 0 || check > 3) {
                    cout << "invalid input" << endl;
                    continue;
                }
                break;
            }

            switch (check) {
                case 1://less
                    if (blind_type) {//big blind
                        return "(BIG_BLIND <= " + std::to_string(blind_amount) + ")";
                    }
                    else {
                        return "(SMALL_BLIND <= " + std::to_string(blind_amount) + ")";
                    }
                    break;
                case 2://more
                    if (blind_type) {//big blind
                        return "(BIG_BLIND >= " + std::to_string(blind_amount) + ")";
                    }
                    else {
                        return "(SMALL_BLIND >= " + std::to_string(blind_amount) + ")";
                    }
                    break;
                case 3://equal
                    if (blind_type) {//big blind
                        return "(BIG_BLIND = " + std::to_string(blind_amount) + ")";
                    }
                    else {
                        return "(SMALL_BLIND = " + std::to_string(blind_amount) + ")";
                    }
                    break;
                default:
                    cout << "PROBLEM BLINDS" << endl;
                    return NULL;

            }
            break;
        case 6:
            //size of pot
            while (true) {
                cout << "Enter the amount you would like to be the cutoff for pot size: ";
                cin >> input;
                check = g.check_input_int(input);
                if (check < 0) {
                    cout << "invalid input" << endl;
                    continue;
                }
                pot_size = check;
                break;
            }
            while (true) {
                cout << "would you like to search for more or less than this pot?" << endl;
                cout << "Enter 1 for more, 0 for less: ";
                cin >> input;
                check = g.check_input_int(input);
                if (check < 0 || check>2) {
                    cout << "invalid input" << endl;
                    continue;
                }
                break;
            }
            pot_string = std::to_string(pot_size);
            if (check) {//more
                return "(PRE_FLOP_POT >= " + pot_string + " OR FLOP_POT >= " + pot_string + " OR TURN_POT >= " + pot_string + " OR RIVER_POT >= " + pot_string + ")";
            }
            else {//less
                return "(PRE_FLOP_POT <= " + pot_string + " OR FLOP_POT <= " + pot_string + " OR TURN_POT <= " + pot_string + " OR RIVER_POT <= " + pot_string + ")";
            }
            break;
        case 7:
            //opponent names
            while (true) {
                cout << "Enter the name of the opponent you would like to find the game of " << endl;
                cin >> input;
                break;
            }
            return "(NAMES LIKE '%," + input + ",%')";
            break;
        case 8:
            //num_players
            while (true) {
                cout << "How many players would you like to be the cutoff: ";
                cin >> input;
                check = g.check_input_int(input);
                if (check < 2) {
                    cout << "invalid input" << endl;
                    continue;
                }
                n_players = check;
                break;
            }
            while (true) {
                cout << "Would you like to search for more, less, or equal to this number?" << endl;
                cout << "Enter 2 for more, 1 for less, or 0 for equal: ";
                cin >> input;
                check = g.check_input_int(input);
                if (check < 0 || check>2) {
                    cout << "invalid input" << endl;
                    continue;
                }
                break;
            }
            switch (check) {
                case 0://equal
                    return "(N_PLAYERS = " + std::to_string(n_players) + ")";
                    break;
                case 1://less
                    return "(N_PLAYERS <= " + std::to_string(n_players) + ")";
                    break;
                case 2://more
                    return "(N_PLAYERS >= " + std::to_string(n_players) + ")";
                    break;
                default:
                    cout << "PROBLEM WITH PLAYERS" << endl;
                    return NULL;
            }
            break;
        case 9:
            //position
            while (true) {
                cout << "what position would you like to be the cutoff: ";
                cin >> input;
                check = g.check_input_int(input);
                if (check < 0) {
                    cout << "invalid input" << endl;
                    continue;
                }
                position = check;
                break;
            }
            while (true) {
                cout << "Would you like to search for more, less, or equal to this number?" << endl;
                cout << "Enter 2 for more, 1 for less, or 0 for equal: ";
                cin >> input;
                check = g.check_input_int(input);
                if (check < 0 || check>2) {
                    cout << "invalid input" << endl;
                    continue;
                }
                break;
            }
            switch (check) {
            case 0://equal
                return "(POSITION = " + std::to_string(position) + ")";
                break;
            case 1://less
                return "(POSITION <= " + std::to_string(position) + ")";
                break;
            case 2://more
                return "(POSITION >= " + std::to_string(position) + ")";
                break;
            default:
                cout << "PROBLEM WITH POSITION" << endl;
                return NULL;
            }
            break;
        case 10:
            //your starting stack_amount
            while (true) {
                cout << "What stack amount of the POV player would you like to be the cutoff: ";
                cin >> input;
                check = g.check_input_int(input);
                if (check < 0) {
                    cout << "invalid input" << endl;
                    continue;
                }
                n_players = check;
                break;
            }
            while (true) {
                cout << "Would you like to search for more, less, or equal to this number?" << endl;
                cout << "Enter 2 for more, 1 for less, or 0 for equal: ";
                cin >> input;
                check = g.check_input_int(input);
                if (check < 0 || check>2) {
                    cout << "invalid input" << endl;
                    continue;
                }
                break;
            }
            switch (check) {
                case 0://equal
                    return "(POV_PRE_STACK = " + std::to_string(n_players) + ")";
                    break;
                case 1://less
                    return "(POV_PRE_STACK <= " + std::to_string(n_players) + ")";
                    break;
                case 2://more
                    return "(POV_PRE_STACK >= " + std::to_string(n_players) + ")";
                    break;
                default:
                    cout << "PROBLEM WITH PLAYERS" << endl;
                    return NULL;
            }
            break;
            break;
        case 11:
            //POV folded or not
            cout << "would you like to see hands where the POV folded or not?" << endl;
            while (true) {
                cout << "Enter 1 for hands where they folded, 0 for hands where they didn't" << endl;
                cin >> input;
                check = g.check_input_int(input);
                if (check < 0 || check > 1) {
                    cout << "invalid input" << endl;
                    continue;
                }
                break;
            }
            switch (check) {
                case 0:
                    return "(INSTR(FOLDED, ',0,')>0)";
                    break;
                case 1:
                    return "(INSTR(FOLDED, ',0,')=0)";
                    break;
                default:
                    cout << "PROBLEM WITH FOLDED" << endl;
                    return NULL;
            }
            
            break;
        case 12:
            //Specific POV
            cout << "Enter the name of the POV you would like to find the games of: ";
            cin >> input;
            return "(PLAYER_NAME = '" + input + "')";
            break;
        default:
            cout << "PROBLEM";
            return NULL;
    }
}

void Query::display() {
    cout << endl << "****************************************" << endl;
    cout << "possible search criteria: " << endl;
    cout << "Enter 1 to search by a specific card POV recieved pre-flop" << endl;
    cout << "Enter 2 to search by the strength of POV hand (at the flop, turn, or river)" << endl;
    cout << "Enter 3 to search by whether or not the hand went to some kind of showdown or not" << endl;
    cout << "Enter 4 to search by whether or not POV won the hand" << endl;
    cout << "Enter 5 to search by the big blind or small blind POV was playing " << endl;
    cout << "Enter 6 to search by the size of the pot" << endl;
    cout << "Etner 7 to search for games played against particular people" << endl;
    cout << "Enter 8 to search by the number of people in the game" << endl;
    cout << "Enter 9 to search by the position POV was in that hand" << endl;
    cout << "Enter 10 to search by your stack amount for that hand" << endl;
    cout << "Enter 11 to search by whether or not POV folded that hand" << endl;
    cout << "Enter 12 to search for a specific POV player" << endl;
    cout << "press f when you are done with your query" << endl;
    cout << "press q to quit" << endl;
    cout << "********************************************" << endl;

}
