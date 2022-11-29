# find the files based on user input and read in the hash
# NOTE: edit the files such that ONLY the hash remains
file1 = input("Enter the first file path, H1: ")
file2 = input("Enter the second file path, H2: ")

file1_hash = open(file1, "r").read()
file2_hash = open(file2, "r").read()

# setup for the hexadecimal conversion
hex = 16
num_bits = 4 * (len(file1_hash) - 1)
#print("Hex characters:", len(file1_hash))
#print("Number of bits:", num_bits)

# convert the hash from the files into bits
file1_binary = bin(int(file1_hash, hex))[2:].zfill(num_bits)
file2_binary = bin(int(file2_hash, hex))[2:].zfill(num_bits)

# perform comparison
count = 0
for i in range(len(file1_binary)):
	if (file1_binary[i] != file2_binary[i]):
		count += 1

# print the output
print("Binary output of File 1:", file1_binary)
print("Binary output of File 2:", file2_binary)
print(num_bits - count, "similar bits between H1 and H2")
print(count, "differing bits between H1 and H2")

# close the files just in case
