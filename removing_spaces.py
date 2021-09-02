def remove_spaces(file):
    f = open(file, "r")
    lines = f.readlines()
    f.close()
    f = open("new_" + file, "w")   
    for line in lines:
        if line != "\n":
            f.write(line.lstrip())
	
remove_spaces("Alice.txt")
