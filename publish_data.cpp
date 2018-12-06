#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"
//#include <iomanip>
//#include <wiringPi.h>

using  namespace std;

#define ADDRESS     "tcp://mqtt.thingspeak.com:1883"
#define CLIENTID    "ExampleClientPub"
#define TOPIC       "channels/620356/publish/T8BTJRYYIMA4AA9P"
//#define PAYLOAD     "field1=20&field2=10"
#define QOS         0
#define TIMEOUT     10000L

void setup(int uart0_filestream ){
    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD ;
    options.c_iflag = IGNPAR;
    options.c_lflag = 0;
    options.c_oflag = 0;
    options.c_cc[VMIN] = 22;
    options.c_cc[VTIME] = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
}

int main() {
	
    int fd = -1;

    fd = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY);
    if (fd == -1) {
        cout << "no uart" << endl;

    } else {
        cout << " uart readable" << endl;
        setup(fd);
    }
    usleep(10000);
    char rx_buf[22];
    char *payload;

     // endless loop for MQTT connection and data transfer
     while(1) {
  
	 // connect MQTT
	MQTTClient client;
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	conn_opts.username = "test";
	conn_opts.password = "7Z95VND9RMGN8W4N";
	MQTTClient_message pubmsg = MQTTClient_message_initializer;
	MQTTClient_deliveryToken token;
	int rc;
	MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;

	if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
	{
		printf("Failed to connect, return code %d\n", rc);
		exit(-1);
	}
	     
        pubmsg.qos = QOS;
        pubmsg.retained = 0;

    int n = read(fd, (void*) rx_buf, 22);
    if(n != -1){
        for(int i = 0; i<22; i++) {
            //cout << (int)rx_buf[i] << endl;
        }
        double sensor = (double)rx_buf[19] * 256 + (double)rx_buf[20];
        //cout << sensor << endl;
        sensor = sensor * 1.17; // spaending i mV efter spaendingsdeler
        sensor = (4 * sensor) / 3; // spaending i mV for maaler  aka æ
        sensor = sensor / 3.2;  // antal steps af 2 cm
        sensor = sensor * 2;    // afstand i cm?
        cout << endl << "Der måles: " << sensor << endl << endl;
        if(sensor >= 300){
            cout << "P-plads ledig" << endl;
            payload = "field1=0&field2=-1";
            pubmsg.payload = payload;
            pubmsg.payloadlen = strlen(payload);
        }
        if(sensor < 300){
            cout << "P-plads optaget" << endl;
	    payload = "field1=1&field2=-1";
	    pubmsg.payload = payload;
 	    pubmsg.payloadlen = strlen(payload);
        }
        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
        printf("Waiting for up to %d seconds for publication of %s\n"
               "on topic %s for client with ClientID: %s\n",
               (int)(TIMEOUT/1000), payload, TOPIC, CLIENTID);
               
        rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
        //printf("Message with delivery token %d delivered\n", token);
        } else{ 
	    cout << "ERROR, no data received on uart" << endl; 
    }  
	if (rc == 0){
	cout << "successfully sent data to thingspeak servers" << endl;
	} else{
	printf("ERROR, data transmission to thingspeak failed.\nReturned with code %d\n", rc);
	}
	
	printf("\nWaiting 15 seconds...");
	flush;
        usleep(15000000); // sleep 15 seconds since thingspeak only accepts data every 15'th second.
    }
}
