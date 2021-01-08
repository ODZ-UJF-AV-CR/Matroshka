#include <stdio.h>
#include <stdlib.h>

#define SINGLE_CHIP_PIXSIZE      65536

int main (int argc, char const* argv[])
{
    int min_frames = atoi(argv[1]);
    int max_frames = atoi(argv[2]);
    double measurement_time = atof(argv[3]);
    int zeros = atoi(argv[4]);
    int num_of_frames = atoi(argv[5]);
    double acq_time = atof(argv[6]);

    double current_frame = 60 / acq_time;
    int out_frame;
    double out_time;
    double next_frame = current_frame;

    FILE *f = fopen("parameter.txt", "w");
    if (f == NULL){
	exit(1);
    }

    double zeros_total = num_of_frames * SINGLE_CHIP_PIXSIZE;;
    double occupancy = (zeros_total - zeros) / zeros_total;
    printf("%f\n", occupancy);

    if (occupancy > 0.03 || occupancy < 0.01){
	next_frame = (occupancy / 0.02) * current_frame;
	if (next_frame > max_frames)
	    next_frame = max_frames;
	if (next_frame < min_frames)
	    next_frame = min_frames;
    }

    out_time = 60.0 / next_frame;
    out_frame = measurement_time / (double)out_time;

    fprintf(f, "%d\n", out_frame);
    fprintf(f, "%f", out_time);
    fclose(f);
    return 0;
} 
