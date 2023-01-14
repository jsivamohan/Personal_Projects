//
// Created by jeeva on 1/9/2023.
//

#ifndef POKER_NEW_REPLAY_H
#define POKER_NEW_REPLAY_H

#include <vector>
#include <string>
#include "Deck.h"
#include "Hand.h"


class Replay {//class specific to handle to replaying of the hand from the info form the db (along with handling all the
    //help stats calculated throughout
public:
    Hand hand{};
    int num_players;
    std::vector<int> stack_amounts;//the starting stack amounts of each player that will be modified as bets come in and out
    int pos;//your position relative to the dealer (iterates on every hand)
    std::vector<std::string> names;//the names of the players you are playing against (if no names given then give default values '1', '2', etc)
    std::vector<int> blinds;//the blinds being played (at this point just an identifier but can be used later)
    std::vector<std::vector<std::vector<int>>> bet_amounts;//2d vector representing the bet amount of each player in the hand (in order of betting) a fold represented by -1
    std::vector<int> pot_size;//size of the pot that will be added to as the hand unfolds, full amount will go to the winner (or split if that happens)
    std::vector<int> cards;//vector referencing the cards being played (first two are the player's cards and the next five are the flop, turn, river)
    bool got_showdown;//bool object to represent if we saw opponent cards
    int pot;
    int last_action;
    std::vector<int> folded;
    std::vector<double> strength;
    double pre_flop_win_p;
    std::string winner;//name of the winner

    Replay(int players, std::vector<int> s_amounts, int position, std::vector<std::string> name, std::vector<int> blind,std::vector<std::vector<std::vector<int>>> b_amounts,
           std::vector<int>p_size, std::vector<int> c, bool g_showdown, int last_action, std::vector<int>folded, std::vector<double>, double p_flop_win, std::string winner);

    Replay();

    void replay_hand();//replays the hand with the initialization from the constructor (which is initialized from the fields retrieved from the db which is taken from the
    //parameters of the game class

    void display(int sit);//display the current state of the hand (variation of display object from the game class), sit is which situation we are in (0 is pre-flop,
    //1 is flop, 2 is turn, 3 is river) need separate display for the showdown

    void summary();//summary of all the info for this replay object to display in the search UI

    double calc_odds(std::vector<int> &this_cards, double num_iterations, const char* s, int players_left);//calculate the number of hands you beat, the number of hands that beat you and the overall odds you win (given the 
    //number of players) also adds to the DB since wasn't already there

};


#endif //POKER_NEW_REPLAY_H
