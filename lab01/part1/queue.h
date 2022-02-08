
// Maximum number of elements in a queue
//
#define MAX_Q_SIZE	10

// Place function prototypes here.
void enq(double);
double deq(); 

void clear_sum();
void clear_prod();
void sum(double);
void prod(double);
double reduce();
double flex_reduce(void (*clear)(), void (*op)(double));
