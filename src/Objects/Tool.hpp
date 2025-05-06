#pragma once

#include <string>

class Tool {
public:
    enum class Type { Shovel, Axe, Hoe };

    Tool(Type type);

    void use();
    bool isBroken() const;

    std::string getName() const;

private:
    Type toolType;
    int durability; // Прочность инструмента
};