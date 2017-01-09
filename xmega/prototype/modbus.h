struct askreadregstruct {
        unsigned int firstreg;
        unsigned int regnumber;
};

struct reqreadcoilsstruct {
        unsigned char bytestofollow;
        unsigned char coils[256];
};

struct reqreadwordstruct {
        unsigned char bytestofollow;
        unsigned int registers[127];
};

struct writeregstruct {
        unsigned int regaddress;
        unsigned int regvalue;
};

struct writemulticoilstruct {
        unsigned int firstreg;
        unsigned int regnumber;
        unsigned char bytestofollow;
        unsigned char coils[256];
};

struct writemultiregstruct {
        unsigned int firstreg;
        unsigned int regnumber;
        unsigned char bytestofollow;
        unsigned int registers[127];
};

union pdudataunion {
        struct askreadregstruct askreadregs;
        struct reqreadcoilsstruct reqreadcoils;
        struct reqreadwordstruct reqreadholdings;
        struct writeregstruct writereg;
        struct writemulticoilstruct writemulticoil;
        struct writemultiregstruct writemultireg;
        unsigned int words[128];
        unsigned char bytes[256];
};

struct pduframestruct {
        unsigned char unitid;
        unsigned char fncode;
        union pdudataunion values;
};

struct mbframestruct {
        unsigned int tsid;
        unsigned int protoid;
        unsigned int length;
        struct pduframestruct pdu;
};
