//
// Created by jeeva on 1/6/2023.
//

#include "Game.h"
#include "Deck.h"
#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <sqlite3.h>

using namespace std;

Game::Game() :last_action{ -1 }, won{ false }, hand{ Hand{} } {}

int Game::check_input_int(string input) {
    istringstream iss{ input };
    int check{};
    if (iss >> check) {
        return check;
    }
    else {
        return -2;
    }
}

void Game::prompt(const string& id) {//help set initial values of the game object (num_players, starting stack_amounts, names, blinds, starting pos)
    int blind{};
    string name{};
    int stack{};
    string input{};
    int check{};
    while (true) {
        std::cout << id << " you've initialized a new game. Time to set the parameters of the game" << endl;
        std::cout << "number of players: ";
        cin >> input;
        check = check_input_int(input);
        if (check > 0) {
            num_players = check;
            break;
        }
        cout << "invalid input" << endl;
    }
    while (true) {
        std::cout << "blinds " << endl;
        std::cout << "big blind: ";
        cin >> input;
        check = check_input_int(input);
        if (check > 0) {
            blinds.push_back(check);
            break;
        }
        cout << "invalid input" << endl;
    }

    while (true) {
        std::cout << "small blind: ";
        cin >> input;
        check = check_input_int(input);
        if (check > 0) {
            blinds.push_back(check);
            break;
        }
        cout << "invalid input" << endl;
    }

    while (true) {
        std::cout << "your starting position (relative to the dealer): ";//the ordering goes dealer, then small blind, then big blind (to the left)
        cin >> input;
        check = check_input_int(input);
        if (check > 0 && check < num_players) {
            pos = check;
            break;
        }
        cout << "invalid input" << endl;
    }

    names.push_back(id);

    while (true) {
        std::cout << "your starting stack: ";
        cin >> input;
        check = check_input_int(input);
        if (check > 0) {
            stack_amounts.push_back(check);
            break;
        }
        cout << "invalid input" << endl;
    }

    std::cout << endl << "now input the names and starting stack amounts of the players at the table (starting from your left and going around)";
    for(int i = 0; i<num_players-1; i++){
        std::cout <<  "name of the player: ";
        cin >> name;
        names.push_back(name);
        while (true) {
            std::cout << "starting stack amount: ";
            cin >> input;
            check = check_input_int(input);
            if (check > 0) {
                stack_amounts.push_back(check);
                break;
            }
            cout << "invalid input" << endl;
        }
    }
    for(int i = 0; i<num_players; i++){
        bet_amounts.push_back({});
    }
    for(auto i : stack_amounts){
        pre_stack.push_back(i);
    }

}

bool Game::display() {//displays the current game state (with the dealer, big blind, small blind identified along with the order relative to the id player and stack amounts
    // as well as the current cards in play)
    string quit{};
    std::cout << "name, stack size, (* for the dealer)" << endl;
    for(int i = 0; i<names.size(); i++){
        std::cout << names.at(i) << " " << stack_amounts.at(i);
        if(i == (num_players- pos)){
            std::cout << "*" << endl;
        }else{
            std::cout << endl;
        }
    }
    Deck d{};
    std::cout <<"Your cards: ";
    if(cards.size()!=0){
        std::cout << d.draw(cards.at(0)) << " " << d.draw(cards.at(1));
    }
    std::cout << endl;

    std::cout << "Cards on the table: ";
    for(int i = 2; i<cards.size(); i++){
        std::cout << d.draw(cards.at(i)) << " ";
    }
    std::cout << endl;
    std::cout << "Pot size: ";
    if(pot_size.size()>0){
        std::cout << (pot_size.at(pot_size.size()-1));
    }
    std::cout << endl;
    cout << "input q to quit the hand (anything else to continue): ";
    cin >> quit;
    if (quit == "q" || quit == "Q") {
        return false;
    }
    cout << endl << "********************************************" << endl;
    return true;
}

int Game::get_index(int value, char suit) {
    int s_value{};
    switch(suit){
        case 'C':
        case 'c':
            s_value = 0;
            break;
        case 'S':
        case 's':
            s_value = 13;
            break;
        case 'H':
        case 'h':
            s_value = 26;
            break;
        case 'D':
        case 'd':
            s_value = 39;
            break;
        default:
            std::cout << "problem with suit identification " << endl;
            return -1;

    }
    if (value < 1 || value >13) {
        cout << "problem with value entered" << endl;
        return -1;
    }
    if(value == 1){
        s_value += 12;
    }
    else{
        s_value += value-2;
    }
    return s_value;
}

