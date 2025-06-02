'''
Add some '\n' to the json file
'''

file_path = r'.\bestdori\296_special.bestdori'
with open(file_path, 'r') as file:
	lines = file.readlines()
string = ""
for line in lines:
	string += line

output = ""
n = len(string)
cnt = 0
for i in range(n):
	if string[i] == '{':
		cnt += 1
	elif string[i] == '}':
		cnt -= 1
	elif string[i] == '\n':
		continue
	output += string[i]
	if cnt == 0 and string[i] == ',':
		output += '\n'
with open(file_path, 'w') as file:
	file.write(output)