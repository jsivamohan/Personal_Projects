package com.company;

import java.util.Arrays;
import java.util.Scanner;

public class Connect_Four {
    public int[][] board;
    int x_2 = 0;//keeps track of streaks for the parameters in the eval function
    int x_3 = 0;
    int A_2 = 0;
    int A_3 = 0;
    Connect_Four(){
        board = new int [6][7];
    }

    enum direction{
        TL,
        L,
        LL,
        D,
        LR,
        R,
        TR
    }

    private int[] place_token(int[][]cBoard, boolean p1, int input){//array is coordinate of input
        int i = 0;
        while(true){
            if(cBoard[i][input-1]!=0){
                i++;
            }
            else{
                if(p1){
                    cBoard[i][input-1] = 1;

                }else{
                    cBoard[i][input-1] = 2;
                }
                break;
            }
        }
        return new int[]{i,input-1};
    }

    private void start_game(){
        Scanner scan = new Scanner(System.in);
        boolean end = false;
        boolean p1 = true;
        int input = 0;
        int e = 0;
        while(!end){
            if(p1){
                System.out.print("Player 1: what column do you what to play (1-7): ");
            }else{
                System.out.print("Player 2: what column do you want to play (1-7): ");
            }
            input = scan.nextInt();
            if(input <0 || input >7){
                System.out.println("not a valid input");
                continue;
            }
            if(board[5][input-1]!=0){
                continue;
            }
            int[] coords = place_token(board,p1, input);
            int check = -1;
            if(p1){
                check = check_over(board,coords[0],coords[1],1);
                if(check==1){
                    System.out.println("p1 (x) wins");
                    end = true;
                }
                else if(check==3){
                    System.out.println("its a tie");
                    end = true;
                }
            }else{
                check = check_over(board, coords[0],coords[1],2);
                if(check==2){
                    System.out.println("p2 (o) wins");
                    end = true;
                }
                else if(check==3){
                    System.out.println("its a tie");
                    end = true;
                }
            }
            p1 = !p1;
            print_board(board);
        }
    }

    private int calc_score_2(int[]streaks){//basic score function based on the streaks of A and x on the board


        int x_score = 3*streaks[0] + (streaks[1]) + 10*(streaks[3]) + 6*(streaks[4]);
        int A_score = 3*streaks[6] + (streaks[7]) + 10*(streaks[9]) + 6*(streaks[10]);

        return x_score - A_score;
    }

    private int calc_score(int[] streaks){
        int x_score = 3*streaks[1] + streaks[0];
        int A_score = 3*streaks[2] + streaks[3];

        return x_score - A_score;
    }

    private int eval_func(boolean p1, int row, int column, int[][]cBoard, int[]streaks){//need to write eval function to deal with intermediate board states (not a leaf) streaks array contains [x_2, x_3, A_2, A_3] eval_func called by minimax for each board state and the last piece placed


        int turn = -1;
        if(p1){
            turn = 1;
        }else{
            turn = 2;
        }
        int check = check_over(cBoard,row,column,turn);
        if(check!=0){//check = 0 means game over
            if(check == 1){
                return Integer.MAX_VALUE;
            }
            else if(check ==2){
                return Integer.MIN_VALUE;
            }
            else{
                return 0;//0 if a tie (check is 3)
            }
        }

        streaks = eval_func_test(p1, row, column, cBoard, streaks);
        int score = calc_score(streaks);
        return score;
    }

    private int eval_func_2(boolean p1, int row, int column, int[][]cBoard, int[]streaks){
        int turn = -1;
        if(p1){
            turn = 1;
        }else{
            turn = 2;
        }
        int check = check_over(cBoard,row,column,turn);
        if(check!=0){//check = 0 means game over
            if(check == 1){
                return Integer.MAX_VALUE;
            }
            else if(check ==2){
                return Integer.MIN_VALUE;
            }
            else{
                return 0;//0 if a tie (check is 3)
            }
        }
        streaks = eval_func_test_2(p1,row,column,cBoard, streaks);
        return calc_score_2(streaks);
    }

