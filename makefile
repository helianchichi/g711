all:
	gcc g711aenc.c -o g711aenc
	gcc g711adec.c -o g711adec
	gcc g711uenc.c -o g711uenc
	gcc g711udec.c -o g711udec

clean:
	rm g711aenc g711adec g711uenc g711udec