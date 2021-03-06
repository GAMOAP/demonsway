//
//  HomeScene.cpp
//  cocos2d
//
//  Created by alexandre gimeno on 15/09/2020.
//

#include "HomeScene.hpp"
#include "GameScene.hpp"

#include "DisplayBackground.hpp"
#include "DisplayPlayerBanner.hpp"

#include "StaticSounds.hpp"

USING_NS_CC;

Scene* HomeScene::createScene()
{
    return HomeScene::create();
}

// init scene
bool HomeScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    // get screen size
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // create and add displayBackground
    auto background = DisplayBackground::create();
    background->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(background, 0);
    
    // create and add title image
    auto title = Sprite::createWithSpriteFrameName("title@2x.png");
    title->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(title, 0);
    
    // create and player banner
    auto playerBanner = DisplayPlayerBanner::create();
    playerBanner->setPosition(Vec2(0, visibleSize.height * 3/4 + origin.y));
    this->addChild(playerBanner);
        
    // create start button images, add to menu and place
    auto startBtnNormal = Sprite::createWithSpriteFrameName("button_start_normal@2x.png");
    auto startBtnSelected = Sprite::createWithSpriteFrameName("button_start_selected@2x.png");
    auto startBtn = MenuItemSprite::create(
                                               startBtnNormal,
                                               startBtnSelected,
                                               CC_CALLBACK_1(HomeScene::menuPlayCallback, this));
        
    startBtn->setPosition(Vec2(visibleSize.width * 1/4 + origin.x, visibleSize.height * 1/5 + origin.y));
    
    // create start button images, add to menu and place
    auto multiBtnNormal = Sprite::createWithSpriteFrameName("button_multi_normal@2x.png");
    auto multiBtnSelected = Sprite::createWithSpriteFrameName("button_multi_selected@2x.png");
    auto multiBtn = MenuItemSprite::create(
                                               multiBtnNormal,
                                               multiBtnSelected,
                                               CC_CALLBACK_1(HomeScene::menuPlayCallback, this));
        
    multiBtn->setPosition(Vec2(visibleSize.width * 3/4 + origin.x, visibleSize.height * 1/5 + origin.y));
        
    // create sound button images, add to menu and place
    
    auto soundBtnNormal = Sprite::createWithSpriteFrameName("sound_on@2x.png");
    auto soundBtnSelected = Sprite::createWithSpriteFrameName("sound_off@2x.png");
    auto soundBtn = MenuItemSprite::create(
                                              soundBtnNormal,
                                              soundBtnSelected,
                                              CC_CALLBACK_1(HomeScene::menuSoundCallback, this));

    soundBtn->setPosition(Vec2(
                                origin.x + visibleSize.width - soundBtn->getContentSize().width/2,
                                origin.y + visibleSize.height - soundBtn->getContentSize().height/2));
    
    // create, place and add menu
    auto menu = Menu::create(startBtn, multiBtn, soundBtn, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    
    /*
    // play home theme music;
    StaticSounds::playMusic("home");
     */
    
    return true;
}

// replace current scene to game scene
void HomeScene::menuPlayCallback(Ref* pSender)
{
    StaticSounds::playSound("window");
    
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene(scene);
}

// replace current scene to score scene
void HomeScene::menuScoresCallback(Ref* pSender)
{
    /*
    auto scene = ScoreScene::createScene();
    Director::getInstance()->replaceScene(scene);
    */
}

//change sound value
void HomeScene::menuSoundCallback(Ref* pSender)
{
    
}
