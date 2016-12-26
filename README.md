# Itertools library for C #
This is my effort to implement [python's itertools](https://docs.python.org/3/library/itertools.html) library for C.

## Methods supported so far - ##
###### For creating iterator ######
* `newArrayIter(array, array_length, size_of_element)`
* `new_list_iter(list)`

###### Itertools library functions ######
* `count(init_val, step)`
* `cycle(array, array_length, size_of_element)`
* `repeat(value)`
* `accumulate(iterator, function, init_val)`
* `compress(iterator1, iterator2)`
* `islice(iterator, start, stop, step)`

###### Some other functions ######
* `permutations(array, n, r, size_of_element)`
* `combinations(array, n, r, size_of_element)`
* `combinations_replacement(array, n, r, size_of_element)`


## Example ##
###### How to create array iterator -  ######
```
iter* i = newArrayIter(a, n, sizeof(*a)); // Creates Array iterator.
while(has_next(i))
  printf("%d ", *(int*) next(i));
free_iter(i);
```

###### Accumulation example - ######
```
int fun(const void* a, const void* b)
{
	return *(int*) a + *(int*) b;
}

...

iter* i = accumulate(iter, fun, 0);   // iter is iterator.
while(has_next(i))
  printf("%d ", *(int*) next(i));   // prints cumulative sum.
free_iter(i);
```

###### Permutations - ######
```
iter* i = permutations(a, n, r, sizeof(*a));  // a is array
int* c;
printf("Permutations are \n");
while (has_next(i))
{
  c = (int*) next(i);
  for (int j = 0; j < r; j++)
    printf("%d ", c[j]);
  printf("\n");
}
free_iter(i);
```

## How to test ##
File `main.c` contains examples of how use this library.
```
make test           # Make files
./test
```

To test permutations
```
make permtest       # Make permutations test
./permtest          # Run it
```
To test combinations
```
make combtest       # Make combinations test
./combtest          # Run it
```

*I hope u like it. ;)*
