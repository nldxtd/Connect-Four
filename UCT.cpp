#include "UCT.h"
#include "Node.h"
#include "Judge.h"
#include "Point.h"
#include <stdlib.h>
#include <time.h>

#define TIME_LIMIT 2700

Node* UCT::UCTsearch(int* top, int** board) {
    //search the UCT and return the best place
    //construct corrent state node
    root = new Node(noX, noY, rowNum, colNum, top, board);
    //repeating monitoring in the time scale
    while(clock() - this->startTime <= TIME_LIMIT) {
        Node* nextNode = treePolicy(root); //search for next node to explore
        int delta = defaultPolicy(nextNode); //calculate delta profit 
        backUp(nextNode, delta);  //backsore delta for parents
    }
    return bestChild(root);
}

Node* UCT::treePolicy(Node* nowNode) {
    //while game doesn't come to an end
    while(!nowNode->isTerminant()) {
        //choose from the node that han't been explored
        if(nowNode->canBeExpand()) 
            return expand(nowNode);
        //else choose the best child to explore
        else 
            return bestChild(nowNode);
    }
    return nowNode;
}

int UCT::defaultPolicy(Node* selectNode) {
    //first get all the states that need to use in monitoring
    int* topState = selectNode->topState(); //top node state
    int** boardState = selectNode->boardState(); //board state
    int depth = selectNode->getDepth(); //exploring depth
    int chessRight = selectNode->getChessRight(); //current right
    Point lastPoint = selectNode->getLastPoint(); //last chessing pointy;
    int profit = calcProfit(lastPoint, changeRight(chessRight), topState, boardState); //to see if the game ends(really nessasary?)
    Point chessPoint(0, 0);
    while(profit == INTERMINANT_PROFIT) {
        depth++;
        //monitor place chess
        chessPoint = addChessIn(chessRight, topState, boardState);
        //calcu the profit
        profit = calcProfit(chessPoint, chessRight, topState, boardState);
        //change the chessing right
        chessRight = changeRight(chessRight);
    }
    //delete dynamic array
    for(int row = 0; row < rowNum; row++)
        delete [] boardState[row];
    delete [] boardState;
    delete [] topState;
    return profit;    
}

void UCT::backUp(Node* selectNode, int deltaProfit) {
    //backsore the parents states
    Node* father = selectNode;
    while(father != nullptr) {
        //notice that deltaProfit need to minus in every layer
        father->backUp(deltaProfit);
        father = father->getFatherNode();
        deltaProfit = -deltaProfit;
    }
}

int UCT::calcProfit(Point last, int right, int* top, int** board) {
    //calculate profit of last step
    if(right == PLAYER_RIGHT && userWin(last.x, last.y, this->rowNum, this->colNum, board))
        return PLAYER_WIN_PROFIT;
    else if(right == COMPUTER_RIGHT && machineWin(last.x, last.y, this->rowNum, this->colNum, board))
        return COMPUTER_WIN_PROFIT;
    else if(isTie(this->colNum, top))
        return TIE_PROFIT;
    return INTERMINANT_PROFIT;
}

Point UCT::addChessIn(int right, int* top, int** board) {
    int y = -1;
    //if there is one way to win, we must choose it
    y = calWin(right, top, board, this->rowNum, this->colNum);
    if(y < 0) {
        int prob[18] = {0};
        for(int i = 0; i < colNum; i++) {
            if(top[i] == 0) 
                continue;
            prob[i] = calConnect(top[i]-1, i, board, right, rowNum, colNum);
        }
        y = getRand(colNum, prob);
    }
    int x = --top[y];
    board[x][y] = right;
    if(x-1== noX && y==noY)
        top[y]--;
    return Point(x, y);
}

int UCT::changeRight(int right) {
    //exchange chessing rights
    if(right == PLAYER_RIGHT)
        return COMPUTER_RIGHT;
    else if(right == COMPUTER_RIGHT)
        return PLAYER_RIGHT;
    else
        return 0;
}

Node* UCT::bestChild(Node* nowNode) {
    return nowNode->bestChild();
}

Node* UCT::expand(Node* nowNode) {
    return nowNode->expand(changeRight(nowNode->getChessRight()));
}

UCT::~UCT() {
    root->clear();
    delete [] root;
}

int calWin(int right, int* top, int** board, int m, int n) {
    int point = -1;
    for(int i = 0; i < n; i++) {
        if(top[i]) {
            board[top[i]-1][i] = right;
            if(right == PLAYER_RIGHT && userWin(top[i]-1, i, m, n, board))
                point = i;
            else if(right == COMPUTER_RIGHT && machineWin(top[i]-1, i, m, n, board))
                point = i;
            board[top[i]-1][i] = 0;
            if(point >= 0)
                break;
            board[top[i]-1][i] = 3 - right;
            if(right == PLAYER_RIGHT && machineWin(top[i]-1, i, m, n, board))
                point = i;
            else if(right == COMPUTER_RIGHT && userWin(top[i]-1, i, m, n, board))
                point = i;
            board[top[i]-1][i] = 0;
            if(point >= 0)
                break;
        }
    }
    return point;
}

int calConnect(int x, int y, int** board, int right, int row, int col) {
    int count = 1;
    board[x][y] = right;
    //heng direction
    int i = 0, j = 0;
    for(i = y; i >= 0; i--)
        if(!(board[x][i] == right))
            break;
    count += (y - i);
    for(i = y; i < col; i++)
        if(!(board[x][i] == right))
            break;
    count += (i - y - 1);

    //shu direction
    for(i = x; i < row; i++)
        if(!(board[i][y] == right))
            break;
    count += (i - x);

    //left-down to up-right
    for (i = x, j = y; i < row && j >= 0; i++, j--)
		if (!(board[i][j] == right))
			break;
	count += (y - j);
	for (i = x, j = y; i >= 0 && j < col; i--, j++)
		if (!(board[i][j] == right))
			break;
	count += (j - y - 1);

    //left-up to right-down
	for (i = x, j = y; i >= 0 && j >= 0; i--, j--)
		if (!(board[i][j] == right))
			break;
	count += (y - j);
	for (i = x, j = y; i < row && j < col; i++, j++)
		if (!(board[i][j] == right))
			break;
	count += (j - y - 1);
	board[x][y] = 0;
	return count;
}

int getRand(int n, int a[]) {
    int sum = 0;
	for (int i = 0; i < n; i++)
		sum += a[i];
	int r = rand() % sum + 1;
	sum = 0;
	for (int i = 0; i < n; i++) {
		sum += a[i];
		if (sum >= r)
			return i;
	}
	return n - 1;
}
