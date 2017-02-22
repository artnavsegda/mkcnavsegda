#include "http.h"
#include "global.h"
#include "settings.h"
#include "bswap.h"
#include "average.h"

unsigned char httpHeader[100] = "HTTP/1.1 200 OK" ;  // HTTP header
const char * httpMimeType;
const char httpMimeTypeHTML[] = "\r\nContent-type: text/html\r\n\r\n" ;              // HTML MIME type
const char httpMimeTypeScript[] = "\r\nContent-type: application/javascript\r\n\r\n" ;           // JS MIME type
const char httpMimeTypeText[] = "\r\nContent-type: text/plain\r\n\r\n" ;           // TEXT MIME type
const char httpMimeTypeCSS[] = "\r\nContent-type: text/CSS\r\n\r\n" ;           // CSS MIME type
char indexpage[] = "/index.htm";

void WebHandler(char c)
{
        SPI_Ethernet_putByte(c);
        len++;
}

void setsingleopt(char *equation)
{
        char *parname;
        char *parvalue;
        parname = strtok(equation,"=");
        parvalue = strtok(0,"=");
        setmyopt(parname,parvalue);
}

void setmultiopt(char *multistring)
{
        char *dispatch[100];
        int amount;
        int i = 0;
        dispatch[i] = strtok(multistring,"&");
        while(dispatch[i] != 0)
                dispatch[++i] = strtok(0, "&");
        amount = i;
        for (i=0;i<amount;i++)
                setsingleopt(dispatch[i]);
}

