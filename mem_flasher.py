from time import sleep
import sys
import os
import serial


#####################
# Display FUNCTIONS #
#####################
def display_title_bar():
    # Clears the terminal screen, and displays a title bar.
    os.system('clear')
              
    print("\t**********************************************")
    print("\t***   KIRNO - Memory Flasher - SST25 -     ***")
    print("\t**********************************************")
    print("\n")


def display_os_info():
    print('Python %s on %s' % (sys.version, sys.platform))
    scriptpath = os.path.realpath(__file__)
    print("[Script path] " + scriptpath)
    # sleep(1)


##################
# Menu Functions #
##################
def read_memory_addr (serial_instance, addr_hex, bytes_dec):
    s = serial_instance

    to_send = f'read_from_mem {addr_hex} {bytes_dec}\n'
    s.write(to_send.encode('utf-8'))

    loop = True

    while loop:
        answer = s.readline().decode()        

        if 'ok' in answer or 'nok' in answer:
            loop = False
        else:
            print(answer)

            
def read_all_memory (serial_instance):
    s = serial_instance
    s.write("read all\n".encode('utf-8'))

    loop = True

    print ("Reading all Memory - will take a while")
    while loop:
        answer = s.readline().decode()        

        if 'OK' in answer or 'NOK' in answer:
            loop = False
        elif 'go to binary' in answer:
            print("saving data on mem.bin")
            needed, saved, crc = save_binary_file(s, "mem.bin")
            print(f'Bytes: {needed} Saved: {saved} CRC: {crc}')
            # falta leer el OK final
            loop = False
        else:
            print(answer)

    print(os.popen("./crc16_ccitt mem.bin").read())


def write_file_to_mem (serial_instance, address, file_name):
    s = serial_instance
    file_size = 0
    with open(file_name, "rb") as f:
        f.seek(0, os.SEEK_END)
        file_size = f.tell()
        f.seek(0, os.SEEK_SET)

    addr_int = int(address, 0)
    if (addr_int % 4096) != 0:
        print(f'address: {address} int: {addr_int} is not multiple of 4KB!')
        quit()
        
    mystr = f'write_file_to_mem {address} {file_size}\n'
    s.write(mystr.encode('utf-8'))

    loop = True

    print (f"Writing file {file_name} to Memory - will take a while")
    while loop:
        answer = s.readline().decode()        

        if 'ok' in answer or 'nok' in answer:
            loop = False
        elif 'go to binary' in answer:
            rx, saved = send_binary_file(s, file_name)
            print(f'Bytes: {rx} Saved: {saved}')
            print(f'Address init: {hex(addr_int)} Address end: {hex(addr_int + int(saved))}')            
            # falta leer el OK final
            loop = False
        else:
            print(answer)

    

def read_silicon_id (serial_instance):
    s = serial_instance

    flush_serial_buffer(s)
    
    s.write("read sid\n".encode('utf-8'))
    loop = True

    while loop:
        answer = s.readline().decode()        

        if 'OK' in answer or 'NOK' in answer:
            loop = False
        else:
            print(answer)
            

def read_manufacturer_id (serial_instance):
    s = serial_instance
    s.write("read mid\n".encode('utf-8'))

    loop = True

    while loop:
        answer = s.readline().decode()        

        if 'OK' in answer or 'NOK' in answer:
            loop = False
        else:
            print(answer)


def read_protected (serial_instance):
    s = serial_instance
    s.write("read prot\n".encode('utf-8'))

    loop = True

    while loop:
        answer = s.readline().decode()        

        if 'OK' in answer or 'NOK' in answer:
            loop = False
        else:
            print(answer)


def blank_check (serial_instance):
    print ("Blank Checking - will take a few seconds")
    s = serial_instance
    s.write("blank check\n".encode('utf-8'))

    loop = True

    while loop:
        answer = s.readline().decode()        

        if 'OK' in answer or 'NOK' in answer:
            loop = False
        else:
            print(answer)


def erase_all_memory (serial_instance):
    print ("Erasing All Memory - will take a few seconds")
    s = serial_instance
    s.write("erase all\n".encode('utf-8'))

    loop = True

    while loop:
        answer = s.readline().decode()        

        if 'OK' in answer or 'NOK' in answer:
            loop = False
        else:
            print(answer)
            

def write_memory_addr (serial_instance):
    s = serial_instance

    flush_serial_buffer(s)
    addr = input("Address to write in decimal: ")
    value = input("Byte value: ")    

    to_send = f'write addr {addr} {value}\n'
    s.write(to_send.encode('utf-8'))

    loop = True

    while loop:
        answer = s.readline().decode()        

        if 'OK' in answer:
            loop = False
            print("Done!")
        elif 'NOK' in answer:
            loop = False
            print("Error")
        else:
            print(answer)


def get_memory_crc (serial_instance):
    print ("Reading CRC - will take a few seconds")
    s = serial_instance
    s.write("read crc\n".encode('utf-8'))

    loop = True

    while loop:
        answer = s.readline().decode()        

        if 'OK' in answer or 'NOK' in answer:
            loop = False
        else:
            print(answer)

            
#####################
# Utility Functions #
#####################
def check_empty_mem (serial_instance):
    s = serial_instance

    flush_serial_buffer(s)
    checked = 0
    while checked < 524288:
        qtty = s.in_waiting
        if qtty:
            read = s.read(qtty)
            for r in read:
                if r != 0xff:
                    print ("Error in memory addr: " + hex(checked) + " value: " + hex(r))
                    checked = 524288
                    break
                

            checked += qtty

