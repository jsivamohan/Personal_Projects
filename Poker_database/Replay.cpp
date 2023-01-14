//
// Created by jeeva on 1/9/2023.
//

#include "Replay.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <sqlite3.h>
#include <fstream>
#include <unordered_set>
#include <random>

using namespace std;

Replay::Replay(int players, std::vector<int> s_amounts, int position, std::vector<std::string> name, std::vector<int> blind, std::vector<std::vector<std::vector<int>>> b_amounts,
    std::vector<int>p_size, std::vector<int> c, bool g_showdown, int last_action, vector<int>folded, vector<double> strength, double p_flop_per, string winner) :num_players{ players }, stack_amounts{ std::move(s_amounts) }, pos{ position },
    names{ std::move(name) }, blinds{ std::move(blind) }, bet_amounts{ std::move(b_amounts) }, pot_size{ std::move(p_size) }, cards{ std::move(c) },
    got_showdown{ g_showdown }, pot{ 0 }, last_action{ last_action }, folded{ std::move(folded) }, strength{ std::move(strength) }, pre_flop_win_p{ p_flop_per }, winner{ winner }, hand{ Hand{} } {}
Replay::Replay() {};

void Replay::replay_hand(){
    //add in pot odds calculation but only for calls for the POV player
    std::cout << "Hand replay" << endl;
    vector<int> i_folded;
    vector<vector<int>> bet_pattern{};//betting pattern for each player (starting with the first to act, for the given situation)
    int situation = 0;//iterator within the loop that will track what situation we are in (0 is pre-flop, 1 is flop, 2 is turn, 3 is river)
    int offset{};
    int act_pos{};//position of the first to act
    int max_size{};
    int raise_amount{};//tracking the current raise amount of the pot
    int accum{};
    int real_pos{};
    string accept{};
    int w_pos{};
    int counter{};
    double w_score{};
    Hand h{};
    vector<int> calc_cards{};
    int num_cards{};
    while(true){//continue looping until hand over (end conditions have a break statement
        bet_pattern.clear();
        max_size = 0;
        raise_amount = 0;
        if(situation==0){//pre-flop
            offset = 3;
        }else{
            offset = 1;
        }
        act_pos = (num_players-pos+offset)%num_players;
        for(int i = act_pos; i< act_pos + num_players; i++){
            bet_pattern.push_back(bet_amounts.at(i%num_players).at(situation));
        }
        //find the max size of a vector in bet_pattern and skip if reach -1 or not big enough
        for(int i = 0; i<bet_pattern.size(); i++){
            if(bet_pattern.at(i).size()>max_size){
                max_size = bet_pattern.at(i).size();
                //w_pos = i;
            }
        }
        switch(situation){
            case 0:
                std::cout << "pre-flop betting" << endl;
                break;
            case 1:
                std::cout << "post-flop betting" << endl;
                break;
            case 2:
                std::cout << "turn card betting" << endl;
                break;
            case 3:
                std::cout << "river betting" << endl;
                break;
            default:
                std::cout << "PROBLEM";
        }
        display(situation);
        std::cout << "press n to continue, q to quit: ";
        std::cin >> accept;
        if (accept == "Q" || accept == "q") {
            return;
        }
        for(int i= 0; i<max_size;i++){//for loop that simulates the betting round
            for(int j = 0; j<num_players; j++){
                real_pos = (num_players-pos+offset+j)%num_players;
                if(i<bet_pattern.at(j).size() && find(i_folded.begin(), i_folded.end(), real_pos)==i_folded.end()){
                    if(bet_pattern.at(j).at(i)==-1){
                        std::cout << names.at(real_pos) << " folded" << endl;//conversion from act_pos back to pos
                        i_folded.push_back(real_pos);
                        if(i_folded.size()==num_players-1){
                            break;
                        }
                    }else{
                        if (real_pos == 0 && situation > 0 && raise_amount>0) {//do not show for pre-flop
                            if (situation == 1) {
                                calc_cards.insert(calc_cards.end(), cards.begin(), cards.begin() + 5);
                            }
                            else if (situation == 2) {
                                calc_cards.insert(calc_cards.end(), cards.begin(), cards.begin() + 6);
                            }
                            else{
                                calc_cards.insert(calc_cards.end(), cards.begin(), cards.begin() + 7);
                            }
                            std::cout << "Pot odds are " << (double)((pot + raise_amount) / raise_amount) << " putting break even at " << double(1.0 / (double)((pot + raise_amount) / raise_amount)) <<
                                " real time win %: " << calc_odds(calc_cards, 100000, "C:\\Users\\jeeva\\SQLite\\Poker.db", num_players - folded.size()) << endl;
                            calc_cards.clear();
                        }
                        
                        accum = accumulate(bet_pattern.at(j).begin(), bet_pattern.at(j).begin() + (i+1), 0);
                        if(accum > raise_amount){//some kind of raise
                            raise_amount = accum;
                            if(i>0){
                                std::cout << names.at(real_pos) << " raised to " << raise_amount << "(added on " << raise_amount - bet_pattern.at(j).at(i-1) << ")" << endl;
                            }else{
                                std::cout << names.at(real_pos) << " raised to " << raise_amount << endl;
                            }
                        }else{//it was a call
                            std::cout << names.at(real_pos) << " called for " << bet_pattern.at(j).at(i) << endl;
                        }
                        stack_amounts.at(real_pos) -= bet_pattern.at(j).at(i);
                        pot += bet_pattern.at(j).at(i);
                    }
                }else{
                    continue;
                }
                display(situation);
                std::cout << "press any n to continue or q to quit: ";//need to code in quit functionality later
                std::cin >> accept;
                if (accept == "Q" || accept == "q") {
                    return;
                }
            }
        }

        //check to see if anyone won through folding
        if(i_folded.size()>num_players-2){
            for(int i = 0; i<num_players; i++){
                if(find(i_folded.begin(), i_folded.end(), i)==i_folded.end()){
                    w_pos = i;
                    break;
                }
            }
            stack_amounts.at(w_pos) += pot;
            pot = 0;
            display(situation);
            std::cout << names.at(w_pos) << " won the pot worth " << pot << " because everyone else folded" << endl;
            std::cout << "press n to continue and q to quit: ";//still need to code that quit functionality
            std::cin >> accept;
            break;
        }
        //check to see if we are on the river (in which case we don't show another card)
        if(situation==3){//on the river (going to showdown)

            std::cout << "going to showdown" << endl;
            //have to pass index of last action because prompted for it the first time
            std::cout << names.at(last_action) << " was the last to make an aggressive action (or no aggressive action was made) so they are first to show" << endl;
            int r_pos = last_action;
            vector<int> resolve_cards{};
            for(int i = 2; i<7; i++){
                resolve_cards.push_back(cards.at(i));
            }
            vector<pair<double,int>> strength_scores{};//strength scores with the position they are associated with
            //have to account for possibility of split pots (code in later)
            if(find(i_folded.begin(), i_folded.end(), 0)==i_folded.end()){//special case where we have not folded
                resolve_cards.push_back(cards.at(0));
                resolve_cards.push_back(cards.at(1));
                strength_scores.push_back(make_pair(h.resolve_hand(resolve_cards), 0));
            }
            for(int i = 0; i<cards.size()-7; i+= 2){
                resolve_cards.resize(5);
                resolve_cards.push_back(cards.at(7+i));
                resolve_cards.push_back(cards.at(7 + i + 1));
                while(find(folded.begin(), folded.end(), r_pos)!=folded.end()){
                    r_pos = (r_pos+=1)%num_players;
                }
                strength_scores.push_back(make_pair(h.resolve_hand(resolve_cards), r_pos));
                r_pos = (r_pos += 1) % num_players;
            }
            counter = 7;
            for(int i = last_action; i<last_action + num_players; i++){
                if (i % num_players == 0) {
                    if (find(i_folded.begin(), i_folded.end(), i % num_players) == i_folded.end()) {
                        std::cout << "you were next to play: ";
                        if (find(folded.begin(), folded.end(), 0) != folded.end()) {
                            std::cout << "you mucked your cards" << endl;
                        }
                        else {
                            std::cout << "you showed " << h.deck.draw(cards.at(0)) << " " << h.deck.draw(cards.at(1)) << endl;
                        }
                    }
                    continue;
                }
                if(find(i_folded.begin(),i_folded.end(), i%num_players)==i_folded.end()){//in the showdown at all
                    if(find(folded.begin(), folded.end(), i%num_players)==folded.end()){//actually showed their cards
                        std::cout << names.at(i) << " showed: " << h.deck.draw(cards.at(counter)) << " " << h.deck.draw(cards.at(counter+1)) << endl;
                        counter+=2;
                    }else{//mucked their cards
                        std::cout << names.at(i) << " mucked their cards" << endl;
                    }
                }
            }
            w_score = 0.0;
            for(int i= 0; i<strength_scores.size();i++){
                if(strength_scores.at(i).first > w_score){
                    w_pos = strength_scores.at(i).second;
                    w_score = strength_scores.at(i).first;
                }
            }
            std::cout << "winner is " << names.at(w_pos) << " they get the pot worth " << pot << endl;
            stack_amounts.at(w_pos) += pot;
            pot = 0;
            display(situation);
            std::cout << "press n to continue and q to quit: ";
            std::cin >> accept;
            break;
        }

        //if not at the river we need to show the next card(s)
        switch(situation){
            case 0:
                std::cout << "flop cards are: " << h.deck.draw(cards.at(2)) << " " <<  h.deck.draw(cards.at(3)) << " " << h.deck.draw(cards.at(4)) << endl;
                break;
            case 1:
                std::cout << "turn card is: " << h.deck.draw(cards.at(5)) << endl;
                break;
            case 2:
                std::cout << "river card is: " << h.deck.draw(cards.at(6)) << endl;
                break;
            default:
                std::cout << "PROBLEM" << endl;
        }
        display(situation+1);

        std::cout << "press n to continue and q to quit: ";
        std::cin >> accept;
        if (accept == "Q" || accept == "q") {
            return;
        }

        situation++;
    }
}

