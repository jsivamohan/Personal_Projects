//
// Created by jeeva on 12/26/2022.
//

#ifndef POKER_NEW_INFO_H
#define POKER_NEW_INFO_H

#include <vector>


class Info {//class will handle all of the information regarding extrapolation of hands and possibly some ML analysis

    double read_pre_flop(int i, int j);//passed in the indices of the two cards and returns their preflop ratio from the table

    double extrapolate_post(std::vector<int> &cards);//given the vector of all of the cards in play (flop along with both player's
    //cards) and determines the EV of the hand

};


#endif //POKER_NEW_INFO_H
