#define NTP_TIME_OFFSET 2208988800

struct ntptime {
        unsigned long timeseconds;
        unsigned long timefraction;
};

struct ntpframestruct {
        unsigned char leapvermode;
        unsigned char stratumlevel;
        unsigned char poll;
        unsigned char precision;
        unsigned long rootdelay;
        unsigned long driftrate;
        unsigned long referenceid;
        struct ntptime reference;
        struct ntptime origin;
        struct ntptime receive;
        struct ntptime transmit;
};

void ntp_recieve(struct ntpframestruct *myframe);
void ntp_send(void);
void PrintHandler(char c);
