#include "StorageHandler.h"

StorageHandler::StorageHandler()
{
}

StorageHandler::~StorageHandler()
{
}

/**
 * @brief Checks if a directory exists.
 * 
 * This function checks if the specified directory exists in the file system.
 * 
 * @param path The path of the directory to check.
 * @return True if the directory exists, false otherwise.
 */
bool StorageHandler::checkDir(const char *path)
{
    if (LittleFS.exists(path))
    {
        DEBUG_PRINTF("Dir %s exists\r\n", path);
        return true;
    }
    else
    {
        DEBUG_PRINTF("Dir %s does not exist\r\n", path);
        return false;
    }
}

/**
 * @brief Checks if a file exists at the specified path.
 * 
 * @param path The path of the file to check.
 * @return true if the file exists, false otherwise.
 */
bool StorageHandler::checkFile(const char *path)
{
    if (LittleFS.exists(path))
    {
        DEBUG_PRINTF("File %s exists\r\n", path);
        return true;
    }
    else
    {
        DEBUG_PRINTF("File %s does not exist\r\n", path);
        return false;
    }
}

/**
 * @brief Tests file input/output functionality.
 * 
 * This function tests the ability to read and write files using the LittleFS file system.
 * It opens the specified file for writing, writes a large amount of data to it, and then
 * opens the same file for reading to verify that the data was written correctly.
 * 
 * @param path The path of the file to test.
 * @return True if the file IO test was successful, false otherwise.
 */
bool StorageHandler::testFileIO(const char *path)
{
    DEBUG_PRINTF("Test file IO: %s\r\n", path);

    static uint8_t buf[512];
    size_t len = 0;
    File file = LittleFS.open(path, FILE_WRITE);
    if (!file)
    {
        DEBUG_PRINTLN("- failed to open file for writing");
        return false;
    }

    size_t i;
    DEBUG_PRINTLN("- writing");
    uint32_t start = millis();
    for (i = 0; i < 2048; i++)
    {
        if ((i & 0x001F) == 0x001F)
        {
            DEBUG_PRINT(".");
        }
        file.write(buf, 512);
    }
    DEBUG_PRINTLN("");
    uint32_t end = millis() - start;
    DEBUG_PRINTF("- %u bytes written in ", 2048 * 512);
    DEBUG_PRINTF("%u ms\r\n", end);
    file.close();

    file = LittleFS.open(path);
    start = millis();
    end = start;
    i = 0;
    if (file && !file.isDirectory())
    {
        len = file.size();
        size_t flen = len;
        start = millis();
        DEBUG_PRINT("- reading");
        while (len)
        {
            size_t toRead = len;
            if (toRead > 512)
            {
                toRead = 512;
            }
            file.read(buf, toRead);
            if ((i++ & 0x001F) == 0x001F)
            {
                Serial.print(".");
            }
            len -= toRead;
        }
        DEBUG_PRINTLN("");
        end = millis() - start;
        DEBUG_PRINTF("- %u bytes read in ", flen);
        DEBUG_PRINTF("%u ms\r\n", end);
        file.close();
    }
    else
    {

        DEBUG_PRINTLN("- failed to open file for reading");
        return false;
    }
    return true;
}

/**
 * @brief Deletes a file from the storage.
 * 
 * This function deletes the file specified by the given path from the storage.
 * 
 * @param path The path of the file to be deleted.
 */
void StorageHandler::deleteFile(const char *path)
{
    DEBUG_PRINTF("Deleting file: %s\r\n", path);
    if (LittleFS.remove(path))
    {
        DEBUG_PRINTLN("- file deleted");
    }
    else
    {
        DEBUG_PRINTLN("- delete failed");
    }
}

/**
 * Renames a file from the specified path to a new path.
 *
 * @param path1 The current path of the file.
 * @param path2 The new path to rename the file to.
 */
void StorageHandler::renameFile(const char *path1, const char *path2)
{
    DEBUG_PRINTF("Renaming file %s ", path1);
    DEBUG_PRINTF("to %s\r\n", path2);
    if (LittleFS.rename(path1, path2))
    {
        DEBUG_PRINTLN("- file renamed");
    }
    else
    {
        DEBUG_PRINTLN("- rename failed");
    }
}

/**
 * Appends a message to a file.
 *
 * @param path The path of the file to append to.
 * @param message The message to append to the file.
 */
void StorageHandler::appendFile(const char *path, const char *message)
{
    DEBUG_PRINTF("Appending to file: %s\r\n", path);

    File file = LittleFS.open(path, FILE_APPEND);
    if (!file)
    {
        DEBUG_PRINTLN("- failed to open file for appending");
        return;
    }
    if (file.println(message))
    {
        DEBUG_PRINTLN("- message appended");
    }
    else
    {
        DEBUG_PRINTLN("- append failed");
    }
    file.close();
}

/**
 * @brief Writes the given message to a file at the specified path.
 * 
 * @param path The path of the file to write.
 * @param message The message to write to the file.
 */
