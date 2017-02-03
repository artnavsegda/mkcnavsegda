#include "http.h"
#include "global.h"
#include "settings.h"

unsigned char httpHeader[100] = "HTTP/1.1 200 OK" ;  // HTTP header
const char * httpMimeType;
const char httpMimeTypeHTML[] = "\nContent-type: text/html\n\n" ;              // HTML MIME type
const char httpMimeTypeScript[] = "\nContent-type: application/javascript\n\n" ;           // JS MIME type
const char httpMimeTypeText[] = "\nContent-type: text/plain\n\n" ;           // TEXT MIME type
const char httpMimeTypeCSS[] = "\nContent-type: text/CSS\n\n" ;           // CSS MIME type
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

unsigned int http(static unsigned char *getRequest,static unsigned char *buf2)
{
        unsigned long filesize, no_bytes;
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
                        len += SPI_Ethernet_putString(strstr(buf2,"\r\n\r\n")+4);
                        //len += SPI_Ethernet_putString(buf2);
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
                        setsingleopt(strstr(buf2,"\r\n\r\n")+4);
                        len += SPI_Ethernet_putConstString("Settings written: ");
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
                                webpage[no_bytes] = 0;
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
        return 0;
}