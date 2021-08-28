

def elimina_client(self):
    f = open("onegin.txt","r")
    lines=f.readlines()
    f.close()
    f = open("onegin1.txt","w")   
    for line in lines:
        if line!="\n":
            f.write(line)

elimina_client(1)
