from elftools.elf.elffile import ELFFile
from pyocd.core.helpers import ConnectHelper
from pyocd.flash.file_programmer import FileProgrammer
from argparse import ArgumentParser


parser = ArgumentParser(description="Patch and flash the Tennis-Dongle firmware.")
parser.add_argument(
    "--netid", type=int, required=True, help="The netID to patch. (0-80)"
)

parser.add_argument(
    "--parts", type=int, required=True, help="The nPartitions to patch. (1-16)"
)

parser.add_argument(
    "--part-pixels",
    type=int,
    required=True,
    help="The pixelsPerPartition to patch.",
)

parser.add_argument(
    "--padding-pixels",
    type=int,
    required=True,
    help="The nPaddingPixels to patch. ",
)


from subprocess import Popen, STDOUT

parser.add_argument(
    "--file",
    action="store",
    type=str,
    default=".pio/build/genericSTM32G030C8/firmware.elf",
    help="The path to the ELF file to patch. (default: .pio/build/genericSTM32G030C8/firmware.elf)",
)
args = parser.parse_args()

DATA_SECTION_NAME = ".rodata"
NET_ID_NAME = "netID"
N_PARTS_NAME = "nPartitions"
PART_PIXELS_NAME = "pixelsPerPartition"
PADDING_PIXELS_NAME = "nPaddingPixels"


def fuckOff(code):
    input("Press Enter to continue...")
    exit(code)


if args.netid < 0 or args.netid > 80:
    print("netID must be between 0 and 80")
    fuckOff(1)

if args.parts < 1 or args.parts > 16:
    print("netID must be between 1 and 16")
    fuckOff(1)

if (args.parts * (args.part_pixels + args.padding_pixels) - args.padding_pixels) > 2048:
    print("Too many pixels")
    fuckOff(1)

try:
    with open(args.file, "r+b") as fuckingFile:
        elf = ELFFile(fuckingFile)
        dataSection = elf.get_section_by_name(DATA_SECTION_NAME)
        symtabSection = elf.get_section_by_name(".symtab")
        if dataSection is None:
            print("Section {} not found".format(DATA_SECTION_NAME))
            fuckOff(1)
        if symtabSection is None:
            print("Symbol table section not found")
            fuckOff(1)

        sectionAddr = dataSection["sh_addr"]
        sectionOffset = dataSection["sh_offset"]

        print("Section {} found".format(DATA_SECTION_NAME))
        print("Section size: {}".format(dataSection.data_size))
        print("Section address: {}".format(hex(sectionAddr)))
        print()

        netID_Offset = -1
        nParts_Offset = -1
        partPixels_Offset = -1
        paddingPixels_Offset = -1

        for sym in symtabSection.iter_symbols():
            if sym.name == NET_ID_NAME:
                print("Symbol {} found".format(NET_ID_NAME))
                addr = sym["st_value"]
                netID_Offset = addr + sectionOffset - sectionAddr
                continue

            if sym.name == N_PARTS_NAME:
                print("Symbol {} found".format(N_PARTS_NAME))
                addr = sym["st_value"]
                nParts_Offset = addr + sectionOffset - sectionAddr
                continue

            if sym.name == PART_PIXELS_NAME:
                print("Symbol {} found".format(PART_PIXELS_NAME))
                addr = sym["st_value"]
                partPixels_Offset = addr + sectionOffset - sectionAddr
                continue

            if sym.name == PADDING_PIXELS_NAME:
                print("Symbol {} found".format(PADDING_PIXELS_NAME))
                addr = sym["st_value"]
                paddingPixels_Offset = addr + sectionOffset - sectionAddr
                continue

        if netID_Offset == -1:
            print("Symbol {} not found".format(NET_ID_NAME))
            fuckOff(1)
        if nParts_Offset == -1:
            print("Symbol {} not found".format(N_PARTS_NAME))
            fuckOff(1)
        if partPixels_Offset == -1:
            print("Symbol {} not found".format(PART_PIXELS_NAME))
            fuckOff(1)
        if paddingPixels_Offset == -1:
            print("Symbol {} not found".format(PADDING_PIXELS_NAME))
            fuckOff(1)

        fuckingFile.seek(netID_Offset)
        fuckingFile.write(args.netid.to_bytes(1, byteorder="little"))
        print("netID patched to {}".format(args.netid))

        fuckingFile.seek(nParts_Offset)
        fuckingFile.write(args.parts.to_bytes(1, byteorder="little"))
        print("nPartitions patched to {}".format(args.parts))

        fuckingFile.seek(partPixels_Offset)
        fuckingFile.write(args.part_pixels.to_bytes(1, byteorder="little"))
        print("pixelsPerPartition patched to {}".format(args.part_pixels))

        fuckingFile.seek(paddingPixels_Offset)
        fuckingFile.write(args.padding_pixels.to_bytes(1, byteorder="little"))
        print("nPaddingPixels patched to {}".format(args.padding_pixels))
        print("Patching complete")

        fuckingFile.close()


except Exception as e:
    print("Failed to open the ELF file:", e)
    fuckOff(1)

try:
    with ConnectHelper.session_with_chosen_probe(
        options={"target_override": "stm32g030c8tx"}
    ) as session:
        FileProgrammer(session).program(args.file)
        session.target.reset()

except Exception as e:
    print("Failed to flash the firmware:", e)
    fuckOff(1)

fuckOff(0)
