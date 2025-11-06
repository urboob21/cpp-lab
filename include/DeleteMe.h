#pragma once
class DeleteMe
{
private:
    int m_lastResult;

public:
    DeleteMe();
    ~DeleteMe();
    bool add(int addend1, int addend2);
    bool mul(int factor1, int factor2);
    bool div(int dividend, int divisor);
    int get_last_result();
};
