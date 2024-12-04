#include "dev_config.h"
#include "DHTesp.h" // Click here to get the library: http://librarymanager/All#DHTesp
#include <Ticker.h>




class inputHandler
{
private:
    DHTesp dht1;
    // DHT dht1;
    // DHT dht2;

    float dht1Temp = 0;
    float dht1Hum = 0;
    float dht2Temp = 0;
    float dht2Hum = 0;
    
    
public:
    inputHandler(/* args */);
    void initSensor();
    void updateSensor();
    float getDHT1Temp();
    float getDHT1Hum();
    float getDHT2Temp();
    float getDHT2Hum();
    
    // TODO: add flow meter
    float getFlowMeterLahan1Flow();
    float getFlowMeterLahan1Volume();
    float getFlowMeterLahan2Flow();
    float getFlowMeterLahan2Volume();

    ~inputHandler();
};


