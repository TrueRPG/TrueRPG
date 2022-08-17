#ifndef RPG_GAME_H
#define RPG_GAME_H

#include "scene/Scene.h"
#include "scene/Entity.h"
#include "client/graphics/SpriteBatch.h"
#include "client/graphics/Font.h"
#include "client/audio/StreamAudioClip.h"
#include "client/audio/CachedAudioClip.h"
#include "client/animation/SpriteAnimator.h"

class Game
{
    Font m_font;
    Texture m_heroTexture;
    Texture m_baseTexture;
    SpriteAnimator m_characterAnimator;

    StreamAudioClip m_steps;
    StreamAudioClip m_music;
    StreamAudioClip m_night;

    Scene m_scene;

    Entity m_clockEntity;
    Entity m_cameraEntity;
    Entity m_playerEntity;
public:
    Game();
    void update(float deltaTime);
    void destroy();
};

#endif //RPG_GAME_H
