#ifndef WifiModuleUtils.h
#define WifiModuleUtils.h

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

#include "PlantsUtils.h"
#include "Arduino.h"


class WifiModule {
    public:
        WifiModule(String SSID, String Password, String TCPServer, String TCPPort);
        void initialize();
        void makeTCPRequest(String body);
        String printResponse(long int timeout);
        String constructAddReq(struct_plante Plant);
        String constructUpdateReq(struct_plante Plant);
    private:
        String _SSID;
        String _Password;
        String _WifiConnectionString;
        String _TCPConnectionString;
};

#endif
