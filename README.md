# JSON Library Documentation

This library provides utilities to parse, manipulate, and query JSON data efficiently. Below is an example demonstrating its usage.

---

## Getting Started

### 1. **Include the Library Header**
Make sure to include the `json.h` header in your source file:
```c
#include <json.h>
```

### 2. **Basic Example**

Here is a simple example that demonstrates loading a JSON file, decoding it into an AST (Abstract Syntax Tree), and extracting specific values:

```c
#include <json.h>

int main() {
  // Load JSON data from a file
  const char* _json = filesystem::get("user.json", false);

  // Decode JSON string into an AST
  json::ast_node* r = json_decode(_json);

  // Query for the value of "user->email"
  json::ast_result* result = __json_get_val(r, user->email);

  // Print the result
  printf("--- Result ------------------------------------------------\n");

  if (result) {
    printf("Value: %s\n", result->value);

    // Determine the type of the value
    if (result->type == json::JSON_AST_STRING) {
      printf("Type: String\n");
    } else if (result->type == json::JSON_AST_NUMBER) {
      int number = atoi(result->value); // Convert to integer
      printf("Type: Number\n");
      printf("Number: %d\n", number);
    } else if (result->type == json::JSON_AST_BOOLEAN) {
      printf("Type: Boolean\n");
      printf("Boolean: %s\n", strcmp(result->value, "true") == 0 ? "True" : "False");
    }

    // Free the result value and object
    free(result->value);
    free(result);
  } else {
    printf("Value not found or invalid path.\n");
  }

  return 0;
}
```

---

## Key Functions

### `json_decode`
**Description:** Decodes a JSON string into an AST (Abstract Syntax Tree).  
**Prototype:**
```c
json::ast_node* json_decode(const char* json_string);
```

- **Parameters:**
  - `json_string`: JSON data as a string.
- **Returns:** A pointer to the root AST node.

---

### `__json_get_val`
**Description:** Retrieves a value from the JSON AST by a given path.  
**Prototype:**
```c
json::ast_result* __json_get_val(json::ast_node* root, const char* path);
```

- **Parameters:**
  - `root`: The root node of the JSON AST.
  - `path`: Path to the desired value (e.g., `user->email`).
- **Returns:** A pointer to `json::ast_result` containing the value and its type, or `NULL` if the path is invalid.

---

### `json::ast_result`
**Structure Fields:**
- `char* value`: The value as a string.
- `int type`: The type of the value. Possible values:
  - `json::JSON_AST_STRING`: Indicates a string value.
  - `json::JSON_AST_NUMBER`: Indicates a numeric value.
  - `json::JSON_AST_BOOLEAN`: Indicates a boolean value.

---

## Memory Management

- Ensure you **free** the memory allocated for results (`json::ast_result`) using `free()`.
- The `value` field in `json::ast_result` must also be explicitly freed.

Example:
```c
free(result->value);
free(result);
```

---

## Notes

- **File Handling:** In the example, `filesystem::get` is used to load the JSON data. Replace it with your file handling logic if necessary.
- **Error Handling:** Always check for `NULL` results to avoid dereferencing invalid pointers.
- **Path Format:** Use the format `key1->key2` to traverse nested JSON structures.

---

## License

This library is distributed under the [MIT License](LICENSE).

---

