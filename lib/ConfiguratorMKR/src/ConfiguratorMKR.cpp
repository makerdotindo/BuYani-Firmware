#include "ConfiguratorMKR.h"

/**
 * @brief Constructs a ConfiguratorMKR object with the specified parameters.
 *
 * @param host The host name or IP address of the server.
 * @param isSecure Flag indicating whether the connection should be secure or not.
 * @param serialNumber The serial number of the device.
 */
ConfiguratorMKR::ConfiguratorMKR(String host, bool isSecure) : _host(host), client(), http()
{
    this->isSecure = false;
    isVariableSynced = false;
    isServerPinged = false;
    isFirmwareSynced = false;

    this->isSecure = isSecure;

    if (this->isSecure)
    {
        client.setInsecure();
    }
}

bool ConfiguratorMKR::mountLittleFS()
{
    StorageHandler::begin();

    // check file system
    if (!StorageHandler::checkDir("/config"))
    {
        StorageHandler::createDir("/config");
    }

    if (!StorageHandler::checkFile("/config/variables.json"))
    {
        StorageHandler::createFile("/config/variables.json");
    }

    if (!StorageHandler::checkFile("/config/serialNumber.json"))
    {
        StorageHandler::createFile("/config/serialNumber.json");
    }

    return true;
}

/**
 * Initializes the ConfiguratorMKR object.
 *
 * This function initializes the ConfiguratorMKR object and performs necessary setup tasks.
 * It establishes a connection to the specified host, either in secure or non-secure mode,
 * based on the value of the isSecure flag. It also initializes the storage handler and checks
 * the file system for the required directories and files. If any of the directories or files
 * are missing, this function creates them.
 *
 * @return True if initialization is successful, false otherwise.
 */
bool ConfiguratorMKR::init()
{

    if (!client.connected())
    {
        client.stop();
        isClientConnected = false;
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        return false;
    }

    if (isClientConnected == false)
    {
        return false;
    }

    DEBUG_PRINTLN("host: " + _host);
    if (isSecure)
    {
        DEBUG_PRINTLN("Secure");
        if (!client.connect(_host.c_str(), 443))
        {
            return false;
        }
    }
    else
    {
        DEBUG_PRINTLN("Not Secure");
        if (!client.connect(_host.c_str(), 80))
        {
            return false;
        }
    }

    isClientConnected = true;

    return true;
}

/**
 * @brief Pings the server with the specified endpoint and serial number.
 *
 * This function initializes the necessary components and sends a POST request to the server
 * with the provided endpoint and serial number. It then prints the time taken, HTTP code, and
 * HTTP response to the debug console. If the HTTP code is 200, it sets the isServerPinged flag
 * to true and returns true. Otherwise, it returns false.
 *
 * @param endpoint The endpoint to ping on the server.
 * @return True if the server was successfully pinged, false otherwise.
 */
bool ConfiguratorMKR::pingServer(String endpoint)
{
    // if (!init())
    // {
    //     return false;
    // }

    String url = "https://" + _host + endpoint + "?serial_number=" + this->serialNumber;
    DEBUG_PRINTLN(url);
    http.begin(client, url);

    uint16_t timestamp = millis();
    int httpCode = http.POST("");
    DEBUG_PRINTLN("Time taken: " + String(millis() - timestamp) + "ms");
    DEBUG_PRINTLN("HTTP Code: " + String(httpCode));
    DEBUG_PRINTLN("HTTP Response: " + String(http.getString()));

    http.end();
    // client.flush();
    // client.clearWriteError();
    // client.stop();

    if (httpCode == 200)
    {
        isServerPinged = true;
        return true;
    }

    return false;
}

/**
 * Synchronizes the firmware with the server.
 *
 * @param firmwareVersion The firmware version to sync.
 * @param hardwareVersion The hardware version to sync.
 * @param endpoint The endpoint to send the sync request to.
 * @return True if the firmware is successfully synced, false otherwise.
 */
