//
//  Grid.cpp
//  Tetris
//
//  Created by Abhishek Marda on 8/28/20.
//  Copyright © 2020 Abhishek Marda. All rights reserved.
//

#include "Grid.h"

Grid::Grid()
{
    for(int row_i = 0; row_i < MAX_ROWS; row_i++)
    {
        for(int col_i = 0; col_i < MAX_ROWS; col_i++)
        {
            m_grid[row_i][col_i] = EMPTY_CELL;
        }
    }
    
    m_activeBlock = nullptr;
}

Grid::~Grid()
{
    if (m_activeBlock)
        delete m_activeBlock;
    
}

void Grid::printGrid()
{
    using namespace std;
    //modify grid info according to active block position
    if(m_activeBlock != nullptr)
    {
        hardcodeActiveBlock(OCCUPIED_CELL);
    }
    
    //Margin from the top
    topMargin();
    
    //print Top Boundary
    leftMargin();
    printBoundary();
    
    //print the grid with side boundaries
    for(int row_i = PRINT_ROW_INDEX; row_i < PRINT_ROWS; row_i++)
    {
        printRowDivider();
        
        leftMargin();
        
        cout << GRID_BOUNDARY << EMPTY_CELL;
        
        for(int col_i = 0; col_i < PRINT_COLS; col_i++)
        {
            cout << GRID_COL_DIVIDER << EMPTY_CELL;
            cout << m_grid[row_i][col_i];
            cout << EMPTY_CELL;
        }
        
        cout << GRID_COL_DIVIDER;
        
        cout << EMPTY_CELL << GRID_BOUNDARY << endl;
    }
    
    //print last row Divider
    printRowDivider();
    
    //print the bottom boundary
    leftMargin();
    printBoundary();
    
    //remove the harcoded active block from the grid
    //cause it will move down till the next delay
    if(m_activeBlock != nullptr)
    {
        hardcodeActiveBlock(EMPTY_CELL);
    }
    
}

void Grid::hardcodeActiveBlock(char info)
{
    //retrieve the position of all the units composing the block
    std::vector<Unit> settled_block = m_activeBlock -> getPositions();
    
    int unit_row;
    int unit_col;
    
    for(size_t i = 0; i < settled_block.size(); i++)
    {
        unit_row = (settled_block.at(i)).m_row;
        unit_col = (settled_block.at(i)).m_col;
        
        m_grid[unit_row][unit_col] = info;
    }
    
}


void Grid::eliminateRow(const int row_number)
{
    //delete targeted row and shift information
    for(int row_i = row_number; row_i > PRINT_ROW_INDEX; row_i--)
    {
        for(int col_i = 0; col_i < MAX_COLS; col_i++)
        {
            m_grid[row_i][col_i] = m_grid[row_i - 1][col_i];
        }
    }
    
    //set top row to be empty
    for(int i = 0; i < MAX_COLS; i++)
    {
        m_grid[PRINT_ROW_INDEX][i] = EMPTY_CELL;
    }
}

//int Grid::isMovable(Block* moving_block , Direction direction)
//{
///
//}



/* PRIVATE FUNCTIONS */
void Grid::printBoundary()
{
    
    using namespace std;
    //loop runs for more than just the number of columns for alignment according to spaces out grid boxes
    for(int i = 0; i < 4 * PRINT_COLS + 5; i++)
    {
        cout << GRID_BOUNDARY;
    }
    
    cout << endl;
}


void Grid::printRowDivider()
{
    
    using namespace std;
    
    //Margin from left
    leftMargin();
    
    //Grid's Left Boundary
    cout << GRID_BOUNDARY << EMPTY_CELL;
    
    for(int i = 0; i < PRINT_COLS; i++)
    {
        cout << GRID_ROW_DIVIDER;
    }
    
    cout << GRID_CORNER;
    
    //Grid's Right Boundary
    cout << EMPTY_CELL << GRID_BOUNDARY << endl;
}


void Grid::topMargin()
{
    
    using namespace std;
    
    for(int i = 0; i < GRID_TOP_MARGIN; i++)
    {
        cout << endl;
    }
}


void Grid::leftMargin()
{
    
    using namespace std;
    
    for(int i = 0; i < GRID_LEFT_MARGIN; i++)
    {
        cout << EMPTY_CELL;
    }
}

void Grid::placeUpcomingBlock(Block *block_p, int position)
{
    
}

int Grid::findRowsToEliminate()
{
    int count=0;
    for (int i=MAX_ROWS-1; i>=0; --i)
    {
        bool rowFull = true;
        
        for (int j = 0; j<MAX_COLS; ++j)
        {
            if (m_grid[i][j]!=OCCUPIED_CELL)
            {
                rowFull = false;
                break;
            }
        }
        
        if (rowFull)
        {
            eliminateRow(i);
            ++i; //don't change i in the next iteration because that row will have been updated
            ++count;
        }
    }
    
    return count;
}

bool Grid::activeBlockFullyAppeared()
{
    const std::vector<Unit>& list_of_units = m_activeBlock->getPositions();
    for (const Unit& u : list_of_units)
    {
        //if the row is less in than the buffer rows (i.e. the rows on top, then the block has not fully appeared.
        //we don't need to check for the column, since we anyway ensure that the block never goes past the column bounds
        if(u.m_row < BUFFER_ROWS) //TODO: check if equality is allowed
            return false;
    }
    
    return true;
}

bool Grid::moveActiveBlock(Direction dir)
{
    Block b = *m_activeBlock;
    switch(dir)
    {
        case DOWN:
        {
            b.move(DOWN);
            if (b.getMax(DOWN)>=MAX_ROWS)
                return false;
            break;
        }
            
        case LEFT:
        {
            Block b = *m_activeBlock;
            b.move(LEFT);
            if (b.getMax(LEFT)<0)
                return false;
            break;
        }
            
        case RIGHT:
        {
            Block b = *m_activeBlock;
            b.move(RIGHT);
            if (b.getMax(RIGHT)>=MAX_COLS)
                return false;
            break;
        }
            
        case UP:
        {
            Block b = *m_activeBlock;
            b.rotate(CLOCKWISE);
            if (b.getMax(RIGHT)>=MAX_COLS || b.getMax(UP)<0 || b.getMax(LEFT)<0 || b.getMax(DOWN)>=MAX_ROWS)
                return false;
            break;
        }
    }
    
    const std::vector<Unit>& list_of_units = b.getPositions();
    for (const Unit& u : list_of_units)
    {
        if (m_grid[u.m_row][u.m_col] == OCCUPIED_CELL)
            return false;
    }
    *m_activeBlock = b;
    return true;
}
