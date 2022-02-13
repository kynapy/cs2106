#!/bin/bash

# Check if we have enough arguments
if [[ $# -eq 0 ]]; then
    echo "Usage: ./grade.sh <filename>"
elif [[ $# -gt 1 ]]; then
    echo "Usage: ./grade.sh <filename>"
else
# Delete temporary files
    rm ref/*.out

# Compile the reference program
    gcc ref/sum.c ref/utils.c -o ref/$1

# Generate reference output files
    for i in ref/*.in; do
        ref/$1 < $i > $i.out
    done

# Now mark submissions

#
# Note: See Lab02Qn.pdf for format of output file. Marks will be deducted for missing elements.
#
    echo "Test date and time: $(date +%A), $(date +%d) $(date +%B) $(date +%Y), $(date +%T)" > results.out
    echo "" >> results.out
    numFiles=0
# Iterate over every submission directory
    for i in subs/*; do 
        out=$(echo $i|cut -d'/' -f 2)
        gcc $i/sum.c $i/utils.c -o $i/a.out &> /dev/null # Compile C code

        if [[ $? -ne 0 ]]; then # Print compile error message to output file
            echo "Directory $out has a compile error." >> results.out 
        fi

    # Generate output from C code using *.in files in ref
        score=0
        sample=1
        for input in ref/*.in; do
            $i/a.out < $input > $i/$sample.out 2>/dev/null
            diff $i/$sample.out ref/"s"$sample".in".out &> /dev/null
            if [[ $? -eq 0 ]]; then
                score=$((score+1)) # Compare with reference output files and award 1 mark if they are identical
            fi
            sample=$((sample+1))
        done
        echo "Directory" "$out" "has a score $score / 3" >> results.out # print score for student
        numFiles=$((numFiles+1))
    done
    echo "" >> results.out
    echo -n "Processed $numFiles files." >> results.out # print total files marked.
fi