    public int[] eval_func_test_2(boolean p1, int row, int column, int[][]cBoard, int[]streaks) {//need to write eval function to deal with intermediate board states (not a leaf) streaks array contains [x_2, x_3, A_2, A_3] eval_func called by minimax for each board state and the last piece placed

        boolean left_open = false;
        boolean right_open = false;
        //streaks is now length 12
        int turn = -1;
        if (p1) {
            turn = 1;
        } else {
            turn = 2;
        }
        int[] boundary = new int[4];//array of coords of boundary ndoes in the streak (to check for open/closed)
        boundary[0] = row;
        boundary[2] = row;
        boundary[1] = column;
        boundary[3] = column;
        //updates the values of the streaks based on the last piece placed and calculates the evaluation metric accordingly
        direction dir = direction.TL;
        int count = 0;
        for (int i = 0; i < 3; i++) {
            if (column - i < 0 || row + i > 5) {
                dir = direction.LR;
            } else if (cBoard[row + i][column - i] == turn) {//turn is 1 if x (max player going) and -1 if o (min player going)
                count++;
                boundary[0] = row + i;//sets the value of the boundary of the streak
                boundary[1] = column - i;
            } else {
                dir = direction.LR;
            }
            if (dir == direction.LR) {
                for (int j = 1; j <= 4 - i; j++) {
                    if (row - j < 0 || column + j > 6) {
                        break;
                    }
                    if (cBoard[row - j][column + j] == turn) {
                        count++;
                        boundary[2] = row - j;
                        boundary[3] = column + j;
                    } else {
                        break;
                    }
                }
                dir = direction.L;
                break;
            }
        }
        if (boundary[0] + 1 < board.length && boundary[1] - 1 >= 0 && board[(boundary[0] + 1)][(boundary[1] - 1)] == 0) {//if the next thing is within the board and open
            left_open = true;
        }
        if (boundary[2] - 1 >= 0 && boundary[3] + 1 < board[0].length && board[(boundary[2] - 1)][(boundary[3] + 1)] == 0) {//if the next thing is within the board and open
            right_open = true;
        }
        if (count == 3) {
            if (p1) {
                if (left_open && right_open) {
                    streaks[3]++;//streaks[3] is x_3 open
                } else if (left_open || right_open) {
                    streaks[4]++;//streaks[4] is x_3 semi open
                } else {
                    streaks[5]++;//streaks[5] is x_3 closed
                    streaks[1]--;
                }
            } else {
                if (left_open && right_open) {
                    streaks[9]++;//streaks[3] is A_3 open
                } else if (left_open || right_open) {
                    streaks[10]++;//streaks[4] is A_3 semi open
                } else {
                    streaks[11]++;//streaks[5] is A_3 closed
                    streaks[7]--;
                }
            }
        } else if (count == 2) {
            if (p1) {
                if (left_open && right_open) {
                    streaks[0]++;//streaks[3] is x_2 open
                } else if (left_open || right_open) {
                    if(left_open && boundary[0]+2 <board.length && boundary[1]-2>=0 && board[boundary[0]+2][boundary[1]-2]!=(turn+1)){
                        streaks[1]++;
                    }
                    else if(right_open && boundary[2]-2>=0 && boundary[3]+2< board.length && board[boundary[2]-2][boundary[3]+2] !=(turn+1)){
                        streaks[1]++;
                    }
                } else {
                    streaks[2]++;//streaks[5] is x_2 closed
                }
            } else {
                if (left_open && right_open) {
                    streaks[6]++;//streaks[3] is A_2 open
                } else if (left_open || right_open) {
                    if(left_open && boundary[0]+2 <board.length && boundary[1]-2>=0 && board[boundary[0]+2][boundary[1]-2]!=(turn-1)){
                        streaks[7]++;
                    }
                    else if(right_open && boundary[2]-2>=0 && boundary[3]+2< board.length && board[boundary[2]-2][boundary[3]+2] !=(turn-1)){
                        streaks[7]++;
                    }
                } else {
                    streaks[8]++;//streaks[5] is A_2 closed
                }
            }
        }
        boundary[0] = row;
        boundary[2] = row;
        boundary[1] = column;
        boundary[3] = column;
        left_open = false;
        right_open = false;
        count = 0;
        for (int i = 0; i < 3; i++) {
            if (column - i < 0) {
                dir = direction.R;
            } else if (cBoard[row][column - i] == turn) {//turn is 1 if x (max player going) and -1 if o (min player going)
                count++;
                boundary[1] = column - i;
            } else {
                dir = direction.R;
            }
            if (dir == direction.R) {
                for (int j = 1; j <= 4 - i; j++) {
                    if (column + j > 6) {
                        break;
                    }
                    if (cBoard[row][column + j] == turn) {
                        count++;
                        boundary[3] = column + j;
                    } else {
                        break;
                    }
                }
                dir = direction.LL;
                break;
            }
        }
        if (boundary[1] - 1 >= 0 && board[(boundary[0])][(boundary[1] - 1)] == 0) {//if the next thing is within the board and open
            left_open = true;
        }
        if (boundary[3] + 1 < board[0].length && board[(boundary[2])][(boundary[3] + 1)] == 0) {//if the next thing is within the board and open
            right_open = true;
        }
        if (count == 3) {
            if (p1) {
                if (left_open && right_open) {
                    streaks[3]++;//streaks[3] is x_3 open
                    streaks[0]--;
                } else if (left_open || right_open) {
                    streaks[4]++;//streaks[4] is x_3 semi open
                    streaks[1]--;
                } else {
                    streaks[5]++;//streaks[5] is x_3 closed
                    streaks[2]--;
                }
            } else {
                if (left_open && right_open) {
                    streaks[9]++;//streaks[3] is A_3 open
                    streaks[6]--;
                } else if (left_open || right_open) {
                    streaks[10]++;//streaks[4] is A_3 semi open
                    streaks[7]--;
                } else {
                    streaks[11]++;//streaks[5] is A_3 closed
                    //streaks[8]--;
                    streaks[7]--;
                }
            }
        } else if (count == 2) {
            if (p1) {
                if (left_open && right_open) {
                    streaks[0]++;//streaks[3] is x_2 open
                } else if (left_open || right_open) {
                    if(left_open && boundary[1]-2>=0 && board[boundary[0]][boundary[1]-2]!=(turn+1)){
                        streaks[1]++;
                    }
                    else if(right_open && boundary[3]+2< board.length && board[boundary[2]][boundary[3]+2] !=(turn+1)){
                        streaks[1]++;
                    }
                } else {
                    streaks[2]++;//streaks[5] is x_2 closed
                }
            } else {
                if (left_open && right_open) {
                    streaks[6]++;//streaks[3] is A_2 open
                } else if (left_open || right_open) {
                    if(left_open && boundary[1]-2>=0 && board[boundary[0]][boundary[1]-2]!=(turn-1)){
                        streaks[7]++;
                    }
                    else if(right_open && boundary[3]+2< board.length && board[boundary[2]][boundary[3]+2] !=(turn-1)){
                        streaks[7]++;
                    }
                } else {
                    streaks[8]++;//streaks[5] is A_2 closed
                }
            }
        }
            boundary[0] = row;
            boundary[2] = row;
            boundary[1] = column;
            boundary[3] = column;
            left_open = false;
            right_open = false;
            count = 0;

            for (int i = 0; i < 3; i++) {
                if (column - i < 0 || row - i < 0) {
                    dir = direction.TR;
                } else if (cBoard[row - i][column - i] == turn) {//turn is 1 if x (max player going) and -1 if o (min player going)
                    boundary[0] = row - i;
                    boundary[1] = column - i;
                    count++;
                } else {
                    dir = direction.TR;
                }
                if (dir == direction.TR) {
                    for (int j = 1; j <= 4 - i; j++) {
                        if (column + j > 6 || row + j > 5) {
                            break;
                        }
                        if (cBoard[row + j][column + j] == turn) {
                            count++;
                            boundary[2] = row + j;
                            boundary[3] = column + j;
                        } else {
                            break;
                        }
                    }
                    dir = direction.D;
                    break;
                }
            }
            if (boundary[0] - 1 >= 0 && boundary[1] - 1 >= 0 && board[(boundary[0] - 1)][(boundary[1] - 1)] == 0) {//if the next thing is within the board and open
                left_open = true;
            }
            if (boundary[2] + 1 < board.length && boundary[3] + 1 < board[0].length && board[(boundary[2] + 1)][(boundary[3] + 1)] == 0) {//if the next thing is within the board and open
                right_open = true;
            }
            if (count == 3) {
                if (p1) {
                    if (left_open && right_open) {
                        streaks[3]++;//streaks[3] is x_3 open
                        streaks[0]--;
                    } else if (left_open || right_open) {
                        streaks[4]++;//streaks[4] is x_3 semi open
                        streaks[1]--;
                    } else {
                        streaks[5]++;//streaks[5] is x_3 closed
                        //streaks[2]--;//never subtract from closed because it can't be manipulated
                        streaks[1]--;//2 semi open became closed
                    }
                } else {
                    if (left_open && right_open) {
                        streaks[9]++;//streaks[3] is A_3 open
                        streaks[6]--;
                    } else if (left_open || right_open) {
                        streaks[10]++;//streaks[4] is A_3 semi open
                        streaks[7]--;
                    } else {
                        streaks[11]++;//streaks[5] is A_3 closed
                        //streaks[8]--;
                        streaks[7]--;
                    }
                }
            } else if (count == 2) {//add in check to account for not putting in piece if enemy piece or boundary is two away for streak length 2
                if (p1) {
                    if (left_open && right_open) {
                        streaks[0]++;//streaks[3] is x_2 open
                    } else if (left_open || right_open) {
                        if(left_open && boundary[0]-2 >=0 && boundary[1]-2>-0 && board[boundary[0]-2][boundary[1]-2]!=(turn+1)){
                            streaks[1]++;
                        }
                        else if(right_open && boundary[2]+2<board.length && boundary[3]+2< board.length && board[boundary[2]+2][boundary[3]+2] !=(turn+1)){
                            streaks[1]++;
                        }
                    } else {
                        streaks[2]++;//streaks[5] is x_2 closed
                    }
                } else {
                    if (left_open && right_open) {
                        streaks[6]++;//streaks[3] is A_2 open
                    } else if (left_open || right_open) {
                        if(left_open && boundary[0]-2 >=0 && boundary[1]-2>-0 && board[boundary[0]-2][boundary[1]-2]!=(turn-1)){
                            streaks[7]++;
                        }
                        else if(right_open && boundary[2]+2<board.length && boundary[3]+2< board.length && board[boundary[2]+2][boundary[3]+2] !=(turn-1)){
                            streaks[7]++;
                        }
                    } else {
                        streaks[8]++;//streaks[5] is A_2 closed
                    }
                }
            }
                boundary[0] = row;
                boundary[2] = row;
                boundary[1] = column;
                boundary[3] = column;
                left_open = false;
                right_open = false;
                count = 0;

                for (int i = 0; i < 3; i++) {
                    if (row - i < 0) {
                        break;
                    }
                    if (cBoard[row - i][column] == turn) {
                        count++;
                        boundary[0] = row-i;
                    } else {
                        break;
                    }
                }
                if(boundary[2] + (4-count) < board.length){
                    if (count == 3) {
                        if (p1) {
                            streaks[0]--;
                            streaks[3]++;
                        } else {
                            streaks[6]--;
                            streaks[9]++;
                        }
                    } else if (count == 2) {
                        if (p1) {
                            if(boundary[2]+2<board.length && board[boundary[2]+2][boundary[3]]!=(turn+1)){
                                streaks[0]++;
                            }
                        } else {
                            if(boundary[2]+2<board.length && board[boundary[2]+2][boundary[3]]!=(turn+1)){
                                streaks[6]++;
                            }
                        }
                    }
                }
                return streaks;
            }

