echo "--SLA Benchmark--"

make

k=1
for instance in instances/*; do
	echo $instance >> ./results.txt

	echo "Running $instance"
	echo "Instance $k of ???" 

	./bpp ${instance} | awk "{print $1}" >> ./results.txt

	k=$(($k + 1))
done

echo "-" >> ./results.txt