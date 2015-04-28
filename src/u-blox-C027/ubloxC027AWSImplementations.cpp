#include <stdio.h>
/* FixMe application.h is Spark's standard library. Defines the Arduino String
 * object, the Arduino delay() procedure, and the Spark TCPClient. */
// FixMe #include <application.h>
// FixMe #include "SparkAWSImplementations.h"
#include "DeviceIndependentInterfaces.h"


int delayTime = 500;

uBloxC027HttpClient::uBloxC027HttpClient() {
}

char* uBloxC027HttpClient::send(const char* request, const char* serverUrl,
        int port) {
    /* Arduino String to build the response with. */
    String responseBuilder = "";
    if (client.connect(serverUrl, port)) {
        /* Send the requests */
        client.println(request);
        client.println();
        /* Read the request into responseBuilder. */
        delay(delayTime);
        while (client.available()) {
            char c = client.read();
            responseBuilder.concat(c);
        }
        client.stop();
    } else {
        client.stop();
        /* Error connecting. */
        return 0;
    }
    /* Copy responseBuilder into char* */
    int len = responseBuilder.length();
    char* response = new char[len + 1]();
    responseBuilder.toCharArray(response, len + 1);
    return response;
}
bool uBloxC027HttpClient::usesCurl() {
    /* Does not use curl command. */
    return false;
}

uBloxC027DateTimeProvider::uBloxC027DateTimeProvider() {
    /* These are initialized to defaults. The defaults will cause the request
     * to fail, but the AWS Services response will contain the correct time
     * which will be fed to sync(). */
    year = 2000;
    month = 1;
    day = 1;
    hour = 0;
    minute = 0;
    second = 0;
    /* Never synced so syncTime is 0. */
    syncedTime = 0;
}

const char* uBloxC027DateTimeProvider::getDateTime() {
    const int secondsPerMinute = 60;
    const int secondsPerHour = secondsPerMinute * 60;
    const long int secondsPerDay = secondsPerHour * 24;
    /* Determine how many seconds have passed since the last sync, break those
     * seconds down into days/hours/minutes/seconds, then form the new time
     * string. */
    long int newSecond = ((millis() - syncedTime) / 1000) + second
            + (secondsPerMinute * minute) + (secondsPerHour * hour)
            + (secondsPerDay * day);
    int newDay = newSecond / secondsPerDay;
    newSecond = newSecond % secondsPerDay;
    int newHour = newSecond / secondsPerHour;
    newSecond = newSecond % secondsPerHour;
    int newMinute = newSecond / secondsPerMinute;
    newSecond = newSecond % secondsPerMinute;

    /* Without cumbersome data about how many days a month we cannot factor the
     * extra time into more months. AWS Services give different errors that do
     * not provide us with the current time if the date is invalid rather than
     * innacurate. However, going past the number of days in a month (i.e
     * giving 30 for February) is valid, so long as the days does not exceed
     *  31. */
    newDay = newDay > 31 ? 31 : newDay;

    /* Copy the values to dateTime and return it. */
    sprintf(dateTime, "%.4d%.2d%.2d%.2d%.2d%.2d", year, month, newDay, newHour,
            newMinute, (int) newSecond);
    return dateTime;
}

uBloxC027DateTimeProvider::uBloxC027DateTimeProvider() {
    /* FixMe do we actually sync on startup on uBlox C027?
    No need to sync, spark sychronizes time on startup. */
}

const char* uBloxC027DateTimeProvider::getDateTime() {
    /* Fixme what happens before time sync with GPS or mobile network happens?
    Spark provides the current time, so all we have to do is format it. */
    sprintf(dateTime, "%.4d%.2d%.2d%.2d%.2d%.2d", Time.year(), Time.month(),
            Time.day(), Time.hour(), Time.minute(), Time.second());
    return dateTime;
}

bool uBloxC027DateTimeProvider::syncTakesArg(void) {
    return false;
}

void uBloxC027DateTimeProvider::sync(const char* dateTime) {
    /* FixMe check what happens on uBlox C027 ?
     Use Spark's servers to synchronize current time. */
    uBloxC027.syncTime();
}