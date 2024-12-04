#include <dev_config.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>

/**
 * @class OverTheAir
 * @brief A class for performing over-the-air updates.
 * 
 * This class provides functionality for updating the device firmware over the air.
 * It supports both secure and non-secure connections.
 */
class OverTheAir
{
private:
    WiFiClientSecure client;
    WiFiClient client_not_secure;
public:
    OverTheAir(/* args */);
    bool update(String server, int port, String endpoint, bool secure = true);
    ~OverTheAir();
};


