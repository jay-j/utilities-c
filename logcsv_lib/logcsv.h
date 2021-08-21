#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[32];
    int precision;
    double value;
    // stale data marker TODO
} LogCSVDataColumn; 

typedef struct {
    FILE* fd;
    char filepath[128];
    int columns;
    LogCSVDataColumn* data;
} LogCSV;

LogCSV* log_csv_init(const char name[]);
void log_csv_column_add(LogCSV* clog, const char name[], int precision);
void log_csv_write_headers(LogCSV* clog);
void log_csv_write(LogCSV* clog);
void log_csv_close(LogCSV* clog);
size_t log_csv_column_getid(LogCSV* clog, const char* str);