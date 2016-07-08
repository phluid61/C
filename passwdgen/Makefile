
TARGET=$(shell basename "${CURDIR}")

.PHONY: default
default: $(TARGET)

README: $(TARGET)
	${CURDIR}/$(TARGET) --help | sed -e 's/\x1B\[[01]m//g' > README

.PHONY: clean
clean:
	-rm -f $(TARGET) README

