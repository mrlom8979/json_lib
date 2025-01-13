# JSON Parsing Library Documentation

This library provides utilities for parsing and querying JSON data structures. The example below demonstrates the main features of the library and how to use it in your projects.

---

## Features

- Parse JSON strings into an Abstract Syntax Tree (AST).
- Traverse and query the AST using paths.
- Retrieve values from JSON objects and arrays.
- Support for primitive JSON types: strings, numbers, booleans, null.

---

## Example Usage

### Code Example
```cpp
#include <json.h>

int main() {
  // Load JSON data from a file
  const char* _json = filesystem::get("test.json", false);

  // Decode JSON into an AST
  json::ast_node* r = json_decode(_json);

  // Print the parsed AST (for debugging purposes)
  print_ast(r);

  // Retrieve a value from the JSON using a path
  json::ast_result* result = __json_get_val(r, user->preferences->notifications->email);

  // Display the result
  printf("--- Result ------------------------------------------------\n");

  if (result) {
    printf("Value: %s\n", result->value);

    // Check the type of the result
    if (result->type == json::JSON_AST_STRING) {
      printf("Type: String\n");
    } else if (result->type == json::JSON_AST_NUMBER) {
      int number = atoi(result->value); // Convert to an integer
      printf("Type: Number\n");
      printf("Number: %d\n", number);
    } else if (result->type == json::JSON_AST_BOOLEAN) {
      printf("Type: Boolean\n");
      printf("Boolean: %s\n", strcmp(result->value, "true") == 0 ? "True" : "False");
    }

    // Free allocated memory for the result
    free(result->value);
    free(result);
  } else {
    printf("Value not found or invalid path.\n");
  }

  return 0;
}
```

### Input JSON (`test.json`)
```json
{
  "user": {
    "id": 3141592,
    "name": "John Doe",
    "email": "example@example.com",
    "age": 27,
    "roles": ["admin", "editor", "user"],
    "status": null,
    "preferences": {
      "notifications": {
        "email": "enabled",
        "sms": "disabled"
      }
    }
  },
  "settings": {
    "version": "1.2.3",
    "enabled": false
  }
}
```

### Expected Output
```
--- Result ------------------------------------------------
Value: enabled
Type: String
```

---

## Functions

### `json_decode`
- **Description**: Parses a JSON string into an Abstract Syntax Tree (AST).
- **Parameters**:
  - `const char* json_string`: The JSON string to parse.
- **Returns**: `json::ast_node*` (root of the AST).

### `print_ast`
- **Description**: Prints the structure of the AST to the console.
- **Parameters**:
  - `json::ast_node* root`: The root node of the AST.

### `__json_get_val`
- **Description**: Retrieves a value from the JSON AST by following a path.
- **Parameters**:
  - `json::ast_node* root`: The root node of the AST.
  - `const char* path`: The path to the desired value (e.g., `user->preferences->notifications->email`).
- **Returns**: `json::ast_result*` containing the value and its type.

---

## Types

### `json::ast_node`
Represents a node in the JSON Abstract Syntax Tree.

- **Fields**:
  - `type`: The type of the node (e.g., object, array, string, number, boolean, null).
  - `key`: The key associated with the node (if applicable).
  - `value`: The value stored in the node (if applicable).
  - `children`: Child nodes (for objects and arrays).

### `json::ast_result`
Represents the result of a query.

- **Fields**:
  - `value`: The string representation of the value.
  - `type`: The type of the value (e.g., `JSON_AST_STRING`, `JSON_AST_NUMBER`, etc.).

---

## Error Handling

- If a path is invalid or the value is not found, `__json_get_val` returns `nullptr`.
- Always check for `nullptr` before using the result to avoid segmentation faults.

---

## Memory Management

- The result of `__json_get_val` must be freed manually:
  ```cpp
  free(result->value);
  free(result);
  ```
- Use appropriate cleanup functions for the AST to release allocated memory.

---

## Limitations

- Path syntax requires `->` for object keys and `[index]` for array indices.
- Currently supports only basic JSON types.

---

## Notes

- Ensure that the input JSON is valid.
- Use `print_ast` to debug and visualize the structure of the parsed JSON.


