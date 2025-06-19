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


## Building

1. Clone the repository
2. Run `cmake -B build` in the root directory
3. Run `cmake --build build` to build the project

For Windows, you need to run `cmake --build build --config Release` to build the project in release mode.

## Usage

```sh
PasswordHook.exe <path to wordList> <username> <password>
```

Based on the documentation of [passwdhk](https://sourceforge.net/projects/passwdhk/), you specify the registry keys `preChangeProg` and `preChangeProgArgs`

In the `preChangeProg` you configure the path to the executable, and in the `preChangeProgArgs` you configure the path to the wordlist. The username and password is provided automatically.

### Configuration options

The application uses a config.ini file for settings. The config file is copied to the build directory automatically during the build process.
You can change the values as you please.

Example `config.ini`:

```ini
[PasswordSettings]
# The minimum score required for a password to be considered valid.
MinimumScore=10

# Allow the use of the provided username in the password.
AllowUsernameAsPassword=false

# The maximum repeat count for any character in the password.
MaxRepeatingCharacters=3

# The maximum repeat count for numbers in the password.
MaxRepeatingDigits=2

```

The project will not run without a config file. 

## Contributing

Contributions are welcome! Please open an issue or submit a pull request.

## License

This project is licensed under the MIT License.