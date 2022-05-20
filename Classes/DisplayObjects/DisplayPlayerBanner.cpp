//
//  DisplayPlayerBanner.cpp
//  demonsway-mobile
//
//  Created by alexandre gimeno on 12/05/2022.
//

#include "DisplayPlayerBanner.hpp"

USING_NS_CC;

DisplayPlayerBanner* DisplayPlayerBanner::create()
{
    //create new instance pointer
    DisplayPlayerBanner* ret = new (std::nothrow) DisplayPlayerBanner();
    if(ret && ret->init ())
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

bool DisplayPlayerBanner::init()
{
    // get screen size
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // init banner container node
    _container = Node::create();
    
    // create and place banner
    auto redBelt = Sprite::createWithSpriteFrameName("belt_red@2x.png");
    _container->addChild(redBelt, 0);
    
    initPseudo();
    
    
    this->addChild(_container);
    
    return true;
}

bool DisplayPlayerBanner::initPseudo()
{
    // init pseudo value
    _playerName = "PLAYER";
    
    // init and place pseudo
    _pseudoLabel = cocos2d::Label::createWithBMFont("fonts/font_score.fnt", _playerName);
    _pseudoLabel->setAnchorPoint(Vec2(0,0));
    if(_playerName == "PLAYER")
    {
        _pseudoLabel->setColor(Color3B::GRAY);
    }
    _container->addChild(_pseudoLabel,2);
    
    // init and place editable textfield
    _pseudoTextField = cocos2d::ui::TextField::create(_playerName,"Arial",90);
    _pseudoTextField->setAnchorPoint(Vec2(0,0.24));
    //_pseudoTextField->setOpacity(0);
    
    _container->addChild(_pseudoTextField, 3);
    
    // edit label text every keyboard touch
    _pseudoTextField->addEventListener([&](Ref *pSender, cocos2d::ui::TextField::EventType type){
        
        // replace player name and make it gray
        if(_pseudoTextField->getString().size() <= 0)
        {
            _pseudoLabel->setString("PLAYER");
            _pseudoLabel->setColor(Color3B::GRAY);
        }
        else if(_pseudoTextField->getString().size() <= 10)
        {
            _playerName = _pseudoTextField->getString();
            for(std::string::iterator cit = _playerName.begin(); cit != _playerName.end(); cit ++)
            {
                int c = *cit;
                
                if((c >= 48 && c <= 57) || //number
                   (c >= 65 && c <= 90) || //upper key
                   (c >= 97 && c <= 122) || // lower key
                   (c == 32 || c == 45 || c == 46 || c == 47 || c == 95)) // space - . /_
                {
                    //replace lower to upper key
                    *cit = ::toupper(c);
                }
                else
                {
                    //replace unexist char by _
                    *cit = 95;
                }
            }
            
            // init new key
            _pseudoLabel->setString(_playerName);
            _pseudoTextField->setString(_playerName);
            _pseudoLabel->setColor(Color3B::WHITE);
        }
        else
        {
            //block new key
            _pseudoTextField->setString(_playerName);
            _pseudoLabel->setColor(Color3B::WHITE);
        }
    });
    
    return true;
}
