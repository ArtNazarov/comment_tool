# comment_tool

C++ console app to manage comments in a some directory

The utility create, update or delete hidden file named ".comment" in the passed directory

# Usage

```
./comment_tool [--comment|--append-comment|--remove-comment|--view-comment] <path> [--from-command-line "text"|--from-text-file /path/to/file.txt|--from-stdin]
```

# Adding a comment about the current directory

```
./comment_tool --comment . --from-command-line "Some strange text"
```

# Appending a comment about the current directory

```
./comment_tool --append-comment . --from-command-line "Other line"
```

# Viewing all comments about the current directory

```
./comment_tool --view-comment .
```

# Removing all comments about the current directory

```
./comment_tool --remove-comment .
```
