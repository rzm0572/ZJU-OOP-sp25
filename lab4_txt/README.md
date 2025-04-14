# Lab 4: Personal Diary

## Compile

Make sure you have `g++`, `make` and `cmake` in your system. Then, run the following commands to compile the program:

```bash
./compile.sh
```

The compiled program will be in the `build` and `test` directory.

## Test

To test the program, run the following command:

```bash
./test.sh
```

The test cases will be run and the results will be printed to the console.

## Usage

- `pdadd`: Add a new diary into the personal diary.

    ```bash
    ./pdadd
    ```

    The program takes no argument and will prompt the user to input the diary content. The program will then add the diary to the personal diary file.

- `pdlist`: List all the diaries in the personal diary by the range of dates specified.

    ```bash
    ./pdlist {{start_date}} {{end_date}}
    ```

    The program takes two arguments, `start_date` and `end_date`, which are the start and end dates of the diaries to be listed. The program will then list all the diaries in the personal diary file within the specified date range.

- `pdshow`: Show the content of a specific diary by its date.

    ```bash
    ./pdshow [date]
    ```

    If date is not specified in the command, the program will prompt the user to input the date. The program will then show the content of the diary with the specified date.

- `pdremove`: Remove a specific diary by its date.

    ```bash
    ./pdremove [date]
    ```

    If date is not specified in the command, the program will prompt the user to input the date. The program will then remove the diary with the specified date from the personal diary file.