    private void test_conditions(int iterations){
        for(int i = 0; i<iterations; i++){
            start_game();
        }
    }

    public int[] eval_func_test(boolean p1, int row, int column, int[][]cBoard, int[]streaks){
        int turn = -1;
        if(p1){
            turn = 1;
        }else{
            turn = 2;
        }
        //updates the values of the streaks based on the last piece placed and calculates the evaluation metric accordingly
        direction dir = direction.TL;
        int count = 0;
        for(int i = 0; i<3; i++){
            if(column-i <0||row+i >5){
                dir = direction.LR;
            }
            else if(cBoard[row+i][column-i]==turn){//turn is 1 if x (max player going) and -1 if o (min player going)
                count++;
            }else{
                dir = direction.LR;
            }
            if(dir == direction.LR){
                for(int j = 1; j<=4-i; j++){
                    if(row-j < 0 ||column +j >6){
                        break;
                    }
                    if(cBoard[row-j][column+j]==turn){
                        count++;
                    }else{
                        break;
                    }
                }
                dir = direction.L;
                break;
            }
        }
        if(count==3){
            if(p1){
                streaks[0]--;
                streaks[1]++;
            }else{
                streaks[2]--;
                streaks[3]++;
            }
        }
        else if(count==2){
            if(p1){
                streaks[0]++;
            }else{
                streaks[2]++;
            }
        }
        count = 0;
        for(int i = 0; i<3; i++){
            if(column-i<0){
                dir = direction.R;
            }
            else if(cBoard[row][column-i]==turn){//turn is 1 if x (max player going) and -1 if o (min player going)
                count++;
            }else{
                dir = direction.R;
            }
            if(dir == direction.R){
                for(int j = 1; j<=4-i; j++){
                    if(column+j > 6){
                        break;
                    }
                    if(cBoard[row][column+j]==turn){
                        count++;
                    }else{
                        break;
                    }
                }
                dir = direction.LL;
                break;
            }
        }
        if(count==3){
            if(p1){
                streaks[0]--;
                streaks[1]++;
            }else{
                streaks[2]--;
                streaks[3]++;
            }
        }
        else if(count==2){
            if(p1){
                streaks[0]++;
            }else{
                streaks[2]++;
            }
        }
        count =0;

        for(int i = 0; i<3; i++){
            if(column-i<0||row-i<0){
                dir = direction.TR;
            }
            else if(cBoard[row-i][column-i]==turn){//turn is 1 if x (max player going) and -1 if o (min player going)
                count++;
            }else{
                dir = direction.TR;
            }
            if(dir == direction.TR){
                for(int j = 1; j<=4-i; j++){
                    if(column+j>6||row+j>5){
                        break;
                    }
                    if(cBoard[row+j][column+j]==turn){
                        count++;
                    }else{
                        break;
                    }
                }
                dir = direction.D;
                break;
            }
        }
        if(count==3){
            if(p1){
                streaks[0]--;
                streaks[1]++;
            }else{
                streaks[2]--;
                streaks[3]++;
            }
        }
        else if(count==2){
            if(p1){
                streaks[0]++;
            }else{
                streaks[2]++;
            }
        }
        count = 0;

        if(row>3){
            for(int i = 0; i<3; i++){
                if(cBoard[row-i][column]==turn){
                    count++;
                }else{
                    break;
                }
            }
        }

        if(count==3){
            if(p1){
                streaks[0]--;
                streaks[1]++;
            }else{
                streaks[2]--;
                streaks[3]++;
            }
        }
        else if(count==2){
            if(p1){
                streaks[0]++;
            }else{
                streaks[2]++;
            }
        }
        return streaks;
    }

