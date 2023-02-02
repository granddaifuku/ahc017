#!/bin/sh

SEEDS=seeds.txt
MAIN=main.cpp
EXE=main
SUM=sum.txt

# Generate Inputs
if [ $1 = "gen" ]; then
	if [ $# -ne 2 ]; then
		echo "Usage: ./test.sh gen \$num_inputs"
		exit 1
	fi
	
	echo "Create seeds.txt"
	if [ -e $SEEDS ]; then
		rm $SEEDS
	fi
	touch $SEEDS
	
	# input seeds
	seq 0 $2 > $SEEDS
	
	echo "Move to the tools directory"
	cd ./tools/
	./target/release/gen ../$SEEDS
	
	cd ..
	rm $SEEDS
elif [ $1 = "test" ]; then
	if [ ! -e $EXE ]; then
		echo "Compiling ..."
		g++ -o $EXE $MAIN
	fi
	
	touch $SUM
	echo "Move to the tools directory"
	cd ./tools/
	
	for file in `ls ./in`; do
		echo "testing $file ..."
		../$EXE < ./in/$file > ./out/$file 2>/dev/null

		if [ $? -ne 0 ]; then
			echo "Test unsuccessfully finished..."
			rm ../$SUM
			exit 1
		fi
		./target/release/vis ./in/$file ./out/$file >> ../$SUM 
	done
	
	cd ..
	awk '{sum+=$1}END{print "Sum", sum}' < $SUM
	rm $SUM
else
	echo "I do not know that command."
fi

# TODO : generate table and records the score and the description for each test
