[![License](https://img.shields.io/badge/License-MIT-blue.svg)](https://github.com/CurrencyFinn/TermSymbolCpp/blob/main/LICENSE)
[![Version](https://img.shields.io/badge/Version-0.0.1.0-blue.svg)](https://github.com/CurrencyFinn/TermSymbolCpp/releases/)

# TermSymbolsCPP

TermsymbolsCPP is a library that uses C++ to generate all possible configuration of a multi-electron atom. With the use of the angular momentum quantum number, l, and the amount of valence electrons a table is generated.

## Usage

First a output csv file is needed to be generated using the calc.cpp file. Here the inputs are the angular momentum quantum number, l, and the amount of valance electrons, vE.

```cpp
int main()
{
    calcPossibleConfig(l,vE);
    return 0;
}
````

Now the path of the output csv is copied over to termTables.py file where a table is generated with all relevant values to extract the term symbols.

```python
if __name__ == "__main__":
    pathToCsv = "output.csv"
    main(pathToCsv)
```

Example output of Carbon (l=1, vE=2) multi-electrons atom:

|      | -5.0 | -4.0 | -3.0 | -2.0 | -1.0 |  0.0 |  1.0 |  2.0 |  3.0 |  4.0 |  5.0 |
|------|------|------|------|------|------|------|------|------|------|------|------|
| -1.5 |  0   |  0   |  1   |  1   |  2   |  2   |  2   |  1   |  1   |  0   |  0   |
| -0.5 |  1   |  2   |  4   |  6   |  8   |  8   |  8   |  6   |  4   |  2   |  1   |
|  0.5 |  1   |  2   |  4   |  6   |  8   |  8   |  8   |  6   |  4   |  2   |  1   |
|  1.5 |  0   |  0   |  1   |  1   |  2   |  2   |  2   |  1   |  1   |  0   |  0   |


## Optimizations
The code should be optimized enough to generate readily the term tables (~350 ms for a 3,7 input)
Several optimizations have caused drastic improvement in performance compared to [TermSymbolsPy](https://github.com/CurrencyFinn/TermSymbolsPy). 
* Language rewrite
* Decrease in array size (3D → 2D)
* Ms, Ml value immediate append
* Generation combined with mixing
* Mixing with permutations
* No limit variable
* No computation with Pauli exclusion ([0, 0.5,-0.5] → [2, 0.5,-0.5])

## Improvements

* Overal generation is still slow and still random therefore can be accessed to a faster path. 
* Immediate import towards python 
* pip install possibilities

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

[MIT](https://github.com/CurrencyFinn/TermSymbolCpp/blob/main/LICENSE)
