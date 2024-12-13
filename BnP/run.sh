echo "--SLA Benchmark--"

make

k=1
for instance in instances/*; do
	echo $instance >> ./combo.txt

	echo "Running $instance"
	echo "Instance $k of ???" 

	./bpp ${instance} | awk "{print $1}" >> ./combo.txt

	k=$(($k + 1))
done

echo "-" >> ./combo.txt