    private int check_over(int[][]cBoard, int row, int column, int turn){//row and column of the piece that was just placed
        int full = 0;
        for(int i= 0; i<cBoard[0].length; i++){//check if board completely full
            if(cBoard[cBoard.length-1][i]!=0){
                full++;
            }
        }
        if(full==cBoard[0].length){
            return 3;
        }
        direction dir = direction.TL;
        int count = 0;
        for(int i = 0; i<4; i++){
            if(column-i <0||row+i >5){
                dir = direction.LR;
            }
            else if(cBoard[row+i][column-i]==turn){//turn is 1 if x (max player going) and -1 if o (min player going)
                count++;
            }else{
                dir = direction.LR;
            }
            if(dir == direction.LR){
                for(int j = 1; j<=4-i; j++){
                    if(row-j < 0 ||column +j >6){
                        break;
                    }
                    if(cBoard[row-j][column+j]==turn){
                        count++;
                    }else{
                        break;
                    }
                }
                dir = direction.L;
                break;
            }
        }
        if(count==4){
            return turn;
        }
        count = 0;
        for(int i = 0; i<4; i++){
            if(column-i<0){
                dir = direction.R;
            }
            else if(cBoard[row][column-i]==turn){//turn is 1 if x (max player going) and -1 if o (min player going)
                count++;
            }else{
                dir = direction.R;
            }
            if(dir == direction.R){
                for(int j = 1; j<=4-i; j++){
                    if(column+j > 6){
                        break;
                    }
                    if(cBoard[row][column+j]==turn){
                        count++;
                    }else{
                        break;
                    }
                }
                dir = direction.LL;
                break;
            }
        }
        if(count==4){
            return turn;
        }
        count =0;

        for(int i = 0; i<4; i++){
            if(column-i<0||row-i<0){
                dir = direction.TR;
            }
            else if(cBoard[row-i][column-i]==turn){//turn is 1 if x (max player going) and -1 if o (min player going)
                count++;
            }else{
                dir = direction.TR;
            }
            if(dir == direction.TR){
                for(int j = 1; j<=4-i; j++){
                    if(column+j>6||row+j>5){
                        break;
                    }
                    if(cBoard[row+j][column+j]==turn){
                        count++;
                    }else{
                        break;
                    }
                }
                dir = direction.D;
                break;
            }
        }
        if(count==4){
            return turn;
        }
        count = 0;

        if(row<3){
            return 0;
        }
        for(int i = 0; i<4; i++){
            if(cBoard[row-i][column]==turn){
                count++;
            }else{
                break;
            }
        }
        if(count==4){
            return turn;
        }
        else {
            return 0;
        }
    }

