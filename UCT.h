/********************************************************
*	UCT.h : 信心上限树类                                  *
*	吴宸昊                                             *
*	nldxtd@gmail.com                                 *
*	2021.4                                           *
*********************************************************/
#include <time.h>
#include "Point.h"
#include "Node.h"
#include "Judge.h"

#ifndef UCT_H_
#define UCT_H_

class UCT {
    private:
        int noX, noY; //plot of node that cannot place chess
        int rowNum, colNum; //number of rows and columns
        Node* root; //root of tree
        int startTime; //start time of one single search

        Node* treePolicy(Node* nowNode); //choose the best node to monitor
        int defaultPolicy(Node* selectNode); //calculate profit in single monitor
        void backUp(Node* selectNode, int deltaProfit); //back sore the profit in parents
        Node* bestChild(Node* nowNode); //todo
        Node* expand(Node* nowNode);
        Point addChessIn(int right, int* top, int** board);
        int calcProfit(Point last, int right, int* top, int** board);
        int changeRight(int right); 

    public:
        UCT(int nX, int nY, int row, int col): noX(nX), noY(nY), rowNum(row), colNum(col), startTime(clock()) {}
        Node* UCTsearch(int* top, int** board); //search the UCT and return the best place
        ~UCT();
};

int calWin(int right, int* top, int** board, int m, int n);
int calConnect(int x, int y, int** board, int right, int row, int col);
int getRand(int n, int a[]);
#endif
