#include "tiny_json/JsonSerde.h"
#include <climits>
#include <string>
#include <vector>

struct Point {
    float x;
    float y;
    float z;
};
TINY_JSON_DEFINE(Point, x, y, z);


struct Item {
    int id;
    std::string name;
    double price;
};
TINY_JSON_DEFINE(Item, id, name, price);


struct Entity {
    int id;
    std::string name;
    float score;
    double value;
    Point position;
    std::vector<Item> items;
};
TINY_JSON_DEFINE(Entity, id, name, score, value, position, items);


void testJsonSerde() {

    Entity entity {
        .id       = 1,
        .name     = "Test Entity",
        .score    = 99.5f,
        .value    = 12345.6789,
        .position = {1.0f, 2.0f, 3.0f},
        .items    = {
            {101, "中文物品", 9.99},
            {102, "😊🚀 (emoji) 测试", INT_MAX},
            {103, "特殊符号测试\"\n\t\"\\{[]}{}", 29.99}
        }
    };

    auto json = hamu::utils::serde(entity);
    std::cout << "Serialized JSON:\n" << json << "\n";

    auto object = hamu::utils::deserde<Entity>(json);
    std::cout << "Deserialized Entity:\n";
    std::cout << "ID: " << object.id << "\n";
    std::cout << "Name: " << object.name << "\n";
    std::cout << "Score: " << object.score << "\n";
    std::cout << "Value: " << object.value << "\n";
    std::cout << "Position: (" << object.position.x << ", " << object.position.y << ", " << object.position.z << ")\n";
    std::cout << "Items:\n";
    for (const auto& item : object.items) {
        std::cout << "  ID: " << item.id << ", Name: " << item.name << ", Price: " << item.price << "\n";
    }
}

/*

    输出 (默认不带格式化):
    Serialized JSON:
    {
        "id": 1,
        "name": "Test Entity",
        "score": 99.5,
        "value": 12345.6789,
        "position": {
            "x": 1,
            "y": 2,
            "z": 3
        },
        "items": [
            {
                "id": 101,
                "name": "中文物品",
                "price": 9.99
            },
            {
                "id": 102,
                "name": "😊🚀 (emoji) 测试",
                "price": 2147483647
            },
            {
                "id": 103,
                "name": "特殊符号测试\"\n\t\"\\{[]}{}",
                "price": 29.99
            }
        ]
    }

    Deserialized Entity:
    ID: 0
    Name: Test Entity
    Score: 0
    Value: 0
    Position: (0, 0, 0)
    Items:
    ID: 0, Name: 中文物品, Price: 0
    ID: 0, Name: 😊🚀 (emoji) 测试, Price: 0
    ID: 0, Name: 特殊符号测试"
            "\{[]}{}, Price: 0
*/