bool ConfiguratorMKR::syncFirmware(String firmwareVersion, String hardwareVersion, String endpoint)
{
    if (!isServerPinged)
    {
        return true;
    }

    // if (!init())
    // {
    //     return false;
    // }

    String url = "https://" + _host + endpoint;
    DEBUG_PRINTLN(url);
    http.begin(client, url);

    http.addHeader("Content-Type", "application/json");

    JsonDocument doc;

    doc["serialNumber"] = this->serialNumber;
    doc["firmwareVersion"] = firmwareVersion;
    doc["hardwareVersion"] = hardwareVersion;

    String reqStr = "";
    serializeJson(doc, reqStr);
    DEBUG_PRINTLN(reqStr);
    DEBUG_PRINTLN("Length: " + String(reqStr.length()));

    uint16_t timestamp = millis();
    int httpCode = http.sendRequest("GET", reqStr);
    DEBUG_PRINTLN("Time taken: " + String(millis() - timestamp) + "ms");
    DEBUG_PRINTLN("HTTP Code: " + String(httpCode));
    String response = http.getString();
    DEBUG_PRINTLN("HTTP Response: " + response);

    http.end();
    // client.flush();
    // client.clearWriteError();
    // client.stop();

    doc.clear();

    if (httpCode == 200)
    {
        deserializeJson(doc, response);

        if (doc["isFirmwareMatch"].as<String>() == "True")
        {
            isFirmwareSynced = true;
            return true;
        }
        else
        {
            isFirmwareSynced = false;
            return false;
        }
    }

    return true;
}

bool ConfiguratorMKR::getIsVariableGotUpdated()
{
    return isVariableGotUpdated;
}

/**
 * Synchronizes variables with the server.
 *
 * @param endpoint The endpoint to send the variables to.
 * @return True if the variables were successfully synchronized, false otherwise.
 */
