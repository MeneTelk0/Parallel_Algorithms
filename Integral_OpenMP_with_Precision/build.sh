#!/bin/bash

echo -e "START \n"
# Building this program on Mac OS
echo "Compilation:"

if clang -Xpreprocessor -fopenmp -lomp integral_OMP_precision.c -o integral_OMP_precision; then 
    echo "Compilation integral_OMP_precision.c - SUCCESS"
else 
    echo "Can't compile integral_OMP_precision.c file. Aborting" 1>&2
    exit 1
fi

if gcc calc_data.c -o calc_data; then 
    echo "Compilation calc_data.c - SUCCESS"
else 
    echo "Can't compile calc_data.c file. Aborting" 1>&2
    exit 1
fi

echo -e "\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% \n"

echo -e "Checking ./res directory:"
# Clean ./res directory if it is present 
# or make it
if file -d ./res; then 
    rm -rf ./res
    mkdir ./res
    echo "Directory ./res is present and cleaned"
else
    echo "Directory ./res doesn't exist. Making ./res"
    mkdir ./res
fi
echo -e "\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"

echo "Making of data.txt file:"
for ((i = 1; i < 5; ++i))
do
    if (echo -n "$i " && OMP_NUM_THREADS=$i ./integral_OMP_precision $1)  >> res/data.txt; then 
        echo "$i proc - SUCCESS"
    else
        echo "$i proc - ERROR. Aborting" 1>&2
        exit 1
    fi
done

echo -e "\nYour data.txt file:"
cat res/data.txt

echo -e "\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"

echo "Making acceleration.txt and efficiency.txt:"
if ./calc_data; then 
    echo "SUCCESS"
else 
    "ERROR. Aborting" 1>&2
    exit 1
fi

echo -e "\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"

echo "Plotting metrics"
if gnuplot <<< "set terminal png size 1024, 720; \
            set xlabel 'Number of processes'; \
            set ylabel 'Acceleration'; \
            set xrange [1:4]; \
            set output 'res/acceleration.png'; \
            plot 'res/acceleration.txt' with lines; \
            set terminal png size 1024, 720; \
            set xlabel 'Number of processes'; \
            set ylabel 'Efficiency'; \
            set xrange [1:4]; \
            set output 'res/efficiency.png'; \
            plot 'res/efficiency.txt' with lines; "; then 
    echo "SUCCESS - Plot images are stored at res"
else
    echo "ERROR. Aborting" 1>&2
    exit 1
fi

echo -e "\nEND"