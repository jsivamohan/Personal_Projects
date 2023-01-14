//
// Created by jeeva on 12/25/2022.
//

#ifndef POKER_NEW_DECK_H
#define POKER_NEW_DECK_H


#include "Card.h"
#include <vector>
#include <string>

class Deck{//class to define the deck of cards the hand class will be using to draw from

    std::vector<Card> deck;

public:

    Deck();

    //~Deck();//detructor to get rid of the dynamically allocated deck

    void Identify_Card(int index);//given index tells you what card it is

    Card draw(unsigned int index);//returns a card drawn from the deck based on the index



};


#endif //POKER_NEW_DECK_H
