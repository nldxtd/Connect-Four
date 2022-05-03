#include "Point.h"
#include "Node.h"
#include "Judge.h"
#include <algorithm>
#include <stdlib.h>

bool Node::canBeExpand() { 
    //check if the node can be expanded
    return this->numToExpand > 0; 
}

bool Node::isTerminant() {
    //check if current state is terminant
    if(lastX == -1 && lastY == -1)
        return false;
    else if((chessRight == PLAYER_RIGHT && machineWin(lastX, lastY, rowNum, colNum, board)) || 
            (chessRight == COMPUTER_RIGHT && userWin(lastX, lastY, rowNum, colNum, board)) || 
             (isTie(colNum, top)))
        return true;
    return false;
}   

int* Node::topState() {
    int* copyTop = new int[colNum];
    for(int i = 0; i < colNum; i++)
        copyTop[i] = top[i];
    return copyTop;
}

int** Node::boardState() {
    int** copyBoard = new int*[rowNum];
    for(int i = 0; i < rowNum; i++) {
        copyBoard[i] = new int[colNum];
        for(int j = 0; j < colNum; j++)
            copyBoard[i][j] = board[i][j];
    }
    return copyBoard;
}

void Node::clear() {
    for(int i = 0; i != rowNum; i++)
        delete [] board[i];
    delete [] board;
    delete [] top;
    delete nodeToExpand;
    for(int i = 0; i < colNum; i++) {
        if(children[i]) {
            children[i]->clear();
            delete children[i];
        }
    }
    delete [] children;
}

int Node::getDepth() { return this->depth; }
int Node::getChessRight() { return this->chessRight; }
Node* Node::getFatherNode() { return this->father; }

Point Node::getLastPoint() {
    return Point(lastX, lastY);
}

void Node::backUp(int profit) {
    this->visitNum++;
    this->winNum += profit;
}

Node* Node::expand(int right) {
    //find a random point
    int index = rand() % numToExpand;
    int* copyTop = topState();
    int** copyBoard = boardState();
    int index_x = nodeToExpand[index], index_y = --copyTop[index_x];
    //place chess at point
    copyBoard[index_y][index_x] = getChessRight();
    //if the point above is unplacable, than revert it
    if(index_y-1 == noX && index_x == noY)
        copyTop[index_x]--;
    //create new child for current child
    children[index_x] = new Node(noX, noY, rowNum, colNum, copyTop, copyBoard, index_y, index_x, this->depth+1, right, this);
    std::swap(nodeToExpand[index], nodeToExpand[--numToExpand]); //swap the chosen to the right
    return children[index_x];
}

int Node::getProfit() { return winNum; }
int Node::getVisitTime() { return visitNum; }

Node* Node::bestChild() {
    Node* bestchild;
    double maxProfit = -RAND_MAX;
    for(int i = 0; i < colNum; i++) {
        if(children[i]) {
            double winRatio = 0;
            if(children[i]->getVisitTime() != 0)
                winRatio = (((chessRight == COMPUTER_RIGHT) ? 1 : -1) * children[i]->getProfit()) / (children[i]->getVisitTime() + EPS);
            double tempRatio = winRatio + COEFFICIENT * sqrt(2*logl(visitNum)/(children[i]->getVisitTime()));
            if(tempRatio > maxProfit || ((tempRatio == maxProfit) && (rand() % 2 == 0))) {
                maxProfit = tempRatio;
                bestchild = children[i];
            }
        }
    }
    return bestchild;
}
