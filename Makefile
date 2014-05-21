
.PHONY: clean

ggnesconv:
	gcc nes.c -o ggnesconv

clean:
	rm ggnesconv
