#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <math.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/wait.h>
#include <utmp.h>


#define BUFFER_SIZE 1024
#define MEMINFO "/proc/meminfo"
#define MEMINFO_MEM_TOTAL_ROW 1
#define PID_MAX 32768
#define FNAME_LEN 128
#define PTS "pts/"
#define PATH_LEN 1024
#define TTY_LEN 32
#define PROC "/proc"
#define FD__ZERO "/fd/0"			// /proc/pid 에서의 0번째 fd 경로
#define DEVNULL "/dev/null"			// 터미널 없을 시 가리키는 /dev/null 절대 경로
#define STAT "/stat"				// /proc/pid에서의 stat 경로
#define STAT_TTY_NR_IDX 6
#define DEV "/dev"					// /dev 절대 경로
#define UPTIME "/proc/uptime"		// /proc/uptime 절대 경로
#define UNAME_LEN 32
#define STAT_LEN 8
#define TIME_LEN 16
#define CMD_LEN 1024
#define MAX_TOKEN 22				// /proc/pid/stat에서 읽어들일 token 갯수
#define TOKEN_LEN 32
#define SYSTEMD "systemd-"			// system USER명의 앞부분
#define STATUS "/status"			// /proc/pid에서의 status 경로
#define CMDLINE "/cmdline"			// /proc/pid에서의 cmdline 경로
#define VMSIZE "VmSize"				// /proc/pid/status에서 VmSize있는지 확인하기 위한 문자열
#define PROCESS_MAX 4096

#define LOADAVG "/proc/loadavg"		// /proc/loadavg 절대 경로
#define CPUSTAT "/proc/stat"		// /proc/stat 절대 경로
#define TICK_CNT 8					// cpu ticks 개수, us sy ni id wa hi si st로 총 8개

// /proc/pid/status에서의 row
#define STATUS_VSZ_ROW 18
#define STATUS_VMLCK_ROW 19
#define STATUS_RSS_ROW 22
#define STATUS_SHR_ROW 24

// /proc/pid/stat에서의 idx
#define STAT_PID_IDX 0
#define STAT_CMD_IDX 1
#define STAT_STATE_IDX 2
#define STAT_SID_IDX 5
#define STAT_TTY_NR_IDX 6
#define STAT_TPGID_IDX 7
#define STAT_UTIME_IDX 13
#define STAT_STIME_IDX 14
#define STAT_PRIORITY_IDX 17
#define STAT_NICE_IDX 18
#define STAT_N_THREAD_IDX 19
#define STAT_START_TIME_IDX 21

// /proc/meminfo 에서의 row
#define MEMINFO_MEM_TOTAL_ROW 1
#define MEMINFO_MEM_FREE_ROW 2
#define MEMINFO_MEM_AVAILABLE_ROW 3
#define MEMINFO_BUFFERS_ROW 4
#define MEMINFO_CACHED_ROW 5
#define MEMINFO_SWAP_TOTAL_ROW 15
#define MEMINFO_SWAP_FREE_ROW 16
#define MEMINFO_S_RECLAIMABLE_ROW 24

// column에 출력할 문자열
#define PID_STR "PID"
#define USER_STR "USER"
#define PR_STR "PR"
#define NI_STR "NI"
#define VSZ_STR "VSZ"
#define VIRT_STR "VIRT"
#define RSS_STR "RSS"
#define RES_STR "RES"
#define SHR_STR "SHR"
#define S_STR "S"
#define STAT_STR "STAT"
#define START_STR "START"
#define TTY_STR "TTY"
#define CPU_STR "%CPU"
#define MEM_STR "MEM"
#define TIME_STR "TIME"
#define TIME_P_STR "TIME+"
#define CMD_STR "CMD"
#define COMMAND_STR "COMMAND"

//출력 시 사용할 columnWidth배열에서의 index
#define PID_IDX 0
#define USER_IDX 1
#define PR_IDX 2
#define NI_IDX 3
#define VIRT_IDX 4
#define RES_IDX 5
#define SHR_IDX 6
#define S_IDX 7
#define CPU_IDX 8
#define MEM_IDX 9
#define TIME_P_IDX 10
#define COMMAND_IDX 11

#define TOP_ROW 0				//top 출력할 행
#define TASK_ROW 1				//task 출력할 행
#define CPU_ROW 2				//cpu 출력할 행
#define MEM_ROW 3				//mem 출력할 행
#define COLUMN_ROW 6			//column 출력할 행

#define COLUMN_CNT 12	//출력할 column 최대 갯수
#define TAB_WIDTH 8					//tab 길이

//process를 추상화 한 myProc 구조체
typedef struct{
	unsigned long pid;
	unsigned long uid;			//USER 구하기 위한 uid
	char user[UNAME_LEN];		//user명
	long double cpu;			//cpu 사용률
	long double mem;			//메모리 사용률
	unsigned long vsz;			//가상 메모리 사용량
	unsigned long rss;			//실제 메모리 사용량
	unsigned long shr;			//공유 메모리 사용량
	int priority;				//우선순위
	int nice;					//nice 값
	char tty[TTY_LEN];			//터미널
	char stat[STAT_LEN];		//상태
	char start[TIME_LEN];		//프로세스 시작 시각
	char time[TIME_LEN];		//총 cpu 사용 시간
	char cmd[CMD_LEN];			//option 없을 경우에만 출력되는 command (short)
	char command[CMD_LEN];		//option 있을 경우에 출력되는 command (long)

}myProc;

int procCnt = 0;				//현재까지 완성한 myProc 갯수
myProc procList[PROCESS_MAX];	//완성한 myProc의 포인터 저장 배열
myProc *sorted[PROCESS_MAX];	//procList를 cpu 순으로 sorting한 myProc 포인터 배열

unsigned long memTotal; //total mem
unsigned int hertz; //hertz of os
time_t now; //current time
time_t before; //before time
unsigned long cpuTimeTable[PID_MAX]; //hash table to store previous time of cpu
pid_t myPid; //my pid
uid_t myUid; //my uid
char myPath[PATH_LEN]; //my path
char myTTY[TTY_LEN]; //my tty
int row,col;
int ch;
unsigned long uptime;
unsigned long beforeUptime = 0;	//이전 실행 시의 os 부팅 후 지난 시각
long double beforeTicks[TICK_CNT] = {0, };	//이전의 cpu ticks 저장하기 위한 배열


