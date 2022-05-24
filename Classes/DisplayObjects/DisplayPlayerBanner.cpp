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
    _bannerName = "player_banner_1@2x.png";
    _banner = Sprite::createWithSpriteFrameName(_bannerName);
    _banner->setPosition(Vec2(_banner->getContentSize().width/2, 0));
    _container->addChild(_banner, 0);
    
    // init and place image
    initAvatar();
    
    // init and place pseudo
    initPseudo();
    
    
    this->addChild(_container);
    
    return true;
}

bool DisplayPlayerBanner::initAvatar()
{
    // create the node avatar container
    _avatar = Node::create();
    _avatar->setPosition(Vec2(80, 16));
    
    // draw the mask
    _avatarMask = DrawNode::create();
    _avatarMask->drawSolidCircle(Vec2(0, 0), 64, 0, 64, Color4F::GREEN);
    
    // create the clipping node and set the stencil
    _avatarClippingNode = ClippingNode::create();
    _avatarClippingNode->setStencil(_avatarMask);
    _avatarClippingNode->setPosition(Vec2(0, 8));

    // create the clipped sprite
    _avatarName = "player_icon_2@2x.png";
    _avatarImage = Sprite::createWithSpriteFrameName(_avatarName);
    _avatarImage->setScale(0.4375);
    _avatarImage->setPosition(Vec2(0, 0));
    
    // create the back sprite
    _avatarBack = Sprite::createWithSpriteFrameName("avatar_icon_back@2x.png");
    _avatarBack->setPosition(Vec2(16, 0));
    
    // create the top sprite
    _avatarTop = Sprite::createWithSpriteFrameName("avatar_icon_top@2x.png");
    _avatarTop->setPosition(Vec2(0, 8));
    
    // add the sprite to the clipping node
    _avatarClippingNode->addChild(_avatarImage, 0);
    
    // add all sprite to node
    _avatar->addChild(_avatarBack, 0);
    _avatar->addChild(_avatarClippingNode, 1);
    _avatar->addChild(_avatarTop, 2);
    
    // add node to container
    _container->addChild(_avatar);
    
    return true;
}

bool DisplayPlayerBanner::initPseudo()
{
    // init pseudo value
    _playerName = "PLAYER";
    
    // create the node pseudo container
    _pseudo = Node::create();
    _pseudo->setPosition(Vec2(160, -35));
    _container->addChild(_pseudo);
    
    // init and place pseudo
    _pseudoLabel = cocos2d::Label::createWithBMFont("fonts/font_score.fnt", _playerName);
    _pseudoLabel->setAnchorPoint(Vec2(0,0));
    if(_playerName == "PLAYER")
    {
        _pseudoLabel->setColor(Color3B::GRAY);
    }
    _pseudo->addChild(_pseudoLabel,2);
    
    // init and place editable textfield
    _pseudoTextField = cocos2d::ui::TextField::create(_playerName,"Arial",90);
    _pseudoTextField->setAnchorPoint(Vec2(0,0.24));
    //_pseudoTextField->setOpacity(0);
    
    _pseudo->addChild(_pseudoTextField, 3);
    
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