def memory_reset (serial_instance):
    s = serial_instance

    flush_serial_buffer(s)
    
    s.write("mem reset\n".encode('utf-8'))
    loop = True

    while loop:
        answer = s.readline().decode()        

        if 'OK' in answer:
            loop = False
            print("Done!")
        elif 'NOK' in answer:
            loop = False
            print("Error")
        else:
            print(answer)

            
# def save_binary_file (serial_instance, file_name):
#     s = serial_instance
#     needed_bytes = 0
#     saved_bytes = 0
#     show_bytes = 0
#     crc = ""
    
#     with open(file_name, "wb") as f:
#         loop = True

#         while loop:
#             answer = s.readline().decode()        
            
#             if 'ok' in answer or 'nok' in answer:
#                 loop = False
#             elif 'next' in answer:
#                 s_next, next_bytes = answer.split(' ')
#                 next_bytes = int(next_bytes)
#                 needed_bytes += next_bytes
#                 s.write("next\n".encode('utf-8'))
#                 qtty_in_this_chunk = 0

#                 if show_bytes >= 65536:
#                     print(needed_bytes)
#                     show_bytes = 0
#                 else:
#                     show_bytes += next_bytes

#                 inner_loop = True
#                 while inner_loop:
#                     qtty = s.in_waiting

#                     if qtty_in_this_chunk == next_bytes:
#                         inner_loop = False
                        
#                     if qtty:
#                         #reviso no leer de mas
#                         if (qtty + qtty_in_this_chunk) > next_bytes:
#                             qtty = next_bytes - qtty_in_this_chunk

#                         read = s.read(qtty)
#                         f.write(read)
#                         saved_bytes += qtty
#                         qtty_in_this_chunk += qtty

#     return needed_bytes, saved_bytes, crc


def send_binary_file (serial_instance, file_name):
    s = serial_instance
    sended_bytes = 0
    padding_bytes = 0    
    rx_bytes = ""
    svd_bytes = ""
    
    with open(file_name, "rb") as f:
        loop = True
        f.seek(0, os.SEEK_END)
        file_size = f.tell()
        f.seek(0, os.SEEK_SET)
        file_end = False
        
        s.write("next\n".encode('utf-8'))    #para el primer paquete
        print("first next")
        while loop:
            answer = s.readline().decode()        

            #envio el primer next            
            if 'Timeout' in answer:
                print("Timeout")
                loop = False
                
            elif 'Rx:' in answer:
                #Rx: %d Svd: %d
                loop = False
                s_rx, rx_bytes, s_svd, svd_bytes = answer.split(' ')
                
            elif 'n' in answer:
                print("n getted")
                # envio primero o proximo paquete
                padding = False
                for i in range(1024):
                    # s.write(10)
                    # print('.', end='')
                    # sended_bytes += 1
                    b = f.read(1)
                    if b == b'':
                        file_end = True
                        s.write(b'0')
                        padding_bytes += 1
                    else:
                        sended_bytes += 1
                        s.write(b)
                    #     print('.', end='')

                print(f'sended: {sended_bytes} padding: {padding_bytes}')
                # end of packet loop to wait for answer
                inner_loop = True
                while inner_loop:
                    answer = s.readline().decode()
                    if '.' in answer:
                        print(". getted")
                        inner_loop = False
                        if file_end:
                            s.write("ended\n".encode('utf-8'))
                            print(f'file size: {file_size} sended: {sended_bytes} padding: {padding_bytes}')
                        else:
                            print("next")
                            s.write("next\n".encode('utf-8'))
                    elif 'Timeout' in answer:
                        print('Timeout at end of pckt')
                        inner_loop = False
                        loop = False
                    else:
                        print(answer)
                        

    return rx_bytes, svd_bytes


                
def flush_serial_buffer (serial_instance):
    s = serial_instance
    qtty = s.in_waiting
    if qtty:
        read = s.read(qtty)
        print("Flushing Buffer")



def functions_options ():
    print("list of functions:")
    print(" read_from_mem")
    print(" write_to_mem")
    print(" write_file_to_mem")
    print()

    
### MAIN PROGRAM ###    
if __name__ == "__main__":
    args_len = len(sys.argv)
    # script_func = sys.argv[1]

    print(f"len of args: {args_len}")
    if args_len < 2:
        print ("use script name and functions")
        functions_options()
        quit()

    display_os_info()
    # para el puerto serie
    # port = '/dev/ttyACM0'
    port = '/dev/ttyUSB0'    
    velocidad = 115200
    # velocidad = 9600    

    try:
        ser = serial.Serial(port, velocidad)
        if (ser != None):
            print ("Serial Port Open on: " + port + " at: " + str(velocidad) + "bps")
            port_open = True
            display_title_bar()
    except:
        print ("Serial Port Not Open")
        quit()

    # for read from mem we need 2 more args        
    if sys.argv[1] == "read_from_mem" and \
       args_len == 4:
        read_memory_addr (ser, sys.argv[2], sys.argv[3])
    elif sys.argv[1] == "write_file_to_mem" and \
       args_len == 4:
        write_file_to_mem (ser, sys.argv[2], sys.argv[3])
    else:
        functions_options()

    print("quitting!")
    sleep(2)
    # TestObjects()
        

        