//proc의 내용을 지우는 함수
void erase_proc(myProc *proc)
{
	proc->pid = 0;
	proc->uid = 0;
	memset(proc->user, '\0', UNAME_LEN);
	proc->cpu = 0.0;
	proc->mem = 0.0;
	proc->vsz = 0;
	proc->rss = 0;
	proc->shr = 0;
	proc->priority = 0;
	proc->nice = 0;
	memset(proc->tty, '\0', TTY_LEN);
	memset(proc->stat, '\0', STAT_LEN);
	memset(proc->start, '\0', TIME_LEN);
	memset(proc->time, '\0', TIME_LEN);
	memset(proc->cmd, '\0', CMD_LEN);
	memset(proc->command, '\0', CMD_LEN);
	return;
}
//src를 소숫점 아래 rdx+1자리에서 반올림하는 함수
long double round_double(long double src, int rdx)
{
	if(!rdx)
		return (long double)((unsigned long long)src);
	long double tmp = src;
	for(int i = 0; i <= rdx; i++)	//소숫점 아래 rdx+1번째 값 구하기
		tmp *= 10;
	int val = (unsigned long long)tmp % 10;	//소숫점 아래 rdx+1번째 값

	tmp /= 10;			//소숫점 아래 rdx번째까지만 소숫점 위로 표현

	tmp = (long double)((unsigned long long )tmp);	//rdx 밑의 자릿수 값 버리기

	if(val >= 5)		//반올림 o
		tmp += 1;

	for(int i = 0; i < rdx; i++)	//원상 복구
		tmp /= 10;

	return tmp;
}
unsigned long get_mem_total(void)
{
	FILE *fp;	// data from proc/meminfo 
	char buf[BUFFER_SIZE]; // data line 
	unsigned long memTotal; //total mem

	if((fp=fopen(MEMINFO,"r"))==NULL){
		fprintf(stderr,"fopen error for %s\n",MEMINFO);
		exit(1);
	}

	int i = 0;
	while(i < MEMINFO_MEM_TOTAL_ROW){ //read line
		memset(buf,'\0',BUFFER_SIZE);
		fgets(buf, BUFFER_SIZE,fp);
		i++;
	}
	char *ptr = buf; //only number data
	while(!isdigit(*ptr)) ptr++;
	sscanf(ptr, "%lu",&memTotal); //only number data
	fclose(fp);

	return memTotal;
}
//path에 대한 tty 얻는 함수
void getTTY(char path[PATH_LEN], char tty[TTY_LEN])
{
	char fdZeroPath[PATH_LEN];			//0번 fd에 대한 절대 경로
	memset(tty, '\0', TTY_LEN);
	memset(fdZeroPath, '\0', TTY_LEN);
	strcpy(fdZeroPath, path);
	strcat(fdZeroPath, FD__ZERO);

	if(access(fdZeroPath, F_OK) < 0){	//fd 0이 없을 경우

		char statPath[PATH_LEN];		// /proc/pid/stat에 대한 절대 경로
		memset(statPath, '\0', PATH_LEN);
		strcpy(statPath, path);
		strcat(statPath, STAT);

		FILE *statFp;
		if((statFp = fopen(statPath, "r")) == NULL){	// /proc/pid/stat open
			fprintf(stderr, "fopen error %s %s\n", strerror(errno), statPath);
			sleep(1);
			return;
		}

		char buf[BUFFER_SIZE];
		for(int i = 0; i <= STAT_TTY_NR_IDX; i++){		// 7행까지 read해 TTY_NR 획득
			memset(buf, '\0', BUFFER_SIZE);
			fscanf(statFp, "%s", buf);
		}
		fclose(statFp);

		int ttyNr = atoi(buf);		//ttyNr 정수 값으로 저장

		DIR *dp;
		struct dirent *dentry;
		if((dp = opendir(DEV)) == NULL){		// 터미널 찾기 위해 /dev 디렉터리 open
			fprintf(stderr, "opendir error for %s\n", DEV);
			exit(1);
		}
		char nowPath[PATH_LEN];

		while((dentry = readdir(dp)) != NULL){	// /dev 디렉터리 탐색
			memset(nowPath, '\0', PATH_LEN);	// 현재 탐색 중인 파일 절대 경로
			strcpy(nowPath, DEV);
			strcat(nowPath, "/");
			strcat(nowPath, dentry->d_name);

			struct stat statbuf;
			if(stat(nowPath, &statbuf) < 0){	// stat 획득
				fprintf(stderr, "stat error for %s\n", nowPath);
				exit(1);
			}
			if(!S_ISCHR(statbuf.st_mode))		//문자 디바이스 파일이 아닌 경우 skip
				continue;
			else if(statbuf.st_rdev == ttyNr){	//문자 디바이스 파일의 디바이스 ID가 ttyNr과 같은 경우
				strcpy(tty, dentry->d_name);	//tty에 현재 파일명 복사
				break;
			}
		}
		closedir(dp);

		if(!strlen(tty))					// /dev에서도 찾지 못한 경우
			strcpy(tty, "?");				//nonTerminal
	}
	else{
		char symLinkName[FNAME_LEN];
		memset(symLinkName, '\0', FNAME_LEN);
		if(readlink(fdZeroPath, symLinkName, FNAME_LEN) < 0){
			fprintf(stderr, "readlink error for %s\n", fdZeroPath);
			exit(1);
		}
		if(!strcmp(symLinkName, DEVNULL))		//symbolic link로 가리키는 파일이 /dev/null일 경우
			strcpy(tty, "?");					//nonTerminal
		else
			sscanf(symLinkName, "/dev/%s", tty);	//그 외의 경우 tty 획득

	}
	return;
}
unsigned long get_uptime(void) //get last time since boot from /proc/uptime
{
    FILE *fp;
    char buf[BUFFER_SIZE];
    long double time;

	memset(buf, '\0', BUFFER_SIZE);

    if ((fp = fopen(UPTIME, "r")) == NULL){	// /proc/uptime open
		fprintf(stderr, "fopen error for %s\n", UPTIME);
        exit(1);
    }
    fgets(buf, BUFFER_SIZE, fp);
    sscanf(buf, "%Lf", &time);	// /proc/uptime의 첫번째 double 읽기
    fclose(fp);

    return (unsigned long)time;
}
// pid 디렉터리 내의 파일들을 이용해 myProc 완성하는 함수
void add_proc_list(char path[PATH_LEN],/*, bool isPPS, bool aOption, bool uOption, bool xOption,*/ unsigned long cpuTimeTable[PID_MAX])
{
	if(access(path, R_OK) < 0){
		fprintf(stderr, "aceess error for %s\n", path);
		return;
	}
	myProc proc;
	erase_proc(&proc);

	char statPath[PATH_LEN];
	strcpy(statPath, path);
	strcat(statPath, STAT);

	if(access(statPath, R_OK) < 0){
		fprintf(stderr, "aceess error for %s\n", statPath);
		return;
	}
	FILE *statFp;
	if((statFp = fopen(statPath, "r")) == NULL){
		fprintf(stderr, "fopen error %s %s\n", strerror(errno), statPath);
		sleep(1);
		return;
	}

	char statToken[MAX_TOKEN][TOKEN_LEN];
	memset(statToken, '\0', MAX_TOKEN * TOKEN_LEN);
	for(int i = 0; i < MAX_TOKEN; i++)
		fscanf(statFp, "%s", statToken[i]);
	fclose(statFp);

	proc.pid = (long)atoi(statToken[STAT_PID_IDX]);	//pid 획득

	//user명 획득
	struct stat statbuf;
	if(stat(statPath, &statbuf) < 0){
		fprintf(stderr, "stat error for %s\n", statPath);
		return;
	}
	proc.uid = statbuf.st_uid;
	struct passwd *upasswd = getpwuid(statbuf.st_uid);

	char tmp[UNAME_LEN];
	strcpy(tmp, upasswd->pw_name);
	tmp[strlen(SYSTEMD)] = '\0';
	if(!strcmp(tmp, SYSTEMD)){				//user명이 systemd-로 시작할 경우
		tmp[strlen(SYSTEMD)-1] = '+';		//systemd+를 user명으로 저장
		strcpy(proc.user, tmp);
	}
	else									//user명이 systemd-로 시작하지 않을 경우 그대로 저장
		strcpy(proc.user, upasswd->pw_name);

	// %cpu 계산
	unsigned long utime = (unsigned long)atoi(statToken[STAT_UTIME_IDX]);
	unsigned long stime = (unsigned long)atoi(statToken[STAT_STIME_IDX]);
	unsigned long startTime = (unsigned long)atoi(statToken[STAT_START_TIME_IDX]);
	unsigned long long totalTime = utime + stime;
	long double cpu = 0;
	bool update = false;
	//if(!isPPS && cpuTimeTable != NULL){			//ttop일 경우
		if(cpuTimeTable[proc.pid] != 0){		//이전에 실행된 내역이 있을 경우 process 시작 시각 이후의 값이 아닌 이전 실행 이후의 값 불러옴
			update = true;
			cpu = (totalTime-cpuTimeTable[proc.pid]) / (long double)(now - before) / hertz * 100;	//%cpu 갱신
		}
		cpuTimeTable[proc.pid] = totalTime;		//현재까지의 process 사용 내역 저장
	//}
	if(!update)
		cpu = ((totalTime) / hertz) / (long double)(uptime-(startTime/hertz)) * 100;	//%cpu 계산
	if(isnan(cpu) || isinf(cpu) || cpu > 100 || cpu < 0)	// error 처리
		proc.cpu = 0.0;
	else
		proc.cpu = round_double(cpu, 2);	// 소숫점 아래 3자리에서 반올림

	char statusPath[PATH_LEN];				// /proc/pid/status 경로 획득
	memset(statusPath, '\0', PATH_LEN);
	strcpy(statusPath, path);
	strcat(statusPath, STATUS);

	FILE *statusFp;							//memory read
	unsigned long vmLck = 0;
	if((statusFp = fopen(statusPath, "r")) == NULL){	// /proc/pid/status open
		fprintf(stderr, "fopen error for %s\n", statusPath);
		return;
	}

	char buf[BUFFER_SIZE];
	int cnt = 0;
	while(cnt < STATUS_VSZ_ROW){				//18행까지 read
		memset(buf, '\0', BUFFER_SIZE);
    	fgets(buf, BUFFER_SIZE, statusFp);
		cnt++;
	}
	char *ptr = buf;
	for(int i = 0; i < strlen(buf); i++)
		if(isdigit(buf[i])){
			ptr = buf+i;
			break;
		}
	buf[strlen(VMSIZE)] = '\0';
	if(strcmp(buf, VMSIZE)){		// /proc/pid/status에 메모리 정보 없을 경우
		proc.vsz = 0;
		proc.rss = 0;
		proc.shr = 0;
		proc.mem = 0.0;
	}
	else{							// /proc/pid/status에 메모리 정보 있을 경우
		sscanf(ptr, "%lu", &proc.vsz);				//18행에서 vsz 획득

		while(cnt < STATUS_VMLCK_ROW){				//19행까지 read
			memset(buf, '\0', BUFFER_SIZE);
			fgets(buf, BUFFER_SIZE, statusFp);
			cnt++;
		}

		ptr = buf;
		for(int i = 0; i < strlen(buf); i++)
			if(isdigit(buf[i])){
				ptr = buf+i;
				break;
			}
		vmLck = 0;									//state에서 사용할 vmLck read
		sscanf(ptr, "%lu", &vmLck);					//19행에서 vmLck 획득

		while(cnt < STATUS_RSS_ROW){				//22행까지 read
			memset(buf, '\0', BUFFER_SIZE);
			fgets(buf, BUFFER_SIZE, statusFp);
			cnt++;
		}
		ptr = buf;
		for(int i = 0; i < strlen(buf); i++)
			if(isdigit(buf[i])){
				ptr = buf+i;
				break;
			}
		sscanf(ptr, "%lu", &proc.rss);				//22행에서 rss 획득

		while(cnt < STATUS_SHR_ROW){				//22행까지 read
			memset(buf, '\0', BUFFER_SIZE);
			fgets(buf, BUFFER_SIZE, statusFp);
			cnt++;
		}
		ptr = buf;
		for(int i = 0; i < strlen(buf); i++)
			if(isdigit(buf[i])){
				ptr = buf+i;
				break;
			}
		sscanf(ptr, "%lu", &proc.shr);				//24행에서 shr 획득

		long double mem = (long double)proc.rss / memTotal * 100;	//%mem 계산
		if(isnan(mem) || isinf(mem) || mem > 100 || mem < 0)	// error 처리
			proc.mem = 0.0;
		else
			proc.mem = round_double(mem, 2);	//소숫점 아래 3자리에서 반올림
	}
	fclose(statusFp);

	//if(isPPS)
	//	getTTY(path, proc.tty);				//TTY 획득

	proc.priority = atoi(statToken[STAT_PRIORITY_IDX]);	//priority 획득
	proc.nice = atoi(statToken[STAT_NICE_IDX]);		//nice 획득

	//STATE 획득
	strcpy(proc.stat, statToken[STAT_STATE_IDX]);		//기본 state 획득

	/*if(isPPS){		//PPS인 경우에만 세부 state 추가

		if(proc.nice < 0)							//nice하지 않은 경우
			strcat(proc.stat, "<");			// < 추가
		else if(proc.nice > 0)						//nice한 경우
			strcat(proc.stat, "N");			// N 추가

		if(vmLck > 0)							// memory lock을 했을 경우
			strcat(proc.stat, "L");			// L 추가

		int sid = atoi(statToken[5]);			// session id 획득
		if(sid == proc.pid)					// session leader일 경우
			strcat(proc.stat, "s");			// s 추가

		int threadCnt = atoi(statToken[STAT_N_THREAD_IDX]);	// thread 개수 획득
		if(threadCnt > 1)						// multi thread일 경우
			strcat(proc.stat, "l");			// l 추가

		int tpgid = atoi(statToken[STAT_TPGID_IDX]);			// foreground process group id 획득
		if(tpgid != -1)
			strcat(proc.stat, "+");			// + 추가
	}*/

	//START 획득
	unsigned long start = time(NULL) - uptime + (startTime/hertz);
	struct tm *tmStart= localtime(&start);
	if(time(NULL) - start < 24 * 60 * 60){
		strftime(proc.start, TIME_LEN, "%H:%M", tmStart);
	}
	else if(time(NULL) - start < 7 * 24 * 60 * 60){
		strftime(proc.start, TIME_LEN, "%b %d", tmStart);
	}
	else{
		strftime(proc.start, TIME_LEN, "%y", tmStart);
	}

	//TIME 획득
	unsigned long cpuTime = totalTime / hertz;
	struct tm *tmCpuTime= localtime(&cpuTime);
	//if(!isPPS || (!aOption && !uOption && !xOption))	//ttop이거나 pps에서 옵션이 없을 경우
		sprintf(proc.time, "%02d:%02d:%02d", tmCpuTime->tm_hour, tmCpuTime->tm_min, tmCpuTime->tm_sec);
	/*else
		sprintf(proc.time, "%1d:%02d", tmCpuTime->tm_min, tmCpuTime->tm_sec);*/

	sscanf(statToken[STAT_CMD_IDX], "(%s", proc.cmd);	//cmd 획득
	proc.cmd[strlen(proc.cmd)-1] = '\0';	//마지막 ')' 제거

	//command 획득
	char cmdLinePath[PATH_LEN];
	memset(cmdLinePath, '\0', PATH_LEN);
	strcpy(cmdLinePath, path);
	strcat(cmdLinePath, CMDLINE);

	FILE *cmdLineFp;

	if((cmdLineFp = fopen(cmdLinePath, "r")) == NULL){
		fprintf(stderr, "fopen error for %s\n", cmdLinePath);
		return;
	}

	while(true){
		char c[2] = {'\0', '\0'};
		fread(&c[0], 1, 1, cmdLineFp);
		if(c[0] == '\0'){					//'\0'여도 한 번 더 읽고 판단
			fread(&c[0], 1, 1, cmdLineFp);
			if(c[0] == '\0')
				break;
			else {
				strcat(proc.command, " ");
			}
		}
		strcat(proc.command, c);
	}
	if(!strlen(proc.command))				//cmdline에서 읽은 문자열 길이 0일 경우
		sprintf(proc.command, "[%s]", proc.cmd);	// [cmd]로 채워기
	fclose(cmdLineFp);

	procList[procCnt].pid = proc.pid;
	procList[procCnt].uid = proc.uid;
	strcpy(procList[procCnt].user, proc.user);
	procList[procCnt].cpu = proc.cpu;
	procList[procCnt].mem = proc.mem;
	procList[procCnt].vsz = proc.vsz;
	procList[procCnt].rss = proc.rss;
	procList[procCnt].shr = proc.shr;
	procList[procCnt].priority = proc.priority;
	procList[procCnt].nice = proc.nice;
	strcpy(procList[procCnt].tty, proc.tty);
	strcpy(procList[procCnt].stat, proc.stat);
	strcpy(procList[procCnt].start, proc.start);
	strcpy(procList[procCnt].time, proc.time);
	strcpy(procList[procCnt].cmd, proc.cmd);
	strcpy(procList[procCnt].command, proc.command);
	procCnt++;

	return;
}

