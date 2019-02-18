import serial
import time
import io

ser = serial.Serial(
    port='COM7',
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=4
)
#ser_io=io.TextIOWrapper(io.BufferedRWPair(ser,ser,1), newline='\r',line_buffering=True)

while True:
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
        MemorySlot=input("Which memory slot? (0-9) ")
        command="1"
        if ser.isOpen():
            ser.close()
        ser.open()
        ser.isOpen()
        ser.write(command.encode())
        ser.write(MemorySlot.encode())
        teststring=ser.read_until(b'\r',300)
        ser.close()
        print(teststring)

    def delete():
        MemorySlot=input("Which memory slot? (0-9) ")
        command="3"
        if ser.isOpen():
            ser.close()
        ser.open()
        ser.isOpen()
        ser.write(command.encode())
        ser.write(MemorySlot.encode())
        ser.close()


    def delete_all():
        command="4"
        if ser.isOpen():
            ser.close()
        ser.open()
        ser.isOpen()
        ser.write(command.encode())
        ser.close()

    def show():
        command="2"
        if ser.isOpen():
            ser.close()
        ser.open()
        ser.isOpen()
        ser.write(command.encode())
        testindex=ser.read_until(b'\r',500)
        ser.close()
        print(testindex)

    def memory():
        command="5"
        if ser.isOpen():
            ser.close()
        ser.open()
        ser.isOpen()
        ser.write(command.encode())
        rip=ser.read(4)
        ser.close()
        print(rip)

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
        time.sleep(1)
        #out = ''
        #time.sleep(1)

        ser.write(length.encode())
        time.sleep(1)
        ser.write(content.encode())

        time.sleep(1)
        # while ser.inWaiting() > 0:
        #     out += ser.read(40)

        # if out != '':
        print(">>{}".format(length))
        ser.close()

    if __name__ == '__main__':
        main()