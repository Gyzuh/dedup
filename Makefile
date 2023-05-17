ENCODER_SRC_FILES = encoder.cc chunking.cc compression.cc hashing.cc
DECODER_SRC_FILES = decoder.cc

ENCODER = encoder
DECODER = decoder

.PHONY: all clean test

all: $(ENCODER) $(DECODER)

test: all
	@dd if=/dev/urandom of=input.bin bs=1M count=1
	cat input.bin input.bin > duplicated.bin
	./$(ENCODER) duplicated.bin output.enc
	./$(DECODER) output.enc test.bin
	@cmp duplicated.bin test.bin && echo "Test was successful" || echo "Test failed"
	@rm input.bin duplicated.bin output.enc test.bin

clean:
	$(RM) $(ENCODER) $(DECODER)

$(ENCODER): $(ENCODER_SRC_FILES)

$(DECODER): $(DECODER_SRC_FILES)
