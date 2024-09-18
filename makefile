obj = ./src/*.cpp

output:$(obj)
	g++ -o output $(obj)

clean:
	rm output



