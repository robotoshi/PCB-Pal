import glob
import os
import sys
import serial
import time
import signal
import getopt
import fileinput

try:
    import _winreg
except:
    pass


def signal_handler(sig, frame):
    """Handle a keyboard interrupt signal (SIGINT or Ctrl-C) and suppress the stacktrace."""
    print('\n\nReceived Ctrl-C.  Exiting...')
    sys.exit(0)


def eprint(*args, **kwargs):
    """Print to stderr"""
    print(*args, file=sys.stderr, **kwargs)


def serialList():
    """Return a list of valid serial ports.  Should work on Windows too?"""
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


def connect(portlist):
    """Given a list of potential serial ports, attempt to connect and then
    handshake with each of them, and return the first that was successful. Exit on failure."""
    for port in portlist:
        try:
            ser = serial.Serial(port, 250000, timeout=1)
            print("Connecting to ", port, "...", sep='')
            time.sleep(2)
            if "HELLO" in ser.readline().decode('ascii'):
                ser.flushInput()
                print("Shaking hands...")
                ser.write("HELLO\n".encode('ascii'))
                print("Handshake successful!")
                return ser
            else:
                eprint(port, "Handshake failed!")
        except serial.SerialTimeoutException:
            eprint("Timed out")
        except serial.SerialException:
            pass
    eprint("No serial ports found.  Exiting...")
    exit(2)


def openfile(name):
    """Open a file with the given name and return the handle. Exit on failure."""
    try:
        return open(name)
    except OSError as ex:
        eprint("No such file.  Exiting...")
        exit(2)


if __name__ == '__main__':
    signal.signal(signal.SIGINT, signal_handler)

    if len(sys.argv) != 2:
        eprint("Usage: python3", __file__, "<filename>")
        exit(1)

    printer = connect(serialList())
    file = openfile(sys.argv[1])
    print("Sending gcode file:", file.name, "\n\n")

    printing = True     # keeps the IDE from complaining
    try:
        while True:
            linebytes = printer.readline()
            try:
                line = linebytes.decode('ascii')
            except UnicodeDecodeError:
                line = None
                eprint("Can't decode", linebytes, "\nExiting...")
                exit(1)

            print("\t\t\t\t\t", line, end='')

            if "FINISHED" in line:
                break
            elif "DONE" in line and not printing:
                printer.write("|\n".encode('ascii'))
                print("\n>> |")
                continue
            elif "READY" in line:
                while True:
                    gcommand = file.readline()
                    if gcommand == '':      # end of file
                        printing = False
                        break
                    else:
                        gcommand = gcommand.strip().split(';')[0] + '\n'
                        if gcommand == '\n': continue       # if the line was just a comment, try the next one

                        print(">>", gcommand, end='')
                        printer.write(gcommand.encode('ascii'))
                        break
    except serial.SerialException:
        eprint("Printer disconnected! Exiting...")
        exit(3)
    finally:
        printer.write("|\n".encode('ascii'))
        file.close()
        printer.close()
