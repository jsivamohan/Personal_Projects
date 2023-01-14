This is the connect four AI I built.

All of the work for the project is done in the Connect_Four class.

In this class the logic for the AI along with the game of Connect Four itself is designed.

The game itself is coded mostly with the check_over and check_over_nc methods which essentially evaluate the board and figure out whether or not the game is still going (the game is over if one of the players has four pieces in a row or if their are no more legal moves). This combined with the begin_game method and the place token method allow the user and AI to interact through the game board which the Connect_Four class represents as a 2d array that is constanly updating to reflect the current board state.

The actual work of the AI begins in the minimax method which executes the minimax algorithm to search the space of possible moves and create a decision tree representing the possible future move orders, the algorithm uses both the eval_func and eval_func_2 methods which look at the board state given to them by the minimax algorithm (which is some future extrapolation from the current board state) and decide whether or not this board state is good for the computer or not. I custom coded these evaluation functions to look at the current streaks on the board as well as possible future streaks in order to assign a score to the board with a higher score being better for the computer.

The minimax algorithm then recursively calls further and further down the decision tree (to some max depth) and picks the move that will lead to the best board state down the line.

I also implemented alpha beta pruning in the minimax algorithm to prune branches that were guarunteed to not lead to a more optimal board state, this saved significant amounts of time and allowed for the max depth to be much higher and therefore increased the overall performance of the algorithm.