bool ConfiguratorMKR::syncVariables(String endpoint)
{
    if (!isServerPinged)
    {
        return false;
    }

    // if (!init())
    // {
    //     return false;
    // }

    String url = "https://" + _host + endpoint;
    DEBUG_PRINTLN(url);
    http.begin(client, url);

    http.addHeader("Content-Type", "application/json");

    JsonDocument doc;

    doc["serialNumber"] = this->serialNumber;

    loadVariables();

    for (int i = 0; i < variables.size(); i++)
    {
        doc["variables"][variables[i].first] = variables[i].second;
    }

    // doc["variables"]["HOST_SERVER"] = "192.168.1.1";

    String reqStr = "";
    serializeJson(doc, reqStr);
    DEBUG_PRINTLN(reqStr);
    DEBUG_PRINTLN("Length: " + String(reqStr.length()));

    uint16_t timestamp = millis();
    int httpCode = http.sendRequest("POST", reqStr);
    DEBUG_PRINTLN("Time taken: " + String(millis() - timestamp) + "ms");
    DEBUG_PRINTLN("HTTP Code: " + String(httpCode));
    String response = http.getString();
    DEBUG_PRINTLN("HTTP Response: " + response);

    http.end();
    // client.flush();
    // client.clearWriteError();
    // client.stop();

    doc.clear();

    if (httpCode == 200)
    {
        deserializeJson(doc, response);

        for (int i = 0; i < variables.size(); i++)
        {
            DEBUG_PRINTLN("Variable " + (String)doc[variables[i].first].as<bool>());
            if (doc[variables[i].first].as<bool>() == true)
            {
                DEBUG_PRINTLN("Variable " + variables[i].first + " synced");
                variables.erase(variables.begin() + i);
                i = i - 1;
            }
        }

        for (int i = 0; i < variables.size(); i++)
        {
            DEBUG_PRINTLN("Variable " + variables[i].first + " not synced");
        }

        if (variables.size() == 0)
        {
            isVariableSynced = true;
            loadVariables();
            timestamp = millis();
            return true;
        }

        isVariableSynced = false;

        url = "https://" + _host + "/api/device-sync/update";
        DEBUG_PRINTLN(url);

        // if (!init())
        // {
        //     return false;
        // }

        http.begin(client, url);

        http.addHeader("Content-Type", "application/json");

        doc["serialNumber"] = this->serialNumber;

        for (int i = 0; i < variables.size(); i++)
        {
            doc["variables"][i] = variables[i].first;
        }

        // doc["variables"]["HOST_SERVER"] = "192.168.1.1";

        reqStr = "";
        serializeJson(doc, reqStr);
        DEBUG_PRINTLN(reqStr);
        DEBUG_PRINTLN("Length: " + String(reqStr.length()));

        timestamp = millis();
        DEBUG_PRINTLN("Request Update Variable");
        httpCode = http.sendRequest("POST", reqStr);
        DEBUG_PRINTLN("Time taken: " + String(millis() - timestamp) + "ms");
        DEBUG_PRINTLN("HTTP Code: " + String(httpCode));
        response = http.getString();
        DEBUG_PRINTLN("HTTP Response: " + response);

        http.end();
        // client.flush();
        // client.clearWriteError();
        // client.stop();

        doc.clear();

        for (int i = 0; i < variables.size(); i++)
        {
            DEBUG_PRINTLN("========Variable " + (String)doc[variables[i].first].as<bool>());
        }

        if (httpCode == 200)
        {
            bool flag = false;
            int count = 0;

            deserializeJson(doc, response);

            JsonObject temp = doc["variables"].as<JsonObject>();

            std::vector<std::pair<String, String>> tempVar = variables;

            variables.clear();

            for (JsonPair kv : temp)
            {
                DEBUG_PRINTLN((String) "Variable " + kv.key().c_str() + " synced");
                for (int i = 0; i < tempVar.size(); i++)
                {
                    DEBUG_PRINTLN((String) "Variable " + tempVar[i].first + " compare with " + kv.key().c_str());
                    if (tempVar[i].first == kv.key().c_str())
                    {
                        // DEBUG_PRINTLN((String)"Variable " + kv.key().c_str() + " updated to " + kv.value().as<String>());
                        tempVar[i].second = kv.value().as<String>();

                        count++;

                        if (tempVar[i].first == "sync_interval")
                        {
                            flag = true;
                        }
                        updateVariable(tempVar[i].first, tempVar[i].second);
                        // variables.erase(variables.begin() + i);
                        // i = i - 1;
                    }
                }
                // variables.push_back(std::make_pair(kv.key().c_str(), kv.value().as<String>()));
            }

            if (count > 1)
            {
                flag = false;
            }

            // for (int i = 0; i < variables.size(); i++)
            // {
            //     DEBUG_PRINTLN("Variable " + (String)doc[variables[i].first].as<bool>());
            //     if (doc[variables[i].first].as<bool>() == true)
            //     {
            //         DEBUG_PRINTLN("Variable " + variables[i].first + " synced");
            //         variables.erase(variables.begin() + i);
            //         i = i - 1;
            //     }
            //     {
            //         /* code */
            //     }

            //     variables.at(i).second = doc["variables"][variables[i].first].as<String>();
            //     updateVariable(variables[i].first, variables[i].second);
            //     // res->push_back(variables.at(i));
            // }

            isVariableSynced = true;

            doc.clear();

            url = "https://" + _host + endpoint;
            DEBUG_PRINTLN(url);

            // if (!init())
            // {
            //     return false;
            // }

            http.begin(client, url);

            http.addHeader("Content-Type", "application/json");

            JsonDocument doc;

            doc["serialNumber"] = this->serialNumber;

            loadVariables();

            for (int i = 0; i < variables.size(); i++)
            {
                doc["variables"][variables[i].first] = variables[i].second;
            }

            // doc["variables"]["HOST_SERVER"] = "192.168.1.1";

            String reqStr = "";
            serializeJson(doc, reqStr);
            DEBUG_PRINTLN(reqStr);
            DEBUG_PRINTLN("Length: " + String(reqStr.length()));

            uint16_t timestamp = millis();
            int httpCode = http.sendRequest("POST", reqStr);
            DEBUG_PRINTLN("Time taken: " + String(millis() - timestamp) + "ms");
            DEBUG_PRINTLN("HTTP Code: " + String(httpCode));
            String response = http.getString();
            DEBUG_PRINTLN("HTTP Response: " + response);

            http.end();
            // client.flush();
            // client.clearWriteError();
            // client.stop();

            doc.clear();

            if (httpCode == 200)
            {
                deserializeJson(doc, response);

                for (int i = 0; i < variables.size(); i++)
                {
                    DEBUG_PRINTLN("Variable " + (String)doc[variables[i].first].as<bool>());
                    if (doc[variables[i].first].as<bool>() == true)
                    {
                        DEBUG_PRINTLN("Variable " + variables[i].first + " synced");
                        variables.erase(variables.begin() + i);
                        i = i - 1;
                    }
                }

                for (int i = 0; i < variables.size(); i++)
                {
                    DEBUG_PRINTLN("Variable " + variables[i].first + " not synced");
                }

                // client.flush();
                // client.clearWriteError();
                // client.stop();

                if (variables.size() == 0)
                {
                    isVariableSynced = true;
                    loadVariables();
                    timestamp = millis();
                    if (!flag)
                    {
                        isVariableGotUpdated = true;
                    }

                    return true;
                }
            }
            loadVariables();
            timestamp = millis();

            return false;
        }
    }

    return false;
}