void Game::play_hands(string id){//play multiple hands looped until we are done playing
//need to clear multiple fields in this object before starting a new hand (cards field, bet amounts, showdown, etc)
    prompt(id);//sets up all the basic information before starting a game
    string input{};
    while(true){
        cout << "press n to log a hand, press q to quit" << endl;
        cin >>input;
        if(input=="Q" || input=="q"){
            cout << "leaving game" << endl;
            break;
        }
        else if(input=="n" || input=="N"){
            cout << "preparing to log hand" << endl << endl;
        }
        else{
            cout << "invalid input" << endl;
            continue;
        }

        if (!play()) {
            cout << "hand will not be input"<<endl;
            break;
        }
        insert_info();//insert the info from the game into the database
        reset();
    }
}

bool Game::play() {//play out the hand to set the values of the bet_amounts vector along with adding to the pot_size vector at each interval (pre-flop, post-flop, turn, river)
    //and changing the stack amounts accordingly
    char suit{};
    int value{};
    bool end = true;
    int w_index{};
    int end_count{};
    bool preflop{};
    int check{};
    string input{};
    display();
    vector<string> phases{"pre-flop", "flop", "turn", "river"};//all the phases of the round of poker
    cout << "please input your cards " << endl;
    for(int i = 0; i<2; i++){

        while (true) {
            cout << "card " << (i + 1) << " value (jack = 11, queen = 12, king = 13, Ace = 14): ";
            cin >> input;
            check = check_input_int(input);
            if (check<0) {
                cout << "invalid input" << endl;
                continue;
            }
            value = check;
            cout << "suit(H for heart, D for diamond, S for spade, C for club) : ";
            cin >> suit;
            check = get_index(value, suit);
            if (check > 0) {
                break;
            }
            cout << "invalid input" << endl;
        }

        if(find(cards.begin(), cards.end(), check)==cards.end()){
            cards.push_back(check);
        }else{
            cout << "card has already been played" << endl;
            i--;
        }

    }
    cout << endl;

    if (!display()) {
        return false;
    }

    for(int p = 0; p<phases.size();p++){//each phase starts with the betting and ends with the card for that phase being shown (will code in the post river betting
        //outside the loop)
        end_count = 0;
        cout << phases.at(p) << endl;
        if(p==0){//we are on the flop
            preflop = true;
        }else{
            preflop = false;
        }
        if(!betting(preflop)){//game is over without getting to showdown
            for(int i = 0; i<num_players; i++){
                if(find(folded.begin(), folded.end(), i)==folded.end()){
                    w_index = i;
                    break;
                }
            }
            got_showdown = false;
            if(w_index==0){
                won = true;
            }
            cout << "hand is over " << names.at(w_index) << " won pot of size: " << pot_size.at(pot_size.size()-1) << endl;
            winner = names.at(w_index);
            stack_amounts.at(w_index) += pot_size.at(pot_size.size()-1);
            break;
        }
        if(p== phases.size()-1){//we've reached showdown
            showdown();
            break;
        }
        show_cards(p);
        if (!display()) {
            return false;
        }
    }
    return true;
}

