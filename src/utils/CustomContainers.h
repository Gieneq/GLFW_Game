#pragma once
#include <array>

enum class Cell3X3 {
    TOP_LEFT,
    TOP_MIDDLE,
    TOP_RIGHT,
    MIDDLE_LEFT,
    MIDDLE_MIDDLE,
    MIDDLE_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_MIDDLE,
    BOTTOM_RIGHT
};

constexpr int cell3X3ToInt(Cell3X3 cell) {
    switch(cell) {
        case Cell3X3::TOP_LEFT:
            return 0;
        case Cell3X3::TOP_MIDDLE:
            return 1; 
        case Cell3X3::TOP_RIGHT:
            return 2;
        case Cell3X3::MIDDLE_LEFT:
            return 3;
        case Cell3X3::MIDDLE_MIDDLE:
            return 4;
        case Cell3X3::MIDDLE_RIGHT:
            return 5;
        case Cell3X3::BOTTOM_LEFT:
            return 6;
        case Cell3X3::BOTTOM_MIDDLE:
            return 7;
        case Cell3X3::BOTTOM_RIGHT:
            return 8;
        default:
            return 4; //M_M
    }
}

constexpr Cell3X3 intToCell3X3(int index) {
    switch(index) {
        case 0:
            return Cell3X3::TOP_LEFT;
        case 1:
            return Cell3X3::TOP_MIDDLE; 
        case 2:
            return Cell3X3::TOP_RIGHT;
        case 3:
            return Cell3X3::MIDDLE_LEFT;
        case 4:
            return Cell3X3::MIDDLE_MIDDLE;
        case 5:
            return Cell3X3::MIDDLE_RIGHT;
        case 6:
            return Cell3X3::BOTTOM_LEFT;
        case 7:
            return Cell3X3::BOTTOM_MIDDLE;
        case 8:
            return Cell3X3::BOTTOM_RIGHT;
        default:
            return Cell3X3::MIDDLE_MIDDLE; //M_M
    }
}

constexpr int cell3X3Count() {
    return 9;
}

template <typename T>
class Segment3X3 {
public:
    using Row = std::array<T, 3>;
    using Grid = std::array<Row, 3>;

    Segment3X3() {
        fill(T());
    }

    template <Cell3X3 cell>
    T get() {
        constexpr auto index = cell3X3ToInt(segment);
        constexpr int row = index / 3;
        constexpr int col = index % 3;
        return grid.at(row).at(col);
    }

    template <Cell3X3 cell>
    T get() const {
        constexpr auto index = cell3X3ToInt(segment);
        constexpr int row = index / 3;
        constexpr int col = index % 3;
        return grid.at(row).at(col);
    }

    template <Cell3X3 cell>
    void set(T t) {
        constexpr auto index = cell3X3ToInt(segment);
        constexpr int row = index / 3;
        constexpr int col = index % 3;
        grid.at(row).at(col) = t;
    }

    T getRelative(int relX, int relY) {
        return grid.at(relX + 1).at(relY + 1);
    }

    T getRelative(int relX, int relY) const {
        return grid.at(relX + 1).at(relY + 1);
    }

    void setRelative(T t, int relX, int relY) {
        grid.at(relX + 1).at(relY + 1) = t;
    }

    T get(int indexX, int indexY) {
        return grid.at(indexX).at(indexY);
    }

    T get(int indexX, int indexY) const {
        return grid.at(indexX).at(indexY);
    }

    void set(T t, int indexX, int indexY) {
        grid.at(indexX).at(indexY) = t;
    }

    T get(int index) {
        const int indexY = index / 3;
        const int indexX = index % 3;
        return get(indexX, indexY);
    }

    T get(int index) const {
        const int indexY = index / 3;
        const int indexX = index % 3;
        return get(indexX, indexY);
    }

    void set(T t, int index) {
        const int indexY = index / 3;
        const int indexX = index % 3;
        set(t, indexX, indexY);
    }

    void fill(T t) {
        for (int i = 0; i < cell3X3Count(); i++) {
            set(t, i);
        }
    }

    typename Grid::const_iterator begin() const { return grid.begin(); }

    typename Grid::const_iterator end() const { return grid.end(); }

    typename Grid::iterator begin() { return grid.begin(); }

    typename Grid::iterator end() { return grid.end(); }
   

private:
    Grid grid;
};


class Segment3X3Mask : public Segment3X3<bool> {
public:
    Segment3X3Mask() {
        fill(false);
    }
};