while True:
    name = input("Jméno: ");
    size = int(input("Velikost: "));
    mosi = int(input("MOSI: "));
    miso = int(input("MISO: "));
    clk = int(input("CLK: "));
    reset = int(input("RESET: "));
    gnd1 = int(input("GND1: "));
    gnd2 = int(input("GND2: "));
    vcc1 = int(input("VCC1: "));
    vcc2 = int(input("VCC2: "));

    def q(i):
        if i==255:
            return 255
        if i > size//2:
            return 3*size//2 - i
        else:
            return i-1
        
    print(f"const Chip {name} = {{{size}, {q(mosi)}, {q(miso)}, {q(clk)}, {q(reset)}, {{{q(gnd1)}, {q(gnd2)}}}, {{{q(vcc1)}, {q(vcc2)}}}}};")
