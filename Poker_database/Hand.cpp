//
// Created by jeeva on 12/25/2022.
//

#include "Hand.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <cstdlib>
#include <math.h>
#include <fstream>
#include <sstream>
#include <random>
#include <cmath>
#include <sqlite3.h>
#include <set>


using namespace std;

Hand::Hand(){
    ratios = read_pre_flop();
}


double Hand::calc_strength(std::vector <int>& cards) {

    //the element in the last position of cards is the type of hand we are dealing with to calc strength score
    int type_hand = cards.at(cards.size()-1);

    double strength_score{};

    if(cards.at(cards.size()-1)==Ace_straight){//don't need to add anything as these are the lowest straights/straight flushes you can have
        strength_score = straight;
    }
    else if(cards.at(cards.size()-1)==Ace_straight_flush){
        strength_score = straight_flush;
    }
    else{
        strength_score = cards.at(cards.size()-1);

        for(double i= 0; i<cards.size(); i++){
            strength_score += (((double)cards.at(i)/13.0)*100.0) * pow(10.0, (-2.0 * (i+1)));//gets a percentage of the higher cards instead to make the calculation
            //more continuous
        }
    }

    return strength_score;
}

double Hand::resolve_hand(std::vector<int> indices) {//temp change to actual cards should end up being indices

    vector<Card> cards {};

    vector<int> seq_vec{};

    int k{};

    vector<int> best_values{};//vector used to help track best value cards (flexible def based on needs of the specific hand)
    vector<pair<int, int>> streaks_values{};//vector that matches the streak with the value of card associated

    for(unsigned int i : indices){
        cards.push_back(deck.draw(i));
    }


    sort(cards.begin(), cards.end(), greater<Card>());//will sort in descending order

    for(int i = 0;i<cards.size(); i++){
        if(i==0){
            seq_vec.push_back(cards.at(i).value);
        }
        else if(cards.at(i-1).value != cards.at(i).value){
            seq_vec.push_back(cards.at(i).value);
        }
    }

    //run through the sorted 5 card hands and track sequential ordering, number of each suit, and pairs/three of a kind/four of
    //a kind
    vector<unsigned int> streaks = {0,0,0,0,0,0,0,0,0};//streaks[0-3] for the number of each of the four suits, streaks[4] = number of pairs, streaks[5] = number of 3 of a kind
    //streaks[6] = number of 4 of a kind,l, streaks[7] are sequential tracking

    vector<int> best_cards{};//vector of the five best cards in the set of 7 (just send values with last number representing the type of hand)

    Card card{};
    int val{};

    for(int i= 0; i<cards.size(); i++){
        streaks[cards.at(i).suit]++;
        card = cards.at(i);
        if(i>0 && cards.at(i-1).value == card.value){
            if(i>1 && cards.at(i-2).value == card.value){
                if(i>2 && cards.at(i-3).value== cards.at(i).value){//four of a kind possible
                    streaks[four_kind]++;
                    streaks_values.push_back(make_pair(four_kind, card.value));
                    continue;
                }
                if((i<cards.size()-1) && cards.at(i+1).value == card.value) {

                }else{
                    streaks[three_kind]++;
                    streaks_values.push_back(make_pair ( three_kind, card.value ) );
                }
                continue;
            }
            if((i<cards.size()-1) && cards.at(i+1).value == card.value){//only push if there are no further pairs

            }else{
                streaks_values.push_back(make_pair(pairs, card.value));
                streaks[pairs]++;
            }
        }

        if(i<seq_vec.size()){
            val = seq_vec.at(i);
            if(streaks[sequential]<4){//only need to check if we haven't reached a streak length yet (because descending order found streak is the highest streak)
                if(i>0 && (seq_vec.at(i-1)-1) == val){//in sequential order
                    k = i;
                    if(i>1 && (seq_vec.at(i-2)-2)!= val){//if true then this is a new sequence
                        streaks[sequential] = 1;
                        continue;
                    }
                    streaks[sequential]++;
                    if(streaks[sequential]==3 && val==0 && seq_vec.at(0) == 12){
                        streaks[sequential]++;
                        streaks_values.push_back(make_pair(sequential, -1));//card.value is the lowest card in the sequence
                    }
                    else if(streaks[sequential]>3){
                        streaks_values.push_back(make_pair(sequential, val));//card.value is the lowest card in the sequence
                    }
                }
            }
        }


    }
    int c_value{};
    //work down the hierarchy to figure out the five best cards
    if((streaks[spades] >4 || streaks[clubs] > 4 || streaks[hearts] > 4 || streaks[diamonds] > 4) && streaks[sequential]>3){//we have a straight flush
//make sure the cards that make the flush are the same ones that make the straight

    int suit{};

    Card c1{};
    Card c2{};
    Card c3{};
    Card c4{};
    Card c5{};

    if(streaks[spades]>4){
        suit = spades;
    }
    else if(streaks[clubs]>4){
        suit = clubs;
    }
    else if(streaks[hearts]>4){
        suit = hearts;
    }
    else{//diamonds
        suit = diamonds;
    }

        for(pair<int,int> p : streaks_values){
            if(p.first==sequential){
                if(p.second==-1){//special case of the Ace low flush

                    c1.value = 0;
                    c1.suit = suit;

                    c2.value = 1;
                    c2.suit = suit;

                    c3.value = 2;
                    c3.suit = suit;

                    c4.value = 3;
                    c4.suit = suit;

                    c5.value = 12;
                    c5.suit = suit;

                    if(find(cards.begin(), cards.end(),c1) != cards.end()&& find(cards.begin(), cards.end(),c2) != cards.end()&& find(cards.begin(), cards.end(),c3)!= cards.end()
                    && find(cards.begin(), cards.end(),c4) != cards.end()&& find(cards.begin(), cards.end(),c5)!= cards.end()){
                        best_cards.push_back(2);//this is all we need if it's straight flush (lowest card)
                        best_cards.push_back(Ace_straight_flush);
                    }

                    else{
                        for(int i = 0; i<cards.size(); i++){
                            if(cards.at(i).suit == suit){
                                best_cards.push_back(cards.at(i).value);
                            }
                            if(best_cards.size()>4){
                                best_cards.push_back(flush);
                                break;
                            }
                        }
                    }

                }

                else{

                    c_value = p.second;

                    c1.value = c_value;
                    c1.suit = suit;

                    c2.value = c_value+1;
                    c2.suit = suit;

                    c3.value = c_value+2;
                    c3.suit = suit;

                    c4.value = c_value+3;
                    c4.suit = suit;

                    c5.value = c_value+4;
                    c5.suit = suit;

                    if(find(cards.begin(), cards.end(),c1) != cards.end() && find(cards.begin(), cards.end(),c2) != cards.end() && find(cards.begin(), cards.end(),c3) != cards.end()
                       && find(cards.begin(), cards.end(),c4) != cards.end() && find(cards.begin(), cards.end(),c5)!= cards.end()){

                        best_cards.push_back(p.second);//this is all we need if it's straight flush (lowest card)
                        best_cards.push_back(straight_flush);
                    }

                    else{
                        for(int i = 0; i<cards.size(); i++){
                            if(cards.at(i).suit == suit){
                                best_cards.push_back(cards.at(i).value);
                            }
                            if(best_cards.size()>4){
                                best_cards.push_back(flush);
                                break;
                            }
                        }
                    }

                }

            }
        }

    }//straight flush

    else if(streaks[four_kind]>0){

        for(pair<int,int> p : streaks_values){
            if(p.first == four_kind){
                c_value = p.second;
                best_cards.push_back(p.second);
                break;
            }

        }



        for(int i = 0; i<cards.size(); i++){
            if(cards.at(i).value != c_value){//find the next highest card to add as high card
                best_cards.push_back(cards.at(i).value);
                best_cards.push_back(four_of_kind);
                break;
            }
        }
    }//four of a kind

    else if(streaks[three_kind]>0 && streaks[pairs] > 0){
        for(pair<int,int> p : streaks_values){
            if(p.first==three_kind){
                best_cards.push_back(p.second);
                break;
            }
        }

        for(pair<int,int> p : streaks_values){
            if(p.first == pairs){
                best_cards.push_back(p.second);
                break;
            }
        }
        best_cards.push_back(full_house);
    }//full house

    else if((streaks[spades] >4 || streaks[clubs] > 4 || streaks[hearts] > 4 || streaks[diamonds] > 4)){
        int suit{};
        if(streaks[spades]>4){
            suit = spades;
        }
        else if(streaks[clubs]>4){
            suit = clubs;
        }
        else if(streaks[hearts]>4){
            suit = hearts;
        }
        else{//diamonds
            suit = diamonds;
        }
        for(int i = 0; i<cards.size(); i++){
            if(cards.at(i).suit == suit){
                best_cards.push_back(cards.at(i).value);
            }
            if(best_cards.size()>4){
                best_cards.push_back(flush);
                break;
            }
        }

    }//flush

    else if(streaks[sequential]>3){
        for(pair<int,int> p : streaks_values){
            if(p.first == sequential){
                if(p.second==-1){
                    best_cards.push_back(2);
                    best_cards.push_back(Ace_straight);

                }else{
                    best_cards.push_back(p.second);
                    best_cards.push_back(straight);

                }
                break;
            }
        }
    }//straight

    else if(streaks[three_kind]>0){
        for(pair<int,int> p : streaks_values){
            if(p.first == three_kind){
                c_value = p.second;
                best_cards.push_back(p.second);
                break;
            }
        }
        for(Card c : cards){
            if(c.value != c_value){
                best_cards.push_back(c.value);
            }
            if(best_cards.size()>2){
                best_cards.push_back(three_of_kind);
                break;
            }
        }
    }//three of a kind

    else if(streaks[pairs]>1){
        vector<int> cvalues{};
        for(pair<int,int> p : streaks_values){
            if(p.first == pairs){
                c_value = p.second;
                best_cards.push_back(c_value);
                cvalues.push_back(c_value);
                if(cvalues.size()>1){//found the two highest pairs
                    break;
                }
            }
        }
        for(Card c : cards){
            if(c.value != cvalues.at(0) && c.value != cvalues.at(1)){
                best_cards.push_back(c.value);
                best_cards.push_back(two_pair);
                break;
            }
        }
    }//two pair

    else if(streaks[pairs]>0){
        for(pair<int,int> p : streaks_values){
            if(p.first == pairs){
                c_value = p.second;
                best_cards.push_back(c_value);
                break;
            }
        }
        for(Card c : cards){
            if(c.value != c_value){
                best_cards.push_back(c.value);
                if(best_cards.size()>3){
                    best_cards.push_back(one_pair);
                    break;
                }
            }
        }
    }//one pair

    else{
        for(int i = 0; i<5; i++){
            best_cards.push_back(cards.at(i).value);
        }
        best_cards.push_back(high_card);
    }//high card


    return calc_strength(best_cards);
}//method to find the five best cards out of the hand and then use calc_strength to find the strength score and return it

