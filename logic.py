import serial
import time

ser = serial.Serial(
    port='COM7',
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS
)


def main():
    print("UART Thing")
    print("1. Write poem from memory")
    print("2. Read poem from memory")
    print("3. Delete one")
    print("4. Delete them all")
    print("5. Show all poems")
    print("6. Show available memory")
    opt = int(input("Whatcha wanna do"))

    if opt == 1:
        write()
    elif opt == 2:
        read()
    elif opt == 3:
        delete()
    elif opt == 4:
        delete_all()
    elif opt == 5:
        show()
    elif opt == 6:
        memory()


def read():
    pass


def delete():
    pass


def delete_all():
    pass


def show():
    pass


def memory():
    pass


def write():
    filename = input("What is the file path? ")
    file = open(filename, "r")
    #content = "0-"
    content = file.read()
    command = "0"
    size = len(content)
    length = str(size)
    if ser.isOpen():
        ser.close()
    ser.open()
    ser.isOpen()
    ser.write(command.encode())
    #out = ''
    #time.sleep(1)
    ser.write(length.encode())
    ser.write(content.encode())

    time.sleep(1)
    # while ser.inWaiting() > 0:
    #     out += ser.read(40)

    # if out != '':
    print(">>{}".format(length))
    ser.close()

if __name__ == '__main__':
    main()