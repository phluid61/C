
TARGET=$(shell basename "${CURDIR}")
README=README.md

.PHONY: default
default: $(TARGET)

.PHONY: README
README: $(README)
$(README): $(TARGET)
	${CURDIR}/$(TARGET) --help | sed -e 's/\x1B\[[01]m/*/g' > $(README)

.PHONY: clean
clean:
	-rm -f $(TARGET) $(README)

