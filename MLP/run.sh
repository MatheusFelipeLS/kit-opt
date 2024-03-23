echo "--MLP Benchmark--"

make

k=1
for instance in instances/*; do
	echo $instance >> ./output.txt

	echo "Running $instance"
	echo "Instance $k of 22" 

	./mlp ${instance} | awk "{print $1}" >> ./output.txt

	k=$(($k + 1))
done

echo "-" >> ./output.txt