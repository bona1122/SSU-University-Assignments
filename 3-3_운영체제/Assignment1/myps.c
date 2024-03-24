#define FNAME_LEN 128
#define PATH_LEN 1024
#define TTY_LEN 32
#define TIME_LEN 16
#define CMD_LEN 1024
#define PID_MAX 32768
#define MAX_TOKEN 22				// /proc/pid/stat에서 읽어들일 token 갯수
#define TOKEN_LEN 32
#define PROCESS_MAX 4096

// column에 출력할 문자열
#define PID_STR "PID"
#define TTY_STR "TTY"
#define TIME_STR "TIME"
#define CMD_STR "CMD"

#define FD__ZERO "/fd/0"			// /proc/pid 에서의 0번째 fd 경로
#define STAT "/stat"				// /proc/pid에서의 stat 경로
#define BUFFER_SIZE 1024
#define STAT_TTY_NR_IDX 6
#define DEV "/dev"					// /dev 절대 경로
#define DEVNULL "/dev/null"			// 터미널 없을 시 가리키는 /dev/null 절대 경로

#define PROC "/proc"
#define PTS "pts/"					// 터미널 문자열의 앞부분

// /proc/pid/stat에서의 idx
#define STAT_PID_IDX 0
#define STAT_CMD_IDX 1
#define STAT_UTIME_IDX 13
#define STAT_STIME_IDX 14

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>

//process를 추상화 한 myProc 구조체
typedef struct{
        unsigned long pid;
        char tty[TTY_LEN];                      //터미널
        char time[TIME_LEN];            //총 cpu 사용 시간
        char cmd[CMD_LEN];                      //option 없을 경우에만 출력>    되는 command (short)
}myProc;

void search_proc();
void add_proc_list(char path[PATH_LEN]);
void getTTY(char path[PATH_LEN],char tty[TTY_LEN]);
void erase_proc(myProc *proc);


myProc procList[PROCESS_MAX];	//완성한 myProc의 포인터 저장 배열
int procCnt = 0;
unsigned int hertz;
pid_t myPid;
uid_t myUid;
char myPath[PATH_LEN];
char myTTY[TTY_LEN];
  