void Game::showdown() {
    //first prompt for the showdown cards of each of the players still in the hand
    int value{};
    char suit{};
    string id_name{};
    int start_pos=-1;
    int real_pos{};
    int check{};
    char muck {};
    string input{};
    got_showdown = true;
    cout << "reached showdown"<< endl;
    do{
        cout << "who was the last to take aggressive action (they show first): ";
        cin >> id_name;
        for(int i= 0; i<num_players; i++){
            if(names.at(i)==id_name){
                start_pos = i;
                last_action = start_pos;
                break;
            }
            if(i==num_players-1){
                cout << "name not found" << endl;
            }
        }
    }while(start_pos==-1);

    for(int i = start_pos;i<num_players + start_pos; i++){
        real_pos = i % num_players;
        if(real_pos==0){//don't need to ask for our own hand
            continue;
        }
        if(find(folded.begin(), folded.end(), real_pos)==folded.end()){
            cout << names.at(real_pos) << endl;
            if(i>start_pos){

                while (true) {
                    cout << "did they muck the hand without showing (Y/N): ";
                    cin >> muck;
                    if (muck == 'n' || muck == 'N' || muck == 'y' || muck == 'Y') {
                        break;
                    }
                    cout << "invalid input" << endl;
                }

            }
            if(muck=='Y' || muck =='y'){
                folded.push_back(real_pos);
            }else{
                for(int j = 0; j<2; j++){
                    while (true) {
                        cout << "card " << (j + 1) << " value (jack = 11, queen = 12, king = 13): ";
                        cin >> input;
                        check = check_input_int(input);
                        if (check<0) {
                            cout << "invalid input" << endl;
                            continue;
                        }
                        value = check;
                        cout << "suit(H for heart, D for diamond, S for spade, C for club) : ";
                        cin >> suit;
                        check = get_index(value, suit);
                        if (check > 0) {
                            break;
                        }

                    }

                    if(find(cards.begin(), cards.end(), check)==cards.end()){
                        cards.push_back(check);
                    }else{
                        cout << "card has already been played" << endl;
                        j--;
                    }
                }
                cout << endl;
            }
        }
    }
    int counter = 0;
    int r_pos = start_pos;
    vector<int> resolve_cards{};
    for(int i = 2; i<7; i++){
        resolve_cards.push_back(cards.at(i));
    }
    vector<pair<double,int>> strength_scores{};//strength scores with the position they are associated with
    if(find(folded.begin(), folded.end(), 0)==folded.end()){//special case where we have not folded
        resolve_cards.push_back(cards.at(0));
        resolve_cards.push_back(cards.at(1));
        strength_scores.push_back(make_pair(hand.resolve_hand(resolve_cards), 0));
    }

    for(int i = 0; i<cards.size()-7; i+= 2){
        resolve_cards.resize(5);
        resolve_cards.push_back(cards.at(7+i));
        resolve_cards.push_back(cards.at(7 + i + 1));
        while(find(folded.begin(), folded.end(), r_pos)!=folded.end()){
            r_pos = (r_pos+=1)%num_players;
        }
        strength_scores.push_back(make_pair(hand.resolve_hand(resolve_cards), r_pos));
        r_pos = (r_pos += 1) % num_players;
    }

    int w_pos{};
    double w_score = -1;

    for(int i= 0; i<strength_scores.size(); i++){
        cout << names.at(strength_scores.at(i).second) << " strength score: " << strength_scores.at(i).first << endl;
    }

    for(int i= 0; i<strength_scores.size();i++){
        if(strength_scores.at(i).first > w_score){
            w_pos = strength_scores.at(i).second;
            w_score = strength_scores.at(i).first;
        }
    }
    if(w_pos==0){
        won = true;
    }
    cout << "winner is " << names.at(w_pos) << " they get the pot worth " << pot_size.at(pot_size.size()-1) << endl;
    winner = names.at(w_pos);
    stack_amounts.at(w_pos) += pot_size.at(pot_size.size()-1);

}

bool Game::betting(bool preflop){

    int amount{};
    int offset=1;
    int player{};
    bool ongoing = true;
    string input{};
    int check{};
    int raise_amount{};//represents current max value that has been bet (all future values must be this value or higher and if higher then we have to go back around
    //until all values are this value)
    int mod_amount{};
    vector<vector<int>> sit_bets{};//tracks the levels of betting through the situation for each player
    for(int i = 0;i<num_players; i++){
        sit_bets.push_back({});//push back a base in each slot for the situation bets of each player
    }

    int counter = 0;
    int raise_index{};
    if(preflop){
        pot_size.push_back(0);
        offset = 3;
        cout << "betting phase (pre-flop so start with left of big blind)" << endl;
    }else{
        pot_size.push_back(pot_size.at(pot_size.size()-1));
        cout << "betting phase (post-flop so start with small blind)" << endl;
    }

    do{
        player = (num_players- pos) + offset + counter;
        mod_amount = 0;
        if(find(folded.begin(), folded.end(), (player%num_players))==folded.end()){
            if(sit_bets.at(player%num_players).size()>0){

                for(int s : sit_bets.at(player%num_players)){
                    mod_amount += s;
                }


                cout << names.at((player)%num_players) << " bet amount (-1 if fold, must add on at least " <<
                raise_amount - mod_amount << "): ";
            }else{
                cout << names.at((player)%num_players) << " bet amount (-1 if fold, must be at least " << raise_amount << "): ";

            }
            cin >> input;
            check = check_input_int(input);
            if (check<-1) {
                cout << "invalid input" << endl;
                continue;
            }
            amount = check;
            if(amount< raise_amount - mod_amount && amount != -1){
                cout << "must bet at least "<< raise_amount - mod_amount << " or fold" << endl;
                continue;
            }
            if(amount + mod_amount> raise_amount){
                cout << "raise to " << amount + mod_amount << endl;
                raise_amount = amount + mod_amount;
                raise_index = counter;
            }
            sit_bets.at(player%num_players).push_back(amount);
            if(amount!=-1){
                pot_size.at(pot_size.size()-1) += amount;
                stack_amounts.at((player)%num_players) -= amount;
            }
            else{
                folded.push_back(player%num_players);
                if(folded.size()==num_players-1){//hand is over
                    ongoing = false;
                    break;
                }
            }
        }else{
            sit_bets.at((player)%num_players).push_back(-1);
        }
        counter++;
    }while(counter < (raise_index + num_players));


    for(int i = 0; i<num_players; i++){
        bet_amounts.at(i).push_back(sit_bets.at(i));
    }




    return ongoing;

}

