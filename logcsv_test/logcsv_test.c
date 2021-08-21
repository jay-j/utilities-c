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

    size_t id_time = log_csv_column_getid(log, "time");
    size_t id_pos = log_csv_column_getid(log, "position");
    size_t id_vel = log_csv_column_getid(log, "velocity");

    // fake some data
    for (size_t i=0; i<128; i++){
        log->data[id_time].value = (double) i;
        log->data[id_pos].value = (double) 2*i+3;
        log->data[id_vel].value = 3333 - (double) i;

        // each loop write new data to disk
        log_csv_write(log);
    }

    // now clean up, free memory, etc.
    log_csv_close(log);

    return 0;
}