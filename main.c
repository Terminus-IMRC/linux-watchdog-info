#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/watchdog.h>

void info_GETSUPPORT(int fd, struct watchdog_info *rident);
void info_GETSTATUS(int fd);
void info_GETBOOTSTATUS(int fd);
void info_GETTEMP(int fd);
void info_GETTIMEOUT(int fd);
void info_GETPRETIMEOUT(int fd);
void info_GETTIMELEFT(int fd);

int main()
{
	int fd = -1;
	struct watchdog_info ident;
	int ret;

	fd = open("/dev/watchdog", O_RDONLY);
	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	info_GETSUPPORT(fd, &ident);

	info_GETSTATUS(fd);
	info_GETBOOTSTATUS(fd);
	if (ident.options & WDIOF_OVERHEAT)
		info_GETTEMP(fd);
	info_GETTIMEOUT(fd);
	if (ident.options & WDIOF_PRETIMEOUT)
		info_GETPRETIMEOUT(fd);
	info_GETTIMELEFT(fd);

	ret = close(fd);
	if (ret == -1) {
		perror("close");
		exit(EXIT_FAILURE);
	}

	return 0;
}

void info_GETSUPPORT(int fd, struct watchdog_info *rident)
{
	unsigned int i;
	struct watchdog_info ident;
	struct {
		const __u32 wdiof;
		const char *wdiof_name;
		const char *desc;
	} wdiof_desc[] = {
#define X(x) x, #x
		{X(WDIOF_OVERHEAT), "Reset due to CPU overheat"},
		{X(WDIOF_FANFAULT), "Fan failed"},
		{X(WDIOF_EXTERN1), "External relay 1"},
		{X(WDIOF_EXTERN2), "External relay 2"},
		{X(WDIOF_POWERUNDER), "Power bad/power fault"},
		{X(WDIOF_CARDRESET), "Card previously reset the CPU"},
		{X(WDIOF_POWEROVER), "Power over voltage"},
		{X(WDIOF_SETTIMEOUT), "Set timeout (in seconds)"},
		{X(WDIOF_MAGICCLOSE), "Supports magic close char"},
		{X(WDIOF_PRETIMEOUT), "Pretimeout (in seconds), get/set"},
		{X(WDIOF_ALARMONLY), "Watchdog triggers a management or other external alarm not a reboot"},
		{X(WDIOF_KEEPALIVEPING), "Keep alive ping reply"}
#undef X
	};
	int ret;

	ret = ioctl(fd, WDIOC_GETSUPPORT, &ident);
	if (ret == -1) {
		perror("ioctl: WDIOC_GETTIMESUPPORT");
		exit(EXIT_FAILURE);
	}

	printf("options:\n");
	for (i = 0; i < sizeof(wdiof_desc) / sizeof(wdiof_desc[0]); i ++)
		if (ident.options & wdiof_desc[i].wdiof)
			printf(" + %s: %s\n", wdiof_desc[i].wdiof_name, wdiof_desc[i].desc);

	printf("firmware_version: 0x%08x\n", ident.firmware_version);

	printf("identity:");
	for (i = 0; i < 32; i ++)
		printf(" 0x%02x", ident.identity[i]);
	printf(" (");
	for (i = 0; i < 32; i ++) {
		if (ident.identity[i] == '\0')
			break;
		printf("%c", ident.identity[i]);
	}
	printf(")\n");

	memcpy(rident, &ident, sizeof(ident));
}

void info_GETSTATUS(int fd)
{
	int ret;
	int t;

	ret = ioctl(fd, WDIOC_GETSTATUS, &t);
	if (ret == -1) {
		perror("ioctl: WDIOC_GETSTATUS");
		exit(EXIT_FAILURE);
	}
	printf("status: %d\n", t);
}

void info_GETBOOTSTATUS(int fd)
{
	int ret;
	int t;

	ret = ioctl(fd, WDIOC_GETBOOTSTATUS, &t);
	if (ret == -1) {
		perror("ioctl: WDIOC_GETBOOTSTATUS");
		exit(EXIT_FAILURE);
	}
	printf("boot status: %d\n", t);
}

void info_GETTEMP(int fd)
{
	int ret;
	int t;

	ret = ioctl(fd, WDIOC_GETTEMP, &t);
	if (ret == -1) {
		perror("ioctl: WDIOC_GETTEMP");
		exit(EXIT_FAILURE);
	}
	printf("temp: %d\n", t);
}

void info_GETTIMEOUT(int fd)
{
	int ret;
	int t;

	ret = ioctl(fd, WDIOC_GETTIMEOUT, &t);
	if (ret == -1) {
		perror("ioctl: WDIOC_GETTIMEOUT");
		exit(EXIT_FAILURE);
	}
	printf("timeout: %d secs\n", t);
}

void info_GETPRETIMEOUT(int fd)
{
	int ret;
	int t;

	ret = ioctl(fd, WDIOC_GETPRETIMEOUT, &t);
	if (ret == -1) {
		perror("ioctl: WDIOC_GETPRETIMEOUT");
		exit(EXIT_FAILURE);
	}
	printf("pretimeout: %d secs\n", t);
}

void info_GETTIMELEFT(int fd)
{
	int ret;
	int t;

	ret = ioctl(fd, WDIOC_GETTIMELEFT, &t);
	if (ret == -1) {
		perror("ioctl: WDIOC_GETTIMELEFT");
		exit(EXIT_FAILURE);
	}
	printf("timeleft: %d secs\n", t);
}