void Game::show_cards(int sit) {
    int number{};//number of cards to show
    int value{};
    char suit{};
    int index{};
    string input{};
    int check{};
    if(sit==0){
        number = 3;
        std::cout << "flop cards" << endl;
    }else if(sit ==1){
        number = 1;
        std::cout << "turn card" << endl;
    }else{
        number = 1;
        std::cout << "river card" << endl;
    }
    for(int i = 0; i<number; i++){
        while (true) {
            std::cout << "card " << (i + 1) << " value (jack = 11, queen = 12, king = 13): ";
            cin >> input;
            check = check_input_int(input);
            if (check<0) {
                std::cout << "invalid input" << endl;
                continue;
            }
            value = check;
            std::cout << "suit(H for heart, D for diamond, S for spade, C for club) : ";
            cin >> suit;
            check = get_index(value, suit);
            if (check<0) {
                std::cout << "invalid input try again" << endl;
            }
            else {
                break;
            }
        }
        if(find(cards.begin(), cards.end(), check)==cards.end()){
            cards.push_back(check);
        }else{
            std::cout << "card has already been played" << endl;
            i--;
        }

    }
    strength.push_back(hand.resolve_hand(cards));
    std::cout << endl;

}

void Game::print_bet_amounts() {
    cout << "bet amounts" << endl;
    for(int i= 0; i<bet_amounts.size(); i++){
        cout << names.at(i) << " ";
        for(int k = 0; k<bet_amounts.at(i).size(); k++){
            cout << "situation " << k << "[" << " ";
            for(int j = 0; j<bet_amounts.at(i).at(k).size(); j++){
                cout << bet_amounts.at(i).at(k).at(j) << " ";
            }
            cout << "]" << endl;
        }
    }
}

void Game::set_bet(int amount, int player, int situation, int iteration) {
    if(bet_amounts.size()>player && bet_amounts.at(player).size() > situation){
        bet_amounts.at(player).at(situation).at(iteration) = amount;
    }else{
        cout << "set_bet element could not be accessed" << endl;
    }

}

void Game::set_pot(int amount, int situation) {
    if(pot_size.size()>situation) {
        pot_size.at(situation) = amount;
    }else{
        cout << "set_pot element could not be accessed" << endl;
    }

}

void Game::set_stack(int amount, int player) {
    if(stack_amounts.size()>player){
        stack_amounts.at(player) = amount;
    }else{
        cout << "set_stack element could not be accessed" << endl;
    }

}

void Game::reset(){
    cards.clear();
    bet_amounts.clear();
    last_action = -1;
    got_showdown = false;
    pot_size.clear();
    folded.clear();
    pos = (pos + 1)%num_players;//the dealer moves to the left
}

