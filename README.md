MCU = atmega328p
F_CPU = 16000000UL
TARGET = main
SRC = main.c lcd_i2c.c

$(TARGET).hex: $(TARGET).elf
	avr-objcopy -O ihex -R .eeprom $< $@

$(TARGET).elf: $(SRC)
	avr-gcc -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -o $@ $^

flash:
	avrdude -c arduino -P /dev/ttyUSB0 -p m328p -U flash:w:$(TARGET).hex

clean:
	rm -f *.o *.elf *.hex