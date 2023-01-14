//
// Created by jeeva on 1/6/2023.
//

#ifndef POKER_NEW_GAME_H
#define POKER_NEW_GAME_H

#include "Hand.h"
#include <vector>
#include <string>


class Game {//class for storing and displaying information about a single game (hand that was played), information to initialize the class will be stored in the
    //database to be called later
public:
    Hand hand{};
    int num_players;
    std::vector<int> stack_amounts;//the starting stack amounts of each player that will be modified as bets come in and out
    int pos;//your position relative to the dealer (iterates on every hand)
    std::vector<std::string> names;//the names of the players you are playing against (if no names given then give default values '1', '2', etc)
    std::vector<int> blinds;//the blinds being played (at this point just an identifier but can be used later)
    std::vector<std::vector<std::vector<int>>> bet_amounts;//2d vector representing the bet amount of each player in the hand (in order of betting) a fold represented by -1, not in hand
    //represented by -2
    std::vector<int> pot_size;//size of the pot that will be added to as the hand unfolds, full amount will go to the winner (or split if that happens)
    std::vector<int> cards;//vector referencing the cards being played (first two are the player's cards and the next five are the flop, turn, river)
    bool got_showdown;//bool object to represent if we saw opponent cards
    int last_action;//index of the last person to take action if we go to showdown
    std::vector<int>folded;//keeps track of the indices of the people who have folded in the game
    bool won{};//boolean to show if you've won or not
    std::vector<int>pre_stack;//save the pre-stack value to use in the DB
    std::vector<double> strength;//vector containing the strength of each hand from post-flop to river
    std::string winner;//name of the winner of the hand

    Game();

    void prompt(const std::string& id);//help set initial values of the game object (num_players, starting stack_amounts, names, blinds, starting pos), name references
    //the id of the player (will always be the first person in the names vector)

    bool display();//display the structure of the table (with an indicator for big blind, small blind, and dealer) as well as the bet amounts referencing the bet_amounts
    //vector (return false if the player wants to quit and not input it in the table)

    bool play();//play out the hand to set the values of the bet_amounts vector along with adding to the pot_size vector at each interval (pre-flop, post-flop, turn, river)
    //and changing the stack amounts accordingly (return false if the player quit)

    void play_hands(std::string id);//play multiple hands looped until we are done playing

    void set_stack(int amount, int player);//change the stack amount of a particular player to another value for some reason

    void set_pot(int amount, int situation);//change the pot_size at any point in the hand for any reason

    void set_bet(int amount, int player, int situation, int iteration);//change the bet amounts of a player at a point for some reason

    int get_index(int value, char suit);//gets the index in the deck of a card with value and suit given

    bool betting(bool preflop);//start the betting phase of the round (folded is a vector with the ints of the positions of each
    // of the players who have folded (pre-flop set to true if pre-flop as order of betting is different pre-flop vs post-flop), return true if game still going, false if
    //game is over

    void show_cards(int sit);//shows the next card(s) (3 cards if flop, 1 card if turn or river), cross-references with the cards vector to ensure no double cards

    void print_bet_amounts();//helper functions to print bet_amounts (because 3d array)

    void showdown();//does the showdown of the remaining players in the hand

    void insert_info();//print all the info needed to send to an SQL database (num_players, stack_amounts (pre and post), pos, names, blinds,bet_amounts,
    //pot_size, cards, bool got_showdown, int last_action, folded)
    
    void reset();//reset in between hands by clearing or modifying necessary fields

    double read_pre_flop(int c1, int c2);//given cards at index c1 and c2 return the preflop win percentages from table

    int check_input_int(std::string input);//ensures the entries given by the player are valid

    void gen_pre_flop_db(const char* s);//fill the database with all the preflop value for each number of players

    double read_pre_flop_db(int c1, int c2, const char *s);//read from the pre_flop db
};


#endif //POKER_NEW_GAME_H
