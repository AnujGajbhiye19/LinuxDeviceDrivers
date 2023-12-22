#define RDWR 0x11
#define WRONLY 0x10
#define RDONLY 0x01

struct pcdev_platform_data{
	int size;
	int perm;
	const char *serial_number;
};
