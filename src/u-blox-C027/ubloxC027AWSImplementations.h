#ifndef AWSUBLOXIMPLEMENTATIONS_H_
#define AWSUBLOXIMPLEMENTATIONS_H_

#include "DeviceIndependentInterfaces.h"
/* FixMe
 application.h is Spark's standard library. Define TCPClient. */
// FixMe what we have to include on uBlox C027? #include <application.h>

/* HttpClient implementation to be used on the uBlox C027 device. */
class uBloxC027HttpClient: public IHttpClient {
    TCPClient client;
public:
    uBloxC027HttpClient();
    /* Send http request and return the response. */
    char* send(const char *request, const char* serverUrl, int port);
    /* Returns false. Client uses raw http/https. */
    bool usesCurl(void);
};

class uBloxC027DateTimeProvider: public IDateTimeProvider {
    /* The time as a cstring in yyyyMMddHHmmss format. Is written to within and
     * returned by getDateTime(). */
    char dateTime[15];
public:
    uBloxC027DateTimeProvider();
    /* Retrieve the current GMT date and time in yyyyMMddHHmmss format. */
    const char* getDateTime(void);
    /* FixMe what about uBlox C027?
    Returns false because Spark has it's own mechanism for syncing that does
     * not require an argument. */
    bool syncTakesArg(void);
    /* FixMe what about uBlox C027?
    Synchronizes Spark's date and time with Spark's servers. The dateTime
     * argument is ignored. */
    void sync(const char* dateTime);
};
#endif /* AWSUBLOXIMPLEMENTATIONS_H_ */
