SRC_FILES = encoder.cc chunking.cc compression.cc hashing.cc

TARGET = encoder

.PHONY: all clean

all: $(TARGET)

clean:
	$(RM) $(TARGET)

$(TARGET): $(SRC_FILES)
