#include <stdio.h>
#include <stdlib.h>

int day_of_the_year(int year, int month, int day, int *yearday);
int month_day(int year, int yearday, int *month, int *day);

int main(void){
    int yearday;
    int month;
    int day;

    if (day_of_the_year(2028, 2,  29, &yearday) == 1){
        return EXIT_FAILURE;
    }
    printf("yearday: %d\n", yearday);

    if (month_day(2028, yearday, &month, &day) == 1){
        return EXIT_FAILURE;
    }
    printf("month: %d day: %d\n", month, day);

    return 0;
}

int daytab[2][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

int day_of_the_year(int year, int month, int day, int *yearday){
    int leap;
    int i;
    int *p;

    if (year < 1 || month < 1 || month > 12 || day < 1){
        return 1;
    }
    
    leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;

    if (day > daytab[leap][month]){
        return 1;
    }

    p = *(daytab + leap);
    
    for (; month > 1; month--){ // i < month so the last month won't get added, because the last month day is given by argument day
        day += *(++p);
    }

    *yearday = day;

    return 0;
}

int month_day(int year, int yearday, int *month, int *day){
    int leap;
    int i;
    int total_yearday;
    int *p;

    if (year < 1 || yearday < 1){
        return 1;
    }

    leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;

    total_yearday = (leap)? 366 : 365;
    if (yearday > total_yearday){
        return 1;
    }

    p = *(daytab + leap);
    
    while (yearday > *++p){
        yearday -= *p;
    }

    *month = (p - *(daytab + leap));
    *day = yearday;

    return 0;
}
