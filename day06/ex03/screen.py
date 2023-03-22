from threading import Thread
import serial
import signal

run = True
def handler(sig, frame):
    global run
    print("here")
    run = False

def getInput(device="/dev/ttyUSB0", baud=115200):
    with serial.Serial(device, baud) as ser:
        while run:
            data = input()

            # Si des données ont été entrées, les envoyer sur le port série
            if data:
                ser.write(data.encode())


def main(device="/dev/ttyUSB0", baud=115200):
    print(" Start ".center(30, "-"))
    with serial.Serial(device, baud) as ser:
        while run:
            print(ser.read().decode(), end='')

    print("")
    print(" Done ".center(30, "-"))

if __name__ == "__main__":
    signal.signal(signal.SIGINT, handler)

    t1 = Thread(target=getInput)
    t2 = Thread(target=main)

    t1.start()
    t2.start()

    t1.join()
    t2.join()

    run = False