/********************************************************
*	UCT.h : 信心上限树节点类                               *
*	吴宸昊                                               *
*	nldxtd@gmail.com                                    *
*	2021.4                                              *
*********************************************************/

#include "Point.h"
#include <math.h>

#define PLAYER_RIGHT 1
#define COMPUTER_RIGHT 2
#define PLAYER_WIN_PROFIT -1 //can be discussed
#define COMPUTER_WIN_PROFIT 1 //can be discussed
#define INTERMINANT_PROFIT 2
#define TIE_PROFIT 0 //when is-tie 
#define COEFFICIENT 0.8 //cofficient can be discussed 
#define MAX_N 12
#define EPS 1e-8

#ifndef NODE_H_
#define NODE_H_

class Node {
    private:
        int noX, noY; //plot of node that cannot place chess
        int rowNum, colNum; //number of rows and columns
        int chessRight; //chess right of current node
        int depth; //depth of current node
        int* top; //top place of the board
        int** board; //each place of the board
        int lastX, lastY; //plot of node last been placed
        int visitNum, winNum; //number of visits and wins
        double profit; //profit of current node
        int numToExpand; //number of children to expand
        int* nodeToExpand; //children to expand

        Node* father; //father of current node
        Node** children; //children of current node

    public:
        Node(int nX, int nY, int row, int col, int* _top, int** _board, int lX = -1, int lY = -1, int _depth = 0, int _chessRight = COMPUTER_RIGHT, Node* _father = nullptr):
            noX(nX), noY(nY), rowNum(row), colNum(col), top(_top), board(_board), lastX(lX), lastY(lY), depth(_depth), chessRight(_chessRight), father(_father) {
                this->visitNum = 0;
                this->winNum = 0;
                this->profit = 0;
                this->numToExpand = 0;
                this->children = new Node*[MAX_N];
                this->nodeToExpand = new int[MAX_N];
                for(int i = 0; i < colNum; i++) {
                    if(top[i] != 0)
                        nodeToExpand[numToExpand++] = i;
                    children[i] = nullptr;
                }
            }
        void clear(); //clear the node
        bool canBeExpand(); //check is node can be expanded
        bool isTerminant(); //check if the state is terminant
        int* topState(); //return top point state
        int** boardState(); //return board state
        int getDepth(); //return depth of current node
        int getChessRight(); //return chessRight of current node
        Node* getFatherNode(); //return father node
        Point getLastPoint(); //return last step
        void backUp(int profit); //update monitoring profit
        Node* expand(int right); //return next node to expand
        Node* bestChild(); //return the best child to monitor
        int getProfit(); //get profit of current node
        int getVisitTime(); //get visit time of current node
};

#endif