void StorageHandler::writeFile(const char *path, const char *message)
{
    DEBUG_PRINTF("Writing file: %s\r\n", path);

    File file = LittleFS.open(path, FILE_WRITE);
    if (!file)
    {
        DEBUG_PRINTLN("- failed to open file for writing");
        return;
    }
    if (file.println(message))
    {
        DEBUG_PRINTLN("- file written");
    }
    else
    {
        DEBUG_PRINTLN("- write failed");
    }
    file.close();
}

/**
 * Begins the storage handler.
 * 
 * This function mounts the LittleFS file system and checks if the mount operation was successful.
 * If the mount operation fails, it prints an error message.
 * 
 * @note This function should be called before accessing any files in the file system.
 */
void StorageHandler::begin()
{
    if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED))
    {
        DEBUG_PRINTLN("LittleFS Mount Failed");
        return;
    }
    DEBUG_PRINTLN("LittleFS Mount Success");
}

/**
 * @brief Reads a file from the specified path and returns its contents as a vector of strings.
 * 
 * @param path The path of the file to be read.
 * @return A pointer to a vector of strings containing the contents of the file. If the file cannot be opened or is a directory, an empty vector is returned.
 */
vector<String> *StorageHandler::readFile(const char *path)
{
    DEBUG_PRINTF("Reading file: %s\r\n", path);

    File file = LittleFS.open(path);
    if (!file || file.isDirectory())
    {
        DEBUG_PRINTLN("- failed to open file for reading");
        return new vector<String>(); // Fix: Return an empty vector
    }

    DEBUG_PRINTLN("- read from file:");
    vector<String> *data = new vector<String>();
    int i = 0;
    while (file.available())
    {
        String temp = file.readStringUntil('\n');
        temp.remove(int(temp.indexOf('\r')));
        data->push_back(temp);
        DEBUG_PRINTLN(data->at(i++));
    }
    file.close();

    // free(data);
    return data;
}

/**
 * @brief Removes a directory from the file system.
 * 
 * This function removes the directory specified by the given path from the file system.
 * 
 * @param path The path of the directory to be removed.
 */
void StorageHandler::removeDir(const char *path)
{
    DEBUG_PRINTF("Removing Dir: %s\r\n", path);
    if (LittleFS.rmdir(path))
    {
        DEBUG_PRINTLN("Dir removed");
    }
    else
    {
        DEBUG_PRINTLN("rmdir failed");
    }
}

/**
 * Creates a directory at the specified path.
 *
 * @param path The path of the directory to be created.
 */
void StorageHandler::createDir(const char *path)
{
    DEBUG_PRINTF("Creating Dir: %s\r\n", path);
    if (LittleFS.mkdir(path))
    {
        DEBUG_PRINTLN("Dir created");
    }
    else
    {
        DEBUG_PRINTLN("mkdir failed");
    }
}

/**
 * Creates a file at the specified path.
 * 
 * @param path The path of the file to be created.
 * 
 * @return void
 */
void StorageHandler::createFile(const char *path)
{
    DEBUG_PRINTF("Creating file: %s\r\n", path);
    if (checkFile(path))
    {
        DEBUG_PRINTLN("- file already exists");
        return;
    }
    File file = LittleFS.open(path, FILE_WRITE);
    if (!file)
    {
        DEBUG_PRINTLN("- failed to create file");
        return;
    }
    DEBUG_PRINTLN("- file created");
    file.close();
}

/**
 * Lists the contents of a directory recursively.
 * 
 * @param dirname The name of the directory to list.
 * @param levels The number of levels to recursively list subdirectories.
 */
void StorageHandler::listDir(const char *dirname, uint8_t levels)
{
    DEBUG_PRINTF("Listing directory: %s\r\n", dirname);

    File root = LittleFS.open(dirname);
    if (!root)
    {
        DEBUG_PRINTLN("- failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        DEBUG_PRINTLN(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            DEBUG_PRINT("  DIR : ");
            DEBUG_PRINTLN(file.name());
            if (levels)
            {
                listDir(file.path(), levels - 1);
            }
        }
        else
        {
            DEBUG_PRINT("  FILE: ");
            DEBUG_PRINT(file.name());
            DEBUG_PRINT("\tSIZE: ");
            DEBUG_PRINTLN(file.size());
        }
        file = root.openNextFile();
    }
}

/**
 * @brief Cleans the data in the specified file.
 * 
 * This function opens the file at the given path and clears its contents.
 * If the file cannot be opened, an error message is printed and the function returns.
 * After cleaning the data, the file is closed.
 * 
 * @param path The path of the file to clean.
 */
void StorageHandler::cleanData(const char *path)
{
    DEBUG_PRINTF("Cleaning data: %s\r\n", path);
    File file = LittleFS.open(path, FILE_WRITE);
    if (!file)
    {
        DEBUG_PRINTLN("- failed to open file for cleaning");
        return;
    }
    file.print("");

    DEBUG_PRINTLN("- data cleaned");

    file.close();
}