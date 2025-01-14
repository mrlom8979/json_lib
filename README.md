# JSON Library Example Documentation

This documentation provides an example of using the `json.h` library to decode a JSON string, traverse its structure, and extract values.

---

## Example Code

```cpp
#include <json.h>

int main() {
  // Load JSON data from a file
  const char* _json = filesystem::get("test.json", false);

  // Decode the JSON string into an abstract syntax tree (AST)
  json::ast_node* r = json_decode(_json);

  // Print the structure of the AST
  print_ast(r);

  // Example usage: Extract values from the JSON AST
  int number = __json_get_int(r, products[1]->id);        // Extract an integer
  double dbl = __json_get_double(r, products[0]->price); // Extract a double
  bool boolean = __json_get_bool(r, user->preferences->notifications->email); // Extract a boolean
  const char* str = __json_get_cstr(r, user->email);     // Extract a string

  // Display the extracted values
  printf("--- Result ------------------------------------------------\n");
  printf("Int: %d\n", number);
  printf("Double: %.2f\n", dbl);
  printf("Bool: %d\n", boolean);
  printf("String: %s\n", str);

  return 0;
}
```

---

## Explanation of Functions

### `json_decode`
**Prototype**: 
```cpp
json::ast_node* json_decode(const char* json_string);
```
- **Description**: Decodes a JSON string into an Abstract Syntax Tree (AST) for traversal and value extraction.
- **Parameters**:
  - `json_string`: A `const char*` representing the JSON content.
- **Returns**: A pointer to the root `json::ast_node` of the decoded JSON structure.

---

### `print_ast`
**Prototype**: 
```cpp
void print_ast(const json::ast_node* root);
```
- **Description**: Prints the structure of the JSON AST for debugging purposes.
- **Parameters**:
  - `root`: A pointer to the root of the JSON AST.

---

### JSON Helper Macros

#### `__json_get_int`
**Prototype**: 
```cpp
int __json_get_int(const json::ast_node* ast, const char* path);
```
- **Description**: Extracts an integer value from the JSON AST based on the specified path.
- **Parameters**:
  - `ast`: The root of the JSON AST.
  - `path`: A JSON path to the target value (e.g., `products[1]->id`).
- **Returns**: The integer value at the specified path.

#### `__json_get_double`
**Prototype**: 
```cpp
double __json_get_double(const json::ast_node* ast, const char* path);
```
- **Description**: Extracts a double value from the JSON AST based on the specified path.
- **Parameters**:
  - `ast`: The root of the JSON AST.
  - `path`: A JSON path to the target value.
- **Returns**: The double value at the specified path.

#### `__json_get_bool`
**Prototype**: 
```cpp
bool __json_get_bool(const json::ast_node* ast, const char* path);
```
- **Description**: Extracts a boolean value from the JSON AST based on the specified path.
- **Parameters**:
  - `ast`: The root of the JSON AST.
  - `path`: A JSON path to the target value.
- **Returns**: The boolean value at the specified path.

#### `__json_get_cstr`
**Prototype**: 
```cpp
const char* __json_get_cstr(const json::ast_node* ast, const char* path);
```
- **Description**: Extracts a string value from the JSON AST based on the specified path.
- **Parameters**:
  - `ast`: The root of the JSON AST.
  - `path`: A JSON path to the target value.
- **Returns**: A `const char*` representing the string value at the specified path.

---

## Output Example

Given a `test.json` file with the following content:
```json
{
  "products": [
    {"id": 1, "price": 19.99},
    {"id": 2, "price": 29.99}
  ],
  "user": {
    "email": "user@example.com",
    "preferences": {
      "notifications": {
        "email": true
      }
    }
  }
}
```

The output of the program will be:
```
--- Result ------------------------------------------------
Int: 2
Double: 19.99
Bool: 1
String: user@example.com
```

---

## Notes
- Ensure `json.h` is correctly included and configured in your project.
- The JSON file should be well-formed and accessible to avoid runtime errors.
- Use `print_ast` for debugging to understand the structure of your JSON data.

