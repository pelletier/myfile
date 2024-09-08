bin.path := .bin

cosmocc.bin := $(bin.path)/cosmocc/bin/cosmocc
cosmocc.path := $$(dirname $$(dirname $(cosmocc.bin)))

zip.bin := $(bin.path)/zip
zip.url := https://cosmo.zip/pub/cosmos/bin/zip


.PHONY: clean fmt

all: myfile

$(cosmocc.bin):
	@mkdir -p $(cosmocc.path)
	@echo Fetching cosmocc
	@cd $(cosmocc.path) && \
		curl -s https://cosmo.zip/pub/cosmocc/cosmocc-3.8.0.zip > cosmocc.zip && \
		unzip -q cosmocc.zip && \
		rm cosmocc.zip

$(zip.bin):
	@mkdir -p $(bin.path)
	@echo Fetching zip APE
	@curl -s $(zip.url) > $@

myfile: myfile.c $(cosmocc.bin) $(zip.bin)
	$(cosmocc.bin) -mcosmo -O2 -Wall -Wextra -Werror -o $@ $<
	zip -Aujq myfile $(zip.bin)

clean:
	rm -rf myfile $(bin.path)

fmt: myfile.c
	clang-format -i $<
