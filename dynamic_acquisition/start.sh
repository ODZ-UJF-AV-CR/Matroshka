extension1=".pmf"
extension2=".pmf.dsc"
extension3=".pmf.idx"
path="/home/user/Documents"

i=0
min_frames=3 # minimum number of frames per minute = 3 (aqcisition time = 20 s)
max_frames=150 # maximum number of frames per minute = 150 (aqcisition time = 0.4 s)
measurement_time=30 # duration of measurement in seconds

# start measurement with minumum frames per minute
num_of_frames=3
acq_time=20

while (( $i<1 ))
do
	# condition which decides whether configuration file should be loaded
        if (( $i==0 )); then
            condition=1
	else
	    condition=0
	fi
        ((i++))

	# run measurement
        ./pxcapi $condition $num_of_frames $acq_time # 1- configuration?, 2- num. of frames, 3- acq. time

	# calculate average occupancy of frames (only detector I08)
	zeros=$(grep -o -i 0 I08_data_$i$extension1 | wc -l) # count zeros in all frames\

	# run C program which calculates the new num_of_frames and acq_time and saves them to file
	./dynamic_acqisition $min_frames $max_frames $measurement_time $zeros $num_of_frames $acq_time
	
	# get num_of_frames and acq_time from .txt file
	num_of_frames=$(cat "parameter.txt" | sed -n '1p')
	acq_time=$(cat "parameter.txt" | sed -n '2p')

	# tar files
	tar -cf I08_data_$i.tar I08_data_$i$extension1 I08_data_$i$extension2 I08_data_$i$extension3
	tar cf H08_data_$i.tar H08_data_$i$extension1 H08_data_$i$extension2 H08_data_$i$extension3
	
	# compress files
    xz -z I08_data_$i.tar
    xz -z H08_data_$i.tar

	# generate time
	NOW=$( date '+%F_%H:%M:%S' )

	# move files
	mv I08_data_$i.tar.xz $path/I08_data_$NOW.tar.xz
	mv H08_data_$i.tar.xz $path/

	# remove original files
	rm I08_data_$i$extension1 I08_data_$i$extension2 I08_data_$i$extension3
	rm H08_data_$i$extension1 H08_data_$i$extension2 H08_data_$i$extension3

done
