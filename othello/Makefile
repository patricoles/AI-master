classes := ./cs380/othello/$(wildcard *.java)

cs380/othello/Test.class: $(classes)
	javac ./cs380/othello/*.java

run: cs380/othello/Test.class
	java cs380.othello.Test

clean:
	rm ./cs380/othello/*.class
