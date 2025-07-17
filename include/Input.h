#ifndef INPUT_H
#define INPUT_H


#define INPUT_QUEUE_SIZE 2

enum InputAction
{
	INPUT_NONE = 0,
	INPUT_UP,
	INPUT_LEFT,
	INPUT_DOWN,
	INPUT_RIGHT
};

enum Direction
{
	DIR_NONE = 0,
	DIR_NORTH,
	DIR_WEST,
	DIR_SOUTH,
	DIR_EAST
};

struct DirectionQueue
{
	Direction dirValues[INPUT_QUEUE_SIZE];
	int head;
	int tail;
	int count;
};


InputAction ReadGameInput();

// Input direction queue
void InitDirectionQueue(DirectionQueue* queue);
bool DirectionQueueEmpty(DirectionQueue* queue);
bool DirectionQueueFull(DirectionQueue* queue);
bool EnqueueDirection(DirectionQueue* queue, Direction dir);
Direction DequeueDirection(DirectionQueue* queue);
Direction GetNextDirection(DirectionQueue* queue);

#endif