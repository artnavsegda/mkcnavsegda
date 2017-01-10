#define MASSIVE_SIZE 64

struct massive {
	unsigned int massive[MASSIVE_SIZE];
	unsigned int position;
};

void Average(void);
long oversample(struct massive *working, unsigned int amount);
long oversample_engine(unsigned int *selekta,int amount, int startpos, int sizeofmassive);
void increment(struct massive *working, unsigned int value);

extern struct massive ad7705_averaging_massive, measurment_averaging_massive, temperature_averaging_massive;