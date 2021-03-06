//
//  DisplayDemonsGrid.cpp
//  cocos2d
//
//  Created by alexandre gimeno on 12/10/2020.
//

#include "DisplayDemonsGrid.hpp"

#include "StaticGrid.hpp"
#include "StaticTest.hpp"

USING_NS_CC;

DisplayDemonsGrid* DisplayDemonsGrid::create(cocos2d::Sprite* divingBoard, std::vector<std::vector<int>> gridArray)
{
    //create new instance pointer
    DisplayDemonsGrid* ret = new (std::nothrow) DisplayDemonsGrid();
    if(ret && ret->init (divingBoard, gridArray))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        ret = nullptr;
        return nullptr;
    }
}

bool DisplayDemonsGrid::init(cocos2d::Sprite* divingBoard, std::vector<std::vector<int>> gridArray)
{
    //init diving board pointer
    _divingBoard = divingBoard;
    
    //init grid array shape
    _gridArray = gridArray;
    
    //clean demons arrays
    _demonsActionList.clear();
    _demonsInGridList.clear();
    
    // add new random demon on diving board with delay(cause opening windows sound)
    auto delay = DelayTime::create(MOVE_TIME);
    auto callFunc = CallFunc::create([this]()
    {
        printf("addNewDemonDiver()");
        addNewDemonDiver();
    });
    
    // run action
    auto seq = Sequence::create(delay, callFunc, NULL);
    this->runAction(seq);
    
    return true;
}

bool DisplayDemonsGrid::addDemonGrid(int l, int c)
{
    // check if demon is ready to jump in demon grid
    if(!_demonDiver)
        return false;
    
    // check if demon grid case is free
    if(!StaticTest::getCaseFree(l, c, _demonsInGridList))
        return false;
    
    // init jumper demon pointer
    Demon* demon = _demonDiver;
    
    // reset the demon diver pointer
    _demonDiver = nullptr;
    
    // get grid pixel position
    Vec2 posDemon = StaticGrid::getPositionXY(Vec2(l, c));
    
    // create animation demon jump to grid position
    auto move = MoveTo::create(MOVE_TIME, posDemon);
    auto scaleUp = ScaleTo::create(MOVE_TIME/2, JUMP_SCALE);
    auto scaleDown = ScaleTo::create(MOVE_TIME/2, DEMON_IN_GRID_SCALE);
    auto scaleSeq = Sequence::create(scaleUp, scaleDown, NULL);
    auto callFunc = CallFunc::create([=]()
    {
        demon->action(waiting);
        addToGrid(demon, l, c);
        startActionGrid();
    });
    auto seq = Sequence::create(Spawn::create(scaleSeq, move, NULL), callFunc, NULL);
    
    // run animation
    demon->runAction(seq);
    demon->action(jump);
    
    return true;
}

bool DisplayDemonsGrid::addNewDemonDiver()
{
    // reset the demon diver pointer
    _demonDiver = nullptr;
    
    // get screen size
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // create new demon and add out of screen
    auto newDemon = Demon::create();
    newDemon->setPosition(Vec2(visibleSize.width/2 + origin.x, origin.y - newDemon->getContentSize().height));
    this->addChild(newDemon);
    
    // create animation move demon to divin board edge an wait push grid button
    auto move = MoveTo::create(MOVE_TIME, Vec2(_divingBoard->getPosition().x,_divingBoard->getContentSize().height*3/4));
    auto callFunc = CallFunc::create([=]()
    {
        newDemon->action(waiting);
        _demonDiver = newDemon;
    });
    auto seq = Sequence::create(move, callFunc, NULL);
    
    // run animation
    newDemon->runAction(seq);
    newDemon->action(enterDiving);
    
    return true;
}

