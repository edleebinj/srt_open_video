#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>
#include<stdlib.h>
#define MAXCOMMANDLENGTH 1000
#define OPEN_OFFSET 5
int two_digit(char *a){
    return ((*a)-'0')*10+(*(a+1)-'0');
}
int main(void){
    FILE *cmd, *command_output;
    command_output = fopen("_command_output", "w+");

    char result[1024];
    char command_str[MAXCOMMANDLENGTH] = {0};
    char str_to_search[100] = {0};
    scanf("%s", str_to_search);
    snprintf(command_str, 100, "grep --color -n -w -B 1 \"%s\" ./srt/*.srt", str_to_search);

    cmd = popen(command_str, "r");
    if (cmd == NULL) {
        perror("popen");
        return 1;
    }

    int counter = 0;
    int line_counter = 1;
    printf("0 --\n");
    fprintf(command_output, "0 --\n");
    while (fgets(result, sizeof(result), cmd)) {
        if(line_counter == 3){
            line_counter = 0;
            counter++;
            printf("%d %s", counter, result);
            fprintf(command_output ,"%d %s", counter, result);
        }else{
            printf("%s", result);
            fprintf(command_output, "%s", result);
        }
        line_counter ++;
    }

    int target = -1;
    printf("which line do you want to open?\n");
    scanf("%d", &target);
    rewind(command_output);
    counter = 0;
    line_counter = 1;
    int time_in_seconds;
    while (fgets(result, sizeof(result), command_output)) {
        if(line_counter == 3){
            line_counter = 0;
            counter++;
        }else if(line_counter == 2 && counter == target){
            char *_file_name = strchr(strchr(result, '/') + 1, '/') + 1;
            int file_name_length = 0;
            while(_file_name[file_name_length] != '.'){
                file_name_length++;
            }
            char file_name[100] = {0};
            strncpy(file_name, _file_name, file_name_length);
            printf("name %s\n", file_name);
            char *time_ = strchr(strchr(result, '-')+1, '-')+1;
            int hour = two_digit(&time_[0]);
            int minute = two_digit(&time_[3]);
            int second = two_digit(&time_[6]);
            time_in_seconds = 3600 * hour + 60 * minute + second;
            printf("time in seconds %d\n", time_in_seconds);
            printf("%s", time_);
            char open_vlc_command[1000];
            snprintf(open_vlc_command, 1000, "/Applications/VLC.app/Contents/MacOS/VLC ./mp4/%s.mp4 --start-time %d", file_name, time_in_seconds - OPEN_OFFSET);
            printf("run %s\n",open_vlc_command);
            system(open_vlc_command);
        }
        line_counter ++;
    }
    fclose(command_output);
    pclose(cmd);
}