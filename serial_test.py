import glob
import os
import sys
import serial
import fileinput

try:
    import _winreg
except:
    pass


def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)


def serialList():
    baselist = []
    if os.name == "nt":
        try:
            key = _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\SERIALCOMM")
            i = 0
            while (1):
                baselist += [_winreg.EnumValue(key, i)[1]]
                i += 1
        except:
            pass
    baselist = baselist \
               + glob.glob("/dev/ttyUSB*") \
               + glob.glob("/dev/ttyACM*") \
               + glob.glob("/dev/tty.usb*") \
               + glob.glob("/dev/cu.*") \
               + glob.glob("/dev/cuaU*") \
               + glob.glob("/dev/rfcomm*")

    # prev = settings().get(["serial", "port"])
    # if prev in baselist:
    #     baselist.remove(prev)
    #     baselist.insert(0, prev)
    # if settings().getBoolean(["devel", "virtualPrinter", "enabled"]):
    #     baselist.append("VIRTUAL")
    return baselist


def connect():
    for port in serialList():
        try:
            return serial.Serial(port, 9600)
        except serial.SerialException:
            pass
    eprint("No serial ports found.  Exiting...")
    exit(1)
    return None


with connect() as ser:
    print("ready to echo: \n")

    for line in fileinput.input():
        ser.write(line.encode('ascii'))
        echo = ser.readline().decode('ascii')
        print("\t\t", echo)
