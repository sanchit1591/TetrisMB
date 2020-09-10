//
//  Engine.cpp
//  Tetris
//
//  Created by Abhishek Marda on 8/28/20.
//  Copyright © 2020 Abhishek Marda. All rights reserved.
//

#include "Engine.h"
#include "Grid.h"
#include "Block.h"
#include <vector>
#include <iostream>
#include <random>
#include <chrono>
Engine::Engine() : m_points(0)
{
    printInstructions();
    
    const int MAX_QUEUED_BLOCKS = 3;
    
    for (int i =0; i< MAX_QUEUED_BLOCKS; ++i)
        queueBlock(generateNewBlock(5));
        
}

Engine::~Engine()
{
    while (!m_upcomingBlocks.empty())
    {
        if (m_upcomingBlocks.front())
            delete m_upcomingBlocks.front();
        m_upcomingBlocks.pop();
    }
    
}

void Engine::printInstructions() const
{
    using namespace std;
    std::cout << "                 #####  #####  #####  #####  #####  ####     #       #  ####"<<std::endl;
    std::cout << "                   #    #        #    #   #    #   #         ##     ##  #   #" <<std::endl;
    std::cout << "                   #    ###      #    ####     #   ####      # #   # #  ####" <<std::endl;
    std::cout << "                   #    #        #    #  #     #       #     #  # #  #  #   #" <<std::endl;
    std::cout << "                   #    #####    #    #   #  ##### ####      #   #   #  ####\n\n" <<std::endl;
    cout << "Welcome to TetrisMB © Abhishek Marda and Sanchit Bawri 2020." << endl;
    
    cout <<"The objective of the game is to score the maximum points possible." << endl;
    cout <<endl<< "How to play:" <<endl;
    
    cout << "A block will advance from the top of the grid to the bottom. You must place the block in such a way that maximum eliminations are possible. An elimination occurs when an entire row is filled. That row is then deleted, giving you more space to place blocks."<<endl;
    
    cout<<"More the eliminations, more your points. Consecutive eliminations will lead to more points, so place your blocks wisely!"<<endl;
    cout <<"You can place the block using the following methods:"<<endl;
    cout <<"1. By pressing the left and right arrow keys to move the block left or right."<<endl;
    cout <<"2. By pressing the up arrow key to rotate the block."<<endl;
    cout <<"3. By pressing the down arrow key to push the block down faster."<<endl;
    
    cout << endl<<"In the case that a block is not able to enter the grid completely, game will be over!"<<endl;
    
    cout <<"Enjoy!"<<endl;
}

void Engine::printBoard()
{
    
    for(int i = 0; i < 4 * PRINT_COLS + 5; i++)
    {
        std::cout << EMPTY_CELL;
    }
    
    std::cout << "          UPCOMING BLOCKS   \n";
    
    Block *temp;
    
    for(int i = 0; i < 3; i++)
    {
        temp = m_upcomingBlocks.front();
        m_grid.placeUpcomingBlock(temp, i);
        m_upcomingBlocks.pop();
        m_upcomingBlocks.push(temp);
    }
    
//    for(int i = 0; i < 5; i++)
//    {
//        for(int j = 0; j < 5; j++)
//        {
//            std::cout << m_upcomingBlocks.front()->blockGrid[i][j];
//        }
//        
//        std::cout << std::endl;
//    }
    m_grid.printGrid();
    
}

void Engine::queueBlock(Block *new_block)
{
    if (m_upcomingBlocks.size() >= 3)
        return;
    
    m_upcomingBlocks.push(new_block);
}

void Engine::updatePoints(size_t points)
{
    m_points+=points;
}

size_t Engine::getPoints() const
{
    return m_points;
}


Block* Engine::generateNewBlock(int num_blocks)
{

    bool board[5][5];
    for(int i=0; i<5; ++i)
        for (int j=0; j<5; ++j)
            board[i][j]=0;
    board[2][2] = 1;
    std::vector<Unit> units(num_blocks);
    units[0] = Unit(2, 2);
     
    std::vector<std::pair<int, int>> possibilities;
    
    //blocks around the middle
    possibilities.push_back(std::make_pair(2, 1));
    possibilities.push_back(std::make_pair(2, 3));
    possibilities.push_back(std::make_pair(1, 2));
    possibilities.push_back(std::make_pair(3, 2));

    for(int blocks_remaining = num_blocks-1; blocks_remaining>0; --blocks_remaining)
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::minstd_rand0 generator (seed);
        int index = generator()%possibilities.size();
        std::pair<int, int> curr_coords = possibilities[index];
        units[blocks_remaining] = Unit(curr_coords.first, curr_coords.second);
        board[curr_coords.first][curr_coords.second] = 1;
        possibilities.erase(possibilities.begin()+index);
        
        curr_coords.first +=1; //one above mid position
        
        if(curr_coords.first<5 &&
           !board[curr_coords.first][curr_coords.second] &&
           std::find(possibilities.begin(),possibilities.end(),curr_coords)==possibilities.end()
           )
            possibilities.push_back(std::make_pair(curr_coords.first, curr_coords.second));
        
        curr_coords.first-=2; //one below mid position
        
        if(curr_coords.first>0 &&
           !board[curr_coords.first][curr_coords.second] &&
           std::find(possibilities.begin(),possibilities.end(),curr_coords)==possibilities.end()
           )
            possibilities.push_back(std::make_pair(curr_coords.first, curr_coords.second));
        
        curr_coords.first+=1;
        curr_coords.second+=1; //right of the mid position
        
        if(curr_coords.second<5 &&
           !board[curr_coords.first][curr_coords.second] &&
           std::find(possibilities.begin(),possibilities.end(),curr_coords)==possibilities.end()
           )
            possibilities.push_back(std::make_pair(curr_coords.first, curr_coords.second));
        
        curr_coords.second-=2; //left of the mid position
        
        if(curr_coords.second>0 &&
           !board[curr_coords.first][curr_coords.second] &&
           std::find(possibilities.begin(),possibilities.end(),curr_coords)==possibilities.end()
           )
            possibilities.push_back(std::make_pair(curr_coords.first, curr_coords.second));
        
    }
    //For printing
//    for (int i=0; i<5; ++i)
//    {
//        for(int j=0; j<5; ++j)
//        {
//            std::cout <<(board[i][j]? '#' : '.' );
//        }
//        std::cout<<std::endl;
//    }
//    std::cout<<std::endl;
    Block* ptr = new Block(units);
    
   return ptr;

}

int Engine::checkAndEliminateRows()
{
    return 100 /*MAGIC NUMBERS YAY*/ * m_grid.findRowsToEliminate(); //FIXME: create the point system
}

int Engine::move(Direction dir)
{
    int status = m_grid.moveActiveBlock(dir);
    if (status == 0)
    {
        if (m_grid.activeBlockFullyAppeared())
        {
            queueBlock(generateNewBlock(BLOCK_SIZE));
            m_grid.hardcodeActiveBlock(OCCUPIED_CELL);
            m_grid.setActiveBlock(m_upcomingBlocks.front());
            m_upcomingBlocks.pop();
            int points = checkAndEliminateRows();
            updatePoints(points);
            return 0;
        }
        else //game over
        {
            return -1;
        }
    }
    else //move successful!
    {
        return 1;
    }
}
