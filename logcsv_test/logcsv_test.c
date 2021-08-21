#include "logcsv.h"

int main(){
    // create an empty csv entity
    LogCSV* log;

    // initialize 
    log = log_csv_init("sample_file.csv");

    // add columns
    log_csv_column_add(log, "time", 3);
    log_csv_column_add(log, "position", 3);
    log_csv_column_add(log, "velocity", 2);

    // write the headers to disk
    log_csv_write_headers(log);

    // fake some data
    for (size_t i=0; i<128; i++){
        log->data[0].value = (double) i;
        log->data[1].value = (double) 2*i+3;
        log->data[2].value = 3333 - (double) i;

        // each loop write new data to disk
        log_csv_write(log);
    }

    // now clean up, free memory, etc.
    log_csv_close(log);

    return 0;
}