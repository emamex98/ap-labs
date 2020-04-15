int infof(char*, ...);
int warnf(char*, ...);
int errorf(char*, ...);
int panicf(char*, ...);

int main() {

	int value1 = 404;
	int value2 = 500;
	float floatVal = 3.1416;
	char *str = "EMERGENCY";

	infof("Info - %f", floatVal);
        warnf("Warning - %d", value1);
        errorf("Error - %d %d", value1, value2);
        panicf("Panic - %s!", str);

	infof("This should not print");

	return 0;
}
