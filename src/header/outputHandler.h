#include "dev_config.h"

class outputHandler
{
private:
    int pin;
    bool isOn;

public:

    outputHandler(int pin);
    outputHandler();

    void init(int pin);

    void on();
    void off();
    void toggle();


};


