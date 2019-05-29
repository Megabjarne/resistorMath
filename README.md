# ResistorMath
Resistormath is a little program i threw together a while back, it takes in a network of resistors, and calculates the equivalent resistance.

## Compile
Compiling the program is easy, clone or download the repository, open a shell in the directory, and run pretty much any c++ compiler, for example `g++ resistorMath.cpp`.

## Usage
The resistors are given as a string argument to the program, it's given as resistor values, and whether they are in parallell or series.

Take for example two resistors, both 1 Ohm, in series, it would be given as `1&1`, 1 and then 1. Two resistors in parallell would be `1|1`.

More complex arrangements can be done with paranthesis, say three 2 Ohm resistors in parallell, which is in series with two 5 Ohm resistors in parallell, this would be given as `(2|2|2)&(5|5)`.

You can also specify the resistor values with magnitudes such as K, M and G, but fractional resistance cannot be done, so 5500 Ohm cannot be given as 5.5K or 5K5, this will result in an error.

## Example
![example circuit](http://media.cheggcdn.com/media/92b/92b56e64-0328-43c6-a50b-95044298a278/image)
To solve this, we would split the circuit into series and parallell parts, so
90 & (330 | (250 & ((1200 & 650) | (75 & 30 & 225 & 47 & 200 & 62)) & 675 & 50))
we remove the spaces and pass it to the program
```
resistorMath "90&(330|(250&((1200&650)|(75&30&225&47&200&62))&675&50))"
```

and get the following output:

```
Parsing
Calculating.........................
Answer: 358.818499 Ohms (358.818 ohms)
```

Which is the equivalent circuit resistance.