    private int[] check_over_nc(int[][]board, int turn){//checks all top values in each column (cannot win with anything else) returns with coords and third value 1 if end, and 0 if not
        for(int i = 0; i<board[0].length; i++){
            int j = board.length-1;
            while(j>=0){
                if(board[j][i]==0){
                    j--;
                    continue;
                }
                if(check_over(board,j,i,turn)==turn){
                    return new int[]{j,i,1};
                }
            }
        }
        return new int[]{0,0,0};
    }

    public int[] minimax(int[][]cBoard, boolean is_maximizing, int depth,int row, int column, int alpha, int beta, int[] streaks,boolean eval1){//returns int representing the eval score of the best possible board state
        int a = 0;


        int check = 0;
        int best_value = 0;
        int best_move = -1;
        if(row>=0 && column>=0){//means its the first call
            if(is_maximizing){
                check = check_over(cBoard, row, column, 2);
                if(check == 2){
                    return new int[]{Integer.MIN_VALUE,-1,alpha, beta};
                }
                if(check==3){
                    return new int[]{0,-2,alpha, beta};
                }
            }else{
                check = check_over(cBoard, row, column, 1);
                if(check == 1){
                    return new int[]{Integer.MAX_VALUE,-1, alpha, beta};
                }
                if(check==3){
                    return new int[]{0,-2, alpha, beta};
                }
            }
        }
        if(is_maximizing){
            best_value = Integer.MIN_VALUE;
        }else{
            best_value = Integer.MAX_VALUE;
        }
        int[] nStreaks = new int[streaks.length];
        for(int i = 0; i<nStreaks.length; i++){//copies the value of streaks
            nStreaks[i] = streaks[i];
        }
        if(depth ==0){
            if(eval1) {
                return new int[]{eval_func(!is_maximizing, row, column, cBoard, nStreaks), -1, alpha, beta};//implement array of current streaks
            }else{
                return new int[]{eval_func_2(!is_maximizing, row, column, cBoard, nStreaks), -1, alpha, beta};//implement array of current streaks
            }
        }

        for(int i = 1; i<=cBoard[0].length; i++){
            if(cBoard[5][i-1]!=0){//skip if column is already full
                continue;
            }
            int[][] newBoard = copyBoard(cBoard);
            int [] placed = place_token(newBoard,is_maximizing, i);
            int hypothetical_value = minimax(newBoard, !is_maximizing, depth-1, placed[0],placed[1],alpha, beta,nStreaks,eval1)[0];
            if(is_maximizing && hypothetical_value>best_value){
                best_value = hypothetical_value;
                best_move = i-1;
                alpha = Integer.max(alpha, best_value);
            }
            if(!is_maximizing && hypothetical_value<best_value){
                best_value = hypothetical_value;
                best_move = i-1;
                beta = Integer.min(beta, best_value);
            }
            if(alpha >= beta){
                break;
            }
        }
        return new int[]{best_value, best_move,alpha, beta};
    }