void Replay::display(int sit) {
    std::cout << "current table position (* for the dealer)" << endl;
    int players_left{};
    vector<int>calc_cards{};
    for(int i = 0; i<num_players; i++){
        std::cout << names.at(i) << " " << stack_amounts.at(i);//have to manually manipulate as the bets come in
        if(i == num_players-pos){
            std::cout << "*";
        }
        std::cout << endl;
    }

    std::cout << "Your cards: " << hand.deck.draw(cards.at(0)) << " " << hand.deck.draw(cards.at(1)) << endl;
    std::cout << "Cards on the table: ";
    if(sit>0){
        std::cout << hand.deck.draw(cards.at(2)) << " " << hand.deck.draw(cards.at(3)) << " " << hand.deck.draw(cards.at(4)) << " ";
        if(sit>1){
            std::cout << hand.deck.draw(cards.at(5)) << " ";
            if(sit>2){
                std::cout << hand.deck.draw(cards.at(6)) << " ";
            }
        }
    }
    std::cout << endl;
    std::cout << "Pot size: " << pot << endl;//should display incrementally throughout the hand not just at the end so need to keep track of pot size manually
    if (sit > 0) {
        std::cout << "current strength of hand: " << strength.at(sit-1) << endl;
        if (find(folded.begin(), folded.end(), 0) == folded.end()) {
            players_left = num_players - folded.size();
        }
        else {
            players_left = num_players - folded.size() + 1;
        }
        if (sit == 1) {
            calc_cards.insert(calc_cards.end(), cards.begin(), cards.begin() + 5);
        }
        else if (sit == 2) {
            calc_cards.insert(calc_cards.end(), cards.begin(), cards.begin() + 6);
        }
        else {
            calc_cards.insert(calc_cards.end(), cards.begin(), cards.begin() + 7);
        }
        std::cout << "real time calculated win percentage: " << calc_odds(calc_cards, 100000, "C:\\Users\\jeeva\\SQLite\\Poker.db", players_left);
        calc_cards.clear();
    }
    else {
        std::cout << "pre flop win percentage " << pre_flop_win_p << endl;
    }

    std::cout << endl;
}