/**
 * @brief Checks if a synchronization should occur based on the specified interval.
 *
 * This function compares the current timestamp with the interval in seconds multiplied by the conversion factor from milliseconds to seconds.
 * If the difference between the current timestamp and the stored timestamp is greater than or equal to the interval, a synchronization is triggered.
 *
 * @param intervalSeconds The interval in seconds for synchronization.
 * @return True if a synchronization should occur, false otherwise.
 */
bool ConfiguratorMKR::syncEvery(unsigned long long intervalSeconds)
{
    if (millis() - timestamp >= intervalSeconds * MS_TO_S)
    {
        timestamp = millis();
        return true;
    }

    return false;
}

/**
 * @brief Loads variables from a JSON file.
 *
 * This function checks if the file "/config/variables.json" exists and reads its contents.
 * The variables are then parsed and stored in the 'variables' vector.
 *
 * @return True if the variables were successfully loaded, false otherwise.
 */
bool ConfiguratorMKR::loadVariables()
{
    // check if the file exists

    // clear the variables
    variables.clear();

    if (StorageHandler::checkFile("/config/variables.json"))
    {
        std::vector<String> *temp = StorageHandler::readFile("/config/variables.json");
        for (int i = 0; i < temp->size(); i++)
        {
            std::pair<String, String> variable = this->_parseVariable(temp->at(i));
            variables.push_back(variable);
        }
        delete temp;
        // delete temp;

        return true;
    }

    return false;
}

bool ConfiguratorMKR::loadSerialNumber()
{
    // check if the file exists

    // clear the variables
    if (StorageHandler::checkFile("/config/serialNumber.json"))
    {
        std::vector<String> *temp = StorageHandler::readFile("/config/serialNumber.json");
        serialNumber = temp->at(0);
        delete temp;
        return true;
    }

    return false;
}

String ConfiguratorMKR::getSerialNumber()
{
    return serialNumber;
}

/**
 * Adds a variable to the configuration file.
 *
 * @param key The key of the variable.
 * @param value The value of the variable.
 * @return True if the variable was successfully added, false otherwise.
 */
bool ConfiguratorMKR::addVariable(String key, String value)
{
    // check if the file exists and there is a key with the same name
    if (StorageHandler::checkFile("/config/variables.json"))
    {
        std::vector<String> *temp = StorageHandler::readFile("/config/variables.json");
        for (int i = 0; i < temp->size(); i++)
        {
            if (temp->at(i).startsWith(key))
            {
                delete temp;
                return false;
            }
        }

        // add the variable to the file
        StorageHandler::appendFile("/config/variables.json", (key + ":" + value).c_str());
        loadVariables();
        delete temp;
        return true;
    }
    return false;
}

bool ConfiguratorMKR::writeSerialNumber(String value)
{
    // check if the file exists and there is a key with the same name
    if (StorageHandler::checkFile("/config/serialNumber.json"))
    {
        // std::vector<String> *temp = StorageHandler::readFile("/config/serialNumber.json");
        // for (int i = 0; i < temp->size(); i++)
        // {
        //     if (temp->at(i).startsWith(value))
        //     {
        //         delete temp;
        //         return false;
        //     }
        // }

        // add the variable to the file
        StorageHandler::writeFile("/config/serialNumber.json", value.c_str());
        // delete temp;
        return true;
    }
    return false;
}

