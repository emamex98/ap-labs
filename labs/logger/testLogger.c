int infof(char*);
int warnf(char*);
int errorf(char*);
int panicf(char*);

int main() {

	infof("Info");
        warnf("Warning");
        errorf("Error");
        panicf("Panic");

	return 0;
}
