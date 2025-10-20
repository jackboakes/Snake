#include "food.h"
#include "raylib.h"


Food::Food()
{
    position = { 0, 0 };
}

void Food::Spawn(const Snake& snake)
{
    bool collision { false };

    do
    {
        GridPosition newFoodPos { 0 };
        newFoodPos.x = GetRandomValue(1, GRID_SIZE - 2);
        newFoodPos.y = GetRandomValue(1, GRID_SIZE - 2);

        collision = false;
        // Check for open space to spawn food
        for (int i { 0 }; i < snake.bodyPart.size(); i++)
        {
            if (snake.bodyPart[i].x == newFoodPos.x &&
                snake.bodyPart[i].y == newFoodPos.y)
            {
                collision = true;
                break;
            }
        }

        if (!collision)
        {
            position.x = newFoodPos.x;
            position.y = newFoodPos.y;
        }
    } while (collision);
}

bool Food::CheckCollision(const Snake& snake)
{
    return (position.x == snake.bodyPart[0].x &&
        position.y == snake.bodyPart[0].y);
}

