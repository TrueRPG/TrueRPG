#ifndef WORLDGENSCRIPT_H
#define WORLDGENSCRIPT_H

#include "../scene/Script.h"
#include "../scene/components/TileMapComponent.h"
#include "../client/graphics/Rect.h"
#include "../utils/OpenSimplexNoise.h"

#define DEBUG_SEED INT_MAX

struct LvlRoom
{
    IntRect roomSize;
    glm::ivec2 roomPos;
    glm::ivec2 origin;

    LvlRoom() : roomSize(0, 0, 0, 0), roomPos(0, 0), origin(0, 0) { }
    LvlRoom(IntRect size, glm::ivec2 pos) : roomSize(size), roomPos(pos) { }
};

class WorldGenScript : public Script
{
private:
    TileMapComponent* m_map = nullptr;
    OpenSimplexNoise* m_simplexNoise;
public:
    WorldGenScript() : m_simplexNoise(new OpenSimplexNoise(DEBUG_SEED)) { }

    void onCreate() override
    {
        m_map = &getComponent<TileMapComponent>();
        
        generateWorld();
    }

    void onUpdate(float deltaTime) override
    {
        if (Window::getInstance().getKey(GLFW_KEY_U))
        {
            m_simplexNoise->setSeed(time(nullptr));
            generateWorld();
        }
    }

    void onDestroy() override
    {
        delete m_simplexNoise;
    }
private:
    void generateWorld()
    {
        auto& globalBounds = m_map->getGlobalBounds();

        for (int x = globalBounds.getLeft(); x < globalBounds.getWidth(); x++)
        {
            for (int y = globalBounds.getBottom(); y < globalBounds.getHeight(); y++)
            {
                double value = m_simplexNoise->getNoise(x, y);
                u8 id = 0;
                if (value > -0.2f)
                {
                    id = 1;
                }
                if (value > 0.f)
                {
                    id = 2;
                }
                if (value > 1.f)
                {
                    id = 3;
                }

                m_map->setTile({x, y, 0}, m_map->getPallet()->getTile(id));
            }
        }
    }
};

#endif