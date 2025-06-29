# PasswordRater

Basic C++ project for enhanced password management with [passwdhk](https://sourceforge.net/projects/passwdhk/).

- Integrates with [passwdhk](https://sourceforge.net/projects/passwdhk/) to encourage stronger passwords. 
- Lets you define wordlists where each word carries a value.
- Exits with a status code indicating whether the password is strong enough.

## Word list

The word list file should be a `.txt` or `.csv` file with each line containing a word and its associated value, separated by a comma. For example:
```csv
password,1
123456,1
qwerty,1
```

I've also tested storing the wordlist on a UNC-path, which works fine. Which means you can store the wordlist in your NETLOGON share, and use it from there.

## Building
**Prerequisites**:
- CMake
- A C++ compiler (e.g., GCC, Clang, MSVC)

**Steps to build**:
1. Clone the repository
2. Run `cmake -B build` in the root directory
3. Run `cmake --build build` to build the project

For Windows, you need to run `cmake --build build --config Release` to build the project in release mode.

## Usage

```sh
PasswordRater.exe <path to wordList> <username> <password>
```

Based on the documentation of [passwdhk](https://sourceforge.net/projects/passwdhk/), you specify the registry keys `preChangeProg` and `preChangeProgArgs`

In the `preChangeProg` you configure the path to the executable, and in the `preChangeProgArgs` you configure the path to the wordlist. The username and password is provided automatically.

### Configuration options

The application uses a config.ini file for settings. The config file is copied to the build directory automatically during the build process.
You can change the values as you please.

Example `config.ini`:

```ini
[PasswordSettings]
# The minimum score a password must reach to be considered valid.
# Increase this value to require stronger passwords.
#
# Default is 10.
MinimumScore=10

# Whether the provided username is allowed to appear in the password.
# Set to 'true' to permit the username as part of the password, or 'false' to forbid it.
#
# Default is false.
AllowUsernameAsPassword=false

# The maximum allowed repeat count for any single character in the password.
# If a character repeats more than this number consecutively, only half of the extra characters will count toward the score.
# For example, with MaxRepeatingCharacters=3, the run 'aaaaa' (5 a's) is scored as 2.
#
# Default is 3. Set to 0 to disable.
MaxRepeatingCharacters=3

# The maximum allowed repeat count for consecutive digits in the password.
# If a digit sequence is longer than this value, only half of the extra digits will count toward the score (integer division).
# Example: With MaxConsecutiveDigits=2, the run '12345' (5 digits) is scored as 2.
#
# Default is 2. Set to 0 to disable.
MaxConsecutiveDigits=2

```

The project will run without a config file. 

## Contributing

Contributions are welcome! Please open an issue or submit a pull request.

## License

This project is licensed under the MIT License.