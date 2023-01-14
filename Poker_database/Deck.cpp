//
// Created by jeeva on 12/25/2022.
//

#include "Deck.h"
#include <vector>
#include <iostream>

using namespace std;

Deck::Deck(){//initializes the deck vector
    Card c{};
    for(int i= 0; i<4; i++){
        for(int j = 0; j<13; j++){
            c.value = j;
            c.suit = i;
            deck.push_back(c);
        }
    }
}

Card Deck::draw(unsigned int index){
    return deck.at(index);
}

void Deck::Identify_Card(int index) {
    Card c = draw(index);
    cout << c;
}

