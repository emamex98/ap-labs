#include <stdio.h>
#include <stdlib.h>

void month_day(int year, int day, char *month[], char *days[]){

    int leap = year%4 == 0 && year%100 != 0 || year%400 == 0;

    if (day>0 && day<=31){
        printf("%s %s %d \n", month[1], days[day], year);
    }
    else if (leap == 1){
        if(day>=32 && day<=60){
            printf("%s %s, %d \n", month[2], days[day-31], year);
        } else if (day>=61 && day<=91){
            printf("%s %s, %d \n", month[3], days[day-60], year);
        } else if (day>=92 && day<=121){
            printf("%s %s, %d \n", month[4], days[day-91], year);
        } else if (day>=122 && day<=152){
            printf("%s %s, %d \n", month[5], days[day-121], year);
        } else if (day>=153 && day<=182){
            printf("%s %s, %d \n", month[6], days[day-152], year);
        } else if (day>=183 && day<=213){
            printf("%s %s, %d \n", month[7], days[day-182], year);
        } else if (day>=214 && day<=244){
            printf("%s %s, %d \n", month[8], days[day-213], year);
        } else if (day>=245 && day<=274){
            printf("%s %s, %d \n", month[9], days[day-244], year);
        } else if (day>=275 && day<=305){
            printf("%s %s, %d \n", month[10], days[day-274], year);
        } else if (day>=306 && day<=335){
            printf("%s %s, %d \n", month[11], days[day-305], year);
        } else if (day>=336 && day<=366){
            printf("%s %s, %d \n", month[12], days[day-335], year);
        } else {
            printf("Invalid Day \n");
        }
    }
    else if (leap == 0) {
        if(day>=32 && day<=59){
            printf("%s %s, %d \n", month[2], days[day-31], year);
        } else if (day>=60 && day<=90){
            printf("%s %s, %d \n", month[3], days[day-59], year);
        } else if (day>=91 && day<=120){
            printf("%s %s, %d \n", month[4], days[day-90], year);
        } else if (day>=121 && day<=151){
            printf("%s %s, %d \n", month[5], days[day-120], year);
        } else if (day>=152 && day<=181){
            printf("%s %s, %d \n", month[6], days[day-151], year);
        } else if (day>=182 && day<=212){
            printf("%s %s, %d \n", month[7], days[day-181], year);
        } else if (day>=213 && day<=243){
            printf("%s %s, %d \n", month[8], days[day-212], year);
        } else if (day>=244 && day<=273){
            printf("%s %s, %d \n", month[9], days[day-243], year);
        } else if (day>=274 && day<=304){
            printf("%s %s, %d \n", month[10], days[day-273], year);
        } else if (day>=305 && day<=334){
            printf("%s %s, %d \n", month[11], days[day-304], year);
        } else if (day>=335 && day<=365){
            printf("%s %s, %d \n", month[12], days[day-334], year);
        } else {
            printf("Invalid Day \n");
        }
    }

}

int main(int argc, char *argv[]) {

    static char *month[] = {
       "", "January", "February", "March",
       "April", "May", "June",
       "July", "August", "September",
       "October", "November", "December"
    };

    static char *days[] = {
       "", "01", "02", "03", "04", "05",
       "06", "07", "08", "09", "10", "11",
       "12", "13", "14", "15", "16", "17",
       "18", "19", "20", "21", "22", "23",
       "24", "25", "26", "27", "28", "29",
       "30", "31"
    };


    month_day(atoi(argv[1]), atoi(argv[2]), month, days);
    return 0;
}