double Hand::play_heads_set_number(int c1, int c2, int num_players) {//method to play a hand of poker (c1 and c2 represent the index of the hand you want to test in a heads up game)

    unordered_set<int> vals{};//set to ensure we don't copy any indices when choosing a hand

    vector<int> indices_2 {};


    uniform_int_distribution<int> distribution(0,51);

    int index {};
    int count = rand() % 150;

    default_random_engine generator(rand() % (100 + count));
    while(true){

        index = distribution(generator);
        if( (index!= c1) && (index != c2) && vals.find(index) == vals.end()){//not in the set
            vals.insert(index);
            if(vals.size()>4){
                break;
            }
        }
        count++;
    }

    vector<int> player_cards{};
    player_cards.push_back(c1);
    player_cards.push_back(c2);

    //loop once for each player and then append to vals at the end
    while(true){
        index = distribution(generator);
        if(find(player_cards.begin(), player_cards.end(), (index)) == player_cards.end() && (vals.find(index)==vals.end())){//not in the set
            player_cards.push_back(index);
            if(player_cards.size()>=(2 * num_players)){
                break;
            }
        }
    }
    
    vector<double> strength_scores{};
    indices_2.insert(indices_2.end(), vals.begin(), vals.end());

    for (int i = 0; i < num_players; i++) {
        indices_2.resize(5);
        indices_2.push_back(player_cards.at(2 * i));
        indices_2.push_back(player_cards.at(2 * i + 1));
        strength_scores.push_back(resolve_hand(indices_2));
    }
    double max_value = -1.0;
    int tie = 1;
    bool won = true;
    max_value = strength_scores.at(0);
    for (int i = 1; i < strength_scores.size(); i++) {
        if (strength_scores.at(i) > max_value) {
            won = false;
            break;
        }
        else if (strength_scores.at(0) == strength_scores.at(i) == max_value) {
            tie++;
        }
    }

    if (won) {
        return 1 / tie;
    }
    else {
        return 0;
    }
}

