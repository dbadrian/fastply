#!/usr/bin/env python
import sys
import struct
import random

from tqdm import tqdm

VERTICES=15000000
BIG_ENDIAN=False

def gen_header():
    encoding = "binary_big_endian 1.0" if BIG_ENDIAN else "binary_little_endian 1.0"
    return """ply
format """ + encoding +\
"\nelement vertex " + str(VERTICES) +\
"""\nproperty float x
property float y
property float z
property float nx
property float ny
property float nz
property uchar red
property uchar green
property uchar blue
end_header\n"""

def main():
    print(gen_header())
    if len(sys.argv) != 2:
        print("Not enough arguments")
        exit()

    out_file = sys.argv[1]

    encoding = '>' if BIG_ENDIAN else '<'
    with open(out_file, "wb") as f:
        with open(out_file + "ascii_data", "w") as fa:
            f.write(gen_header().encode('utf-8'))

            for i in tqdm(range(VERTICES)):
                p = random.random()
                f.write(struct.pack(encoding + 'f', p))
                fa.write(str(p) + " ")
                p = random.random()
                f.write(struct.pack(encoding + 'f', p))
                fa.write(str(p) + " ")
                p = random.random()
                f.write(struct.pack(encoding + 'f', p))
                fa.write(str(p) + " ")
                p = random.random()
                f.write(struct.pack(encoding + 'f', p))
                fa.write(str(p) + " ")
                p = random.random()
                f.write(struct.pack(encoding + 'f', p))
                fa.write(str(p) + " ")
                p = random.random()
                f.write(struct.pack(encoding + 'f', p))
                fa.write(str(p) + " ")
                i = random.getrandbits(7)
                f.write(struct.pack(encoding + 'B', i))
                fa.write(str(i) + " ")
                f.write(struct.pack(encoding + 'B', i))
                fa.write(str(i) + " ")
                f.write(struct.pack(encoding + 'B', i))
                fa.write(str(i) + "\n")




if __name__ == '__main__':
    main()