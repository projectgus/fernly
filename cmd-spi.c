#include <string.h>
#include "bionic.h"
#include "serial.h"
#include "printf.h"
#include "spi.h"

int cmd_spi(int argc, char **argv)
{
	uint32_t recv_bytes_count;
	uint8_t xmit_bytes[argc - 1];
	int i;

	if (argc > 0 && !_strcasecmp(argv[0], "id")) {
		uint8_t out[1] = {0x9f};
		uint8_t in[3];

		spi_cmd_txrx(sizeof(out), sizeof(in), out, in);

		printf("ID: %02x %02x %02x\n", in[0], in[1], in[2]);

		return 0;
	}

	if (argc < 2) {
		printf("Send SPI bytes out and read the response.\n");
		printf("Usage:\n");
		printf(" spi [count] [byte 1] [byte 2] ...\n");
		printf("     Where \"count\" is the number of bytes to expect"
			     " in response.\n");
		printf(" spi id   -- get device id\n");
		return 1;
	}

	recv_bytes_count = strtoul(argv[0], NULL, 0);
	uint8_t recv_bytes[recv_bytes_count];

	for (i = 1; i < argc; i++)
		xmit_bytes[i - 1] = strtoul(argv[i], NULL, 0);

	printf("Transmitting %d bytes and expecting a response with %d bytes:\n",
			sizeof(xmit_bytes), sizeof(recv_bytes));
	serial_print_hex(xmit_bytes, sizeof(xmit_bytes));

	spi_cmd_txrx(sizeof(xmit_bytes), sizeof(recv_bytes),
			xmit_bytes, recv_bytes);

	printf("Response:\n");
	serial_print_hex(recv_bytes, sizeof(recv_bytes));

	return 0;
}
