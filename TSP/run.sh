echo "--TSP Benchmark--"

make

k=1
for instance in instances/*; do
	echo $instance >> ./output.txt

	echo "Running $instance"
	echo "Instance $k of 68" 

	./tsp ${instance} | awk "{print $1}" >> ./output.txt

	k=$(($k + 1))
done

echo "-" >> ./output.txt