void search_proc(/*bool isPPS, bool aOption, bool uOption, bool xOption,*/ unsigned long cpuTimeTable[PID_MAX])
{
	uptime = get_uptime();
	DIR *dirp;
	if((dirp = opendir(PROC)) == NULL){	// /proc 디렉터리 open
		fprintf(stderr, "dirp error for %s\n", PROC);
		exit(1);
	}
	struct dirent *dentry;
	while((dentry = readdir(dirp)) != NULL){	// /proc 디렉터리 내 하위 파일들 탐색 시작

		char path[PATH_LEN];			//디렉터리의 절대 경로 저장
		memset(path, '\0', PATH_LEN);
		strcpy(path, PROC);
		strcat(path, "/");
		strcat(path, dentry->d_name);

		struct stat statbuf;
		if(stat(path, &statbuf) < 0){	//디렉터리의 stat 획득
			fprintf(stderr, "stat error for %s\n", path);
			exit(1);
		}

		if(!S_ISDIR(statbuf.st_mode))	//디렉터리가 아닐 경우 skip
			continue;

		int len = strlen(dentry->d_name);
		bool isPid = true;
		for(int i = 0; i < len; i++){	//디렉터리가 PID인지 찾기
			if(!isdigit(dentry->d_name[i])){	//디렉터리명 중 숫자 아닌 문자가 있을 경우
				isPid = false;
				break;
			}
		}
		if(!isPid)				//PID 디렉터리가 아닌 경우 skip
			continue;
		/*if(isPPS && !aOption)			//aOption이 없을 경우 자기 자신의 process만 보여줌
			if(statbuf.st_uid != myUid)		//uid가 자기 자신과 다를 경우 skip
				continue;
		if(isPPS && !xOption){			//xOption이 없을 경우 nonTerminal process는 생략함
			char tty[TTY_LEN];
			memset(tty, '\0', TTY_LEN);
			getTTY(path, tty);		//TTY 획득
			if(!strlen(tty) || !strcmp(tty, "?"))	//nonTerminal일 경우
				continue;
		}
		if(isPPS && !aOption && !uOption && !xOption){	//모든 Option 없을 경우
			char tty[TTY_LEN];
			memset(tty, '\0', TTY_LEN);
			getTTY(path, tty);		//TTY 획득
			if(strcmp(tty, myTTY))	//자기 자신과 tty 다를 경우
				continue;
		}*/
		add_proc_list(path, /*isPPS, aOption, uOption, xOption,*/ cpuTimeTable);	//PID 디렉터리인 경우 procList에 추가
	}
	closedir(dirp);
	return;
}


