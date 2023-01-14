#pragma once

//class for managing all of the database compatibility with the user in term of looking up information and sorting through hands to replay
#include "../../../CLionProjects/Poker_new/Replay.h"
#include "../../../CLionProjects/Poker_new/Game.h"



class Query
{
public:

    static const int options = 12;//number of possible query options (make increase in the future)

    static const int num_cols = 26;//number of columns of the DB
    static const int N_PLAYERS = 1;
    static const int PRE_STACK_AMOUNTS = 2;
    static const int POST_STACK_AMOUNTS = 3;
    static const int POSITION = 4;
    static const int NAMES = 5;
    static const int BIG_BLIND = 6;
    static const int SMALL_BLIND = 7;
    static const int BET_AMOUNTS = 8;
    static const int PRE_FLOP_POT = 9;
    static const int FLOP_POT = 10;
    static const int TURN_POT = 11;
    static const int RIVER_POT = 12;
    static const int PLAYER_CARD_ONE = 13;
    static const int PLAYER_CARD_TWO = 14;
    static const int CARDS = 15;
    static const int SHOWDOWN = 16;
    static const int LAST_ACTION = 17;
    static const int FOLDED = 18;
    static const int PLAYER_NAME = 19;
    static const int FLOP_STRENGTH = 20;
    static const int TURN_STRENGTH = 21;
    static const int RIVER_STRENGTH = 22;
    static const int PRE_FLOP_WIN_PER = 23;
    static const int POV_PRE_STACK = 24;
    static const int WINNER = 25;

    Replay r{};
    Game g{};

    Query();

    std::vector<Replay> interpret_extract(std::vector<std::string>& col_vals, int num_entries);

    std::vector<Replay> extract_info(const char* s, const char* sql);

    Replay create_object(std::vector<std::string>& entry);

    void request_info();//method to ask the user how they want to search the DB

    void display();//display all the possible search options

    std::string resolve_query(int option);//given a specific coded set of input, return the corresponding SQL string


};

