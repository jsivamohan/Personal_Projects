//
// Created by jeeva on 12/25/2022.
//

#ifndef POKER_NEW_HAND_H
#define POKER_NEW_HAND_H


#include "Card.h"
#include "Deck.h"
#include <vector>
#include <string>


class Hand {//plays a single hand of poker (heads up for now)

public:
    enum streak_vals{
        clubs,
        spades,
        hearts,
        diamonds,
        pairs,
        three_kind,
        four_kind,
        sequential
    };

    enum hands{
        high_card,
        one_pair,
        two_pair,
        three_of_kind,
        straight,
        flush,
        full_house,
        four_of_kind,
        straight_flush,//royal flush is just a special straight flush do don't include
        Ace_straight,
        Ace_straight_flush//special case to consider
    };

    Deck deck;
    std::vector<double> ratios;//vector initialized during construction which has all the pre-flop ratios read from the text file

    class nest_Compare{
    public:
        bool operator()(int const &a, int const &b) {
            return (a%13) < (b%13);
        }
    };

    double calc_strength(std::vector<int> &cards);//takes in the five best cards in the hand and returns the strength value of the hand

    Hand();//constructor to initialize the pre-flop table

    double resolve_hand(std::vector<int> indices);//takes in the seven indices representing the 5 table cards and the cards of the player
    //and determining the strength of their hand

    double play_heads_set_number(int c1, int c2, int num_players);//plays a hand with num_players and reports winner

    double sim_hands(double num_trials, int c1, int c2, int num_players);//sim num_trial number of hands with a specified starting hand

    std::vector<double> read_pre_flop();//read the pre-flop table to get the ratio

    double get_pre_flop(int i, int j);//get the pre-flop ratio of the cards with index i and j (i must always be < j)

    double extract_post_flop(std::vector<int> &cards);//given the cards currently on the table (flop and your cards) calculate
    //the EV of the hand (using standard formula for EV with CDF uniform)

    double test_EV(int num_trials, int c1, int c2, int c3, int c4);//test the correlation of the relative EV values with actual results simulated over num_trials

    std::pair<double, double> EV_correlation(int num_trials, std::vector<int> &cards);//used to collected pairs connecting EV values post flop with the actual win % of the hand
    //over num_trials heads up

    int find_distance(int i, int j);//find the distance between the cards at index i and j

    void gen_EV_correlation(double num_trials, double num_iterations);//gens the EV correlation data with num_trials to get ratio and num_iterations number of data points

    void gen_pre_flop_adjusted(double num_trials, int num_players);//gens the preflop stats over num_trials of hands for each preflop hand (just suited and non-suited not full combos)

    void test_resolve(int num_trials);//helper method to test resolve hand for accuracy by genning num_trials number of hands as well as the results from resolve hand analysis

    void gen_strength_ratio(int num_trials, int num_iterations);//gen num_iterations of data points comparing a hands strength score vs the win % of that hand

    double convert_strength_to_win(double strength);//converts a given strength score to a corresponding win rate based on the trained multi-variable polynomial regression model

    void gen_EV_correlation_plus(double num_trials, double num_iterations);//gens a new EV correlation file with extra information for card composition to hopefully improve
    //accuracy

    void gen_pre_flop(int num_players, double num_trials);//gen pre-flop number for the number of players specified

    double pre_flop_vs(std::vector<int> cards, const char* s, double num_iterations);//calculate the win percentage for c1 and c2 vs c3 and c4 (consult DB and if not there then calculate by hand and return)


};


#endif //POKER_NEW_HAND_H