double Hand::sim_hands(double num_trials, int c1, int c2, int num_players) {
    double wins = 0;
    for(int i= 0; i<num_trials; i++){
        wins += play_heads_set_number(c1, c2, num_players);
    }
    return wins / num_trials;
}

double Hand::get_pre_flop(int i, int j) {
    int index{};
    for(int k = 0; k<i; k++){
        index += (51-k);
    }

    index += (j - (i+1));
    return ratios.at(index);
}

vector<double> Hand::read_pre_flop() {//must sort higher index second always
    vector<double> ratios_ret{};

    int line{};

    ifstream input_stream("../preliminary_preflop_ratios.txt");

    string ratio_line {};
    input_stream.seekg(std::ios::beg);
    string line_new{};
    string line_new_2{};
    double ratio {};
    int count = 0;
    while(getline(input_stream,line_new)){
        count++;
        if(count %2==0){
            
            istringstream iss(line_new);
            iss >> line_new_2;
            iss >> ratio;
            ratios_ret.push_back(ratio);
        }

    }

    input_stream.close();

    return ratios_ret;

}

double Hand::extract_post_flop(std::vector<int> &cards) {//cards is all of the indices already accounted for
    unordered_set<int> set_cards{};
    std::copy(cards.begin(),
              cards.end(),
              std::inserter(set_cards, set_cards.end()));

    double sum = 0;

    int count = 0;

    for(int i= 0;i<52; i++){
        for(int j = i+1; j<52; j++){
            if(set_cards.find(i) == set_cards.end() && set_cards.find(j)== set_cards.end()){//neither i or j is in the set of cards
                cards.push_back(i);
                cards.push_back(j);
                sum += convert_strength_to_win(resolve_hand(cards));
                cards.resize(set_cards.size());//resizes the vector back down to its original size (cuts out the extra two elements just added)
            }else{
                if(set_cards.find(i) != set_cards.end()){
                    i++;
                }
            }
        }
    }
    return sum/1081.0;
}

