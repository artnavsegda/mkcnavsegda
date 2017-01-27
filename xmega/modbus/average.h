#define MASSIVE_SIZE 64

struct massive {
        unsigned int massive[MASSIVE_SIZE];
        unsigned int position;
};

long average(unsigned int *selekta,int amount, int startpos, int sizeofmassive);
long oversample(struct massive *working, unsigned int amount);
void increment(struct massive *working, unsigned int value);