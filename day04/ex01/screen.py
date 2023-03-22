import serial
import signal

def handler(signum, frame):
    raise Exception

def main(device="/dev/ttyUSB0", baud=115200):
    try:
        with serial.Serial(device, baud) as ser:
            while True:
                print(ser.read().decode(), end='')
    except Exception:
        print("")
        print(" Done ".center(30, "-"))

if __name__ == "__main__":
    signal.signal(signal.SIGINT, handler)
    main()