double Hand::test_EV(int num_trials, int c1, int c2, int c3, int c4) {
    default_random_engine generator;

    double denom = (double)num_trials;

    uniform_int_distribution<int> distribution(0,51);

    double hit=0;


    unordered_set<int> vals{};//set to ensure we don't copy any indices when choosing a hand
    vector<int> indices1{};
    vector<int> indices2{};

    double s1{};
    double s2{};

    vector<int> indices_1 {};
    vector<int> indices_2 {};

    //procedure will be first to allocate the three pre-flop cards, do extract_post_flop on them with each of the player's cards,
    //then do resolve hand on the full 7 after you add
    int index {};
    double ev1{};
    double ev2{};



    for(int i = 0; i<num_trials; i++){
        indices2.clear();
        indices1.clear();
        indices_2.clear();
        indices_1.clear();
        vals.clear();

        while(true){//generates the flop
            index = distribution(generator);
            if((index!=c1) && (index!=c2) && (index!=c3) && (index!=c4) && (vals.find(index) == vals.end())){//not in the set
                vals.insert(index);
                if(vals.size()>2){
                    break;
                }
            }
        }

        indices_1.insert(indices_1.end(), vals.begin(), vals.end());
        indices_2.insert(indices_2.end(), vals.begin(), vals.end());

        indices_1.push_back(c1);
        indices_1.push_back(c2);

        indices_2.push_back(c3);
        indices_2.push_back(c4);

        ev1 = extract_post_flop(indices_1);
        ev2 = extract_post_flop(indices_2);

        while(true){//generates the flop
            index = distribution(generator);
            if((index!=c1) && (index!=c2) && (index!=c3) && (index!=c4) && (vals.find(index) == vals.end())){//not in the set
                vals.insert(index);
                if(vals.size()>4){
                    break;
                }
            }
        }

        indices1.insert(indices1.end(), vals.begin(), vals.end());
        indices2.insert(indices2.end(), vals.begin(), vals.end());

        indices1.push_back(c1);
        indices1.push_back(c2);

        indices2.push_back(c3);
        indices2.push_back(c4);

        s1 = resolve_hand(indices1);
        s2 = resolve_hand(indices2);

        if(abs((ev1-ev2))>0.5){
            if(ev1>ev2){
                if(s1>s2){
                    hit++;
                }
            }else{
                if(s2>s1){
                    hit++;
                }
            }
        }else{
            denom--;
        }
    }

    return hit/denom;


}