//proc의 내용을 지우는 함수
void erase_proc(myProc *proc)
{
	proc->pid = 0;
	memset(proc->tty, '\0', TTY_LEN);
	memset(proc->time, '\0', TIME_LEN);
	memset(proc->cmd, '\0', CMD_LEN);
	return;
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
void search_proc()
{
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
		if(statbuf.st_uid != myUid)		//uid가 자기 자신과 다를 경우 skip
				continue;
			
		char tty[TTY_LEN];
		memset(tty, '\0', TTY_LEN);
		getTTY(path, tty);		//TTY 획득
		if(!strlen(tty) || !strcmp(tty, "?"))	//nonTerminal일 경우
				continue;
		//char tty[TTY_LEN];
		//memset(tty, '\0', TTY_LEN);
		//getTTY(path, tty);		//TTY 획득	
		if(strcmp(tty, myTTY))	//자기 자신과 tty 다를 경우
			continue;
		add_proc_list(path);	//PID 디렉터리인 경우 procList에 추가
	}
	closedir(dirp);
	return;
}
void add_proc_list(char path[PATH_LEN]){
	if(access(path,R_OK) <0){
		fprintf(stderr,"access error for %s\n", path);
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
	getTTY(path,proc.tty);//TTY 획득

	
	//TIME 획득
	unsigned long utime = (unsigned long)atoi(statToken[STAT_UTIME_IDX]);
	unsigned long stime = (unsigned long)atoi(statToken[STAT_STIME_IDX]);
	unsigned long long totalTime = utime + stime;
	unsigned long cpuTime = totalTime / hertz;
	struct tm *tmCpuTime= localtime(&cpuTime);
	
	sprintf(proc.time, "%02d:%02d:%02d", tmCpuTime->tm_hour, tmCpuTime->tm_min, tmCpuTime->tm_sec);

	sscanf(statToken[STAT_CMD_IDX], "(%s", proc.cmd);	//cmd 획득
	proc.cmd[strlen(proc.cmd)-1] = '\0';	//마지막 ')' 제거
	

	procList[procCnt].pid = proc.pid;
	strcpy(procList[procCnt].tty, proc.tty);
	strcpy(procList[procCnt].time, proc.time);
	strcpy(procList[procCnt].cmd, proc.cmd);
	procCnt++;

	return;
}
void print_myps(){
	int columnWidth[4] = {
		strlen(PID_STR),strlen(TTY_STR),strlen(TIME_STR),strlen(CMD_STR) };

	char buf[BUFFER_SIZE];
	for(int i = 0; i < procCnt; i++){			//PID 최대 길이 저장
		sprintf(buf, "%lu", procList[i].pid);
		if(columnWidth[0] < strlen(buf))
			columnWidth[0] = strlen(buf);
	}
	
	for(int i = 0; i < procCnt; i++){			//TTY 최대 길이 저장
		if(columnWidth[1] < strlen(procList[i].tty))
			columnWidth[1] = strlen(procList[i].tty);
	}

	for(int i = 0; i < procCnt; i++){			//TIME 최대 길이 저장
		if(columnWidth[2] < strlen(procList[i].time))
			columnWidth[2] = strlen(procList[i].time);
	}
	
	for(int i = 0; i < procCnt; i++){			//CMD 최대 길이 저장
		if(columnWidth[3] < strlen(procList[i].cmd))
			columnWidth[3] = strlen(procList[i].cmd);
	}
	
	/*****		column 출력 시작	*****/
	int gap = 0;
	memset(buf, '\0', BUFFER_SIZE);	
	
	//PID 출력
	gap = columnWidth[0] - strlen(PID_STR);	//PID의 길이 차 구함
	for(int i = 0; i < gap; i++)				//PID 우측 정렬
		strcat(buf, " ");
	strcat(buf, PID_STR);

	strcat(buf, " ");

	//TTY 출력
	gap = columnWidth[1] - strlen(TTY_STR);	//TTY의 길이 차 구함
	strcat(buf, TTY_STR);
	for(int i = 0; i < gap; i++)				//TTY 좌측 정렬
		strcat(buf, " ");

	strcat(buf, " ");

	//TIME 출력
	gap = columnWidth[2] - strlen(TIME_STR);	//TIME의 길이 차 구함
	for(int i = 0; i < gap; i++)				//TIME 우측 정렬
		strcat(buf, " ");
	strcat(buf, TIME_STR);

	strcat(buf, " ");

	//CMD
	strcat(buf, CMD_STR);
	
	printf("%s\n",buf);

	/*****		process 출력 시작	*****/

	char token[TOKEN_LEN];
	memset(token, '\0', TOKEN_LEN);

	for(int i = 0; i < procCnt; i++){
		memset(buf, '\0', BUFFER_SIZE);

		//PID 출력
		memset(token, '\0', TOKEN_LEN);
		sprintf(token, "%lu", procList[i].pid);
		gap = columnWidth[0] - strlen(token);		//PID의 길이 차 구함
		for(int i = 0; i < gap; i++)				//PID 우측 정렬
			strcat(buf, " ");
		strcat(buf, token);

		strcat(buf, " ");


		//TTY 출력
		gap = columnWidth[1] - strlen(procList[i].tty);	//TTY의 길이 차 구함
		strcat(buf, procList[i].tty);
		for(int i = 0; i < gap; i++)						//TTY 좌측 정렬
			strcat(buf, " ");

		strcat(buf, " ");


		//TIME 출력
		gap = columnWidth[2] - strlen(procList[i].time);	//TIME의 길이 차 구함
		for(int i = 0; i < gap; i++)				//TIME 우측 정렬
			strcat(buf, " ");
		strcat(buf, procList[i].time);

		strcat(buf, " ");

		//CMD 출력
		strcat(buf, procList[i].cmd);			//CMD 바로 출력
	//	buf[COLS] = '\0';							//터미널 너비만큼만 출력
		printf("%s\n", buf);
	}
	
	return;
}
int main()
{
	hertz = (unsigned int)sysconf(_SC_CLK_TCK);
	myPid = getpid();

	char pidPath[FNAME_LEN];

	memset(pidPath, '\0',FNAME_LEN);
	sprintf(pidPath, "/%d",myPid);

	strcpy(myPath, PROC);
	strcat(myPath, pidPath);

	getTTY(myPath, myTTY);
	for(int i = strlen(PTS); i < strlen(myTTY); i++)
		if(!isdigit(myTTY[i])){
			myTTY[i] = '\0';
			break;
		}

	myUid = getuid();			//자기 자신의 uid

	search_proc();
	print_myps();

	return 0;
}

