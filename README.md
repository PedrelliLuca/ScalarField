# ScalarField
A "DA: Origins" - like game, where magic works by interacting with the environment.

## Coding Style
I follow ZURU Tech's coding style, which is just https://github.com/Allar/ue4-style-guide#table-of-contents but with some exceptions: 
Example:
```cpp
class MyClass {

  public:
    bool FuncPub();
    float VarPub;
  protected:
    int _funcPro();
    bool _varPro;
  private:
    void _funcPvt();
    int _varPvt;
}
```
Exceptions:
1. macros: all UPPER_SNAKE_CASE. e.g. #define SOMETHING_SNEAKY artigianato.
2. public/private static const(expr) all UPPER_SNAKE_CASE. e.g. Class::PRIVATE_VARIABLE; or class T { const int CONSTANT_VALUE = 0; }.
3. private and protected class variables: always begin with _ and followed by camelCase - NO exceptions, also for acronyms. E.g. _xmlVariable is OK, while _XMLVariable is NOT OK.
4. private and protected class methods: always begin with _ and followed by camelCase - NO exceptions. e.g. _thisIsPrivate(yes).
5. public class variables: PascalCase - (here we can also respect the acronym rule of having them all UPPERCASE, e.g. JSONVariable and IsThisJSONVariableSetted).
6. public class methods: PascalCase e.g. object->ThisIsPublic(yes).
7. Function parameters and local variables: always use camelCase.
8. const usage: follow const T, const T&, and const *T.
9. boolean variables: follow the same rules of private/public variables, but with the b prefix. e.g. private variable _bDead. public variable bCamelCase (because is still camel case, since the first letter is lower case).