//두 proc *를 cpu와 pid로 비교해 a < b인지 return하는 함수
bool isGreater(myProc *a, myProc *b)
{
	if(a->cpu < b->cpu)
		return true;
	else if(a->cpu > b->cpu)
		return false;
	else{
		if(a->pid > b->pid)
			return true;
		else return false;
	}
}
//procList를 cpu 순으로 sorting해 sorted 배열을 완성하는 함수
void sort_by_cpu(void)
{
	for(int i = 0; i < procCnt; i++)		//포인터 복사
		sorted[i] = procList + i;
	for(int i = procCnt - 1; i > 0; i--){
		for(int j = 0; j < i; j++){
			if(isGreater(sorted[j], sorted[j+1])){
				myProc *tmp = sorted[j];
				sorted[j] = sorted[j+1];
				sorted[j+1] = tmp;
			}
		}
	}
	return;
}
//실제 화면에 출력하는 함수
void print_mytop(void)
{
	uptime = get_uptime();			//os 부팅 후 지난 시각
    char buf[BUFFER_SIZE];

	/*****	1행 UPTIME 출력	*****/

	char nowStr[128];				//현재 시각 문자열
	memset(nowStr, '\0', 128);
	struct tm *tmNow = localtime(&now);
	sprintf(nowStr, "top - %02d:%02d:%02d ", tmNow->tm_hour, tmNow->tm_min, tmNow->tm_sec);

	struct tm *tmUptime = localtime(&uptime);

	char upStr[128];				//uiptime 문자열
	memset(upStr, '\0', 128);
	if(uptime < 60 * 60)
		sprintf(upStr, "%2d min", tmUptime->tm_min);
	else if(uptime < 60 * 60 * 24)
		sprintf(upStr, "%2d:%02d", tmUptime->tm_hour, tmUptime->tm_min);
	else
		sprintf(upStr, "%3d days, %02d:%02d", tmUptime->tm_yday, tmUptime->tm_hour, tmUptime->tm_min);

	int users = 0;							//active user 수 구하기

	struct utmp *utmp;
	setutent();								// utmp 처음부터 읽기
	while((utmp = getutent()) != NULL)		// /proc/utmp 파일에서 null일 때까지 읽어들이기
		if(utmp->ut_type == USER_PROCESS)	// ut_type이 USER일 경우에만 count 증가
			users++;

	FILE *loadAvgFp;
	long double loadAvg[3];
	if((loadAvgFp = fopen(LOADAVG, "r")) == NULL){
		fprintf(stderr, "fopen error for %s\n", LOADAVG);
		exit(1);
	}
	memset(buf, '\0', BUFFER_SIZE);
	fgets(buf, BUFFER_SIZE, loadAvgFp);
	fclose(loadAvgFp);
	sscanf(buf, "%Lf%Lf%Lf", &loadAvg[0], &loadAvg[1], &loadAvg[2]);

	mvprintw(TOP_ROW, 0, "%sup %s, %d users, load average: %4.2Lf, %4.2Lf, %4.2Lf", nowStr, upStr, users, loadAvg[0], loadAvg[1], loadAvg[2]);

	/*****	2행 Task 출력	*****/
	char *ptr;

	unsigned int total = 0, running = 0, sleeping = 0, stopped = 0, zombie = 0;
	total = procCnt;
	for(int i = 0; i < procCnt; i++){
		if(!strcmp(procList[i].stat, "R"))
			running++;
		else if(!strcmp(procList[i].stat, "D"))
			sleeping++;
		else if(!strcmp(procList[i].stat, "S"))
			sleeping++;
		else if(!strcmp(procList[i].stat, "T"))
			stopped++;
		else if(!strcmp(procList[i].stat, "t"))
			stopped++;
		else if(!strcmp(procList[i].stat, "Z"))
			zombie++;
	}
	mvprintw(TASK_ROW, 0, "Tasks:  %4u total,  %4u running, %4u sleeping,  %4u stopped, %4u zombie", total, running, sleeping, stopped, zombie);

	/*****	3행 %CPU 출력	*****/

	long double us, sy, ni, id, wa, hi, si, st;

	FILE *cpuStatFp;
	if((cpuStatFp = fopen(CPUSTAT, "r")) == NULL){				// /proc/stat fopen
		fprintf(stderr, "fopen error for %s\n", CPUSTAT);
		exit(1);
	}
	memset(buf, '\0', BUFFER_SIZE);
    fgets(buf, BUFFER_SIZE, cpuStatFp);
	fclose(cpuStatFp);
	ptr = buf;
	while(!isdigit(*ptr)) ptr++;

	long double ticks[TICK_CNT] = {0.0, };

	sscanf(ptr, "%Lf%Lf%Lf%Lf%Lf%Lf%Lf%Lf", ticks+0, ticks+1, ticks+2, ticks+3, ticks+4, ticks+5, ticks+6, ticks+7);	//ticks read

	unsigned long tickCnt = 0;
	long double results[TICK_CNT] = {0.0, };			//출력할 ticks 값
	if(beforeUptime == 0){
		 tickCnt = uptime * hertz;						//부팅 후 현재까지 일어난 context switching 횟수
		 for(int i = 0; i < TICK_CNT; i++)				//읽은 ticks 그대로 출력
			 results[i] = ticks[i];
	}
	else{
		tickCnt = (uptime - beforeUptime) * hertz;		//부팅 후 현재까지 일어난 context switching 횟수
		for(int i = 0; i < TICK_CNT; i++)
			results[i] = ticks[i] - beforeTicks[i];		//이전에 저장한 tick수를 빼서 출력
	}
	for(int i = 0; i < TICK_CNT; i++){
		results[i] = (results[i] / tickCnt) * 100;		//퍼센트로 저장
		if(isnan(results[i]) || isinf(results[i]))		//예외 처리
			results[i] = 0;
	}


	mvprintw(CPU_ROW, 0, "%%Cpu(s):  %4.1Lf us, %4.1Lf sy, %4.1Lf ni, %4.1Lf id, %4.1Lf wa, %4.1Lf hi, %4.1Lf si, %4.1Lf st",
	results[0], results[2], results[1], results[3], results[4], results[5], results[6], results[7]);

	beforeUptime = uptime;								//갱신
	for(int i = 0; i < TICK_CNT; i++)
		beforeTicks[i] = ticks[i];

	/*****	4,5행 MEM SWAP출력	*****/

	unsigned long memTotal, memFree, memUsed, memAvailable, buffers, cached, sReclaimable, swapTotal, swapFree, swapUsed;

    FILE *meminfoFp;

    if ((meminfoFp = fopen(MEMINFO, "r")) == NULL){	// /proc/meminfo open
		fprintf(stderr, "fopen error for %s\n", MEMINFO);
        exit(1);
    }

	int i = 0;


	while(i < MEMINFO_MEM_TOTAL_ROW){	//memTotal read
		memset(buf, '\0', BUFFER_SIZE);
    	fgets(buf, BUFFER_SIZE, meminfoFp);
		i++;
	}
	ptr = buf;
	while(!isdigit(*ptr)) ptr++;
    sscanf(ptr, "%lu", &memTotal);		// /proc/meminfo의 1행에서 memFree read


	while(i < MEMINFO_MEM_FREE_ROW){	//memFree read
		memset(buf, '\0', BUFFER_SIZE);
    	fgets(buf, BUFFER_SIZE, meminfoFp);
		i++;
	}
	ptr = buf;
	while(!isdigit(*ptr)) ptr++;
    sscanf(ptr, "%lu", &memFree);	// /proc/meminfo의 2행에서 memFree read


	while(i < MEMINFO_MEM_AVAILABLE_ROW){	//memAvailable read
		memset(buf, '\0', BUFFER_SIZE);
    	fgets(buf, BUFFER_SIZE, meminfoFp);
		i++;
	}
	ptr = buf;
	while(!isdigit(*ptr)) ptr++;
    sscanf(ptr, "%lu", &memAvailable);	// /proc/meminfo의 3행에서 memAvailable read


	while(i < MEMINFO_BUFFERS_ROW){	//buffers read
		memset(buf, '\0', BUFFER_SIZE);
    	fgets(buf, BUFFER_SIZE, meminfoFp);
		i++;
	}
	ptr = buf;
	while(!isdigit(*ptr)) ptr++;
    sscanf(ptr, "%lu", &buffers);	// /proc/meminfo의 4행에서 buffers read


	while(i < MEMINFO_CACHED_ROW){	//cached read
		memset(buf, '\0', BUFFER_SIZE);
    	fgets(buf, BUFFER_SIZE, meminfoFp);
		i++;
	}
	ptr = buf;
	while(!isdigit(*ptr)) ptr++;
    sscanf(ptr, "%lu", &cached);	// /proc/meminfo의 5행에서 cached read


	while(i < MEMINFO_SWAP_TOTAL_ROW){	//swapTotal read
		memset(buf, '\0', BUFFER_SIZE);
    	fgets(buf, BUFFER_SIZE, meminfoFp);
		i++;
	}
	ptr = buf;
	while(!isdigit(*ptr)) ptr++;
    sscanf(ptr, "%lu", &swapTotal);	// /proc/meminfo의 15행에서 swapTotal read


	while(i < MEMINFO_SWAP_FREE_ROW){	//swapFree read
		memset(buf, '\0', BUFFER_SIZE);
    	fgets(buf, BUFFER_SIZE, meminfoFp);
		i++;
	}
	ptr = buf;
	while(!isdigit(*ptr)) ptr++;
    sscanf(ptr, "%lu", &swapFree);	// /proc/meminfo의 16행에서 swapFree read


	while(i < MEMINFO_S_RECLAIMABLE_ROW){	//sReclaimable read
		memset(buf, '\0', BUFFER_SIZE);
    	fgets(buf, BUFFER_SIZE, meminfoFp);
		i++;
	}
	ptr = buf;
	while(!isdigit(*ptr)) ptr++;
    sscanf(ptr, "%lu", &sReclaimable);	// /proc/meminfo의 23행에서 sReclaimable read

	memUsed = memTotal - memFree - buffers - cached - sReclaimable;		//memUsed 계산
	swapUsed = swapTotal - swapFree;	//swapUsed 계산

	mvprintw(MEM_ROW, 0, "Kib Mem : %8lu total,  %8lu free,  %8lu used,  %8lu buff/cache", memTotal, memFree, memUsed, buffers+cached+sReclaimable);
	mvprintw(MEM_ROW+1, 0, "Kib Swap: %8lu total,  %8lu free,  %8lu used,  %8lu avail Mem", swapTotal, swapFree, swapUsed, memAvailable);

    fclose(meminfoFp);

	int columnWidth[COLUMN_CNT] = {					//column의 x축 길이 저장하는 배열
		strlen(PID_STR), strlen(USER_STR), strlen(PR_STR), strlen(NI_STR),
		strlen(VIRT_STR), strlen(RES_STR), strlen(SHR_STR), strlen(S_STR),
		strlen(CPU_STR), strlen(MEM_STR), strlen(TIME_P_STR), strlen(COMMAND_STR) };

	for(int i = 0; i < procCnt; i++){			//PID 최대 길이 저장
		sprintf(buf, "%lu", procList[i].pid);
		if(columnWidth[PID_IDX] < strlen(buf))
			columnWidth[PID_IDX] = strlen(buf);
	}

	for(int i = 0; i < procCnt; i++)			//USER 최대 길이 저장
		if(columnWidth[USER_IDX] < strlen(procList[i].user)){
			columnWidth[USER_IDX] = strlen(procList[i].user);
		}

	for(int i = 0; i < procCnt; i++){			//PR 최대 길이 저장
		sprintf(buf, "%d", procList[i].priority);
		if(columnWidth[PR_IDX] < strlen(buf))
			columnWidth[PR_IDX] = strlen(buf);
	}

	for(int i = 0; i < procCnt; i++){			//NI 최대 길이 저장
		sprintf(buf, "%d", procList[i].nice);
		if(columnWidth[NI_IDX] < strlen(buf))
			columnWidth[NI_IDX] = strlen(buf);
	}

	for(int i = 0; i < procCnt; i++){			//VIRT 최대 길이 저장
		sprintf(buf, "%lu", procList[i].vsz);
		if(columnWidth[VIRT_IDX] < strlen(buf))
			columnWidth[VIRT_IDX] = strlen(buf);
	}

	for(int i = 0; i < procCnt; i++){			//RES 최대 길이 저장
		sprintf(buf, "%lu", procList[i].rss);
		if(columnWidth[RES_IDX] < strlen(buf))
			columnWidth[RES_IDX] = strlen(buf);
	}

	for(int i = 0; i < procCnt; i++){			//SHR 최대 길이 저장
		sprintf(buf, "%lu", procList[i].shr);
		if(columnWidth[SHR_IDX] < strlen(buf))
			columnWidth[SHR_IDX] = strlen(buf);
	}

	for(int i = 0; i < procCnt; i++){			//S 최대 길이 저장
		if(columnWidth[S_IDX] < strlen(procList[i].stat))
			columnWidth[S_IDX] = strlen(procList[i].stat);
	}


	for(int i = 0; i < procCnt; i++){			//CPU 최대 길이 저장
		sprintf(buf, "%3.1Lf", procList[i].cpu);
		if(columnWidth[CPU_IDX] < strlen(buf))
			columnWidth[CPU_IDX] = strlen(buf);
	}

	for(int i = 0; i < procCnt; i++){			//MEM 최대 길이 저장
		sprintf(buf, "%3.1Lf", procList[i].mem);
		if(columnWidth[MEM_IDX] < strlen(buf))
			columnWidth[MEM_IDX] = strlen(buf);
	}

	for(int i = 0; i < procCnt; i++){			//TIME 최대 길이 저장
		if(columnWidth[TIME_P_IDX] < strlen(procList[i].time))
			columnWidth[TIME_P_IDX] = strlen(procList[i].time);
	}

	for(int i = 0; i < procCnt; i++){			//COMMAND 최대 길이 저장
		if(columnWidth[COMMAND_IDX] < strlen(procList[i].command))
			columnWidth[COMMAND_IDX] = strlen(procList[i].command);
	}

	int startX[COLUMN_CNT] = {0, };				//각 column의 시작 x좌표

	int startCol = 0, endCol = 0;
	int maxCmd = -1;							//COMMAND 출력 가능한 최대 길이

	if(col >= COLUMN_CNT - 1){					//COMMAND COLUMN만 출력하는 경우 (우측 화살표 많이 누른 경우)
		startCol = COMMAND_IDX;
		endCol = COLUMN_CNT;
		maxCmd = COLS;							//COMMAND 터미널 너비만큼 출력 가능
	}
	else{
		int i;
		for(i = col + 1; i < COLUMN_CNT; i++){
			startX[i] = columnWidth[i-1] + 2 + startX[i-1];
			if(startX[i] >= COLS){				//COLUMN의 시작이 이미 터미널 너비 초과한 경우
				endCol = i;
				break;
			}
		}
		startCol = col;
		if(i == COLUMN_CNT){
			endCol = COLUMN_CNT;					//COLUMN 전부 출력하는 경우
			maxCmd = COLS - startX[COMMAND_IDX];	//COMMAND 최대 출력 길이: COMMAND 터미널 너비 - COMMAND 시작 x좌표
		}
	}


	/*****		6행 column 출력 시작	*****/

	attron(A_REVERSE);
	for(int i = 0; i < COLS; i++)
		mvprintw(COLUMN_ROW, i, " ");

	int gap = 0;

	//PID 출력
	if(startCol <= PID_IDX && PID_IDX < endCol){
		gap = columnWidth[PID_IDX] - strlen(PID_STR);	//PID의 길이 차 구함
		mvprintw(COLUMN_ROW, startX[PID_IDX] + gap, "%s", PID_STR);	//우측 정렬
	}

	//USER 출력
	if(startCol <= USER_IDX && USER_IDX < endCol)
		mvprintw(COLUMN_ROW, startX[USER_IDX], "%s", USER_STR);	//좌측 정렬

	//PR 출력
	if(startCol <= PR_IDX && PR_IDX < endCol){
		gap = columnWidth[PR_IDX] - strlen(PR_STR);		//PR 의 길이 차 구함
		mvprintw(COLUMN_ROW, startX[PR_IDX] + gap, "%s", PR_STR);	//우측 정렬
	}

	//NI 출력
	if(startCol <= NI_IDX && NI_IDX < endCol){
		gap = columnWidth[NI_IDX] - strlen(NI_STR);		//NI 의 길이 차 구함
		mvprintw(COLUMN_ROW, startX[NI_IDX] + gap, "%s", NI_STR);	//우측 정렬
	}

	//VIRT 출력
	if(startCol <= VIRT_IDX && VIRT_IDX < endCol){
		gap = columnWidth[VIRT_IDX] - strlen(VIRT_STR);	//VSZ의 길이 차 구함
		mvprintw(COLUMN_ROW, startX[VIRT_IDX] + gap, "%s", VIRT_STR);	//우측 정렬
	}

	//RES 출력
	if(startCol <= RES_IDX && RES_IDX < endCol){
		gap = columnWidth[RES_IDX] - strlen(RES_STR);	//RSS의 길이 차 구함
		mvprintw(COLUMN_ROW, startX[RES_IDX] + gap, "%s", RES_STR);	//우측 정렬
	}

	//SHR 출력
	if(startCol <= SHR_IDX && SHR_IDX < endCol){
		gap = columnWidth[SHR_IDX] - strlen(SHR_STR);	//SHR의 길이 차 구함
		mvprintw(COLUMN_ROW, startX[SHR_IDX] + gap, "%s", SHR_STR);	//우측 정렬
	}

	//S 출력
	if(startCol <= S_IDX && S_IDX < endCol){
		mvprintw(COLUMN_ROW, startX[S_IDX], "%s", S_STR);	//우측 정렬
	}

	//%CPU 출력
	if(startCol <= CPU_IDX && CPU_IDX < endCol){
		gap = columnWidth[CPU_IDX] - strlen(CPU_STR);	//CPU의 길이 차 구함
		mvprintw(COLUMN_ROW, startX[CPU_IDX] + gap, "%s", CPU_STR);	//우측 정렬
	}

	//%MEM 출력
	if(startCol <= MEM_IDX && MEM_IDX < endCol){
		gap = columnWidth[MEM_IDX] - strlen(MEM_STR);	//MEM의 길이 차 구함
		mvprintw(COLUMN_ROW, startX[MEM_IDX] + gap, "%s", MEM_STR);	//우측 정렬
	}

	//TIME+ 출력
	if(startCol <= TIME_P_IDX && TIME_P_IDX < endCol){
		gap = columnWidth[TIME_P_IDX] - strlen(TIME_P_STR);	//TIME의 길이 차 구함
		mvprintw(COLUMN_ROW, startX[TIME_P_IDX] + gap, "%s", TIME_P_STR);	//우측 정렬
	}

	//COMMAND 출력
	mvprintw(COLUMN_ROW, startX[COMMAND_IDX], "%s", COMMAND_STR);	//좌측 정렬

	attroff(A_REVERSE);

	/*****		column 출력 종료	*****/


	/*****		process 출력 시작	*****/

	char token[TOKEN_LEN];
	memset(token, '\0', TOKEN_LEN);

	for(int i = row; i < procCnt; i++){

		//PID 출력
		if(startCol <= PID_IDX && PID_IDX < endCol){
			memset(token, '\0', TOKEN_LEN);
			sprintf(token, "%lu", sorted[i]->pid);
			gap = columnWidth[PID_IDX] - strlen(token);	//PID의 길이 차 구함
			mvprintw(COLUMN_ROW+1+i-row, startX[PID_IDX]+gap, "%s", token);	//우측 정렬
		}

		//USER 출력
		if(startCol <= USER_IDX && USER_IDX < endCol){
			gap = columnWidth[USER_IDX] - strlen(sorted[i]->user);	//TIME의 길이 차 구함
			mvprintw(COLUMN_ROW+1+i-row, startX[USER_IDX], "%s", sorted[i]->user);	//좌측 정렬
		}

		//PR 출력
		if(startCol <= PR_IDX && PR_IDX < endCol){
			memset(token, '\0', TOKEN_LEN);
			sprintf(token, "%d", sorted[i]->priority);
			gap = columnWidth[PR_IDX] - strlen(token);	//PR의 길이 차 구함
			mvprintw(COLUMN_ROW+1+i-row, startX[PR_IDX]+gap, "%s", token);	//우측 정렬
		}

		//NI 출력
		if(startCol <= NI_IDX && NI_IDX < endCol){
			memset(token, '\0', TOKEN_LEN);
			sprintf(token, "%d", sorted[i]->nice);
			gap = columnWidth[NI_IDX] - strlen(token);	//NI의 길이 차 구함
			mvprintw(COLUMN_ROW+1+i-row, startX[NI_IDX]+gap, "%s", token);	//우측 정렬
		}

		//VIRT 출력
		if(startCol <= VIRT_IDX && VIRT_IDX < endCol){
			memset(token, '\0', TOKEN_LEN);
			sprintf(token, "%lu", sorted[i]->vsz);
			gap = columnWidth[VIRT_IDX] - strlen(token);	//VIRT의 길이 차 구함
			mvprintw(COLUMN_ROW+1+i-row, startX[VIRT_IDX]+gap, "%s", token);	//우측 정렬
		}

		//RES 출력
		if(startCol <= RES_IDX && RES_IDX < endCol){
			memset(token, '\0', TOKEN_LEN);
			sprintf(token, "%lu", sorted[i]->rss);
			gap = columnWidth[RES_IDX] - strlen(token);	//RES의 길이 차 구함
			mvprintw(COLUMN_ROW+1+i-row, startX[RES_IDX]+gap, "%s", token);	//우측 정렬
		}

		//SHR 출력
		if(startCol <= SHR_IDX && SHR_IDX < endCol){
			memset(token, '\0', TOKEN_LEN);
			sprintf(token, "%lu", sorted[i]->shr);
			gap = columnWidth[SHR_IDX] - strlen(token);	//SHR의 길이 차 구함
			mvprintw(COLUMN_ROW+1+i-row, startX[SHR_IDX]+gap, "%s", token);	//우측 정렬
		}

		//S 출력
		if(startCol <= S_IDX && S_IDX < endCol){
			gap = columnWidth[S_IDX] - strlen(sorted[i]->stat);	//S의 길이 차 구함
			mvprintw(COLUMN_ROW+1+i-row, startX[S_IDX], "%s", sorted[i]->stat);	//좌측 정렬
		}

		//%CPU 출력
		if(startCol <= CPU_IDX && CPU_IDX < endCol){
			memset(token, '\0', TOKEN_LEN);
			sprintf(token, "%3.1Lf", sorted[i]->cpu);
			gap = columnWidth[CPU_IDX] - strlen(token);	//CPU의 길이 차 구함
			mvprintw(COLUMN_ROW+1+i-row, startX[CPU_IDX]+gap, "%s", token);	//우측 정렬
		}

		//%MEM 출력
		if(startCol <= MEM_IDX && MEM_IDX < endCol){
			memset(token, '\0', TOKEN_LEN);
			sprintf(token, "%3.1Lf", sorted[i]->mem);
			gap = columnWidth[MEM_IDX] - strlen(token);	//MEM의 길이 차 구함
			mvprintw(COLUMN_ROW+1+i-row, startX[MEM_IDX]+gap, "%s", token);	//우측 정렬
		}

		//TIME+ 출력
		if(startCol <= TIME_P_IDX && TIME_P_IDX < endCol){
			gap = columnWidth[TIME_P_IDX] - strlen(sorted[i]->time);	//TIME의 길이 차 구함
			mvprintw(COLUMN_ROW+1+i-row, startX[TIME_P_IDX]+gap, "%s", sorted[i]->time);	//우측 정렬
		}

		//COMMAND 출력
		int tap = col - COMMAND_IDX;
		if((col == COMMAND_IDX) && (strlen(sorted[i]->command) < tap*TAB_WIDTH))		//COMMAND를 출력할 수 없는 경우
			continue;
		if(col < COLUMN_CNT - 1)	//다른 column도 함께 출력하는 경우
			tap = 0;
		sorted[i]->cmd[maxCmd] = '\0';
		mvprintw(COLUMN_ROW+1+i-row, startX[COMMAND_IDX], "%s", sorted[i]->cmd + tap*TAB_WIDTH);	//좌측 정렬

	}

	/*****		process 출력 종료	*****/

	return;
}
// procList 내용 지우는 함수
void erase_proc_list(void)
{
	for(int i = 0; i < procCnt; i++)
		erase_proc(procList + i);
	procCnt = 0;
	return;
}
int main(){
	//memTotal = get_mem_total(); //total memory
	hertz = (unsigned int)sysconf(_SC_CLK_TCK); //hertz of os
	now = time(NULL); //current time
	

	memset(cpuTimeTable, (unsigned long)0, PID_MAX);

	myPid = getpid(); //my pid

	char pidPath[FNAME_LEN];
	memset(pidPath, '\0', FNAME_LEN);
	sprintf(pidPath, "/%d",myPid);

	strcpy(myPath,PROC);  //my /proc path
	strcat(myPath,pidPath);

	getTTY(myPath, myTTY);
	for(int i=strlen(PTS);i<strlen(myTTY);i++)
		if(!isdigit(myTTY[i])){
			myTTY[i] = '\0';
			break;
		}
	myUid = getuid(); //my uid

	initscr();
	halfdelay(10);
	noecho();
	keypad(stdscr,TRUE);
	curs_set(0);

	search_proc(/*false,false,false,false,*/cpuTimeTable);

	row = 0;
	col = 0;
		
	ch = 0;

	bool print = false;
	pid_t pid;

	before = time(NULL);

	sort_by_cpu();
	print_mytop();
	refresh();

	do{
		now = time(NULL);	//현재 시각 갱신

		switch(ch){			//방향키 입력 좌표 처리
			case KEY_LEFT:
				col--;
				if(col < 0)
					col = 0;
				print = true;
				break;
			case KEY_RIGHT:
				col++;
				print = true;
				break;
			case KEY_UP:
				row--;
				if(row < 0)
					row = 0;
				print = true;
				break;
			case KEY_DOWN:
				row++;
				if(row > procCnt)
					row = procCnt;
				print = true;
				break;
		}

		if(print || now - before >= 3){	//3초 경과 시 화면 갱신
			erase();
			erase_proc_list();
			//search_proc(false, false, false, false, cpuTimeTable);
			search_proc(cpuTimeTable);
			sort_by_cpu();			//cpu 순으로 정렬
			print_mytop();
			refresh();
			before = now;
			print = false;
		}
	}while((ch = getch())!='q');

	endwin();

	return 0;
}
