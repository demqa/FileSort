def remove_spaces(self):
    f = open("hamlet.txt","r")
    lines=f.readlines()
    f.close()
    f = open("hamlet_1.txt","w")   
    for line in lines:
        if line!="\n":
            f.write(line.lstrip())
	
remove_spaces(1)
