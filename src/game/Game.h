#include "GameBase.h"

class Game : public GameBase {
public:
    Game() : GameBase() {};
    virtual ~Game() = default;

    virtual bool init() override;
    virtual bool input() override;
    virtual void update(float dt) override;
    virtual void render() override;
};
