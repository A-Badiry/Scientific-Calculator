# Scientific Calculator

Welcome to the Scientific Calculator GitHub repository! This project is a command-line scientific calculator that supports various mathematical functions and constants. It is designed to handle mathematical expressions such as "3*sin(21) - (22/11 +3)".

## Features

1. **Expression Format:**
   - The calculator accepts input expressions of the form: "3*sin(21) - (22/11 +3)".

2. **Commands:**
   - `config`: Configure the calculator settings, including angle unit, notation, and float precision.
   - `sconfig`: Show the current configuration (e.g., RAD, Nrm, 4).
   - `exit`: Exit the calculator.

3. **Math Functions:**
   - `sin`: Sine function.
   - `cos`: Cosine function.
   - `tan`: Tangent function.
   - `exp`: Exponential function.
   - `log`: Logarithmic function.
   - `sqrt`: Square root function.

4. **Constants:**
   - `C`: Speed of light.
   - `R`: Universal gas constant.
   - `G`: Gravitational constant.
   - `Me`: Electron mass.
   - `Mp`: Proton mass.
   - `Mn`: Neutron mass.
   - `E`: Electron charge.
   - `Na`: Avogadro's number.
   - `Vm`: Molar volume of ideal gas.
   - `ANS`: Result of the last calculation.
   - `PI`: Mathematical constant Pi.

## Getting Started

1. Clone the repository to your local machine:

   ```bash
   git clone https://github.com/A-Badiry/scientific-calculator.git
   ```

2. Navigate to the project directory:

   ```bash
   cd scientific-calculator
   ```

3. Compile the calculator:

   ```bash
    make
   ```
4. Run the calculator

   ```bash
    ./Calculator
   ```

## Usage

1. Input mathematical expressions using the specified format.
2. Use the commands (`config`, `sconfig`, `exit`) to configure settings and control the calculator.
3. Explore various math functions and constants.

## Configuration

The `config` command allows you to customize the calculator's configuration:

```bash
config
```

Example output:

```
Enter angle unit (D(deg)/R(rad)/G(gra)): R
Enter notation (N(Nrm)/S(Sci)): D
Enter float precision (integer): 4
```
### Default Configuration

The calculator comes pre-configured with the following settings:

Angle Unit: RAD
Notation: Sci (Scientific)
Float Precision: 4

## Show Configuration

The `sconfig` command displays the current configuration:

```bash
sconfig
```

Example output:

```
RAD, Sci, 4
```

## Exit the Calculator

To exit the calculator, use the `exit` command:

```bash
exit
```
## Context-Free Grammar (CFG)

The calculator's input expressions can be described by the following Context-Free Grammar:

```
E      → E + T | E - T | T
T      → T * F | T / F | F
F      → (-#) | (E) | Fct | Const | Cmd | #
Fct    → "cos()" | "sin()" | "tan()" | "exp()" | "log()" | "sqrt()"
Const  → "C" | "G" | "Me" | "Mp" | "Mn" | "E" | "Na" | "Vm" | "R" | "ANS" | "PI"
Cmd    → "exit" | "config" | "sconfig"
#      → {0,1,2,3,4,5,6,7,8,9}* . {0,1,2,3,4,5,6,7,8,9}*
```

### Grammar Explanation:

- `E`: Represents the expression, which can be composed of addition, subtraction, or just a term.
- `T`: Represents a term, which can be composed of multiplication, division, or just a factor.
- `F`: Represents a factor, which can be a negation, a parenthesized expression, a function, a constant, a command, or an empty string.
- `Fct`: Represents the mathematical functions supported by the calculator.
- `Const`: Represents the constants recognized by the calculator.
- `Cmd`: Represents the commands recognized by the calculator.
- `#`: Represents a double numeric value.

## Contributing

Contributions are welcome! If you have any suggestions, bug reports, or enhancements, feel free to contact me: achraf.badiry@edu.bme.hu

## License

This project is licensed under the GNU General Public License - see the [LICENSE](https://www.gnu.org/licenses/gpl-3.0.html) website for details.