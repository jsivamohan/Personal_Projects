//
// Created by jeeva on 12/25/2022.
//

#ifndef POKER_NEW_CARD_H
#define POKER_NEW_CARD_H

#include <iostream>
#include <string>
#include <iostream>
//#include <map>

struct Card{

    friend std::ostream& operator<<(std::ostream& os, const Card& c);

    int value;
    unsigned int suit;//include an enumeration between suits and integers
    enum Suits{
        clubs,
        spades,
        hearts,
        diamonds
    };

    enum values{//map the values to the actual card values
        two,
        three,
        four,
        five,
        six,
        seven,
        eight,
        nine,
        ten,
        Jack,
        Queen,
        King,
        Ace
    };



    Card(int v, unsigned int s);

    Card();

    bool operator <(const Card &c)const;//will check to see if a card is better or worse than another by comparing their value

    bool operator >(const Card &c)const;

    bool operator==(const Card &c);//equal if they have the same value (should never have the same value and suit)

    bool compareVal(const Card &c);

    bool compareSuit(const Card &c);

};




#endif //POKER_NEW_CARD_H
