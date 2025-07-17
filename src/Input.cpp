#include "raylib.h"
#include "Input.h"


InputAction ReadGameInput()
{
	if (IsKeyPressed(KEY_W))
	{
		return INPUT_UP;
	}

	if (IsKeyPressed(KEY_A))
	{
		return INPUT_LEFT;
	}

	if (IsKeyPressed(KEY_S))
	{
		return INPUT_DOWN;
	}

	if (IsKeyPressed(KEY_D))
	{
		return INPUT_RIGHT;
	}

	return INPUT_NONE;
}

void InitDirectionQueue(DirectionQueue* queue)
{
    queue->head = 0;
    queue->tail = 0;
    queue->count = 0;
}

bool DirectionQueueEmpty(DirectionQueue* queue)
{
    return (queue->count == 0);
}

bool DirectionQueueFull(DirectionQueue* queue)
{
    return (queue->count >= INPUT_QUEUE_SIZE);
}

bool EnqueueDirection(DirectionQueue* queue, Direction dir)
{
    if (DirectionQueueFull(queue))
    {
        return false;
    }
    queue->dirValues[queue->tail] = dir;
    queue->count++;
    queue->tail = (queue->tail + 1) % INPUT_QUEUE_SIZE; // wrap

    return true;
}

Direction DequeueDirection(DirectionQueue* queue)
{
    Direction result;

    if (DirectionQueueEmpty(queue))
    {
        return DIR_NONE;
    }

    result = queue->dirValues[queue->head];
    queue->head = (queue->head + 1) % INPUT_QUEUE_SIZE; // wrap
    queue->count--;

    return result;
}

Direction GetNextDirection(DirectionQueue* queue)
{
    if (DirectionQueueEmpty(queue))
    {
        return DIR_NONE;
    }

    return queue->dirValues[queue->head];
}