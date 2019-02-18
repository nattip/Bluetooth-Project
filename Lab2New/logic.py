import serial
import time
import io

#set up serial communication
ser = serial.Serial(
    port='COM7',
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=4
)

#infinite while loop
while True:
    #give users options
    def main():
        print("\nOptions:\n")
        print("1. Write poem from memory")
        print("2. Read poem from memory")
        print("3. Delete one")
        print("4. Delete them all")
        print("5. Show all poems")
        print("6. Show available memory\n")
        opt = int(input("Whatcha wanna do? "))

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

    #read the chosen index from flash and display poem stored there
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
        teststring_str=teststring.decode("UTF-8")
        ser.close()
        print("\n{}".format(teststring_str))

    #delete the poem in the selected index location
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

    #clear all of flash
    def delete_all():
        command="4"
        if ser.isOpen():
            ser.close()
        ser.open()
        ser.isOpen()
        ser.write(command.encode())
        ser.close()

    #show directory of what is stored in flash
    def show():
        command="2"
        if ser.isOpen():
            ser.close()
        ser.open()
        ser.isOpen()
        ser.write(command.encode())
        testindex=ser.read_until(b'\r',500)
        testindex_str=testindex.decode("UTF-8")

        location = 0
        look = "//"
        while location != -1:
            location = testindex_str.find(look, location+1, len(testindex_str))
            if location == -1:
                break
            testindex_str = testindex_str[:location] + 'Address: ' + testindex_str[location+2:]
            location = testindex_str.find(look,location+1, len(testindex_str))
            testindex_str = testindex_str[:location+5] + '\n' + testindex_str[location+5:]
            testindex_str = testindex_str[:location] + ' Size: ' + testindex_str[location+2:]

        ser.close()
        #print(testindex)
        print("\nDirectory:\n")
        print(testindex_str)
        
    #display the available memory in flash
    def memory():
        command="5"
        if ser.isOpen():
            ser.close()
        ser.open()
        ser.isOpen()
        ser.write(command.encode())
        mem=ser.read(4)
        mem_str=mem.decode("UTF-8")
        ser.close()
        print("\nAvailable memory: {} bytes / 7691 bytes".format(mem_str))

    #write a new poem in a chosen file location to next index in flash
    def write():
        filename = input("What is the file path? ")
        file = open(filename, "r")
        content = file.read()
        command = "0"
        size = len(content)
        length = str(size)
        if ser.isOpen():
            ser.close()
        ser.open()
        ser.isOpen()
        ser.write(command.encode())
        time.sleep(0.5)

        ser.write(length.encode())
        time.sleep(0.5)
        ser.write(content.encode())

        time.sleep(0.5)

        ser.close()

    #calls main
    if __name__ == '__main__':
        main()