pair<double,double> Hand::EV_correlation(int num_trials, vector<int> &cards) {//cards is a vector size 5 where the last two cards are the players cards and the first three
    //are the flop

    double ev = extract_post_flop(cards);
    double s1{};
    double s2{};
    int index {};
    double ratio= 0;

    vector<int> opp_hand{};

    int c1 = -1;

    default_random_engine generator;
    uniform_int_distribution<int> distribution(0,51);

    for(int k = 0; k<num_trials; k++){
        cards.resize(5);
        opp_hand.clear();
        c1 = -1;
        while(true){
            index = distribution(generator);
            if(find(cards.begin(), cards.end(), index)== cards.end() && index != c1){//not in the set
                c1 = index;
                opp_hand.push_back(index);
                if(opp_hand.size()>1){
                    break;
                }
            }
        }

        while(true){
            index = distribution(generator);
            if(find(cards.begin(), cards.end(), index)== cards.end() && find(opp_hand.begin(), opp_hand.end(), index)== opp_hand.end()){//not in the set
                cards.push_back(index);
                opp_hand.push_back(index);
                if(cards.size()>6){
                    break;
                }
            }
        }

        for(int i= 0; i<3;i++){
            opp_hand.push_back(cards.at(i));
        }

        s1 = resolve_hand(cards);
        s2 = resolve_hand(opp_hand);

        if(s1>s2){
            ratio++;
        }
    }
    return make_pair(ev,ratio/(double)num_trials);

}

int Hand::find_distance(int i, int j) {
    Card c1 = deck.draw(i);
    Card c2 = deck.draw(j);
    if(c1.value==12 && (c2.value==0 || c2.value==1 || c2.value==2 || c2.value==3)){
        return c2.value +1;
    }
    else if(c2.value==12 && (c1.value==0 || c1.value==1 || c1.value==2 || c1.value==3)){
        return c1.value +1;
    }
    return abs((c1.value - c2.value));
}

void Hand::gen_EV_correlation(double num_trials, double num_iterations) {
    ofstream ofs("../EV_new_correlations.txt");

    vector<int> cards{};

    uniform_int_distribution<int> distribution(0,51);
    pair<double,double> EV_corr{};
    int index{};

    double percent=0;

    for(double i = 0; i<num_iterations; i++){
        cards.clear();
        default_random_engine generator(rand() %(100+5*(int)i));
        while(true){
            index = distribution(generator);
            if(find(cards.begin(), cards.end(), index)== cards.end()){
                cards.push_back(index);
            }
            if(cards.size()>4){
                break;
            }
        }
        EV_corr = EV_correlation(num_trials,cards);
        ofs << EV_corr.first << "," << EV_corr.second << endl;
        cout << "percent done: " << (i/num_iterations)*100.0 << endl;
    }

    ofs.close();
}

void Hand::gen_pre_flop_adjusted(double num_trials, int num_players) {
    ofstream ofs("../pre_flop_adjusted.txt");

    int suit{};
    double count{};
    double ratio{};

    ofs << "i_value,j_value,distance,suited,ratio" << endl;

    for(int i = 0; i<26; i++){//only have to do suited and offsuit as the rest are essentially the sane idea
        for(int j = i+1; j<26; j++){
            ratio = sim_hands(num_trials, i,j, num_players);
            if(deck.draw(i).suit!=deck.draw(j).suit){
                suit = 0;
            }else{
                suit = 1;
            }

            ofs << deck.draw(i).value << "," << deck.draw(j).value << "," << find_distance(i,j) << "," << suit << "," << ratio << endl;
            count++;
            cout << "percent done: " << (count/325.0)*100.0 << endl;
        }
    }

    ofs.close();
}

void Hand::test_resolve(int num_trials){
    int index{};
    uniform_int_distribution<int> distribution(0,51);

    vector<int> cards{};

    for(int i= 0; i<20; i++){
        cards.clear();
        default_random_engine generator(rand() %250);
        while(true){
            index = distribution(generator);
            if(find(cards.begin(), cards.end(), index)== cards.end()){
                cards.push_back(index);
            }
            if(cards.size()>6){
                break;
            }
        }
        for(auto c : cards){
            deck.Identify_Card(c);
            cout << endl;
        }
        cout << resolve_hand(cards) << endl;
        cout << endl << endl;
    }
}