void Game::insert_info() {//inserts all the info needed to send to an SQL database (num_players, stack_amounts (pre and post), pos, names, blinds,bet_amounts,
    //pot_size, cards, bool got_showdown, int last_action, folded), send it all into database with SQL insert statement modified by return type
    string p_stack = "";
    for(int i = 0; i<pre_stack.size(); i++){//pre stack string
        p_stack = p_stack + std::to_string(pre_stack.at(i));
        if(i<pre_stack.size()-1){
            p_stack += ",";
        }
    }

    string post_stack = "";
    for(int i = 0; i<stack_amounts.size(); i++){//post stack string
        post_stack = post_stack + std::to_string(stack_amounts.at(i));
        if(i<stack_amounts.size()-1){
            post_stack += ",";
        }
    }

    string names_DB = ",";
    for(int i = 0; i<names.size(); i++){//names
        names_DB = names_DB + names.at(i);
        if(i<names.size()-1){
            names_DB += ",";
        }
    }
    names_DB += ",";

    string blinds_DB = "";
    for(int i = 0; i<blinds.size(); i++){//blinds
        blinds_DB = blinds_DB + std::to_string(blinds.at(i));
        if(i<names.size()-1){
            blinds_DB += ",";
        }
    }

    string bet_amounts_DB = "";
    for(int j = 0; j<bet_amounts.size(); j++){//bet_amounts
        if(j!=0){
            bet_amounts_DB += "!";
        }
        for(int k = 0; k<bet_amounts.at(j).size(); k++){
            if(k!=0){
                bet_amounts_DB += "|";
            }
            for(int i = 0;i<bet_amounts.at(j).at(k).size(); i++){
                bet_amounts_DB += std::to_string(bet_amounts.at(j).at(k).at(i));
                if(i<bet_amounts.at(j).at(k).size()-1){
                    bet_amounts_DB += ",";
                }
            }

        }

    }

    string pot_size_DB = "";
    for(int i = 0; i<pot_size.size(); i++){//pot_size
        pot_size_DB = pot_size_DB + std::to_string(pot_size.at(i));
        if(i<pot_size.size()-1){
            pot_size_DB += ",";
        }
    }

    string cards_DB = "";
    for(int i = 0; i<cards.size(); i++){//cards
        cards_DB = cards_DB + std::to_string(cards.at(i));
        if(i<cards.size()-1){
            cards_DB += ",";
        }
    }

    string folded_DB = ",";
    for(int i = 0; i<folded.size(); i++){//folded
        folded_DB = folded_DB + std::to_string(folded.at(i));
        if(i<folded.size()-1){
            folded_DB += ",";
        }
    }
    folded_DB += ",";

    int showdown{};
    if(got_showdown){
        showdown = 1;
    }else{
        showdown = 0;
    }

    vector<int> pots_known{};//modification of pot_size to account for it not always being size 4
    for(int i = 0; i<pot_size.size(); i++){
        pots_known.push_back(pot_size.at(i));
    }
    for(int i= pot_size.size(); i<4; i++){
        pots_known.push_back(-1);//-1 to represent that it's not known
    }

    vector<double> ss_known{};
    for (int i = 0; i < strength.size(); i++) {
        ss_known.push_back(strength.at(i));
    }
    for (int i = ss_known.size(); i < 3; i++) {
        ss_known.push_back(-1.0);
    }

    double pre_flop_per = read_pre_flop_db(cards.at(0), cards.at(1), "C:\\Users\\jeeva\\SQLite\\Poker.db");

    int POV_start_stack = pre_stack.at(0);

    string SQL_statement = "INSERT INTO HANDS (N_PLAYERS, PRE_STACK_AMOUNTS, POST_STACK_AMOUNTS, POSITION, NAMES, BIG_BLIND, SMALL_BLIND, BET_AMOUNTS, "
                           "PRE_FLOP_POT, FLOP_POT, TURN_POT, RIVER_POT, PLAYER_CARD_ONE, PLAYER_CARD_TWO, CARDS, "
                           " SHOWDOWN, LAST_ACTION, FOLDED, PLAYER_NAME, FLOP_STRENGTH, TURN_STRENGTH, RIVER_STRENGTH, PRE_FLOP_WIN_PER, POV_PRE_STACK, WINNER) "
                           "VALUES(" + std::to_string(num_players) + ", '" + p_stack + "', '" + post_stack + "', " + std::to_string(pos) + ", '" + names_DB + "', "
                           + std::to_string(blinds.at(0)) + ", " + std::to_string(blinds.at(1)) + ", '" + bet_amounts_DB + "', " + std::to_string(pots_known.at(0)) + ", "
                           + std::to_string(pots_known.at(1)) + ", " + std::to_string(pots_known.at(2)) + ", " + std::to_string(pots_known.at(3)) + ", " 
                           + std::to_string(cards.at(0))
                           + ", " + std::to_string(cards.at(1)) + ", '" + cards_DB + "', " + std::to_string(showdown) + ", " + std::to_string(last_action)
                           + ", '" + folded_DB + "', '" + names.at(0) + "', " + std::to_string(ss_known.at(0)) + ", " + std::to_string(ss_known.at(1))
                           + ", " + std::to_string(ss_known.at(2)) + ", " + std::to_string(pre_flop_per) + ", " + std::to_string(POV_start_stack) + ", '" + winner + "');";

    sqlite3* db;

    char* messageerror;

    int exit = sqlite3_open("C:\\Users\\jeeva\\SQLite\\Poker.db", &db);

    exit = sqlite3_exec(db, SQL_statement.c_str(), NULL, 0, &messageerror);

    if (exit != SQLITE_OK) {
        cerr << "error with insert" << endl;
        sqlite3_free(messageerror);
    }
    else {
        cout << "record inserted successfully" << endl;

    }

    sqlite3_close(db);

}