    public void print_board(int[][]board){
        for(int i = 0; i<board.length; i++){
            System.out.print(board.length-i + " |");
            for(int j = 0; j<board[0].length; j++){
                if(board[board.length-i-1][j]==1){
                    System.out.print(" X |");
                }
                else if(board[board.length-i-1][j]==2){
                    System.out.print(" A |");
                }
                else{
                    System.out.print("   |");
                }
            }
            System.out.println();
            System.out.println("--------------------------------");
        }

        System.out.print("    ");
        for(int i= 1; i<=board[0].length; i++){
            System.out.print(i + "   ");
        }
        System.out.println();
    }

    private int[][] copyBoard(int[][]board){
        int[][] cBoard = new int[board.length][board[0].length];
        for(int i = 0; i< board.length; i++){
            for(int j = 0; j<board[0].length; j++){
                cBoard[i][j] = board[i][j];
            }
        }
        return cBoard;
    }

    public void begin_game(boolean eval1){
        Scanner scan = new Scanner(System.in);
        boolean end = false;
        boolean p1 = true;
        int e = 0;
        System.out.print("would you like to be player 1 or 2: ");
        int player = scan.nextInt();
        System.out.println("what difficulty would you like the AI (1-10)");
        int depth = scan.nextInt();
        int input = 0;
        int[] coords = new int[2];
        int[] streaks = new int [1];
        if(eval1){
            streaks = new int[4];
        }else{
            streaks = new int[12];
        }

        int check = 0;
        boolean a_comp = false;
        boolean p_comp = true;
        int p_over = 1;
        int a_over = 2;
        int ai_input = 0;
        if(player!=1) {
            board[0][3] = 1;
            print_board(board);
            a_comp = true;
            p_comp = false;
            p_over = 2;
            a_over = 1;
        }
        while(true){
            System.out.println("What column would you like to play in (1-7)");
            input = scan.nextInt();
            if(input <1 || input >7){
                System.out.println("Invalid column");
                continue;
            }
            if(board[5][input-1]!=0){
                System.out.println("sorry, full column");
                continue;
            }
            coords = place_token(board, p_comp,input);
            if(player==1){
                check = check_over(board, coords[0],coords[1],1);
            }else{
                check = check_over(board, coords[0],coords[1],2);
            }

            if(check!=0){
                if(check==p_over){
                    print_board(board);
                    System.out.println("you win");
                    end= true;
                    break;
                }else{
                    print_board(board);
                    System.out.println("its a tie");
                    end = true;
                    break;
                }
            }
            print_board(board);
            if(eval1){
                streaks = eval_func_test(p_comp,coords[0],coords[1],board,streaks);
            }else{
                streaks = eval_func_test_2(p_comp,coords[0],coords[1],board,streaks);
            }

            ai_input = AI_play(board, a_comp, depth, coords[0],coords[1], streaks,eval1);
            coords = place_token(board, a_comp,ai_input+1);
            if(player!=1){
                check = check_over(board, coords[0],coords[1],1);
            }else{
                check = check_over(board, coords[0],coords[1],2);
            }

            if(check!=0){
                if(check==a_over){
                    print_board(board);
                    System.out.println("computer wins");
                    end = true;
                    break;
                }else{
                    print_board(board);
                    System.out.println("its a tie");
                    end = true;
                    break;
                }
            }
            if(eval1){
                streaks = eval_func_test(!p_comp,coords[0],coords[1],board,streaks);
            }else{
                streaks = eval_func_test_2(!p_comp,coords[0],coords[1],board,streaks);
            }
            String[] Sstreaks = new String[]{"x_2o", "x_2s", "x_2c", "x_3o", "x_3s", "x_3c", "a_2o", "a_2s", "a_2c", "a_3o", "a_3s", "s_3c"};
            for(int i= 0; i<streaks.length; i++){
                System.out.print(Sstreaks[i] + ": " + streaks[i] + "  ");
            }
            System.out.println();

            System.out.println("streaks are " + Arrays.toString(streaks));
            print_board(board);
            }
        }