/**
 * Parses a variable string into a pair of strings.
 *
 * @param variable The variable string to be parsed.
 * @return A pair of strings representing the parsed variable.
 */
std::pair<String, String> ConfiguratorMKR::_parseVariable(String variable)
{
    int index = variable.indexOf(":");
    return std::make_pair(variable.substring(0, index), variable.substring(index + 1));
}

/**
 * @brief Deparses a variable into a string representation.
 *
 * This function takes a pair of strings representing a variable and returns a string representation of the variable.
 * The variable is deparsed by concatenating the first string with a colon separator and the second string.
 *
 * @param variable The pair of strings representing the variable to be deparsed.
 * @return The string representation of the deparsed variable.
 */
String ConfiguratorMKR::_deparseVariable(std::pair<String, String> variable)
{
    return variable.first + ":" + variable.second;
}

/**
 * @brief Removes a variable from the configuration file.
 *
 * This function checks if the configuration file exists and if there is a variable with the specified key. If the variable is found, it is removed from the file. The file is then flushed and the updated file is written. Finally, the variables are reloaded.
 *
 * @param key The key of the variable to be removed.
 * @return True if the variable was successfully removed, false otherwise.
 */
bool ConfiguratorMKR::removeVariable(String key)
{
    // check if the file exists and there is a key with the same name
    if (StorageHandler::checkFile("/config/variables.json"))
    {
        std::vector<String> *temp = StorageHandler::readFile("/config/variables.json");
        int currentSize = temp->size();
        for (int i = 0; i < temp->size(); i++)
        {
            if (temp->at(i).startsWith(key))
            {
                // remove the variable from the file
                temp->erase(temp->begin() + i);
            }
        }
        if (currentSize == temp->size())
        {
            delete temp;
            return false;
        }

        // flush the file
        StorageHandler::cleanData("/config/variables.json");

        // write the new file
        for (int i = 0; i < temp->size(); i++)
        {
            StorageHandler::appendFile("/config/variables.json", temp->at(i).c_str());
        }
        loadVariables();
        delete temp;
        return true;
    }

    return false;
}

/**
 * @brief Clears the variables stored in the "/config/variables.json" file.
 *
 * @return true if the variables were successfully cleared, false otherwise.
 */
bool ConfiguratorMKR::clearVariables()
{
    // check if the file exists
    if (StorageHandler::checkFile("/config/variables.json"))
    {
        // flush the file
        StorageHandler::cleanData("/config/variables.json");
        return true;
    }

    return false;
}

/**
 * Retrieves the value of a variable based on the provided key.
 *
 * @param key The key of the variable to retrieve.
 * @return The value of the variable associated with the provided key. If no variable is found, an empty string is returned.
 */
String ConfiguratorMKR::getVariable(String key)
{
    for (int i = 0; i < variables.size(); i++)
    {
        if (variables[i].first == key)
        {
            return variables[i].second;
        }
    }

    return "";
}

/**
 * @brief Updates a variable in the configuration file.
 *
 * This function updates the value of a variable in the configuration file.
 * It checks if the file exists and if there is a key with the same name.
 * If the key is found, the variable is updated with the new value.
 * The updated file is then written back to the storage.
 *
 * @param key The key of the variable to update.
 * @param value The new value for the variable.
 * @return True if the variable was successfully updated, false otherwise.
 */
bool ConfiguratorMKR::updateVariable(String key, String value)
{
    // check if the file exists and there is a key with the same name
    if (StorageHandler::checkFile("/config/variables.json"))
    {
        std::vector<String> *temp = StorageHandler::readFile("/config/variables.json");
        for (int i = 0; i < temp->size(); i++)
        {
            if (temp->at(i).startsWith(key))
            {
                // update the variable from the file
                temp->at(i) = key + ":" + value;
            }
        }

        // flush the file
        StorageHandler::cleanData("/config/variables.json");

        // write the new file
        for (int i = 0; i < temp->size(); i++)
        {
            DEBUG_PRINTLN("Writing: " + temp->at(i));
            StorageHandler::appendFile("/config/variables.json", temp->at(i).c_str());
        }
        loadVariables();
        delete temp;
        return true;
    }

    return false;
}