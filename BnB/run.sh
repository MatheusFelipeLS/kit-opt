echo "--BnB Benchmark--"

make

k=1
for instance in instances/*; do
	echo $instance >> ./result.txt

	echo "Running $instance"
	echo "Instance $k of 9" 

	./bnb ${instance} | awk "{print $1}" >> ./result.txt

	k=$(($k + 1))
done

echo "-" >> ./output.txt