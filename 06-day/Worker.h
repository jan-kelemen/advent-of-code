#ifndef WORKER_H

#define WORKER_H

#include "Command.h"
#include "LightGrid.h"

class Worker
{
public:
    Worker() = default;

    void execute(const std::vector<Command> &commands, LightGrid &grid);

    void execute(const Command &command, LightGrid &grid);

    ~Worker() = default;
};

#endif // WORKER_H
