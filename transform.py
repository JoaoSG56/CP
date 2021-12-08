file = open('list.txt','r')

lines = file.readlines()
output = ""
i = 0
for line in lines:
    line = line.replace('\n',"")
    output = output + line + ","
    i+=1
   
print(i*5)

cookedfile = open('cookedList','w+')
cookedfile.write(output[:-1])