    private int AI_play(int[][]board,boolean p1, int depth, int row, int column, int[] streaks, boolean eval1){
        int play = minimax(board, p1, depth, row, column, Integer.MIN_VALUE, Integer.MAX_VALUE, streaks, eval1)[1];//second value of the array is the move that should be played
        return play;
    }

    public double two_AI_play(int num, int depth){//returns the percentage won by the new eval function
        int play= 0;
        int won_by_1 = 0;
        int won_by_2 = 0;
        int[] coords = new int[2];
        coords[0] = -1;
        coords[1] = -1;
        int[] streaks1 = new int[4];
        int[] streaks2 = new int[12];
        boolean p1 = true;
        int check = 0;
        boolean swap = true;
        for(int i = 0;i<num; i++){
            while(true){
                if(i>num/2){
                    swap = !swap;
                }
                play = AI_play(board, true,depth,coords[0], coords[1],streaks1,true);
                System.out.println("play1 is " + play);
                coords = place_token(board,p1,play+1);
                check = check_over(board,coords[0],coords[1],1);
                if(check!=0){
                    if(check==1){
                        won_by_1++;
                    }
                    break;
                }
                print_board(board);
                streaks1 = eval_func_test(p1,coords[0],coords[1],board,streaks1);
                streaks2 = eval_func_test_2(p1,coords[0],coords[1],board,streaks2);
                System.out.println("streak1: " + Arrays.toString(streaks1));
                System.out.println("streak2: " + Arrays.toString(streaks2));

                p1 = !p1;
                play = AI_play(board,false,depth,coords[0],coords[1],streaks2, false);
                System.out.println("play2 is " + play);
                coords = place_token(board,p1,play+1);
                check = check_over(board,coords[0],coords[1],1);
                if(check!=0){
                    if(check==2){
                        won_by_2++;
                    }
                    break;
                }
                print_board(board);
                streaks1 = eval_func_test(p1,coords[0],coords[1],board,streaks1);
                streaks2 = eval_func_test_2(p1,coords[0],coords[1],board,streaks2);
                System.out.println("streak1: " + Arrays.toString(streaks1));
                System.out.println("streak2: " + Arrays.toString(streaks2));
                p1 = !p1;
            }
        }
        return (double)won_by_2/won_by_1;
    }
}
