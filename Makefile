build:
	g++ -std=c++11 -o project2.out src/*.cpp
tasks:
	./project2.out output/part11.tga input/circles.tga multiply input/layer1.tga
	./project2.out output/part12.tga input/layer1.tga flip
	./project2.out output/part13.tga input/layer2.tga subtract input/layer1.tga

clean:
	rm project2.out