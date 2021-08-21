#include "logcsv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* USEAGE
 * CSVLog* csvlog;
 * locsvlogg = log_csv_init(filename);
 * add variables (for headers)
 * write headers
 * modify values 
 * write (write an entire row)
 * log_csv_close(csvlog)
 */

// init a csv file
LogCSV* log_csv_init(const char name[]){
    LogCSV* clog;
    clog = (LogCSV*) malloc(sizeof (LogCSV));

    strcpy(clog->filepath, name);
    printf("Opening CSV log file %s ...", clog->filepath);
    clog->fd = fopen(clog->filepath, "w");
    if(clog->fd == NULL){
        printf("Error in creating the file\n");
        exit(1);
    }
    printf("done.\n");

    clog->data = (LogCSVDataColumn*) malloc(sizeof (LogCSVDataColumn ));
    clog->columns = 0;

    return clog;
}


void log_csv_write_headers(LogCSV* clog){
    for (size_t i = 0; i < clog->columns; i++){
        if (i != 0){
            fprintf(clog->fd, ",");
        }
        fprintf(clog->fd, "%s", clog->data[i].name);
    }
    fprintf(clog->fd, "\n");
}

// TODO accumulate new data - by assigning values into a structure? dictionary-like lookup

void log_csv_column_add(LogCSV* clog, const char name[], int precision){
    // expand the array
    clog->columns++;
    clog->data = (LogCSVDataColumn*) realloc(clog->data, clog->columns * sizeof (LogCSVDataColumn));

    // write metadata for the new column
    clog->data[clog->columns - 1].precision = precision;
    strcpy(clog->data[clog->columns - 1].name, name);
}


void log_csv_write(LogCSV* clog){

    for (size_t i=0; i < clog->columns; i++){
        if (i != 0){
            fprintf(clog->fd, ",");
        }
        fprintf(clog->fd, "%.*f", clog->data[i].precision, clog->data[i].value);
    }
    fprintf(clog->fd, "\n");

}


void log_csv_close(LogCSV* clog){
    fclose(clog->fd);
    printf("Closed CSV log file %s\n", clog->filepath);
    free(clog->data);
    free(clog);
}