void Hand::gen_strength_ratio(int num_trials, int num_iterations) {

    vector<int> cards{};
    double ratio{};
    int index{};
    uniform_int_distribution<int> distribution(0,51);
    uniform_int_distribution<int> opp_distribution(0,6);
    vector<int> opp_cards{};
    double s1{};
    double s2{};
    int c1 = -1;
    double done = 0;
    vector<int> no_vals{};

    ofstream ofs("../Strength_score_vs_win.txt");

    ofs << "strength_score,win_rate" << endl;

    for(int i= 0; i<num_iterations; i++){
        cards.clear();
        ratio = 0;
        default_random_engine generator(rand() %250);
        while(true){
            index = distribution(generator);
            if(find(cards.begin(), cards.end(), index)== cards.end()){
                cards.push_back(index);
            }
            if(cards.size()>6){
                break;
            }
        }
        s1 = resolve_hand(cards);
        for(int k = 0; k<num_trials; k++){
            c1 = -1;
            opp_cards.clear();
            no_vals.clear();
            while(true){
                index = distribution(generator);
                if(find(cards.begin(), cards.end(), index)== cards.end() && find(opp_cards.begin(), opp_cards.end(), index)==opp_cards.end()){
                    opp_cards.push_back(index);
                }
                if(opp_cards.size()>1){
                    break;
                }
            }

            while (true){
                index = opp_distribution(generator);
                if(index != c1){
                    c1 = index;
                    no_vals.push_back(index);
                }
                if(no_vals.size()>1){
                    break;
                }
            }

            for(int j = 0; j<cards.size(); j++){
                if(find(no_vals.begin(), no_vals.end(), j)!= no_vals.end()){
                    continue;
                }
                opp_cards.push_back(cards.at(j));
            }


            s2 = resolve_hand(opp_cards);
            if(s1>s2){
                ratio++;
            }
        }
        done++;
        cout << "Percent done: " << (done/(double)num_iterations) * 100.0 << endl;
        ofs << s1 << "," << (ratio/(double)num_trials) << endl;
    }

    ofs.close();
}

double Hand::convert_strength_to_win(double strength) {
    return (0.50187506) * (log(strength)) + (-0.05574417) * (strength) + 0.29152419686993136;
}

void Hand::gen_EV_correlation_plus(double num_trials, double num_iterations) {
    ofstream ofs("../EV_full_cur_strength_correlations.txt");

    vector<int> cards{};

    ofs << "1_value,2_value,3_value,4_value,5_value,EV,cur_strength,max_suit,win_rate" << endl;//max_suit is the value of whatever suit is most prevalent in the hand
    //add in cur_strength to represent the current strength of the hand (with just the base 5 cards)

    uniform_int_distribution<int> distribution(0,51);
    pair<double,double> EV_corr{};
    int index{};
    vector<int> suits = {0,0,0,0};
    int max{};
    double percent=0;
    double cur_strength{};

    for(double i = 0; i<num_iterations; i++){
        suits = {0,0,0,0};
        cards.clear();
        default_random_engine generator(rand() %(100+5*(int)i));
        while(true){
            index = distribution(generator);
            if(find(cards.begin(), cards.end(), index)== cards.end()){
                cards.push_back(index);
            }
            if(cards.size()>4){
                break;
            }
        }
        sort(cards.begin(), cards.end(), nest_Compare());
        for(int c : cards){
            ofs << c % 13 << ",";
            if(c<13){
                suits[0]++;
            }
            else if(c>=13 && c<26){
                suits[1]++;
            }
            else if(c>=26 && c<39){
                suits[2]++;
            }
            else{
                suits[3]++;
            }
        }
        cur_strength = resolve_hand(cards);
        EV_corr = EV_correlation(num_trials,cards);
        ofs << EV_corr.first << "," << cur_strength << ",";

        auto it = max_element(suits.begin(), suits.end());
        ofs << *it << "," << EV_corr.second << endl;


        cout << "percent done: " << (i/num_iterations)*100.0 << endl;
    }

    ofs.close();
}

