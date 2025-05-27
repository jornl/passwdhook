# PasswordHook

Basic C++ project for enhanced password management with [passwdhk](https://sourceforge.net/projects/passwdhk/).

- Integrates with passwdhk to encourage stronger passwords 
- Lets you define wordlists where each word carries a value
- Exits with a status code indicating whether the password is strong enough.

## Word list format

The word list file should be a `.txt` or `.csv` file with each line containing a word and its associated value, separated by a comma. For example:
```csv
password,1
123456,1
qwerty,1
```

## Contributing

Contributions are welcome! Please open an issue or submit a pull request.

## Building

1. Clone the repository
2. Compile with a C++17-compatible compiler

For example `cl /EHsc /std:c++17 .\PasswordHook.cpp`

## Usage

```sh
PasswordHook.exe <path to wordList> <username> <password>
```

Based on the documentation of [passwdhk](https://sourceforge.net/projects/passwdhk/), you specify the registry keys `preChangeProg` and `preChangeProgArgs`

In the `preChangeProg` you configure the path to the executable, and in the `preChangeProgArgs` you configure the path to the wordlist. The username and password is provided automatically.

### Configuration options

`config.ini` contains, which is all commented out, but the values there are the defaults. You can change them as you please.

```ini
[PasswordSettings]
# The minimum score required for a password to be considered valid.
#MinimumScore=10

# The maximum repeat count for any character in the password.
#MaxRepeatingCharacters=3

# Allow the use of the provided username in the password.
#AllowUsernameAsPassword=false

```

The project will run without a config file, it will then just use the default values. 

## License

This project is licensed under the MIT License.