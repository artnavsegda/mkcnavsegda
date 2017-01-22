#define MASSIVE_SIZE 64

struct massive {
        int massive[MASSIVE_SIZE];
        int position;
};

long average(int *selekta,int amount, int startpos, int sizeofmassive);
long oversample(struct massive *working, int amount);
void increment(struct massive *working, int value);