void Hand::gen_pre_flop(int num_players, double num_trials) {
    //don't gen everything, just suited and non-suited as pre-flop the other two suits are irrelevant
    ofstream ofs{ "../pre_flop_table.txt" };
    double win_per{};
    double win{};
    double counter = 0;
    for (int i = 0; i < 26;i++) {
        for (int j = i+1; j < 26; j++) {
            win_per = sim_hands(num_trials,i, j, num_players);
            ofs << i << "," << j << "," << win_per << endl;
            counter++;
            cout << "Percent done: " << (double)counter / 325.0 << endl;
        }
    }

    ofs.close();
}

double Hand::pre_flop_vs(vector<int> cards, const char *s, double num_iterations) {
    sqlite3* db;


    char* messageerror;

    int exit = sqlite3_open(s, &db);
    string SQL_statement{};

    int num_players = (int)(cards.size() / 2);
    bool flipped = false;
    int t1{};
    int t2{};
    double ratio = 0;
    //first check to see if it's already in the table


    if (num_players == 2) {
        sort(cards.begin(), cards.begin() + 2);
        sort(cards.begin() + 2, cards.begin() + 4);
        if (cards.at(0) > cards.at(2)) {
            cards.at(0) = t1;
            cards.at(1) = t2;
            cards.at(0) = cards.at(2);
            cards.at(1) = cards.at(3);
            cards.at(2) = t1;
            cards.at(3) = t2;
            flipped = true;
        }
        string sql = "SELECT WIN_PER FROM PRE_FLOP_VS WHERE (C_ONE = " + to_string(cards.at(0)) + ") AND (C_TWO = " + to_string(cards.at(1)) +
            ") AND (C_THREE = " + to_string(cards.at(2)) + ") AND (C_FOUR = " + to_string(cards.at(3)) + ");";

        sqlite3_stmt* stmt;
        int j{};
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
                string text{};
                j = 0;
                while (text_char[j] != '\0') {
                    text += text_char[j];
                    j++;
                }
                ratio = stod(text);
                if (flipped) {
                    return 1 - ratio;
                }
                return ratio;
            }
            sqlite3_finalize(stmt);
        }
    }

    sqlite3_close(db);

    uniform_int_distribution<int> distribution(0, 51);
    int index{};
    vector<int> table_cards{};
    vector<double> ss{};
    double max_val{};
    int w_index{};
    double tie;
    bool won{};
    for (double i = 0; i < num_iterations; i++) {
        default_random_engine generator(rand() % (100 + 5 * (int)i));
        table_cards.clear();
        ss.clear();
        tie = 1;
        won = true;
        while (true) {
            index = distribution(generator);
            if (find(cards.begin(), cards.end(), index) == cards.end() && find(table_cards.begin(), table_cards.end(), index) == table_cards.end()) {
                table_cards.push_back(index);
                if (table_cards.size() > 4) {
                    break;
                }
            }
        }
        for (int j = 0; j < num_players; j++) {
            table_cards.resize(5);
            table_cards.push_back(cards.at(2 * j));
            table_cards.push_back(cards.at(2 * j + 1));
            ss.push_back(resolve_hand(table_cards));
        }
        max_val = ss.at(0);
        w_index = 0;
        for (int i = 1; i < ss.size(); i++) {
            if (w_index == 0 && ss.at(i) == max_val) {
                tie++;
            }
            if (ss.at(i) > max_val) {
                won = false;
                break;
            }
        }
        if (won) {
            ratio += (double)(1.0 / tie);
        }
    }
    ratio = (double)(ratio / num_iterations);
    if (num_players != 2) {
        if (flipped) {
            return 1 - ratio;
        }
        return ratio;
    }
    sqlite3* db1;

    exit = sqlite3_open(s, &db1);

    SQL_statement = "INSERT INTO PRE_FLOP_VS(C_ONE, C_TWO, C_THREE, C_FOUR, WIN_PER) values(" + to_string(cards.at(0)) + ", "
        + to_string(cards.at(1)) + ", " + to_string(cards.at(2)) + ", " + to_string(cards.at(3)) + ", " + to_string(ratio) + ");";


    exit = sqlite3_exec(db1, SQL_statement.c_str(), NULL, 0, &messageerror);

    if (exit != SQLITE_OK) {
        cerr << "error with insert" << endl;
        sqlite3_free(messageerror);
        return NULL;
    }
    else {
        cout << "record inserted successfully" << endl;
    }

    sqlite3_close(db1);

    return ratio;
}

