#include <Arduino.h>
#include <FS.h>
#include <errno.h>

int loadFileAsString(char *fileName, char *buffer, int bufferSize)
{
    memset(buffer, 0, bufferSize);

    Dir dir = SPIFFS.openDir("/");

    Serial.printf("Reading dir:\n");

    while (dir.next())
    {
        Serial.println("\"" + dir.fileName() + "\" : " +
                       dir.fileSize() + " bytes");
    }
    fs::File f;
    f = SPIFFS.open(fileName, "r");

    int res = f.read((uint8_t *)buffer, bufferSize - 1);
    if(res<0)
    {
        Serial.printf("Error reading file \"%s\" : %d (%s)\n", 
        fileName, errno, strerror(errno));
    }
    // Serial.printf("%d bytes read: \"%s\"\n", res, buffer);
    
    f.close();

    return (res);
}
