echo "--SLA Benchmark--"

make

k=1
for instance in instances/*; do
	echo $instance >> ./column_generation_result.txt

	echo "Running $instance"
	echo "Instance $k of ???" 

	./bpp ${instance} | awk "{print $1}" >> ./column_generation_result.txt

	k=$(($k + 1))
done

echo "-" >> ./column_generation_result.txt