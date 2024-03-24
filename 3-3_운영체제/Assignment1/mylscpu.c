#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 1024
#define CPUINFO "/proc/cpuinfo"		// /proc/cpuinfo absolute route
#define CPUINFO_VENDOR_ID 2
#define CPUINFO_MODEL_NAME 5
#define CPUINFO_CPU_MHZ 8
#define VENDOR_STRING "Vendor ID:"
#define MODEL_STRING "Model name:"
#define CPU_MHZ_STRING "CPU MHz:"
#define L1D "L1d cache:"
#define L1I "L1i cache:"
#define L2 "L2 cache:"
#define L1D_ROUTE "/sys/devices/system/cpu/cpu0/cache/index0/size"
#define L1I_ROUTE "/sys/devices/system/cpu/cpu0/cache/index1/size"
#define L2_ROUTE "/sys/devices/system/cpu/cpu0/cache/index2/size"


void getL1i(char* buf){
        FILE *fp;

        if((fp = fopen(L1I_ROUTE,"r"))==NULL){
                fprintf(stderr, "fopen error for %s\n", L1D_ROUTE);
                exit(1);
        }
                memset(buf, '\0', BUFFER_SIZE);
                fgets(buf,BUFFER_SIZE,fp);
        return;
}
void getL2(char* buf){
        FILE *fp;

        if((fp = fopen(L2_ROUTE,"r"))==NULL){
                fprintf(stderr, "fopen error for %s\n", L1D_ROUTE);
                exit(1);
        }
                memset(buf, '\0', BUFFER_SIZE);
                fgets(buf,BUFFER_SIZE,fp);
        return;
}
void getL1d(char* buf){
	FILE *fp;

        if((fp = fopen(L1D_ROUTE,"r"))==NULL){
                fprintf(stderr, "fopen error for %s\n", L1D_ROUTE);
                exit(1);
        }
                memset(buf, '\0', BUFFER_SIZE);
                fgets(buf,BUFFER_SIZE,fp);
        return;
}
void getModelName(char* buf){
	FILE *fp;

        if((fp = fopen(CPUINFO,"r"))==NULL){
                fprintf(stderr, "fopen error for %s\n", CPUINFO);
                exit(1);
        }
        int i=0;
        while(i<CPUINFO_MODEL_NAME){
                memset(buf, '\0', BUFFER_SIZE);
                fgets(buf,BUFFER_SIZE,fp);
                i++;
        }
        char *ptr = buf;
        while(*ptr != ':') ptr++;
        while(*ptr == ' ' | *ptr == ':') ptr++;
        strcpy(buf,ptr);
        return;

}
void getCPUMHz(char* buf){
        FILE *fp;

        if((fp = fopen(CPUINFO,"r"))==NULL){
                fprintf(stderr, "fopen error for %s\n", CPUINFO);
                exit(1);
        }
        int i=0;
        while(i<CPUINFO_CPU_MHZ){
                memset(buf, '\0', BUFFER_SIZE);
                fgets(buf,BUFFER_SIZE,fp);
                i++;
        }
        char *ptr = buf;
        while(*ptr != ':') ptr++;
        while(*ptr == ' ' | *ptr == ':') ptr++;
        strcpy(buf,ptr);
        return;

}
void getVendorID(char* buf){
	FILE *fp;

	if((fp = fopen(CPUINFO,"r"))==NULL){
		fprintf(stderr, "fopen error for %s\n", CPUINFO);
		exit(1);
	}
	int i=0;
	while(i<CPUINFO_VENDOR_ID){
		memset(buf, '\0', BUFFER_SIZE);
		fgets(buf,BUFFER_SIZE,fp);
		i++;
	}
	//printf("here : %s\n",buf);
	char *ptr = buf;
	while(*ptr != ':') ptr++;
	while(*ptr == ' ' | *ptr == ':') ptr++;
	strcpy(buf,ptr);
	//printf("%s\n",buf);
	return;
}
int main(){
	char buf[BUFFER_SIZE];
	char vendor_id[BUFFER_SIZE];
	char model_name[BUFFER_SIZE];
	char cpu_MHz[BUFFER_SIZE];
	char l1d[BUFFER_SIZE];
	char l1i[BUFFER_SIZE];
	char l2[BUFFER_SIZE];

	getVendorID(buf);
	strcpy(vendor_id,buf);

	getModelName(buf);
	strcpy(model_name,buf);

	getCPUMHz(buf);
	strcpy(cpu_MHz,buf);

	getL1d(buf);
	strcpy(l1d,buf);

	getL1d(buf);
	strcpy(l1i,buf);

	getL2(buf);
	strcpy(l2,buf);
	
	printf("%-21s",VENDOR_STRING);
	printf("%s",vendor_id);
	printf("%-21s",MODEL_STRING);
	printf("%s",model_name);
	printf("%-21s",CPU_MHZ_STRING);
	printf("%s",cpu_MHz);
	printf("%-21s",L1D);
	printf("%s",l1d);
	printf("%-21s",L1I);
        printf("%s",l1i);
	printf("%-21s",L2);
	printf("%s",l2);

	
	return 0;
}