void DisplayDemonsGrid::startActionGrid(int actionTurn)
{
    _actionTurn = actionTurn;
    
    // first action turn init action list.
    if(_actionTurn <= 0)
    {
        _demonsActionList.clear();
        
        _demonsActionList.insert(_demonsActionList.begin(), _demonsInGridList.begin(), _demonsInGridList.end());
    }
    
    // first action turn init action list.
    if(_demonsActionList.size() >= GRID_NBR_CASE*GRID_NBR_CASE)
    {
        // event end game and out of function
        removeAllDemons();
        _eventDispatcher->dispatchCustomEvent("GAME_OVER");
        return;
    }
    
    // remove demon into the grid
    bool demonRemoved = removeDemonsGrid();
    
    // move demons into the grid
    bool demonMoved = true;
    if(!demonRemoved)
    {
        demonMoved = moveDemonsGrid();
    }
    
    if(_demonsActionList.size() <= 0 || (!demonRemoved && !demonMoved))
    {
        // event action end (score)
        _eventDispatcher->dispatchCustomEvent("END_ACTION_GRID");
        
        // finish action and add new demon on diver board
        _actionTurn = 0;
        addNewDemonDiver();
    }
    else
    {
        // start new turn after all animation ends
        _actionTurn ++;
        _eventDispatcher->addCustomEventListener("DEMONS_ANIMATIONS_ENDS",[this](EventCustom* event)
        {
            _eventDispatcher->removeCustomEventListeners("DEMONS_ANIMATIONS_ENDS");
            startActionGrid(_actionTurn);
        });
    }
}

bool DisplayDemonsGrid::removeDemonsGrid()
{
    // is a demon remove boolean init
    bool demonRemoved = false;
    
    // init demon can remove list
    _demonsToRemoveList.clear();
    std::vector<Demon*>::iterator removeIt;
    _demonsToRemoveList = StaticTest::getRemoveDemonList(_demonsInGridList);
    
    for(removeIt = _demonsToRemoveList.begin(); removeIt != _demonsToRemoveList.end(); removeIt++)
    {
        demonRemoved = true;
        
        // intit remove demon of the list
        Demon* removingDemon = *removeIt;
        removeDemon(removingDemon);
        
        // remove demon from action list
        std::vector<Demon*>::iterator findIt = std::find(_demonsActionList.begin(), _demonsActionList.end(), removingDemon);
        if(findIt != _demonsActionList.end())
        {
            _demonsActionList.erase(findIt);
        }
    }
    
    return demonRemoved;
}

bool DisplayDemonsGrid::moveDemonsGrid()
{
    // is a demon move boolean init
    bool demonMoved = false;
    
    // init demon can move list
    _demonsToMoveList.clear();
    std::vector<Demon*>::iterator moveIt;
    _demonsToMoveList = StaticTest::getMoveDemonList(_demonsActionList, _demonsInGridList, _gridArray);
    
    for(moveIt = _demonsToMoveList.begin(); moveIt != _demonsToMoveList.end(); moveIt++)
    {
        demonMoved = true;
        
        // init move demon of the list
        Demon* movingDemon = *moveIt;
        moveDemon(movingDemon);
        
        // remove demon from the action list
        std::vector<Demon*>::iterator findIt = std::find(_demonsActionList.begin(), _demonsActionList.end(), movingDemon);
        if(findIt != _demonsActionList.end())
        {
            _demonsActionList.erase(findIt);
        }
    }
    
    return demonMoved;
}

bool DisplayDemonsGrid::removeDemon(Demon* demon)
{
    //event demon removed (score)
    demon->displayScore(3);
    _eventDispatcher->dispatchCustomEvent("DEMON_REMOVED");
        
    // get demon name and start remove animation
    std::string demonName  = demon->getName();
    demon->action(removing);
    
    // listen demon remove animation end
    _eventDispatcher->addCustomEventListener(demonName + "_REMOVE_END",[demon, demonName, this](EventCustom* event)
    {
        // remove demon pointer from remove list
        _demonsToRemoveList.erase(std::find(_demonsToRemoveList.begin(), _demonsToRemoveList.end(), demon));
        
        // remove demon pointer from demon in grid list
        _demonsInGridList.erase(std::find(_demonsInGridList.begin(), _demonsInGridList.end(), demon));
        
        // remove demon sprite
        demon->removeFromParent();
        
        // test if is the last action
        areDemonsAnimationsEnds();
        
        // remove listener
        _eventDispatcher->removeCustomEventListeners(demonName + "_REMOVE_END");
    });
    
    return true;
}

