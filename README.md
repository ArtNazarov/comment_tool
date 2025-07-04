# comment_tool

C++ console app to manage comments in a some directory

The utility create, update or delete hidden file named ".comment" in the passed directory

# How to compile

```
g++ comment_tool.cpp -o comment_tool
```

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

# Appending a comment from standard input using the pipe

```
echo "Test comment" | comment_tool --append-comment . --from-stdin
```

# Viewing all comments about the current directory

```
./comment_tool --view-comment .
```

# Removing all comments about the current directory

```
./comment_tool --remove-comment .
```
# Using with bash aliases

Add to the ~/.bashrc like:
```
alias set_comment="comment_tool --comment . --from-command-line "
alias append_comment="./comment_tool --append-comment . --from-command-line "
alias get_comments="comment_tool --view-comment . "
```
and apply changes:
```
source ~/.bashrc
```
After that you can use
```
set_comment "comment about the current dir"
get_comments
```

# Creating directory with comment

Add lines to the ~/.bashrc

```
mkdir-with-comments() {
  # Создаём директорию, учитывая пробелы в имени
  mkdir -p "$1"
  
  # Добавляем комментарий с помощью comment_tool
  comment_tool --comment "$1" --from-command-line "$2"
}

```

and apply changes:

```
source ~/.bashrc
```

After that you can use

```
mkdir-with-comments test_dir "Test comment"
```

# Finding directories with passed comment

Add to ~/.bashrc:

```
find_comment_dirs() {
    local search_path="$1"
    local target_substr="$2"
    local current_user
    current_user=$(id -un)

    find "$search_path" -type d ! -user "$current_user" -prune -o \
        -type f -name ".comment" -user "$current_user" -exec sh -c '
            for file; do
                if grep -q "$0" "$file"; then
                    dirname "$file"
                fi
            done
        ' "$target_substr" {} + | sort -u
}
```

Note: the search will skip directories if their owner is not the current user

For example,

```
find_comment_dirs /home/username "substring"
```

# GUI frontend

[comment_tool_gui_qt](https://github.com/ArtNazarov/comment_tool_gui_qt)