void Replay::summary() {
    int situation{};//value is 1 if reach flop, 2 if reach turn, 3 if reach river
    int f_number{};
    std::cout << "Perspective of: " << names.at(0) << " with starting stack amount (" << stack_amounts.at(0) << ")" << endl;
    std::cout << "blinds are: " << blinds.at(0) << " " << blinds.at(1) << endl;
    std::cout << "people involved (with starting stack amount):" << endl;
    for (int i = 1; i < names.size(); i++) {
        std::cout << names.at(i) << " (" << stack_amounts.at(i) << ")";
        if (i != names.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << endl;
    std::cout << "position relative to dealer: " << pos << endl;
    std::cout << "dealer was: " << names.at((num_players - pos)) << endl;
    std::cout << "POV pre-flop cards: " << hand.deck.draw(cards.at(0)) << " " << hand.deck.draw(cards.at(1)) << endl;
 
    situation = 4;
        ;
    for (int i = 0; i < pot_size.size(); i++) {
        if (pot_size.at(i) < 0) {
            situation = i;
            break;
        }
    }

    if (cards.size() > 4) {
        std::cout << "flop cards: " << hand.deck.draw(cards.at(2)) << " " << hand.deck.draw(cards.at(3)) << " " << hand.deck.draw(cards.at(4)) << endl;;
        if (cards.size() > 5) {
            std::cout << "turn card: " << hand.deck.draw(cards.at(5)) << endl;
            if (cards.size() > 6) {
                std::cout << "river card: " << hand.deck.draw(cards.at(6)) << endl;
                if (cards.size() == 7) {
                    std::cout << "folded after river, winner is: " << winner << endl;
                    return;
                }
                std::cout << "went to showdown, winner was " << winner << endl;
                return;
            }
            std::cout << "folded after turn, winner was: " << winner << endl;
            return;
        }
        std::cout << "Folded after flop, winner was: " << winner << endl;
    }

}

double Replay::calc_odds(vector<int> &this_cards, double num_iterations, const char *s, int players_left) {
    if (this_cards.size() > 7) {
        this_cards.resize(7);
    }

    sqlite3* db;


    char* messageerror;
    bool recieved = false;

    int exit = sqlite3_open(s, &db);
    string SQL_statement{};

    vector<int> cards_db{ this_cards.begin(), this_cards.end() };
    //first check to see if it's already in the table
    sort(cards_db.begin(), cards_db.begin()+2);
    sort(cards_db.begin() + 2, cards_db.end());
    while (cards_db.size() < 7) {
        cards_db.push_back(-1);
    }


    string sql = "SELECT WIN_PER FROM POST_FLOP WHERE (C_ONE = " + to_string(cards_db.at(0)) + ") AND (C_TWO = " + to_string(cards_db.at(1)) +
        ") AND (C_THREE = " + to_string(cards_db.at(2)) + ") AND (C_FOUR = " + to_string(cards_db.at(3)) + ") AND (C_FIVE = " + to_string(cards_db.at(4)) + 
            ") AND (C_SIX = " + to_string(cards_db.at(5)) + ") AND (C_SEVEN = " + to_string(cards_db.at(6)) + ");";

    sqlite3_stmt* stmt;
    int j{};
    string text{};
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cout << "error with prepare in test" << endl;
    }
    else {
        for (;;) {
            int rc = sqlite3_step(stmt);
            if (rc == SQLITE_DONE)
                break;
            if (rc != SQLITE_ROW) {
                std::cout << "error with step in test" << endl;
                break;
            }

            const unsigned char* text_char = sqlite3_column_text(stmt, 0);
            j = 0;
            while (text_char[j] != '\0') {
                text += text_char[j];
                j++;
            }
            recieved = true;
            
        }
        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);
    if (recieved) {
        return stod(text);
    }

    int sit{};
    vector<int> cards_2{};
    uniform_int_distribution<int> distribution(0, 51);
    vector<double> ss{};
    double percent = 0;
    int index{};
    int w_index{};
    double max_val{};
    double win{};
    double tie{};
    double hands_beat{};
    unordered_set<int> vals{};
    int size{};
    int iteration{};
    if (this_cards.size() == 5) {//flop calculation
        size = 2;
        
    }
    else if (this_cards.size() == 6) {//turn
        size = 1;
    }
    else {//river (can only deterministically calculate the number of hands you beat here)
        size = 0;
        cards_2.insert(cards_2.end(), this_cards.begin()+2, this_cards.end());
        for (int i = 0; i < 52; i++) {
            for (int k = i + 1;k < 52;k++) {
                cards_2.resize(5);
                if (find(this_cards.begin(), this_cards.begin() + 3, i) != this_cards.begin() +3 || find(cards_2.begin(), cards_2.end(), i)!=cards_2.end()) {
                    i++;
                    continue;
                }
                if (find(this_cards.begin(), this_cards.begin() + 3, k) != this_cards.begin() + 3 || find(cards_2.begin(), cards_2.end(), k) != cards_2.end()) {
                    continue;
                }
                cards_2.push_back(i);
                cards_2.push_back(k);
                if (hand.resolve_hand(this_cards) > hand.resolve_hand(cards_2)) {
                    hands_beat++;
                }
            }
        }
        std::cout << "hands POV beats: " << hands_beat << " hands they lose to: " << (990 - hands_beat) << endl;
        cards_2.clear();
    }
    cards_2.insert(cards_2.end(), this_cards.begin() + 2, this_cards.end());
    this_cards.resize(2);
    for (double i = 0; i < num_iterations; i++) {
        max_val = -1;
        tie = 0;
        iteration = 0;
        w_index = -1;
        this_cards.resize(2);
        ss.clear();
        cards_2.resize(5 - size);
        default_random_engine generator(rand() % (100 + 5 * (int)i));
        while (true) {

            index = distribution(generator);
            if (find(this_cards.begin(), this_cards.end(), index) == this_cards.end() && find(cards_2.begin(), cards_2.end(), index) == cards_2.end()) {
                if (iteration < size) {
                    cards_2.push_back(index);
                    iteration++;
                    continue;
                }
                this_cards.push_back(index);
            }
            if (this_cards.size() >= 2 * players_left) {
                break;
            }

        }

        for (int i = 0; i < players_left; i++) {
            cards_2.resize(5);
            cards_2.push_back(this_cards.at(2 * i));
            cards_2.push_back(this_cards.at(2 * i + 1));
            ss.push_back(hand.resolve_hand(cards_2));
        }
        for (int i = 0; i < ss.size(); i++) {
            if (w_index == 0 && ss.at(i) == max_val) {
                tie++;
                continue;
            }
            if (max_val < ss.at(i)) {
                w_index = i;
                max_val = ss.at(i);
            }
            if (w_index > 0) {
                break;
            }

        }
        if (w_index == 0) {
            win += (double)(1.0 / (tie + 1.0));
        }


    }

    if (size == 2) {
        cards_2.resize(3);
        sort(cards_2.begin(), cards_2.end());
        cards_2.push_back(-1);
        cards_2.push_back(-1);
    }
    else if (size == 1) {
        cards_2.resize(4);
        sort(cards_2.begin(), cards_2.end());
        cards_2.push_back(-1);
    }
    else {
        cards_2.resize(5);
        sort(cards_2.begin(), cards_2.end());
    }

    sort(this_cards.begin(), this_cards.begin() + 2);

    sqlite3* db1;

    exit = sqlite3_open(s, &db1);


    SQL_statement = "INSERT INTO POST_FLOP(C_ONE, C_TWO, C_THREE, C_FOUR, C_FIVE, C_SIX, C_SEVEN, WIN_PER) values(" + to_string(this_cards.at(0)) + ", "
        + to_string(this_cards.at(1)) + ", " + to_string(cards_2.at(0)) + ", " + to_string(cards_2.at(1)) + ", " + to_string(cards_2.at(2)) + ", "
         + to_string(cards_2.at(3)) + ", " + to_string(cards_2.at(4)) + ", " + to_string((double)(win / num_iterations)) + ");";


    exit = sqlite3_exec(db1, SQL_statement.c_str(), NULL, 0, &messageerror);

    if (exit != SQLITE_OK) {
        cerr << "error with insert" << endl;
        sqlite3_free(messageerror);
        return NULL;
    }
    else {
        std::cout << "record inserted successfully" << endl;
    }

    sqlite3_close(db1);

    return (double)(win / num_iterations);

    
}

