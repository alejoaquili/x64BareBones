char * v = (char*)0xB8000 + 79 * 2;

int main() {
	*v = 'X';
	return 0xDEADBEEF;
}
