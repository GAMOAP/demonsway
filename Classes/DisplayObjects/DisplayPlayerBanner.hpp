//
//  DisplayPlayerBanner.hpp
//  demonsway-mobile
//
//  Created by alexandre gimeno on 12/05/2022.
//

#ifndef DisplayPlayerBanner_hpp
#define DisplayPlayerBanner_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class DisplayPlayerBanner : public cocos2d::Node
{
public:
    static DisplayPlayerBanner* create();

    virtual bool init();
    
private:
    
    cocos2d::Node* _container = nullptr;
    
    std::string _bannerName;
    cocos2d::Sprite* _banner = nullptr;
    
    //avatar variables
    std::string _avatarName;
    cocos2d::Node* _avatar = nullptr;
    cocos2d::ClippingNode* _avatarClippingNode = nullptr;
    cocos2d::Sprite* _avatarTop = nullptr;
    cocos2d::Sprite* _avatarBack = nullptr;
    cocos2d::Sprite* _avatarImage = nullptr;
    cocos2d::DrawNode* _avatarMask = nullptr;
    
    //text variables
    std::string _playerName;
    cocos2d::Node* _pseudo = nullptr;
    cocos2d::ui::TextField* _pseudoTextField = nullptr;
    cocos2d::Label* _pseudoLabel = nullptr;
    
    bool initAvatar();
    
    /**
     * @brief init and place player pseudo.
     * @return action type OK.
    */
    bool initPseudo();
    
    int _score = 0;
    int _combo = 0;
    int _bestCombo = 0;
    
};

#endif /* DisplayPlayerBanner_hpp */
