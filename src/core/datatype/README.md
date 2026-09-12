# Data types

| Example           | Topic                                                           |
|-------------------|-----------------------------------------------------------------|
| `Fundamental`     | bool, character, integer, floating point; sizes and `std::numeric_limits` |
| `Array`           | C arrays, array-to-pointer decay, `std::size`, `std::span`       |
| `Pointer`         | address-of/dereference, const pointers, pointer arithmetic, `void*`, function pointers |
| `Reference`       | lvalue/rvalue references, const& lifetime extension, `std::move` |
| `Enum`            | unscoped vs `enum class`, underlying types, `using enum`, flags  |
| `Struct`          | aggregates, designated initializers, padding and alignment      |
| `Union`           | the active member rule, `std::bit_cast`, `std::variant`          |
| `TypeConversions` | implicit conversions, the four casts, slicing, `auto`/`decltype` |
| `UserDefinedLiteral` | raw string literals, standard literal suffixes and user-defined literals *(draft)* |
| `Byte` | std::byte as raw memory, versus char and unsigned char *(draft)* |
| `BitOperations` | the `<bit>` header: popcount, countl_zero, bit_width, rotl, endian and bit_cast *(draft)* |

## 1. Fundamental types

The standard only fixes minimum sizes (`int` has at least 16 bits, `long` at least 32). When the
exact width matters, use `<cstdint>`: `std::int32_t`, `std::uint8_t`, and so on.

## 2. Pointers vs references

|                     | Pointer `T*`          | Reference `T&`        |
|---------------------|-----------------------|-----------------------|
| can be null         | yes                   | no                    |
| can be re-seated    | yes                   | no (bound once)       |
| must be initialized | no                    | yes                   |
| arithmetic          | yes                   | no                    |

Read `const` declarations from right to left:
`const int* p` means "p is a pointer to a const int", and `int* const p` means "p is a const
pointer to an int".

## 3. Casts

| Cast               | Use for                                                   |
|--------------------|-----------------------------------------------------------|
| `static_cast`      | well-defined conversions between related types            |
| `dynamic_cast`     | safe down-casts in polymorphic hierarchies (checked at run time) |
| `const_cast`       | adding or removing `const` (writing to a truly const object is UB) |
| `reinterpret_cast` | low-level reinterpretation of bits and addresses          |
| `(T)x` C-style     | avoid it: it silently picks any of the above              |

## 4. Unions and variants

Only the member written last may be read. Reading another member is fine in C but undefined
behavior in C++. To inspect the bytes of a value, use `std::bit_cast` or `std::memcpy`. For a
type-safe "one of several types", use `std::variant`.
