//
//  DisplayGrid.hpp
//  cocos2d
//
//  Created by alexandre gimeno on 18/09/2020.
//

#ifndef DisplayGrid_hpp
#define DisplayGrid_hpp

#include "cocos2d.h"
#include "../Models/StaticGrid.hpp"

struct Grid
{
    std::vector<std::vector<int>> gridArray;
};


class DisplayGrid : public cocos2d::Node
{
public:
    static DisplayGrid* create(Grid grid);

    virtual bool init(Grid grid);
    
private:
    
    const int BTN_SHIFT_HEIGHT = 24;
    
    //Used grid attribute
    Grid _grid;
    
    /**
     * @brief Dispath event "GRID_BTN_PUSH" and vector content button pushed line and collumn.
     * @param pSender button grid reference.
     * @param line button touch line number.
     * @param collumn button touch collumn number.
    */
    void gridButtonCallback(cocos2d::Ref* pSender, int line, int collumn);
    
    /**
     * @brief Get the corner image code position.
     * @param line gridArray line.
     * @param collumn gridArray collumn.
     * @return code position std::string.
    */
    std::string cornerName(int line, int collumn);
};

#endif /* DisplayGrid_hpp */
