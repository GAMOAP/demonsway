//
//  DisplayGrid.cpp
//  cocos2d
//
//  Created by alexandre gimeno on 18/09/2020.
//

#include "DisplayGrid.hpp"
#include "StaticGrid.hpp"

USING_NS_CC;

DisplayGrid* DisplayGrid::create(Grid grid)
{
    //create new instance pointer
    DisplayGrid* ret = new (std::nothrow) DisplayGrid();
    if(ret && ret->init (grid))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        return nullptr;
    }
}

bool DisplayGrid::init(Grid grid)
{
    //init used grid type.
    _grid = grid;
    
    //init used grid array.
    std::vector<std::vector<int>> gridArray = _grid.gridArray;
    
    // create grid container node
    auto gridContainer = Node::create();
    this->addChild(gridContainer, 1);
    
    // create grid buttons container
    auto gridButtonsMenu = Menu::create();
    gridButtonsMenu->setPosition(Vec2::ZERO);
    gridContainer->addChild(gridButtonsMenu, 2);
    
    // init randomization
    srand((unsigned)time(NULL));
    
    //init gird display
    for(int l = 0; l < GRID_NBR_CASE + 1; l++)
    {
        for(int c = 0; c < GRID_NBR_CASE + 1; c++)
        {
            std::string gridCornerCaseName = cornerName(l, c);
            
            cocos2d::Vec2 positionLC  = cocos2d::Vec2(l, c);
            cocos2d::Vec2 positionXY  = StaticGrid::getPositionXY(positionLC);
            
            //add grid button
            if(l < gridArray.size() && c < gridArray[l].size() && gridArray[l][c] != 0)
            {
                int buttonColor = rand() % 6;
                
                //init button image
                auto gridBtnNormal = Sprite::createWithSpriteFrameName("grid_button_normal_" + std::to_string(buttonColor) + "@2x.png");
                auto gridBtnSelected = Sprite::createWithSpriteFrameName("grid_button_selected_" + std::to_string(buttonColor) + "@2x.png");
                
                //init menu and callback button line and collumn
                auto gridBtn = MenuItemSprite::create(
                                                     gridBtnNormal,
                                                     gridBtnSelected,
                                                     CC_CALLBACK_1(DisplayGrid::gridButtonCallback, this, l, c));
                // place and add button
                gridBtn->setPosition(Vec2(positionXY.x, positionXY.y - BTN_SHIFT_HEIGHT));
                gridButtonsMenu->addChild(gridBtn);
            }
          
            //add back backDisplayGrid
            if(gridCornerCaseName != "0000")
            {
                auto backDisplayGrid = Sprite::createWithSpriteFrameName("grid_down_" + gridCornerCaseName + "@2x.png");
            
                int gridW = backDisplayGrid->getContentSize().width;
                int gridH = backDisplayGrid->getContentSize().height;
                
                backDisplayGrid->setPosition(Vec2(positionXY.x - gridW/2, positionXY.y + gridH/2));
                gridContainer->addChild(backDisplayGrid, 1);
            }
            
            //add top topDisplayGrid
            if(gridCornerCaseName != "0000" && gridCornerCaseName != "1111")
            {
                auto topDisplayGrid = Sprite::createWithSpriteFrameName("grid_up_" + gridCornerCaseName + "@2x.png");
                
                int gridW = topDisplayGrid->getContentSize().width;
                int gridH = topDisplayGrid->getContentSize().height;
                
                topDisplayGrid->setPosition(Vec2(positionXY.x - gridW/2, positionXY.y + gridH/2));
                gridContainer->addChild(topDisplayGrid, 3);
            }
        }
    }
    
    return true;
}

void DisplayGrid::gridButtonCallback(cocos2d::Ref* pSender, int line, int collumn)
{
    std::vector<int> lineCollumn = {line, collumn};
    _eventDispatcher->dispatchCustomEvent("GRID_BTN_PUSH", &lineCollumn);
}

std::string DisplayGrid::cornerName(int line, int collumn)
{
    //create grid array 8 by 8 whith border 0
    std::vector<std::vector<int>> gridArray;
    for(int l = 0; l < GRID_NBR_CASE + 2; l++)
    {
        std::vector<int> gridLine;
        for(int c = 0; c < GRID_NBR_CASE + 2; c++)
        {
            int cel = 0;
            if (l > 0 && l < GRID_NBR_CASE + 1 && c > 0 && c < GRID_NBR_CASE + 1) {
                cel = _grid.gridArray[l-1][c-1];
            }
            gridLine.push_back(cel);
        }
        gridArray.push_back(gridLine);
    }
    
    //init corner var
    int cornerLeftUp = gridArray[line][collumn];
    int cornerRightUp = gridArray[line][collumn+1];
    int cornerRightDown = gridArray[line+1][collumn+1];
    int cornerLeftDown = gridArray[line+1][collumn];
    
    // corner image code
    std::string result = std::to_string(cornerLeftUp) +
                         std::to_string(cornerRightUp) +
                         std::to_string(cornerRightDown) +
                         std::to_string(cornerLeftDown);
     
    return result;
}