bool DisplayDemonsGrid::moveDemon(Demon* demon)
{
    //event demon moved (score)
    demon->displayScore(1);
    _eventDispatcher->dispatchCustomEvent("DEMON_MOVED");
    
    //get demon grid pixel position
    DemonPosition demonWayPos = demon->getDemonGridWayPosition();
    const int newL = demonWayPos.line;
    const int newC = demonWayPos.collumn;
    Vec2 posDemon = StaticGrid::getPositionXY(Vec2(newL, newC));
    
    // move demon to grid position
    auto move = MoveTo::create(MOVE_TIME, posDemon);
    auto callFunc = CallFunc::create([demon,newL,newC,this]()
    {
        // init action ended
        demon->action(waiting);
        addToGrid(demon, newL, newC);
        
        // remove demon from move list
        _demonsToMoveList.erase(std::find(_demonsToMoveList.begin(), _demonsToMoveList.end(), demon));
        
        // test if is the last action
        areDemonsAnimationsEnds();
    });
    
    // run action
    auto seq = Sequence::create(move, callFunc, NULL);
    demon->runAction(seq);
    demon->action(walk);
    
    return true;
}

void DisplayDemonsGrid::addToGrid(Demon* demon, int line, int collumn)
{
    // init demon : name and position
    demon->setName("DEMON_" + std::to_string(line) + "_" + std::to_string(collumn));
    demon->setDemonGridPosition(line, collumn);
    
    // push demon in demon grid list
    std::vector<Demon*>::iterator demonIt;
    demonIt = std::find(_demonsInGridList.begin(), _demonsInGridList.end(), demon);
    if(demonIt == _demonsInGridList.end())
    {
        _demonsInGridList.push_back(demon);
    }
}

bool DisplayDemonsGrid::areDemonsAnimationsEnds()
{
    // test move and remove list
    if(_demonsToMoveList.size() > 0 || _demonsToRemoveList.size() > 0)
    {
        return false;
    }
    _eventDispatcher->dispatchCustomEvent("DEMONS_ANIMATIONS_ENDS");
    
    return true;
}

bool DisplayDemonsGrid::removeAllDemons()
{
    // get screen size
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    // push demon in demon grid list
    std::vector<Demon*>::iterator demonIt;
    for(demonIt = _demonsInGridList.begin(); demonIt != _demonsInGridList.end(); demonIt++)
    {
        // remove demon pointer
        Demon* removeDemon = *demonIt;
        
        // create animation fall demon out screen and remove
        // delay value betwen 0 and 0,3 second
        float delayValue = (rand() % 10)/30;
        auto delay = DelayTime::create(delayValue);
        // animation time 1 second
        auto move = MoveTo::create(MOVE_TIME * 2 - delayValue, Vec2(removeDemon->getPosition().x, removeDemon->getPosition().x + -visibleSize.height));
        auto moveEaseInBack = EaseBackIn::create(move->clone());
        auto callFunc = CallFunc::create([=]()
        {
            removeDemon->removeFromParent();
            if(this->getChildrenCount() <= 0)
            {
                _eventDispatcher->dispatchCustomEvent("ALL_DEMONS_REMOVED");
            }
            
        });
        auto seq = Sequence::create(delay, moveEaseInBack, callFunc, NULL);
        
        // run animation
        removeDemon->runAction(seq);
        removeDemon->action(gameOver);
    }
    
    return true;
}