double Game::read_pre_flop(int c1, int c2) {
    if (num_players > 7) {
        return -1.0;
    }

    vector<int> cards{};
    int line{};
    cout << hand.deck.draw(c1) << endl;
    cout << hand.deck.draw(c2) << endl;

    cards.push_back((hand.deck.draw(c1).value) % 13);
    cards.push_back((hand.deck.draw(c2).value) % 13);
    sort(cards.begin(), cards.end());

    if (hand.deck.draw(c1).suit != hand.deck.draw(c2).suit) {
        cards.at(1) += 13;
    }
    for (int i = 0; i < cards.at(0); i++) {
        line += (25 - i);
    }
    line += 2;
    line += (cards.at(1) - (cards.at(0) + 1));
    line += 326 * (num_players - 1);

    ifstream ifs("../pre_flop_table.txt");

    string ratio_line{};
    ifs.seekg(std::ios::beg);
    string line_new{};
    string line_new_2{};
    string line_new_3{};
    double ratio{};
    int count = 0;
    while (getline(ifs, line_new, ',')) {
        count++;
        if (count == line) {
            istringstream iss{ line_new };
            iss >> line_new_2;
            iss >> line_new_3;
            iss >> ratio;
            break;
        }
    }

    ifs.close();
    return ratio;

}

void Game::gen_pre_flop_db(const char *s) {
    sqlite3* db;
    string c1{};
    string c2{};
    int num = 6;

    char* messageerror;

    int exit = sqlite3_open(s, &db);
    string SQL_statement{};

    vector<double> ratios_ret{};

    ifstream input_stream("../pre_flop_table.txt");

    input_stream.seekg(std::ios::beg);
    string line{};
    string values{};
    string ratio{};
    int count = 1;
    vector<string> vals{};
    while (getline(input_stream, line)) {
        vals.clear();
        if ((count - 1) % 325 == 0) {
            num++;
        }        
        istringstream iss{ line };
        while (getline(iss, values, ',')) {
            vals.push_back(values);
        }

        SQL_statement = "INSERT INTO PRE_FLOP(PLAYER_CARD_ONE, PLAYER_CARD_TWO, WIN_PER, N_PLAYERS) values(" + vals.at(0) + ", " + vals.at(1) +
            ", " + vals.at(2) + ", " + std::to_string(num) + ");";


        exit = sqlite3_exec(db, SQL_statement.c_str(), NULL, 0, &messageerror);

        if (exit != SQLITE_OK) {
            cerr << "error with insert" << endl;
            sqlite3_free(messageerror);
            break;
        }
        else {
            cout << "record inserted successfully" << endl;

        }
        count++;
    }
    sqlite3_close(db);
    input_stream.close();


}

double Game::read_pre_flop_db(int c1, int c2, const char *s) {
    if (num_players > 7) {
        return -1.0;
    }

    vector<int> cards{};
    int line{};
    bool recieved = false;

    cards.push_back((hand.deck.draw(c1).value) % 13);
    cards.push_back((hand.deck.draw(c2).value) % 13);
    sort(cards.begin(), cards.end());

    if (hand.deck.draw(c1).suit != hand.deck.draw(c2).suit) {
        cards.at(1) += 13;
    }

    sqlite3* db;
    int exit = sqlite3_open(s, &db);

    if (exit != SQLITE_OK) {
        cout << "error with test column" << endl;
    }

    sqlite3_stmt* ppstmt{};

    string sql = "SELECT WIN_PER FROM PRE_FLOP WHERE (PLAYER_CARD_ONE = " + to_string(cards.at(0)) + ") AND (PLAYER_CARD_TWO = " + to_string(cards.at(1)) +
        ") AND (N_PLAYERS = " + to_string(num_players) + ");";

    sqlite3_stmt* stmt;
    int j{};
    string text{};
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
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
    return NULL;
    
}




