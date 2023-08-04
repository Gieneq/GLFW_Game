#include "GameBase.h"

class Game : public GameBase {
public:
    virtual ~Game() = default;

    virtual void init() override;
    virtual bool input() override;
    virtual void update(float dt) override;
    virtual void render() override;

protected:
    World world;
    Entity player;

private:
    bool createWorld();
    bool createPlayer();
};
