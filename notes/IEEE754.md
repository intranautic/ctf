### IEEE745

The IEEE 754 is a standard for representing real numbers in a floating point format, which allows us to represent such numerals in a fairly accurate albeit somewhat flawed format.

Computers obviously dont have infinite memory, so we cant truly represent irrational numbers, although there are various software implementations which allow for arbitrary precision in applications which demand accuracy.

The standard also defines various representations of numerals to handle exceptions which occur in an invalid arithmetic expression. For example, it defines numbers such as NaN and Infinity, as well as various rules for rounding, exceptions and error handling.

One of the major drawbacks is the fact that it cant store recurring digits, so an irrational number such as 1/3 will always be rounded slightly.

The term floating point refers to the fact that the position of the radix/base point is not fixed, so it is considered floating. This is useful for representing both very large and small real values.

The general concept surrounding the representation of floating point values is scientific notation. Computers utilize base 2 rather than base 10, so our base/radix e will be 2 rather than 10.

The position of the floating point is determined by the exponent, same as scientific notation. The mantissa/significand represents the significant digits as an integer with fixed precision (non floating point).

The memory representation is as follows, where the base is assumed to be 2.

```
msb [sign][exponent][mantissa] lsb
        |
        V
[sign (+/-)][mantissa]*[base (2)]^[exponent]
example: - 1.1 x2 ^5
```

Modern x86 processors also have an auxiliary x87 floating point unit which utilizes 80 bit floating point values, sometimes referred to as extended precision numbers.

The size of each respective component depends on the size of the value.

```
         sign   exponent     mantissa
32 bits: [1 bit][  8 bits   ][    23 bits    ]
64 bits: [1 bit][  11 bits  ][    52 bits    ]
80 bits: [1 bit][  15 bits  ][    64 bits    ]
```

Most modern microprocessors come equipped with an auxiliary FPU which stands for Floating Point Unit; specifically designed to carry out arithmetic operations on floating point values.

On x86 there is an extension called the x87 FPU.

The exponent is represented as an unsigned value, but when its being processed it will subtract by the bias 127 to get the signed exponent. This is apparently due to the difficultly of orderering from least to greatest without offset ordering and allows for easy comparison for the cpu/fpu.