unsigned int http(static unsigned char *getRequest,static unsigned char *buf2)
{
	char *reqstring = 0;
        int i;
        unsigned long filesize, no_bytes;
        if (strchr(getRequest,'?') != 0)
        {
                strtok(getRequest,"?");
                reqstring = strtok(0,"\r\n");
                //buf2 = reqstring+strlen(reqstring)+2;//maybe not today but who knows
        }
        if (strcmp(getRequest,"/")==0)
                 getRequest = indexpage;
        if (sd_init == 0)
        {
                if (strcmp("/dir.htm",getRequest)==0)
                {
                        sprintf(httpHeader,"HTTP/1.1 %d OK",(int)200);
                        len = SPI_Ethernet_putString(httpHeader);
                        len += SPI_Ethernet_putConstString(httpMimeTypeHTML);
                        len += SPI_Ethernet_putConstString("<!doctype html><html><head><title>File list</title></head><body><pre>");
                        Mmc_Fat_Dir(WebHandler);
                        len += SPI_Ethernet_putConstString("</pre></body></html>");
                }
                else if (strcmp("/special",getRequest)==0)
                {
                        sprintf(httpHeader,"HTTP/1.1 %d OK",(int)200);
                        len = SPI_Ethernet_putString(httpHeader);
                        len += SPI_Ethernet_putConstString(httpMimeTypeText);
                        len += SPI_Ethernet_putConstString("You're so special: ");
                        //len += SPI_Ethernet_putString(strstr(buf2,"\r\n\r\n")+4);
                        len += SPI_Ethernet_putString(buf2);
                }
                else if (strcmp("/getdata",getRequest)==0)
                {
                        sprintf(httpHeader,"HTTP/1.1 %d OK",(int)200);
                        len = SPI_Ethernet_putString(httpHeader);
                        len += SPI_Ethernet_putConstString("\r\nCache-Control: no-cache\r\nAccess-Control-Allow-Origin: *");
                        len += SPI_Ethernet_putConstString(httpMimeTypeText);
                        if (strcmp("raw",strstr(buf2,"\r\n\r\n")+4)==0)
                                PrintOut(WebHandler, "%d", BSWAP_16(result));
                        else if (strcmp("x16",strstr(buf2,"\r\n\r\n")+4)==0)
                                PrintOut(WebHandler, "%d", oversample(&firststage,64)/64);
                }
                else if (strcmp("/getraw",getRequest)==0)
                {
                        sprintf(httpHeader,"HTTP/1.1 %d OK",(int)200);
                        len = SPI_Ethernet_putString(httpHeader);
                        len += SPI_Ethernet_putConstString("\r\nCache-Control: no-cache\r\nAccess-Control-Allow-Origin: *");
                        len += SPI_Ethernet_putConstString(httpMimeTypeText);
                        PrintOut(WebHandler, "%u", BSWAP_16(result));
                }
                else if (strcmp("/getrawrun",getRequest)==0)
                {
                        sprintf(httpHeader,"HTTP/1.1 %d OK",(int)200);
                        len = SPI_Ethernet_putString(httpHeader);
                        len += SPI_Ethernet_putConstString("\r\nCache-Control: no-cache\r\nAccess-Control-Allow-Origin: *");
                        len += SPI_Ethernet_putConstString(httpMimeTypeText);
                        len += SPI_Ethernet_putConstString("[ ");
                        for (i=1; i<10; i++)
                                PrintOut(WebHandler, "%u, ", wayback(&burnstage,i));
                        PrintOut(WebHandler, "%u ]", wayback(&burnstage,10));
                }
                else if (strcmp("/getx16",getRequest)==0)
                {
                        sprintf(httpHeader,"HTTP/1.1 %d OK",(int)200);
                        len = SPI_Ethernet_putString(httpHeader);
                        len += SPI_Ethernet_putConstString("\r\nCache-Control: no-cache\r\nAccess-Control-Allow-Origin: *");
                        len += SPI_Ethernet_putConstString(httpMimeTypeText);
                        PrintOut(WebHandler, "%u", oversample(&firststage,64)/64);
                }
                else if (strcmp("/getrun",getRequest)==0)
                {
                        sprintf(httpHeader,"HTTP/1.1 %d OK",(int)200);
                        len = SPI_Ethernet_putString(httpHeader);
                        len += SPI_Ethernet_putConstString("\r\nCache-Control: no-cache\r\nAccess-Control-Allow-Origin: *");
                        len += SPI_Ethernet_putConstString(httpMimeTypeText);
                        len += SPI_Ethernet_putConstString("[ ");
                        for (i=1; i<100; i++)
                                PrintOut(WebHandler, "%u, ", wayback(&secondstage,i));
                        PrintOut(WebHandler, "%u ]", wayback(&secondstage,100));
                }
                else if (strcmp("/getsample",getRequest)==0)
                {
                        sprintf(httpHeader,"HTTP/1.1 %d OK",(int)200);
                        len = SPI_Ethernet_putString(httpHeader);
                        len += SPI_Ethernet_putConstString("\r\nCache-Control: no-cache\r\nAccess-Control-Allow-Origin: *");
                        len += SPI_Ethernet_putConstString(httpMimeTypeText);
                        len += SPI_Ethernet_putConstString("[ ");
                        for (i=1; i<100; i++)
                                PrintOut(WebHandler, "%u, ", wayback(&firststage,i));
                        PrintOut(WebHandler, "%u ]", wayback(&firststage,100));
                }
                else if (strcmp("/getopt",getRequest)==0)
                {
                        sprintf(httpHeader,"HTTP/1.1 %d OK",(int)200);
                        len = SPI_Ethernet_putString(httpHeader);
                        len += SPI_Ethernet_putConstString(httpMimeTypeText);
                        len += SPI_Ethernet_putString(getmyopt(strstr(buf2,"\r\n\r\n")+4));
                }
                else if (strcmp("/setopt",getRequest)==0)
                {
                        sprintf(httpHeader,"HTTP/1.1 %d OK",(int)200);
                        len = SPI_Ethernet_putString(httpHeader);
                        len += SPI_Ethernet_putConstString(httpMimeTypeText);
                        len += SPI_Ethernet_putString(buf2);
                        setmultiopt(strstr(buf2,"\r\n\r\n")+4);
                        breakopt();
                }
                else
                {
                        if (Mmc_Fat_Assign(&getRequest[1],0) == 1)
                        {
                                sprintf(httpHeader,"HTTP/1.1 %d OK",(int)200);
                                Mmc_Fat_Reset(&filesize);
                                if (filesize > 1400)
                                        filesize = 1400;
                                no_bytes = Mmc_Fat_ReadN(webpage, filesize);
                                webpage[no_bytes] = '\0';
                                if (strcmp(strchr(getRequest,'.'),".htm")==0)
                                        httpMimeType = httpMimeTypeHTML;
                                else if (strcmp(strchr(getRequest,'.'),".js")==0)
                                        httpMimeType = httpMimeTypeScript;
                                else if (strcmp(strchr(getRequest,'.'),".txt")==0)
                                        httpMimeType = httpMimeTypeText;
                                else if (strcmp(strchr(getRequest,'.'),".css")==0)
                                        httpMimeType = httpMimeTypeCSS;
                                Mmc_Fat_Close();
                        }
                        else
                        {
                                sprintf(httpHeader,"HTTP/1.1 %d Not Found",(int)404);
                                httpMimeType = httpMimeTypeHTML;
                                sprintf(webpage,"<!doctype html><html><head><title>404 Not Found</title></head><body><p>%s not found</p></body></html>",getRequest);
                        }
                        len = SPI_Ethernet_putString(httpHeader);
                        len += SPI_Ethernet_putConstString(httpMimeType);
                        len += SPI_Ethernet_putString(webpage);
                }
                return len;
        }
        else
        {
                sprintf(httpHeader,"HTTP/1.1 %d OK",(int)200);
                httpMimeType = httpMimeTypeHTML;
                sprintf(webpage,"<!doctype html><html><head><title>Not avalible</title></head><body><p>SD card inaccesible</p></body></html>",getRequest);
                len = SPI_Ethernet_putString(httpHeader);
                len += SPI_Ethernet_putConstString(httpMimeType);
                len += SPI_Ethernet_putString(webpage);
                return len;
        }
        return 0;
}