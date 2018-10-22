#!/usr/bin/env python
import sys
import struct
import random
import argparse
from collections import OrderedDict
import json

BIG_ENDIAN=False

# Maps the ply-supported string type identifies
# to the special characters used by struct and the equivalent C name
# (although the later could be system dependent)
# So standard identifiers are used. If necessary replace them by
# cstdint types, or define them specifically to your platform/compile
STR2TYPEC = {
    # "char": ("c", "char"),
    # "uchar": ("B", "unsigned char"),
    # "short": ("h", "short"),
    # "ushort": ("H", "unsigned short"),
    # "int": ("i", "int"),
    # "uint": ("I", "unsigned int"),
    # "float": ("f", "float"),
    # "double": ("d", "double"),
    "char": ("c", "int8_t"),
    "uchar": ("B", "uint8_t"),
    "short": ("h", "int16_t"),
    "ushort": ("H", "uint16_t"),
    "int": ("i", "int32_t"),
    "uint": ("I", "uint32_t"),
    "float": ("f", "float"),
    "double": ("d", "double"),
}

def generate_header(elements):
    header = ["ply",
    "format " + ("binary_big_endian 1.0" if BIG_ENDIAN else "binary_little_endian 1.0"),
    ]

    for name, spec in elements.items():
        header += ["element " + name + " " + str(spec["n"])]
        header += [" ".join(["property"]+prop[:-1]) if prop[0] == "list" else " ".join(["property"]+prop) for prop in spec["properties"]]

    header += ["end_header\n"]
    return "\n".join(header)

def write_ply(path, elements, big_endian=False):
    count = 0
    encoding = '>' if big_endian else '<'
    with open(path + ".ply", "wb") as f:
        f.write(generate_header(elements).encode('ascii'))
        for name, el in elements.items():
            for i in range(el["n"]):
                for p in el["properties"]:
                    if p[0] == "list":
                        f.write(struct.pack(encoding + STR2TYPEC[p[1]][0], p[-1]))
                        for j in range(p[-1]):
                            f.write(struct.pack(encoding + STR2TYPEC[p[2]][0], count%128))
                            count += 1
                    else:
                        f.write(struct.pack(encoding + STR2TYPEC[p[0]][0], count%128))
                        count += 1

def write_code(path, elements, pad="    ", append_name=""):
    file = "/*****************************************************\n" \
           " * Automatically generated. Please verify that types *\n" \
           " * used have the correct size on your platform.      *\n" \
           " *****************************************************/\n" \
           "#include <cstdint>\n#include <cstring>\n\n"

    for name, spec in elements.items():
        file += "struct __attribute__ ((packed)) {}{} {{\n".format(name.capitalize(), append_name)

        # the data fields
        for prop in spec["properties"]:
            if prop[0] == "list":
                file += pad + "const {} {} = {};\n".format(STR2TYPEC[prop[1]][1], prop[3]+"_length", prop[-1])
                file += pad + "const {} {}[{}];\n".format(STR2TYPEC[prop[2]][1], prop[3], prop[-1])
            else:
                file += pad + "const {} {};\n".format(STR2TYPEC[prop[0]][1], prop[1])

        # some convenience functions like bool operator==
        file += "\n" + pad + "bool operator==(const {}& rhs) const {{\n".format(name.capitalize())
        file += 2*pad + "return "
        for idx, prop in enumerate(spec["properties"]):
            if idx > 0:
                file += 2*pad+7*" "

            if prop[0] == "list":
                file += "!std::memcmp(&{}, &rhs.{}, sizeof({})*{})".format(prop[3], prop[3], STR2TYPEC[prop[2]][1], prop[-1])
            else:
                file += "{} == rhs.{}".format(prop[1], prop[1])

            file += " &&\n" if idx < len(spec["properties"])-1 else ";\n"

        file += pad + "}\n"
        file += "};\n\n"

    with open(path + ".h", "w") as f:
        f.write(file)


def main():
    parser = argparse.ArgumentParser(description='Generate the c-code struct definitions and ply files for testing.')
    parser.add_argument('--header', '-i', type=str, required=True, help='Header definition')
    parser.add_argument('--out', '-o', type=str, required=True, help='Output path + the basename of files to generate')
    parser.add_argument('--ply', '-p', dest='gen_ply', action='store_true', help="Generate a ply files with some bogus data")
    parser.add_argument('--code', '-c', dest='gen_code', action='store_true', help="Generate struct definitions for usage in C++")
    parser.add_argument('--verbose', '-v', dest='verbose', action='store_true', help="Additional outputs")
    parser.add_argument('--big_endian', dest='big_endian', action='store_true', help="Encode big endian instead of little endian")
    args = parser.parse_args()

    with open(args.header) as f:
        elements = json.load(f, object_pairs_hook=OrderedDict)

    if args.verbose:
        print("Generated header:")
        print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>")
        print(generate_header(elements))
        print("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<")

    if args.gen_ply:
        ply = write_ply(args.out, elements, args.big_endian)

    if args.gen_code:
        code = write_code(args.out, elements)



if __name__ == '__main__':
    main()