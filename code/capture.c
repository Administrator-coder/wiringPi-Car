// your_program.c
#include <stdio.h>
#include "capture.h"
#define MAX_BUFFER 256

int capture() {
    char buffer[MAX_BUFFER];
    FILE *pipe_in, *pipe_out;
    // 使用一个popen调用打开Python程序以写入命令
    pipe_in = popen("python3 test.py", "w");
    if (pipe_in == NULL) {
        perror("popen");
        return 1;
    }
    // 使用另一个popen调用打开Python程序以读取输出
    pipe_out = popen("python3 test.py", "r");
    if (pipe_out == NULL) {
        perror("popen");
        return 1;
    }
    // 向Python程序发送一个命令
    fprintf(pipe_in, "capture\n");
    fflush(pipe_in);

    // 读取Python程序的输出
    if (fgets(buffer, sizeof(buffer), pipe_out) != NULL) {
        // printf("Result from Python script: %s", buffer);
        if (buffer == "Red") return 3;
        else if (buffer == "Green") return 2;
        else return 1;
    }
    pclose(pipe_in);
    pclose(pipe_out);
}
