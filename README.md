Object-Oriented Programming in C

Features:

- Class
  - meta class
  - super class
  - vtable

- Interface
  - enable inherit from multiple interfaces

- Inherit
  - single class
  - multiple interfaces

- Constructor
  - New
  - Copy
  - Move

- Destructor
  - automatically call super's destructor

Example:
```C

Interface(Printable,
          void virtual(Print, const char *);
)

CLASS(A_Class, CObject,
    int x;
)I(Printable)
    // member function A_Class::VMethod
    void MF(A_Class, VMethod);
END_CLASS(A_Class, CObject)

```