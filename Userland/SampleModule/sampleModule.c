char * v = (char*)0xB8000 + 79 * 2;
char anim[] = { '|', '/', '-', '\\' };

int main() {
	unsigned int a = 0;

	while(1) {
		v[160*0] = anim[a++ % 4];
	}
}
