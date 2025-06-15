#pragma once

class IJournal {
public:
    virtual ~IJournal() = default;
    virtual void run() = 0;
};

