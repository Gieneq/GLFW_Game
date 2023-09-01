#include <World.h>
#include <gmock/gmock.h>

class MockElevation : public ElevationDepth {
public:
    MOCK_METHOD(float, z, (), (const));
    MOCK_METHOD(int, getIndex, (), (const));
    MOCK_METHOD(void, setIndex, (const int));
};
