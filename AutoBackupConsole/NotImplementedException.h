#pragma once
#include <stdexcept>
class NotImplementedException : public std::logic_error
{
public:
    NotImplementedException() : std::logic_error("") {}
    virtual char const* what() const { return "Function not yet implemented."; }
};

