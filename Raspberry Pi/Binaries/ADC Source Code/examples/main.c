#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include "ADS1263.h"
#include "stdio.h"
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

/// Convert seconds to milliseconds
#define SEC_TO_MS(sec) ((sec)*1000)
/// Convert seconds to microseconds
#define SEC_TO_US(sec) ((sec)*1000000)
/// Convert seconds to nanoseconds
#define SEC_TO_NS(sec) ((sec)*1000000000)

/// Convert nanoseconds to seconds
#define NS_TO_SEC(ns)   ((ns)/1000000000)
/// Convert nanoseconds to milliseconds
#define NS_TO_MS(ns)    ((ns)/1000000)
/// Convert nanoseconds to microseconds
#define NS_TO_US(ns)    ((ns)/1000)
#define REF			5.10		//Modify according to actual voltage
								//external AVDD and AVSS(Default), or internal 2.5V

#define PORT     2000
#define MAXLINE 1024

uint64_t millis()
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    uint64_t ms = SEC_TO_MS((uint64_t)ts.tv_sec) + NS_TO_MS((uint64_t)ts.tv_nsec);
    return ms;
}

void  Handler(int signo)
{
    //System Exit
    printf("\r\n END \r\n");
    DEV_Module_Exit();
    exit(0);
}

int main(void)
{
	char labels_raw[10][100] = {"high_press_raw", "low_press_raw", "lox_raw", "fuel_raw", "lox_fill_raw", "pneumatics_raw", "u", "i", "o", "p"};
	char labels[10][100] = {"high_press", "low_press", "lox", "fuel", "lox_fill", "pneumatics", "a", "b", "c", "d"};
	double a[10] = {2479.88234, 251.14054, 250.66866, 250.91580, 250.90833, 1.0, 1.0, 1.0, 1.0, 1.0};
	double b[10] = {-1396.01010, -272.60412, -280.04629, -278.51658, -277.05566, 0.0, 0.0, 0.0, 0.0, 0.0};
	double c[10] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    UDOUBLE ADC[10];
	double adc_data[10];
	double pressure[10];
	UWORD i;
	FILE *log;
	FILE *log_raw;
	int sockfd;
	char log_dir_raw[100] = "/home/rocket/logs/adc_raw.csv";
	log_raw = fopen(log_dir_raw, "a+");
	char log_dir[100] = "/home/rocket/logs/adc.csv";
	log = fopen(log_dir, "a+");
	int pipe = 0;
	char data[10000];

    // Exception handling:ctrl + c
    signal(SIGINT, Handler);
    DEV_Module_Init();

	ADS1263_SetMode(0);
	if(ADS1263_init_ADC1(ADS1263_38400SPS) == 1) {
		printf("\r\n END \r\n");
		DEV_Module_Exit();
		exit(0);
	}

	uint64_t prev = millis() * 1000000;

	struct sockaddr_in     servaddr;
	// Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        exit(0);
    }

	while(1) {
		uint64_t now = millis();

		for(i = 0; i < 9; i++)
		{
			ADC[i] = ADS1263_GetChannalValue(i);
		}

		if (now - prev > 100) {
			prev = now;
			pipe = 1;
		}

		sprintf(data, "%" PRId64, now);
		fputs(data, log_raw);
		fputs(data, log);

		for(i=0; i<9; i++) {
			if((ADC[i]>>31) == 1) {
				adc_data[i] = REF*2 - ADC[i]/2147483648.0 * REF; //7fffffff + 1
			}
			else {
				adc_data[i] = ADC[i]/2147483647.0 * REF; //7fffffff
			}
			pressure[i] = a[i] * adc_data[i] + b[i] + c[i];
			if (pipe) {
				uint64_t nano_now = now * 1000000;
				//int n = sprintf(data, "%s %s=%.5lf %" PRId64, labels_raw[i], labels_raw[i], adc_data[i], nano_now);
				//sendto(sockfd, data, n, 0, (struct sockaddr*)NULL, sizeof(servaddr));
				int n = sprintf(data, "%s %s=%.5lf %" PRId64, labels[i], labels[i], pressure[i], nano_now);
				sendto(sockfd, data, n, 0, (struct sockaddr*)NULL, sizeof(servaddr));
			}
			sprintf(data, ",%.5lf", adc_data[i]);
			fputs(data, log_raw);
			sprintf(data, ",%.5lf", pressure[i]);
			fputs(data, log);
		}
		data[0] = '\n';
		data[1] = '\0';
		fputs(data, log_raw);
		fputs(data, log);
		pipe = 0;
	}
	return 0;
}
